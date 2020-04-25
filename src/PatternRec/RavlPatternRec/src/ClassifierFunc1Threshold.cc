// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierFunc1Threshold.cc"

#include "Ravl/PatternRec/ClassifierFunc1Threshold.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: Constructor.
  
  ClassifierFunc1ThresholdBodyC::ClassifierFunc1ThresholdBodyC(const Function1C &nfunc,RealT nthreshold)
    : ClassifierBodyC(2),
      func(nfunc),
      threshold(nthreshold)
  {
    if(nfunc.IsValid())
      InputSize(nfunc.InputSize());
  }
  

  //: Load from stream.
  
  ClassifierFunc1ThresholdBodyC::ClassifierFunc1ThresholdBodyC(std::istream &strm) {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> func >> threshold; 
  }
  
  //: Load from binary stream.
  
  ClassifierFunc1ThresholdBodyC::ClassifierFunc1ThresholdBodyC(BinIStreamC &strm) {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> func >> threshold; 
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierFunc1ThresholdBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << func << ' ' << threshold;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierFunc1ThresholdBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << func << threshold;
    return true;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierFunc1ThresholdBodyC::Classify(const VectorC &data) const {
    RealT x = func.Apply1(data);
    return (x > threshold) ? 1 : 0;
  }
  
  //: Estimate the confidence for each label.
  
  VectorC ClassifierFunc1ThresholdBodyC::Confidence(const VectorC &data) const {
    VectorC ret(2);
    ret.Fill(0);
    RealT x = func.Apply1(data);
    ret[1] = x - threshold;
    ret[0] = threshold - x;
    return ret;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierFunc1ThresholdBodyC,ClassifierFunc1ThresholdC,ClassifierC);

}
