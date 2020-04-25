// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNCLASSIFIERNEURALNETWORK_HEADER
#define RAVL_DESIGNCLASSIFIERNEURALNETWORK_HEADER 1
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignClassifierGaussianMixture.hh"

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Design a neural network classifier .

  class DesignClassifierNeuralNetworkBodyC
     : public DesignClassifierSupervisedBodyC
  {
    public:
      DesignClassifierNeuralNetworkBodyC(UIntT nLayers, UIntT nInputs, UIntT nHidden, UIntT nOutputs, RealT desiredError,
        UIntT maxEpochs, UIntT displayEpochs);
    //: Constructor.

      DesignClassifierNeuralNetworkBodyC(const XMLFactoryContextC & factory);
      //: factory constructor

      DesignClassifierNeuralNetworkBodyC(std::istream &strm);
      //: Load from stream.

      DesignClassifierNeuralNetworkBodyC(BinIStreamC &strm);
      //: Load from binary stream.

      virtual bool Save(std::ostream &out) const;
      //: Writes object to stream, can be loaded using constructor

      virtual bool Save(BinOStreamC &out) const;
      //: Writes object to stream, can be loaded using constructor

      virtual ClassifierC Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out);
      //: Train a neural network.  The number of input dimensions must be the same as specified in the
      //: constructor.  The number of different labels must be the same as the number of outputs.  Also
      //: the output labels must run from 0 to nOutputs-1.

      virtual ClassifierC Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out, const SampleC<RealT> &weight);
      //: Create a classifier with weights for the samples.

      using DesignClassifierSupervisedBodyC::Apply;

      virtual FunctionC Apply(const SampleC<VectorC>&in, const SampleC<VectorC>&out);

    protected:
      ClassifierC Train(const FilenameC & datafile);

      UIntT m_nLayers;
      UIntT m_nInputs;
      UIntT m_nHidden;
      UIntT m_nOutputs;
      RealT m_desiredError;
      UIntT m_maxEpochs;
      UIntT m_displayEpochs;
      bool m_cascade; //!< perform cascade training, look at FANN documentation
  };

  //! userlevel=Normal
  //: Design a neural network classifier

  class DesignClassifierNeuralNetworkC: public DesignClassifierSupervisedC {
    public:
      DesignClassifierNeuralNetworkC() {
      }
      //: Default constructor.
      // Creates an invalid handle.

      DesignClassifierNeuralNetworkC(UIntT nLayers, UIntT nInputs, UIntT nHidden, UIntT nOutputs, RealT desiredError =
        0.00001, UIntT maxEpochs = 10000, UIntT displayEpochs=100) :
        DesignClassifierSupervisedC(
            *new DesignClassifierNeuralNetworkBodyC(nLayers, nInputs, nHidden, nOutputs, desiredError, maxEpochs,
                displayEpochs)) {
      }
    //: Create a new designer.

      DesignClassifierNeuralNetworkC(const XMLFactoryContextC &factory)
         :  DesignClassifierSupervisedC(*new DesignClassifierNeuralNetworkBodyC(factory))
       {}
       //: Construct from XML factory

      DesignClassifierNeuralNetworkC(std::istream &strm);
      //: Load from stream.

      DesignClassifierNeuralNetworkC(BinIStreamC &strm);
      //: Load from binary stream.

    protected:
      DesignClassifierNeuralNetworkC(DesignClassifierNeuralNetworkBodyC &bod)
          : DesignClassifierSupervisedC(bod) {
      }
      //: Body constructor.

      DesignClassifierNeuralNetworkC(DesignClassifierNeuralNetworkBodyC *bod)
          : DesignClassifierSupervisedC(bod) {
      }
      //: Body ptr constructor.

      DesignClassifierNeuralNetworkBodyC &Body() {
        return static_cast<DesignClassifierNeuralNetworkBodyC &>(DesignClassifierSupervisedC::Body());
      }
      //: Access body.

      const DesignClassifierNeuralNetworkBodyC &Body() const {
        return static_cast<const DesignClassifierNeuralNetworkBodyC &>(DesignClassifierSupervisedC::Body());
      }
      //: Access body.

    public:
      FunctionC Apply(const SampleC<VectorC> &in,const SampleC<VectorC> &out) {
        return Body().Apply(in, out);
      }
      //: Create function from the given data.

  };

  inline std::istream &operator>>(std::istream &strm, DesignClassifierNeuralNetworkC &obj) {
    obj = DesignClassifierNeuralNetworkC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out, const DesignClassifierNeuralNetworkC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm, DesignClassifierNeuralNetworkC &obj) {
    obj = DesignClassifierNeuralNetworkC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out, const DesignClassifierNeuralNetworkC &obj) {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
