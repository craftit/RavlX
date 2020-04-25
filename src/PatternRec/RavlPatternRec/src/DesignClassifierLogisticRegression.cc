// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignClassifierLogisticRegression.cc"

#include "Ravl/PatternRec/DesignClassifierLogisticRegression.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/PatternRec/Cost.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/OptimiseConjugateGradient.hh"
#include "Ravl/PatternRec/OptimiseDescent.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/FunctionCascade.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/PatternRec/ClassifierLogisticRegression.hh"
#include "Ravl/PatternRec/CostLogisticRegression.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#include "Ravl/StrStream.hh"
#include "Ravl/OS/Date.hh"
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Copy constructor
  DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(const DesignClassifierLogisticRegressionBodyC &other)
   : m_featureExpand(other.m_featureExpand),
     m_optimiser(other.m_optimiser.Copy()),
     m_regularisation(other.m_regularisation),
     m_prependUnit(other.m_prependUnit),
     m_doNormalisation(other.m_doNormalisation),
     m_verbose(other.m_verbose)
  {}

  //: Constructor.
  
  DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(RealT regularisation,
                                                                                   const FunctionC &featureExpand,
                                                                                   bool prependUnit,
                                                                                   const OptimiseC &optimiser,
                                                                                   bool verbose
                                                                                   )
   : m_featureExpand(featureExpand),
     m_optimiser(optimiser),
     m_regularisation(regularisation),
     m_prependUnit(prependUnit),
     m_doNormalisation(true),
     m_verbose(verbose)
  {
    if(!m_optimiser.IsValid()) {
      m_optimiser = OptimiseConjugateGradientC(1000);
      //m_optimiser = OptimiseDescentC(1000,1e-3);
    }
  }
  
  //: Load from XML factory
  
  DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(const XMLFactoryContextC & factory)
    : DesignClassifierSupervisedBodyC(factory),
      m_regularisation(factory.AttributeReal("regularisation",0)),
      m_prependUnit(factory.AttributeBool("prependUnit",true)),
      m_doNormalisation(factory.AttributeBool("doNormalisation", true)),
      m_verbose(factory.AttributeBool("verbose",true))
  {
    if(!factory.UseChildComponent("FeatureMap",m_featureExpand,true)) { // Optional feature expansion.
      //m_featureExpand = FuncOrthPolynomialC(2);
    }
    if(!factory.UseChildComponent("Optimiser",m_optimiser,true)) {
      m_optimiser = OptimiseConjugateGradientC(1000);
      //m_optimiser = OptimiseDescentC(1000,1e-3);
    }
  }
  
  //: Load from stream.
  
  DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm),
      m_regularisation(0),
      m_prependUnit(false),
      m_doNormalisation(false),
      m_verbose(true)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(std::istream &), Unrecognised version number in stream. ");
    RavlAssertMsg(0,"not supported");
  }
  
  //: Load from binary stream.
  
  DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm),
      m_regularisation(0),
      m_prependUnit(false),
      m_doNormalisation(false),
      m_verbose(true)
  {
    int version;
    strm >> version;
    if(version < 1 || version > 2)
      throw ExceptionOutOfRangeC("DesignClassifierLogisticRegressionBodyC::DesignClassifierLogisticRegressionBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_featureExpand >> m_optimiser >> m_regularisation >> m_prependUnit >> m_doNormalisation;
    if(version > 1)
      strm >> m_verbose;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierLogisticRegressionBodyC::Save(std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    RavlAssertMsg(0,"not supported");
    int version = 0;
    out << ' ' << version;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClassifierLogisticRegressionBodyC::Save(BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    ByteT version = 2;
    out << version << m_featureExpand << m_optimiser << m_regularisation << m_prependUnit << m_doNormalisation << m_verbose;
    return true;
  }

  RCBodyVC &DesignClassifierLogisticRegressionBodyC::Copy() const
  {
    return *new DesignClassifierLogisticRegressionBodyC(*this);
  }
  //: Make copy of body.

  //: Get the default parameter values and their limits.
  void DesignClassifierLogisticRegressionBodyC::ParameterLimits(
      VectorC &defaultValues,
      VectorC &min,
      VectorC &max,
      SArray1dC<StringC> &names
      ) const
  {
    defaultValues = VectorC(1);
    defaultValues[0] = 0.01;

    min = VectorC(1);
    min[0] = 0;

    max = VectorC(1);
    max[0] = 1e5;

    names = SArray1dC<StringC>(1);
    names[0] = "Regularisation";
  }

  //: Get the current parameters.
  VectorC DesignClassifierLogisticRegressionBodyC::Parameters() const {
    VectorC vec(1);
    vec[0] = m_regularisation;
    return vec;
  }

  //: Set the current parameters.
  // Returns the current parameters, which may not be exactly those
  // set in 'params', but will be the closest legal values.
  VectorC DesignClassifierLogisticRegressionBodyC::SetParameters(const VectorC &params)
  {
    m_regularisation = params[0];
    if(m_regularisation < 0)
      m_regularisation = 0;
    return params;
  }

  //: Create a classifier.
  
  ClassifierC DesignClassifierLogisticRegressionBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out)
  {
    RavlAssertMsg(in.Size() == out.Size(),"DesignClassifierLogisticRegressionBodyC::Apply(), Sample of vector and labels should be the same size.");
    ONDEBUG(RavlDebug("Designing logistic regression classifier."));

    if(m_featureExpand.IsValid() && !m_doNormalisation) {
      RavlError("Normalisation required if using a feature expander. ");
      RavlAssert(0);
      return ClassifierC();
    }

    SampleVectorC inVec(in);

    // Need to expand features ?
    if(m_featureExpand.IsValid()) {
      //m_featureExpand
      inVec = m_featureExpand.Apply(in);
    }

    MeanCovarianceC meanCov;
    SampleVectorC normVec;
    SampleLabelC labels(out);

    if(m_doNormalisation) {
      meanCov = inVec.MeanCovariance();
      normVec = SampleVectorC(in.Size());
      inVec.Normalise(meanCov,normVec,true);
    } else {
      normVec = in;
    }

    unsigned features = normVec.VectorSize();

    VectorC theta(features);
    theta.Fill(0);


    unsigned maxLabel = labels.MaxValue() ;
    ONDEBUG(RavlDebug("Features:%u Labels:%u",features,maxLabel));

    MatrixC weights(maxLabel+1,features);
    // If its a two class problem we only need 1 classifier.
    if(maxLabel == 1) maxLabel = 0;
    for(unsigned i = 0;i <= maxLabel;i++) {
      ONDEBUG(RavlDebug("Processing column %i ",i));
      CostC costFunc(new CostLogisticRegressionC(i,features,normVec,labels,m_regularisation,m_verbose));
      ONDEBUG(RavlDebug("Start at:%s",RavlN::StringOf(costFunc.StartX()).c_str()));

#if 0
    if(!costFunc.CheckJacobian(costFunc.StartX())) {
      RavlError("Gradient check failed!");
    } else {
      RavlInfo("Gradient check passed!");
    }
#endif

      RealT minimumCost;
      VectorC result = m_optimiser.MinimalX (costFunc,minimumCost);
      weights.SetRow(i,result);
    }

    if(m_doNormalisation) {
      FunctionC normFunc = inVec.NormalisationFunction(meanCov);

      if(m_featureExpand.IsValid()) {
        normFunc = FunctionCascadeC(m_featureExpand,normFunc);
      }
      return ClassifierLogisticRegressionC(normFunc,weights,true);
    }

    return ClassifierLogisticRegressionC(weights,true);
  }
  
  //: Create a classifier with weights for the samples.
  
  ClassifierC DesignClassifierLogisticRegressionBodyC::Apply(const SampleC<VectorC> &in,
						  const SampleC<UIntT> &out,
						  const SampleC<RealT> &weight)
  {
    RavlAssertMsg(0,"DesignClassifierLogisticRegressionBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
 
  //////////////////////////////////////////////////////////
  
  //static RavlN::TypeNameC g_type(typeid(RavlN::DesignClassifierLogisticRegressionC),"RavlN::DesignClassifierLogisticRegressionC");
  static RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierLogisticRegressionC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignClassifierLogisticRegression("RavlN::DesignClassifierLogisticRegressionC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignClassifierLogisticRegressionBodyC,DesignClassifierLogisticRegressionC,DesignClassifierSupervisedC);

  void linkDesignClassifierLogisticRegression()
  {}

}
