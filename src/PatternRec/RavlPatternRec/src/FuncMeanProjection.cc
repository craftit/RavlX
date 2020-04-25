// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/Basic/FuncMeanProjection.cc"

#include "Ravl/PatternRec/FuncMeanProjection.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#pragma instantiate RavlN::FuncMeanProjectionBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::FuncMeanProjectionBodyC*)
#pragma instantiate RavlN::FuncMeanProjectionBodyC* RavlN::VCLoad(std::istream &,RavlN::FuncMeanProjectionBodyC*)
#endif 

namespace RavlN {

  //: Default constructor.
  
  FuncMeanProjectionBodyC::FuncMeanProjectionBodyC()
  {}
  
  //: Default constructor.
  
  FuncMeanProjectionBodyC::FuncMeanProjectionBodyC(const VectorC &nMean,const MatrixC &nProj)
    : mean(nMean),
      proj(nProj)
  { 
    RavlAssert(mean.Size() == proj.Cols());
    InputSize(mean.Size());
    OutputSize(proj.Rows());
  }
  
  //: Load from stream.
  
  FuncMeanProjectionBodyC::FuncMeanProjectionBodyC(std::istream &strm) 
    : FunctionBodyC(strm)
  { strm >> mean >> proj; }
  
  //: Load from binary stream.
  
  FuncMeanProjectionBodyC::FuncMeanProjectionBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  { strm >> mean >> proj; }
  
  //: Writes object to stream.
  
  bool FuncMeanProjectionBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << mean << ' ' << proj;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool FuncMeanProjectionBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << mean << proj;
    return true;
  }
  
  //: Apply function to 'data'
  
  VectorC FuncMeanProjectionBodyC::Apply(const VectorC &data) const {
    return proj * (data - mean);
  }
  
  //: Return the inverse function
  FuncLinearC FuncMeanProjectionBodyC::Reverse() const {
    return FuncLinearC(proj.Inverse(), mean);
  }

  ///////////////////////////////////////////////////////////
  
  void LinkFuncMeanProjection()
  {}
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncMeanProjectionBodyC,FuncMeanProjectionC,FunctionC);
  //FileFormatStreamC <FuncMeanProjectionC> FileFormatStream_FuncMeanProjectionC;
  //FileFormatBinStreamC <FuncMeanProjectionC> FileFormatBinStream_FuncMeanProjectionC;
  //static TypeNameC typenameFuncMeanProjection(typeid(FuncMeanProjectionC),"FuncMeanProjectionC");

}
