// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncPDF.cc"

#include "Ravl/PatternRec/FuncPDF.hh"

namespace RavlN {
  
  //: Constructor.
  
  FuncPDFBodyC::FuncPDFBodyC(UIntT inSize,UIntT outSize)
    : FunctionBodyC(inSize,outSize)
  {}
  
  //: Load from stream.
  
  FuncPDFBodyC::FuncPDFBodyC(std::istream &strm)
    : FunctionBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  FuncPDFBodyC::FuncPDFBodyC(BinIStreamC &strm)
    : FunctionBodyC(strm)
  {}
  
  //: Writes object to stream.
  
  bool FuncPDFBodyC::Save (std::ostream &out) const {
    return FunctionBodyC::Save(out);
  }
  
  //: Writes object to binary stream.
  
  bool FuncPDFBodyC::Save (BinOStreamC &out) const {
    return FunctionBodyC::Save(out);
  }
  
  ///////////////////////////////////////////////////////
  
  //: Load from stream.
  
  FuncPDFC::FuncPDFC(std::istream &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,FunctionBodyC))
  {}
  
  //: Load from binary stream.
  
  FuncPDFC::FuncPDFC(BinIStreamC &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,FunctionBodyC))
  {}


}
