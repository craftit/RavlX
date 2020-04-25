// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: ClassifierNeuralNetwork.cc,v 1.4 2004/03/18 17:14:36 ees1wc Exp $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierNeuralNetwork.cc"

#include "Ravl/PatternRec/ClassifierNeuralNetwork.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  
  //: Create classifier from function.
  
  ClassifierNeuralNetworkBodyC::ClassifierNeuralNetworkBodyC(UIntT nLayers, UIntT nInputs, UIntT nHidden, UIntT nOutputs, const DListC<StringC> & nn)
    : ClassifierBodyC(nOutputs), m_nLayers(nLayers), m_nInputs(nInputs), m_nHidden(nHidden), m_nOutputs(nOutputs), m_data(nn)
  {
    init();
  }
  
  //: Load from stream.
  
  ClassifierNeuralNetworkBodyC::ClassifierNeuralNetworkBodyC(std::istream &strm)
    : ClassifierBodyC(strm)
  { 
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierNeuralNetworkBodyC::ClassifierNeuralNetworkBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_nLayers;
    strm >> m_nInputs;
    strm >> m_nHidden;
    strm >> m_nOutputs;
    strm >> m_data;
    init();
  }
  
  //: Load from binary stream.
  
  ClassifierNeuralNetworkBodyC::ClassifierNeuralNetworkBodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierNeuralNetworkBodyC::ClassifierNeuralNetworkBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_nLayers;
    strm >> m_nInputs;
    strm >> m_nHidden;
    strm >> m_nOutputs;
    strm >> m_data;
    init();
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierNeuralNetworkBodyC::Save(std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << std::endl;
    out << m_nLayers << std::endl;
    out << m_nInputs << std::endl;
    out << m_nHidden << std::endl;
    out << m_nOutputs << std::endl;
    out << m_data << std::endl;
    return true;
  }
  

  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierNeuralNetworkBodyC::Save(BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version;
    out << m_nLayers;
    out << m_nInputs;
    out << m_nHidden;
    out << m_nOutputs;
    out << m_data;
    return true;    
  }
  
  //: Classifier vector 'data' return the most likely label.
  UIntT ClassifierNeuralNetworkBodyC::Classify(const VectorC &data) const {
    return Apply(data).MaxIndex().V();
  }
  
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  VectorC ClassifierNeuralNetworkBodyC::Apply(const VectorC &data) const {
     //: Convert input vector to one that fann can use
    SArray1dC<fann_type> inp(data.Size());
    for(UIntT i=0;i<data.Size();i++)
      inp[i] = static_cast<fann_type>(data[i]);

    //: Run it through the network
    m_output = fann_run(m_ann, &(inp[0]));
 
    VectorC out(m_nOutputs);
    for(UIntT i=0;i<m_nOutputs;i++) {
      out[i] = m_output[i];
    }

    return out;    
  }
  
  bool ClassifierNeuralNetworkBodyC::init() {
    FilenameC fn = "/tmp/nn";
    fn = fn.MkTemp();
    {    
      OStreamC os(fn);
      for(DLIterC<StringC>it(m_data);it;it++) {
	StringC line = it.Data().Copy();
	line.gsub("#", " ");
	os << line << std::endl;
      }
    }   
    m_ann = fann_create_from_file(fn.chars());  
    fn.Remove();
    return true;
  }

  

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierNeuralNetworkBodyC,ClassifierNeuralNetworkC,ClassifierC);
  
  
  void InitRavlClassifierNeuralNetwork() {
  }

}
