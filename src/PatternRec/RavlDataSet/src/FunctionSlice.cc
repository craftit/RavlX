// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: FunctionSlice.cc 4106 2004-03-18 16:30:45Z kier $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionSlice.cc"

#include "Ravl/PatternRec/FunctionSlice.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"
#include "Ravl/SArray1dIter.hh"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/VirtualConstructor.hh"
#pragma instantiate RavlN::FunctionSliceBodyC* RavlN::VCLoad(RavlN::BinIStreamC&,RavlN::FunctionSliceBodyC*)
#pragma instantiate RavlN::FunctionSliceBodyC* RavlN::VCLoad(std::istream &,RavlN::FunctionSliceBodyC*)
#endif 

namespace RavlN {

  //: Default constructor.
  
  FunctionSliceBodyC::FunctionSliceBodyC()
  {}
  
  //: Construct from XML factory

  FunctionSliceBodyC::FunctionSliceBodyC(const XMLFactoryContextC &factory)
    : FunctionBodyC(factory),
      m_indexRange(factory.AttributeUInt("minIndex",0),
                   factory.AttributeUInt("maxIndex",0))
  {}

  //: Default constructor.
  
  FunctionSliceBodyC::FunctionSliceBodyC(UIntT inputSize, const IndexRangeC & indexRange)
    : FunctionBodyC(inputSize, indexRange.Size()),
      m_indexRange(indexRange)
  { 
    RavlAssertMsg(indexRange.Size() <= (IntT)inputSize, "Oops.  Slice bigger than vector");
  }
  
  //: Load from stream.
  
  FunctionSliceBodyC::FunctionSliceBodyC(std::istream &strm) 
    : FunctionBodyC(strm)
  { strm >> m_indexRange; }
  
  //: Load from binary stream.
  
  FunctionSliceBodyC::FunctionSliceBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  { strm >> m_indexRange; }
  
  //: Writes object to stream.
  
  bool FunctionSliceBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << m_indexRange;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool FunctionSliceBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << m_indexRange;
    return true;
  }
  
  //: Apply function to 'data'  
  VectorC FunctionSliceBodyC::Apply(const VectorC &data) const {
    // FIXME: Can do this lots more efficiently
    VectorC vec(m_indexRange.Size());
    IndexC index = m_indexRange.Min();
    for(SArray1dIterC<RealT> vecIt(vec);vecIt;vecIt++) {
      *vecIt = data[index];
      index++;
    }
    return vec;
  }
  
  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FunctionSliceBodyC,FunctionSliceC,FunctionC);
  //FileFormatStreamC <FunctionSliceC> FileFormatStream_FunctionSliceC;
  //FileFormatBinStreamC <FunctionSliceC> FileFormatBinStream_FunctionSliceC;
  //static TypeNameC typenameFunctionSlice(typeid(FunctionSliceC),"FunctionSliceC");

}
