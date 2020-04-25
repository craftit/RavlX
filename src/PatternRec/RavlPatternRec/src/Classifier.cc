// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/Classifier.cc"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PatternRec/SampleIter.hh"

namespace RavlN {

  //: Default constructor.
  
  ClassifierBodyC::ClassifierBodyC(UIntT maxLabels)
    : FunctionBodyC(0,maxLabels)
  {}
  
  //: Load from stream.
  
  ClassifierBodyC::ClassifierBodyC(istream &strm) 
    : FunctionBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  ClassifierBodyC::ClassifierBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierBodyC::Save (ostream &out) const {
    return FunctionBodyC::Save(out);
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierBodyC::Save (BinOStreamC &out) const {
    return FunctionBodyC::Save(out);
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierBodyC::Classify(const VectorC &data) const {
    RavlAssertMsg(0,"ClassifierBodyC::Classifier(), Abstract method called. ");
    return 0;
  }

  //: Classify each vector in the set

  SampleC<UIntT> ClassifierBodyC::Classify(const SampleC<VectorC> &data) const {
    SampleC<UIntT> ret(data.Size());
    for(SampleIterC<VectorC> it(data);it;it++) {
      ret.Append(Classify(*it));
    }
    return ret;
  }
  
  //: Generate a probability for each label.
  // Note: Not all classifiers return a true probability vector.
  
  VectorC ClassifierBodyC::Apply(const VectorC &data) const {
    VectorC ret(NoLabels());
    ret.Fill(0);
    UIntT res = Classify(data);
    RavlAssert(res < NoLabels());
    ret[res] = 1;
    return ret;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierBodyC::Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const {
    VectorC reduced(featureSet.Size());
    for(SArray1dIter2C<IndexC,RealT> itf(featureSet,reduced); itf; itf++)
      itf.Data2() = data[itf.Data1()];
    return Classify(reduced);
  }
  
  //: Generate a probability for each label.
  // Note: Not all classifiers return a true probablility vector.
  
  VectorC ClassifierBodyC::Apply(const VectorC &data,const SArray1dC<IndexC> &featureSet) const {
    VectorC reduced(featureSet.Size());
    for(SArray1dIter2C<IndexC,RealT> itf(featureSet,reduced); itf; itf++)
      itf.Data2() = data[itf.Data1()];
    return Apply(reduced);
  }
  
  RealT ClassifierBodyC::LabelProbability(const VectorC & data, UIntT label) const {
    RavlAssertMsg(label<NumLabels(), "label bigger than number of classes");
    VectorC conf = Apply(data);
    return conf[label]/conf.Sum();
  }
     //: Compute the 'posterior' probability of a given class

  RealT ClassifierBodyC::LabelProbability(const VectorC & data, const SArray1dC<IndexC> &featureSet, UIntT label) const {
    RavlAssertMsg(label<NumLabels(), "label bigger than number of classes");
    VectorC conf = Apply(data, featureSet);
    return conf[label]/conf.Sum();
  }
  //: Compute the 'posterior' probability of a given class

  
  ///////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  ClassifierC::ClassifierC(istream &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,ClassifierBodyC))    
  {}
  
  //: Load from binary stream.
  
  ClassifierC::ClassifierC(BinIStreamC &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,ClassifierBodyC))
  {}

}
