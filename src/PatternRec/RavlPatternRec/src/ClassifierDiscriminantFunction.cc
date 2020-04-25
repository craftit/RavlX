// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierDiscriminantFunction.cc"

#include "Ravl/PatternRec/ClassifierDiscriminantFunction.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Create classifier from function.
  
  ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(const FunctionC &nfunc)
    : ClassifierBodyC(nfunc.OutputSize()),
      func(nfunc)
  {}
  
  //: Load from stream.
  
  ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(std::istream &strm)
    : ClassifierBodyC(strm)
  { 
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(std::istream &), Unrecognised version number in stream. ");

    strm >> func; 
  }
  
  //: Load from binary stream.
  
  ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(BinIStreamC &strm)
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierDiscriminantFunctionBodyC::ClassifierDiscriminantFunctionBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> func; 
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierDiscriminantFunctionBodyC::Save(std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << func;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierDiscriminantFunctionBodyC::Save(BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << func;
    return true;    
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierDiscriminantFunctionBodyC::Classify(const VectorC &data) const {
    VectorC vec = func(data);
    return vec.MaxIndex().V();
  }
  
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  VectorC ClassifierDiscriminantFunctionBodyC::Apply(const VectorC &data) const {
    VectorC vec = func(data);
    return vec.MakeUnit();
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierDiscriminantFunctionBodyC,ClassifierDiscriminantFunctionC,ClassifierC);
  
  
}
