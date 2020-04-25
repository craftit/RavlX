// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierNeuralNetwork2.cc"

#include "Ravl/PatternRec/ClassifierNeuralNetwork2.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Exception.hh"

namespace RavlN {

  //: Constructor
  NeuralNetworkLayerC::NeuralNetworkLayerC()
   : m_useSigmoid(true)
  {}

  //: Constructor
  NeuralNetworkLayerC::NeuralNetworkLayerC(const MatrixC &weights,const VectorC &bias,bool useSigmoid)
   : m_weights(weights),
     m_bias(bias),
     m_useSigmoid(useSigmoid)
  {}

  //: Constructor
  NeuralNetworkLayerC::NeuralNetworkLayerC(size_t numInputs,size_t numOutputs,bool useSigmoid)
   : m_weights(numOutputs,numInputs),
     m_bias(numOutputs),
     m_useSigmoid(useSigmoid)
  {
#if 0
    // Creates a layer initialised with random values in the range from -1 to +1
    for(unsigned i = 0;i < numOutputs;i++) {
      for(unsigned j = 0;j < numInputs;j++) {
        m_weights[i][j] = Random1() * 2.0 - 1.0;
      }
      m_bias[i] = Random1() * 2.0 - 1.0;
    }
#endif
  }


  //: Constructor from binary stream
  NeuralNetworkLayerC::NeuralNetworkLayerC(BinIStreamC &strm)
   : m_useSigmoid(true)
  {
    ByteT version = 0;
    strm >> version;
    if(version < 1 || version > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("NeuralNetworkLayerC");
    strm >> m_weights >> m_bias;
    if(version > 1)
      strm >> m_useSigmoid;
  }

  //: Constructor from text stream
  NeuralNetworkLayerC::NeuralNetworkLayerC(std::istream &strm)
   : m_useSigmoid(true)
  {
    IntT version = 0;
    strm >> version;
    if(version < 1 || version > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("NeuralNetworkLayerC");
    strm >> m_weights >> m_bias;
    if(version > 1)
      strm >> m_useSigmoid;
  }

  //! Have to binary stream
  bool NeuralNetworkLayerC::Save(BinOStreamC &strm) const
  {
    //RCBodyC::Save(strm);
    ByteT version = 2;
    strm << version << m_weights << m_bias << m_useSigmoid;
    return true;
  }

  //! Have to text stream
  bool NeuralNetworkLayerC::Save(std::ostream &strm) const
  {
    //RCBodyC::Save(strm);
    IntT version = 2;
    strm << version << " " << m_weights << " " << m_bias << " " << m_useSigmoid;
    return true;
  }


  // ----------------------------------------------------------------------

  //: Create classifier from function.

  ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(const FunctionC &norm,
                                                               const SArray1dC<NeuralNetworkLayerC::RefT> &layers)
    : ClassifierBodyC(layers[layers.Size()-1]->NumOutputs()),
      m_norm(norm),
      m_layers(layers)
  {}

  ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(const SArray1dC<NeuralNetworkLayerC::RefT> &layers)
    : ClassifierBodyC(layers[layers.Size()-1]->NumOutputs()),
      m_layers(layers)
  {}

  //: Load from stream.

  ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(std::istream &strm)
    : ClassifierBodyC(strm)
  {
    ByteT version;
    strm >> version;
    if(version != 1)
      throw ExceptionUnexpectedVersionInStreamC("ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(std::istream &), Unrecognised version number in stream. ");

    strm >> m_norm;
    strm >> m_layers;
  }

  //: Load from binary stream.

  ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm)
  {
    ByteT version;
    strm >> version;
    if(version != 1)
      throw ExceptionUnexpectedVersionInStreamC("ClassifierNeuralNetwork2BodyC::ClassifierNeuralNetwork2BodyC(BinIStreamC &), Unrecognised version number in stream. ");
    bool haveNorm = false;
    strm >> haveNorm;
    if(haveNorm)
      strm >> m_norm;
    strm >> m_layers;
  }

  //: Writes object to stream, can be loaded using constructor

  bool ClassifierNeuralNetwork2BodyC::Save(std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 1;
    out << version << " " << m_norm << " " << m_layers << std::endl;
    return true;
  }


  //: Writes object to stream, can be loaded using constructor

  bool ClassifierNeuralNetwork2BodyC::Save(BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    ByteT version = 1;
    out << version;
    out << m_norm.IsValid();
    if(m_norm.IsValid())
      out << m_norm;
    out << m_layers;
    return true;
  }

  //: Classifier vector 'data' return the most likely label.
  UIntT ClassifierNeuralNetwork2BodyC::Classify(const VectorC &data) const {
    return Apply(data).MaxIndex().V();
  }

  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.

  VectorC ClassifierNeuralNetwork2BodyC::Apply(const VectorC &data) const {
    VectorC work;
    if(m_norm.IsValid())
      work = m_norm.Apply(data);
    else
      work = data;
    VectorC res;
    for(unsigned i = 0;i < m_layers.Size();i++) {
      //res = m_layers[i]->Weights() * work + m_layers[i]->Bias();
      RavlN::MulAdd(m_layers[i]->Weights(),work,m_layers[i]->Bias(),res);
      // FIXME:- This could be faster and avoid lots of allocations.
      if(m_layers[i]->UseSigmoid())
        work = Sigmoid(res);
      else
        work = res;
    }
    return work;
  }


  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierNeuralNetwork2BodyC,ClassifierNeuralNetwork2C,ClassifierC);


  void InitRavlClassifierNeuralNetwork2() {
  }

}
