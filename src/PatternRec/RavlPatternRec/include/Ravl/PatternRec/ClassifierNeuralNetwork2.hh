// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ClassifierNeuralNetwork22_HEADER
#define RAVL_ClassifierNeuralNetwork22_HEADER 1
///////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierNeuralNetwork2.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/DList.hh"
#include "Ravl/String.hh"

namespace RavlN {

  //! Layer in neural network.
  class NeuralNetworkLayerC
   : public RCBodyC
  {
  public:
    //: Constructor
    NeuralNetworkLayerC();

    //: Constructor
    NeuralNetworkLayerC(size_t numInputs,size_t numOutputs,bool useSigmoid = true);

    //: Constructor
    NeuralNetworkLayerC(const MatrixC &weights,const VectorC &bias,bool useSigmoid = true);

    //: Constructor from binary stream
    NeuralNetworkLayerC(BinIStreamC &strm);

    //: Constructor from text stream
    NeuralNetworkLayerC(std::istream &strm);

    //! Have to binary stream
    bool Save(BinOStreamC &strm) const;

    //! Have to text stream
    bool Save(std::ostream &strm) const;

    //: Weights for layer
    const MatrixC &Weights() const
    { return m_weights; }

    //: Set weights
    void SetWeights(const MatrixC &weights)
    { m_weights = weights; }

    //: Get bias
    const VectorC &Bias() const
    { return m_bias; }

    //: Set bias vector
    void SetBias(const VectorC &bias)
    { m_bias = bias; }

    //: Get number of outputs.
    size_t NumOutputs() const
    { return m_bias.Size(); }

    //: Get number of inputs expected to layer
    size_t NumInputs() const
    { return m_weights.Cols().V(); }

    //: Use sigmoid on output.
    bool UseSigmoid() const
    { return m_useSigmoid; }

    //: Set use sigmoid flag.
    void SetUseSigmoid(bool useSigmoid)
    { m_useSigmoid = useSigmoid; }

    //: Handle
    typedef SmartPtrC<NeuralNetworkLayerC> RefT;
  protected:
    MatrixC m_weights;
    VectorC m_bias;
    bool m_useSigmoid;
  };

  //! userlevel=Develop
  //: A neural network classifier, natively implemented in RAVL.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierNeuralNetwork2C.html"> DesignClassifierNeuralNetwork2C </a> class.

  class ClassifierNeuralNetwork2BodyC
    : public ClassifierBodyC
  {
  public:
    ClassifierNeuralNetwork2BodyC(const FunctionC &norm,
                                  const SArray1dC<NeuralNetworkLayerC::RefT> &layers);
    //: Create classifier

    ClassifierNeuralNetwork2BodyC(const SArray1dC<NeuralNetworkLayerC::RefT> &layers);
    //: Create classifier

    ClassifierNeuralNetwork2BodyC(std::istream &strm);
    //: Load from stream.

    ClassifierNeuralNetwork2BodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.

    virtual VectorC Apply(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the function used. The higher the confidence the more likely
    // it is the label is correct. The sum of the vector has been normalised
    // to 1.

  protected:
    FunctionC m_norm;
    SArray1dC<NeuralNetworkLayerC::RefT> m_layers;
  };

  //! userlevel=Normal
  //: A neural network classifier, natively implemented in RAVL.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierNeuralNetwork2C.html"> DesignClassifierNeuralNetwork2C </a> class.

  class ClassifierNeuralNetwork2C
    : public ClassifierC
  {
  public:
    ClassifierNeuralNetwork2C()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    ClassifierNeuralNetwork2C(const FunctionC &norm,const SArray1dC<NeuralNetworkLayerC::RefT> &layers)
      : ClassifierC(*new ClassifierNeuralNetwork2BodyC(norm,layers))
    {}
    //: Create classifier from a set of layers

    ClassifierNeuralNetwork2C(const SArray1dC<NeuralNetworkLayerC::RefT> &layers)
      : ClassifierC(*new ClassifierNeuralNetwork2BodyC(layers))
    {}
    //: Create classifier from a set of layers

    ClassifierNeuralNetwork2C(std::istream &strm);
    //: Load from stream.

    ClassifierNeuralNetwork2C(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    ClassifierNeuralNetwork2C(ClassifierNeuralNetwork2BodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierNeuralNetwork2C(ClassifierNeuralNetwork2BodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierNeuralNetwork2BodyC &Body()
    { return static_cast<ClassifierNeuralNetwork2BodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierNeuralNetwork2BodyC &Body() const
    { return static_cast<const ClassifierNeuralNetwork2BodyC &>(ClassifierC::Body()); }
    //: Access body.

  public:

  };

  inline std::istream &operator>>(std::istream &strm,ClassifierNeuralNetwork2C &obj) {
    obj = ClassifierNeuralNetwork2C(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out,const ClassifierNeuralNetwork2C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierNeuralNetwork2C &obj) {
    obj = ClassifierNeuralNetwork2C(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierNeuralNetwork2C &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
