// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignClassifierNeuralNetwork2.cc"

#include "Ravl/PatternRec/DesignClassifierNeuralNetwork2.hh"
#include "Ravl/PatternRec/ClassifierNeuralNetwork2.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/fann/fann.h"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/PatternRec/OptimiseConjugateGradient.hh"
#include "Ravl/PatternRec/OptimiseDescent.hh"
#include "Ravl/PatternRec/FunctionCascade.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"

#define DODEBUG 1
#if DODEBUG
#define ONDEBUG(x) x
#include "Ravl/StrStream.hh"
#include "Ravl/OS/Date.hh"
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  void Sigmoid(const VectorC &z,VectorC &ret) {
    if(ret.Size() != z.Size())
      ret = VectorC(z.Size());
    for(unsigned i = 0;i < z.Size();i++)
      ret[i] = Sigmoid(z[i]);
  }

  //: Compute the derivative of sigmoid at z
  inline RealT SigmoidDerivative(const RealT z)
  {
    RealT sig = Sigmoid(z);
    return sig * (1-sig);
  }

  inline VectorC SigmoidDerivative(const VectorC &z) {
    VectorC ret(z.Size());
    for(unsigned i = 0;i < z.Size();i++)
      ret[i] = SigmoidDerivative(z[i]);
    return ret;
  }


  //! Cost function for logistic regression.

  class CostNeuralNetwork2C
   : public CostBodyC
  {
  public:
    //! Constructor
    CostNeuralNetwork2C(const SArray1dC<NeuralNetworkLayerC::RefT> &layers,
                            const SampleC<VectorC> &in,
                            const SampleC<UIntT> &out,
                            RealT regularisation,
                            bool verbose,
                            unsigned threads = 1);

    //! Compute the cost of a solution.
    virtual RealT Cost (const VectorC &X) const;

    //! Compute the jacobian.
    virtual VectorC Jacobian1(const VectorC &X) const;

    size_t NumParameters() const {
      size_t at = 0;
      for(unsigned i = 0;i < m_layers.Size();i++) {
        size_t numIn = m_layers[i]->NumInputs();
        size_t numOut = m_layers[i]->NumOutputs();
        size_t matSize = numIn * numOut;
        at += matSize;
        at += numOut;
      }
      return at;
    }

    //! Unroll parameters
    void Unroll(const VectorC &theta,SArray1dC<MatrixC> &w,SArray1dC<VectorC> &bias) const;

    //! Compute cost for section.
    bool SubCost (const SArray1dC<MatrixC> &w,const SArray1dC<VectorC> &bias,unsigned section,double *cost);

    //! Compute cost for section.
    bool SubGrad (const SArray1dC<MatrixC> &w,const SArray1dC<VectorC> &bias,unsigned section,VectorC &grad);

    //! Handle
    typedef SmartPtrC<CostNeuralNetwork2C> RefT;
  protected:
    SArray1dC<NeuralNetworkLayerC::RefT> m_layers;
    SampleC<VectorC> m_in;
    SampleC<UIntT> m_out;

    SArray1dC<Tuple2C<SampleC<VectorC>,SampleC<UIntT> > > m_sections;

    RealT m_regularisation;
    bool m_verbose;
    unsigned m_threads;
  };

   //! Constructor
   CostNeuralNetwork2C::CostNeuralNetwork2C(const SArray1dC<NeuralNetworkLayerC::RefT> &layers,
                                           const SampleC<VectorC> &in,
                                           const SampleC<UIntT> &out,
                                           RealT regularisation,
                                           bool verbose,
                                           unsigned threads)
    : m_layers(layers),
      m_in(in),
      m_out(out),
      m_regularisation(regularisation),
      m_verbose(verbose),
      m_threads(threads)
   {
     size_t vecSize = NumParameters();
     ONDEBUG(RavlDebug("Parameters:%s ",RavlN::StringOf(vecSize).c_str()));
     ParametersC parameters(vecSize,true);

     VectorC startX(vecSize);
     RealT eint = 0.12;
     for(unsigned i = 0;i < vecSize;i++)
       startX[i] = Random1() * 2.0 * eint - eint;
     parameters.SetConstP(startX);
     SetParameters(parameters);
     RavlAssert(parameters.StartX().Size() == vecSize);
     //m_a = SArray1dC<VectorC>(layers.Size());

     if(threads > 1) {
       m_sections = SArray1dC<Tuple2C<SampleC<VectorC>,SampleC<UIntT> > >(threads);
       size_t total = m_in.Size();
       size_t sectionSize = total / threads;
       IndexC at = 0;
       unsigned i = 0;
       for(;i < (threads-1);i++) {
         Tuple2C<SampleC<VectorC>,SampleC<UIntT> > &sec = m_sections[i];
         sec.Data1() = in.CompactFrom(at,sectionSize);
         sec.Data2() = out.CompactFrom(at,sectionSize);
         at += sectionSize;
       }
       // Do last block to ensure all elements are used.
       Tuple2C<SampleC<VectorC>,SampleC<UIntT> > &sec = m_sections[i];
       SizeT remainingBlocks = in.Size()-at;
       sec.Data1() = in.CompactFrom(at,remainingBlocks);
       sec.Data2() = out.CompactFrom(at,remainingBlocks);
       at += remainingBlocks;
       RavlAssert(at == total);
     } else {
       m_sections = SArray1dC<Tuple2C<SampleC<VectorC>,SampleC<UIntT> > >(1);
       m_sections[0] = Tuple2C<SampleC<VectorC>,SampleC<UIntT> >(in,out);
     }
   }

   //! Unroll parameters
   void CostNeuralNetwork2C::Unroll(const VectorC &theta,SArray1dC<MatrixC> &w,SArray1dC<VectorC> &bias) const {
     w = SArray1dC<MatrixC>(m_layers.Size());
     bias = SArray1dC<VectorC>(m_layers.Size());
     unsigned at = 0;
     for(unsigned i = 0;i < m_layers.Size();i++) {
       size_t numIn = m_layers[i]->NumInputs();
       size_t numOut = m_layers[i]->NumOutputs();
       size_t matSize = numIn * numOut;
       w[i] = MatrixC(m_layers[i]->Weights().Rows(),m_layers[i]->Weights().Cols(),const_cast<VectorC &>(theta).From(at,matSize));
       at += matSize;
       bias[i] = const_cast<VectorC &>(theta).From(at,numOut);
       at += numOut;
     }
   }

   bool CostNeuralNetwork2C::SubCost (const SArray1dC<MatrixC> &w,
                                      const SArray1dC<VectorC> &bias,
                                      unsigned section,
                                      double *costPtr)
   {
     SArray1dC<VectorC> a(m_layers.Size()+1); // Stop vectors from being repeatedly allocated.
     RealT cost = 0;
     for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > it(m_sections[section].Data1(),m_sections[section].Data2());it;it++)
     {
       // Go forward.
       VectorC work = it.Data1();
       for(unsigned i = 0;i < m_layers.Size();i++) {
         VectorC &res = a[i+1];
         //res = w[i] * work + bias[i];
         RavlN::MulAdd(w[i],work,bias[i],res);
         SigmoidIP(res);
         work = res;
       }

      //ONDEBUG(RavlDebug("Data:%s Theta:%s ",RavlN::StringOf(it.Data1()).c_str(),RavlN::StringOf(theta).c_str()));
      for (unsigned i = 0; i < work.Size(); i++) {
        if (it.Data2() == i) {
          // Got it right
          cost += -Log(work[i]);
        } else {
          cost += -Log(1 - work[i]);
        }
      }
     } // end data set
     *costPtr = cost;
     return true;
   }

  //! Compute the cost of a solution.

  RealT CostNeuralNetwork2C::Cost (const VectorC &theta) const
  {
    RealT cost = 0;
    ONDEBUG(RavlDebug("Theta:%s",RavlN::StringOf(theta).c_str()));

    // Unroll parameters.
    SArray1dC<MatrixC> w;
    SArray1dC<VectorC> bias;

    Unroll(theta,w,bias);

    // Weights
    //  Row - Outputs
    //  Col - input weight.

    if(m_threads <= 1) {
      const_cast<CostNeuralNetwork2C *>(this)->SubCost(w,bias,0,&cost);
    } else {
      //RavlDebug("Using threaded costs.");
      VectorC costs(m_threads);
      SArray1dC<LaunchThreadC> threads(m_threads);
      costs.Fill(0);
      for(unsigned i = 1;i < m_threads;i++) {
        threads[i] = RavlN::LaunchThread(RavlN::TriggerPtr(const_cast<CostNeuralNetwork2C *>(this),&CostNeuralNetwork2C::SubCost,w,bias,i,&costs[i]));
      }
      // Do a bit of processing on this thread.
      const_cast<CostNeuralNetwork2C *>(this)->SubCost(w,bias,0,&costs[0]);
      for(unsigned i = 1;i < m_threads;i++) {
        threads[i].WaitForExit();
      }
      cost = costs.Sum();
    }

    if(m_regularisation > 0) {
      RealT sum = 0;
      // Ignore the bias vectors
      for(unsigned i = 1;i < m_layers.Size();i++)
        sum += w[i].SumOfSqr();
      cost += m_regularisation * sum;
    }

    RealT fcost = cost / ((RealT) m_in.Size());
    //ONDEBUG(RavlDebug("Theta=%s Cost=%f",RavlN::StringOf(theta).c_str(),fcost));
    // Print cost if requested.
    if(m_verbose)
      RavlDebug("Cost=%f",fcost);
    //RavlN::Sleep(1.0);
    return fcost;
  }


  //! Compute cost for section.
  bool CostNeuralNetwork2C::SubGrad (const SArray1dC<MatrixC> &w,
                                     const SArray1dC<VectorC> &bias,
                                     unsigned section,
                                     VectorC &grad)
  {
    SArray1dC<MatrixC> gw;
    SArray1dC<VectorC> gbias;
    Unroll(grad,gw,gbias);

    SArray1dC<VectorC> a(m_layers.Size()+1);
    SArray1dC<VectorC> z(m_layers.Size()+1);
    SArray1dC<VectorC> eps(m_layers.Size()+1);

    for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > it(m_sections[section].Data1(),m_sections[section].Data2());it;it++)
    {
      // Go forward.
      VectorC work = it.Data1();
      a[0] = work;
      for(unsigned i = 0;i < m_layers.Size();i++) {
        // FIXME:- This could be faster and avoid lots of allocations.
        //res = w[i] * work + bias[i];
        RavlN::MulAdd(w[i],work,bias[i],z[i+1]);
        Sigmoid(z[i+1],a[i+1]);
        work = a[i+1];
      }

      // Work backward through the network.

      // Compute errors at final layer
      VectorC err(work.Size());
      for (unsigned j = 0; j < err.Size(); j++) {
        if (j == it.Data2()) {
          err[j] = work[j] - 1;
        } else {
          err[j] = work[j];
        }
      }
      eps[m_layers.Size()] = err;

      for(int l = m_layers.Size() -1;l > 0;l--) {
        //RavlDebug("Back prop err.");
        VectorC newErr;
        // FIXME:- Could be faster.
        w[l].TMul(err).ElemMul(SigmoidDerivative(z[l]),newErr);
        err = newErr;
        eps[l] = err;
      }

      for(int l = m_layers.Size() -1;l >= 0;l--) {
        MatrixC &dw = gw[l];
        VectorC &db = gbias[l];
        //RavlDebug("Error layer %d size:%u   dw:%u %u db:%u la:%u ",l,(unsigned) err.Size(),(unsigned) dw.Rows().V(),(unsigned) dw.Cols().V(),(unsigned) db.Size().V(),(unsigned) a[l].Size().V());

        const VectorC &la = a[l];
        const VectorC &err = eps[l+1];
        dw.AddOuterProduct(err,la);
        db += err;
      }
    }

    return true;
  }

  //! Compute the jacobian.

  VectorC CostNeuralNetwork2C::Jacobian1(const VectorC &theta) const
  {
    ONDEBUG(RavlDebug("Reg %f Theta %s ",m_regularisation,RavlN::StringOf(theta).c_str()));
    VectorC grad(theta.Size());
    grad.Fill(0);

    // Unroll parameters.
    SArray1dC<MatrixC> w;
    SArray1dC<VectorC> bias;
    Unroll(theta,w,bias);

    SArray1dC<VectorC> a(m_layers.Size()+1);
    SArray1dC<VectorC> z(m_layers.Size()+1);
    SArray1dC<VectorC> eps(m_layers.Size()+1);


    // Weights
    //  Row - Outputs
    //  Col - input weight.
    if(m_threads <= 1) {
      const_cast<CostNeuralNetwork2C *>(this)->SubGrad(w,bias,0,grad);
    } else {
      //RavlDebug("Using threaded grads.");

      SArray1dC<LaunchThreadC> threads(m_threads);
      SArray1dC<VectorC> grads(m_threads);

      for(unsigned i = 1;i < m_threads;i++) {
        grads[i] = VectorC(theta.Size());
        grads[i].Fill(0);
        threads[i] = RavlN::LaunchThread(RavlN::TriggerPtr(const_cast<CostNeuralNetwork2C *>(this),&CostNeuralNetwork2C::SubGrad,w,bias,i,grads[i]));
      }
      // Do a bit of processing on this thread.
      const_cast<CostNeuralNetwork2C *>(this)->SubGrad(w,bias,0,grad);
      for(unsigned i = 1;i < m_threads;i++) {
        threads[i].WaitForExit();
        // Add in results.
        grad += grads[i];
      }
    }


    // Include regularisation term.
    if(m_regularisation > 0) {
      for(unsigned i = 1;i < grad.Size();i++) {
        grad[i] += m_regularisation * theta[i];
      }
    }

    grad /= ((RealT) m_in.Size());

    //ONDEBUG(RavlDebug("Grad @ %s = %s",RavlN::StringOf(theta).c_str(),RavlN::StringOf(grad).c_str()));
    return grad;
  }


  //: Constructor.

  DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(UIntT nLayers,
       UIntT nHidden,
       bool doNorm,
       RealT regularisation,
       RealT desiredError,
       UIntT maxEpochs,
       UIntT displayEpochs,
       bool useSigmoid,
       UIntT threads)
    : m_nLayers(nLayers),
      m_nHidden(nHidden),
      m_hiddenFraction(-1),
      m_desiredError(desiredError),
      m_maxEpochs(maxEpochs),
      m_displayEpochs(displayEpochs),
      m_regularisation(regularisation),
      m_doNormalisation(doNorm),
      m_threads(threads),
      m_useSigmoidOnOutput(useSigmoid)
  {
    if(!m_optimiser.IsValid()) {
      m_optimiser = OptimiseConjugateGradientC(m_maxEpochs, m_desiredError, true, true, 6, 1e-2);
      //m_optimiser = OptimiseDescentC(1000,1e-3);
    }
  }

  DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(const XMLFactoryContextC & factory)
   : DesignClassifierSupervisedBodyC(factory),
        m_nLayers(factory.AttributeInt("numberOfLayers", 3)),
        m_nHidden(factory.AttributeInt("numberOfHiddenUnits", 7)),
        m_hiddenFraction(-1),
        m_desiredError(factory.AttributeReal("desiredError", 0.0001)),
        m_maxEpochs(factory.AttributeInt("maxEpochs", 50000)),
        m_displayEpochs(factory.AttributeInt("displayEpochs", 100)),
        m_regularisation(factory.AttributeReal("regularisation", 0.0)),
        m_doNormalisation(factory.AttributeBool("doNormalisation", true)),
        m_threads(factory.AttributeInt("threads", 1)),
        m_useSigmoidOnOutput(factory.AttributeBool("useSigmoidOnOutput",true))
  {

    if(!factory.UseChildComponent("FeatureMap",m_featureExpand,true)) { // Optional feature expansion.
      //m_featureExpand = FuncOrthPolynomialC(2);
    }
    if(!factory.UseChildComponent("Optimiser",m_optimiser,true)) {
      m_optimiser = OptimiseConjugateGradientC(m_maxEpochs,m_desiredError,true,true,10);
      //m_optimiser = OptimiseDescentC(1000,1e-3);
    }
  }

  //: Load from stream.

  DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(std::istream &strm)
   : DesignClassifierSupervisedBodyC(strm),
     m_nLayers(0),
     m_nHidden(0),
     m_hiddenFraction(0),
     m_desiredError(0),
     m_maxEpochs(0),
     m_displayEpochs(0),
     m_regularisation(0),
     m_doNormalisation(false),
     m_threads(1),
     m_useSigmoidOnOutput(true)
  {
    int version;
    strm >> version;
    if (version < 1 || version > 2)
      throw ExceptionUnexpectedVersionInStreamC("DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_featureExpand;
    strm >> m_nLayers;
    strm >> m_nHidden;
    strm >> m_desiredError;
    strm >> m_maxEpochs;
    strm >> m_displayEpochs;
    strm >> m_regularisation;
    if(version > 1)
      strm >> m_useSigmoidOnOutput;
  }

  //: Load from binary stream.

  DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(BinIStreamC &strm)
   : DesignClassifierSupervisedBodyC(strm),
    m_nLayers(0),
    m_nHidden(0),
    m_hiddenFraction(0),
    m_desiredError(0),
    m_maxEpochs(0),
    m_displayEpochs(0),
    m_regularisation(0),
    m_doNormalisation(false),
    m_threads(1),
    m_useSigmoidOnOutput(true)
  {
    ByteT version;
    strm >> version;
    if (version != 1)
      throw ExceptionUnexpectedVersionInStreamC("DesignClassifierNeuralNetwork2BodyC::DesignClassifierNeuralNetwork2BodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_featureExpand;
    strm >> m_nLayers;
    strm >> m_nHidden;
    strm >> m_desiredError;
    strm >> m_maxEpochs;
    strm >> m_displayEpochs;
    strm >> m_regularisation;
    strm >> m_doNormalisation;
    if(version > 1)
      strm >> m_useSigmoidOnOutput;
  }

  //: Writes object to stream, can be loaded using constructor

  bool DesignClassifierNeuralNetwork2BodyC::Save(std::ostream &out) const
  {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 1;
    out << version << std::endl;
    out << m_featureExpand << std::endl;
    out << m_nLayers << std::endl;
    out << m_nHidden << std::endl;
    out << m_desiredError << std::endl;
    out << m_maxEpochs << std::endl;
    out << m_displayEpochs << std::endl;
    out << m_regularisation << std::endl;
    out << m_doNormalisation << std::endl;
    out << m_useSigmoidOnOutput << std::endl;
    return true;
  }

  //: Writes object to stream, can be loaded using constructor

  bool DesignClassifierNeuralNetwork2BodyC::Save(BinOStreamC &out) const
  {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    ByteT version = 2;
    out << version;
    out << m_featureExpand;
    out << m_nLayers;
    out << m_nHidden;
    out << m_desiredError;
    out << m_maxEpochs;
    out << m_displayEpochs;
    out << m_regularisation;
    out << m_doNormalisation;
    out << m_useSigmoidOnOutput;
    return true;
  }


  //: Get the default parameter values and their limits.
  void DesignClassifierNeuralNetwork2BodyC::ParameterLimits(
      VectorC &defaultValues,
      VectorC &min,
      VectorC &max,
      SArray1dC<StringC> &names
      ) const
  {
    defaultValues = VectorC(3);
    defaultValues[0] = 0.01;
    defaultValues[1] = 3.0;
    defaultValues[2] = 0.1;

    min = VectorC(3);
    min[0] = 0;
    min[1] = 2;
    min[2] = 0.1;

    max = VectorC(3);
    max[0] = 1e4;
    max[1] = 6;
    max[2] = 3.0;

    names = SArray1dC<StringC>(3);
    names[0] = "Regularisation";
    names[1] = "Layers";
    names[2] = "HiddenUnits";
  }

  //: Get the current parameters.
  VectorC DesignClassifierNeuralNetwork2BodyC::Parameters() const {
    VectorC vec(1);
    vec[0] = m_regularisation;
    vec[1] = m_nLayers;
    vec[2] = m_hiddenFraction;
    return vec;
  }

  //: Set the current parameters.
  // Returns the current parameters, which may not be exactly those
  // set in 'params', but will be the closest legal values.
  VectorC DesignClassifierNeuralNetwork2BodyC::SetParameters(const VectorC &params)
  {
    m_regularisation = params[0];
    if(m_regularisation < 0)
      m_regularisation = 0;
    m_nLayers = Round(params[1]);
    m_hiddenFraction = Round(params[2]);

    return params;
  }


  //: Create a classifier.

  ClassifierC DesignClassifierNeuralNetwork2BodyC::Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out)
  {
    RavlAssertMsg(in.Size() == out.Size(),
        "DesignClassifierNeuralNetwork2BodyC::Apply(), Sample of vector and labels should be the same size.");

    if(m_featureExpand.IsValid() && !m_doNormalisation) {
      RavlError("Normalisation required if using a feature expander. ");
      RavlAssert(0);
      return ClassifierC();
    }


    //size_t nInputs = in.First().Size();

    SampleLabelC labels(out);
    UIntT maxLabel = labels.MaxValue();
    size_t nOutputs = maxLabel + 1;

    RavlAssertMsg(in.Size() == out.Size(),"DesignClassifierLogisticRegressionBodyC::Apply(), Sample of vector and labels should be the same size.");
    ONDEBUG(RavlDebug("Designing logistic regression classifier."));

    SampleVectorC inVec(in);

    // Need to expand features ?
    if(m_featureExpand.IsValid()) {
      //m_featureExpand
      inVec = m_featureExpand.Apply(in);
    }

    MeanCovarianceC meanCov;
    SampleVectorC normVec;
    if(m_doNormalisation) {
      meanCov = inVec.MeanCovariance();
      normVec = SampleVectorC(in.Size());
      inVec.Normalise(meanCov,normVec,false);
    } else {
      normVec = inVec;
    }

    unsigned features = normVec.VectorSize();

    // Assemble the network.

    ONDEBUG(RavlDebug("Layers:%u ",m_nLayers));
    SArray1dC<NeuralNetworkLayerC::RefT> layers(m_nLayers -1);
    unsigned hiddenUnits = m_nHidden;
    if(m_hiddenFraction > 0) {
      hiddenUnits = Round((double) (nOutputs + features) * m_hiddenFraction);
    }
    unsigned lastLayer = features;
    for(unsigned i = 0;i < layers.Size();i++) {
      size_t outputs;
      if(i == (layers.Size() -1)) // Last layer?
        outputs = nOutputs;
      else
        outputs = hiddenUnits;
      ONDEBUG(RavlDebug("Layer %u : In=%u Out=%u",i,lastLayer,outputs));
      layers[i] = new NeuralNetworkLayerC(lastLayer,outputs);
      lastLayer = outputs;
    }

    CostNeuralNetwork2C::RefT costnn = new CostNeuralNetwork2C(layers,normVec,labels,m_regularisation,m_displayEpochs > 0,m_threads);
    CostC costFunc(costnn.BodyPtr());

#if 0
    if(!costnn->CheckJacobian(costnn->StartX())) {
      RavlError("Gradient check failed!");
    } else {
      RavlInfo("Gradient check passed!");
    }
#endif

    RealT minimumCost;
    VectorC result = m_optimiser.MinimalX (costFunc,minimumCost);

    // Sort out classifier.

    SArray1dC<MatrixC> w;
    SArray1dC<VectorC> bias;
    costnn->Unroll(result,w,bias);

    for(unsigned i = 0;i < layers.Size();i++) {
      layers[i]->SetWeights(w[i]);
      layers[i]->SetBias(bias[i]);
    }
    if(!m_useSigmoidOnOutput) {
      layers[layers.Size()-1]->SetUseSigmoid(false);
    }

    if(m_doNormalisation) {
      FunctionC normFunc = inVec.NormalisationFunction(meanCov);

      if(m_featureExpand.IsValid()) {
        normFunc = FunctionCascadeC(m_featureExpand,normFunc);
      }
      return ClassifierNeuralNetwork2C(normFunc,layers);
    }

    return ClassifierNeuralNetwork2C(layers);
  }


  //: Create a classifier with weights for the samples.

  ClassifierC DesignClassifierNeuralNetwork2BodyC::Apply(const SampleC<VectorC> &in,
      const SampleC<UIntT> &out,
      const SampleC<RealT> &weight)
  {
    RavlAssertMsg(0,
        "DesignClassifierNeuralNetwork2BodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
  //////////////////////////////////////////////////////////

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignClassifierNeuralNetwork2BodyC,
      DesignClassifierNeuralNetwork2C,
      DesignClassifierSupervisedC)
  ;

  RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierNeuralNetwork2C,
      DesignClassifierSupervisedC>
      g_registerXMLFactoryDesignClassifierNeuralNetwork2(
          "RavlN::DesignClassifierNeuralNetwork2C");

  void linkDesignClassifierNeuralNetwork2()
  {}

}
