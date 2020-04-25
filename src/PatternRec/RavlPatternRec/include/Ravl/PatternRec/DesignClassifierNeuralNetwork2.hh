// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003 University of Surrey, 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DesignClassifierNeuralNetwork22_HEADER
#define RAVL_DesignClassifierNeuralNetwork22_HEADER 1
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignClassifierGaussianMixture.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/PatternRec/Optimise.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Design a neural network classifier .

  class DesignClassifierNeuralNetwork2BodyC
    : public DesignClassifierSupervisedBodyC
  {
  public:
    DesignClassifierNeuralNetwork2BodyC(UIntT nLayers,
                                        UIntT nHidden,
                                        bool doNorm = true,
                                        RealT regularisation = 0,
                                        RealT desiredError = 0.00001,
                                        UIntT maxEpochs = 5000,
                                        UIntT displayEpochs = 1,
                                        bool useSigmoid = true,
                                        UIntT threads = 1);
    //: Constructor.

    DesignClassifierNeuralNetwork2BodyC(const XMLFactoryContextC & factory);
    //: factory constructor

    DesignClassifierNeuralNetwork2BodyC(std::istream &strm);
    //: Load from stream.

    DesignClassifierNeuralNetwork2BodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual void ParameterLimits(VectorC &defaultValues,VectorC &min,VectorC &max,SArray1dC<StringC> &names) const;
    //: Get the default parameter values and their limits.

    virtual VectorC Parameters() const;
    //: Get the current parameters.

    virtual VectorC SetParameters(const VectorC &params);
    //: Set the current parameters.
    // Returns the current parameters, which may not be exactly those
    // set in 'params', but will be the closest legal values.

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out);
    //: Train a neural network.  The number of input dimensions must be the same as specified in the
    //: constructor.  The number of different labels must be the same as the number of outputs.  Also
    //: the output labels must run from 0 to nOutputs-1.

    virtual ClassifierC Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out, const SampleC<RealT> &weight);
    //: Create a classifier with weights for the samples.

  protected:
    OptimiseC m_optimiser;
    FunctionC m_featureExpand;
    UIntT m_nLayers;
    UIntT m_nHidden;
    RealT m_hiddenFraction; // Hidden units as a fraction of number of inputs + outputs.  -1 = Use default
    RealT m_desiredError;
    UIntT m_maxEpochs;
    UIntT m_displayEpochs;
    RealT m_regularisation;
    bool m_doNormalisation; //!< No normalisation.
    int m_threads;
    bool m_useSigmoidOnOutput;

  };

  //! userlevel=Normal
  //: Design a neural network classifier

  class DesignClassifierNeuralNetwork2C: public DesignClassifierSupervisedC
  {
  public:
    DesignClassifierNeuralNetwork2C()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.

    DesignClassifierNeuralNetwork2C(UIntT nLayers,
        UIntT nHidden,
        bool doNormalisation = true,
        RealT regularisation = 0,
        RealT desiredError = 0.00001,
        UIntT maxEpochs = 5000,
        UIntT displayEpochs = 0,
        bool useSigmoid = true,
        UIntT threads = 1)
    : DesignClassifierSupervisedC(
              *new DesignClassifierNeuralNetwork2BodyC(nLayers, nHidden, doNormalisation,regularisation,desiredError, maxEpochs, displayEpochs,useSigmoid,threads))
    {}
    //: Create a new designer.

    DesignClassifierNeuralNetwork2C(const XMLFactoryContextC &factory)
    : DesignClassifierSupervisedC(*new DesignClassifierNeuralNetwork2BodyC(factory))
    {}
    //: Construct from XML factory

    DesignClassifierNeuralNetwork2C(std::istream &strm);
    //: Load from stream.

    DesignClassifierNeuralNetwork2C(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignClassifierNeuralNetwork2C(DesignClassifierNeuralNetwork2BodyC &bod) :
      DesignClassifierSupervisedC(bod)
    {}
    //: Body constructor.

    DesignClassifierNeuralNetwork2C(DesignClassifierNeuralNetwork2BodyC *bod) :
      DesignClassifierSupervisedC(bod)
    {}
    //: Body ptr constructor.

    DesignClassifierNeuralNetwork2BodyC &Body()
    { return static_cast<DesignClassifierNeuralNetwork2BodyC &> (DesignClassifierSupervisedC::Body()); }
    //: Access body.

    const DesignClassifierNeuralNetwork2BodyC &Body() const
    { return static_cast<const DesignClassifierNeuralNetwork2BodyC &> (DesignClassifierSupervisedC::Body()); }
    //: Access body.

  public:

  };

  inline std::istream &operator>>(std::istream &strm, DesignClassifierNeuralNetwork2C &obj)
  {
    obj = DesignClassifierNeuralNetwork2C(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out,
      const DesignClassifierNeuralNetwork2C &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm,
      DesignClassifierNeuralNetwork2C &obj)
  {
    obj = DesignClassifierNeuralNetwork2C(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out,
      const DesignClassifierNeuralNetwork2C &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
