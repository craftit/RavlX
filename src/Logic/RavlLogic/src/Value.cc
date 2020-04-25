// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
// Logic2/LValue.cc     
//! date="10/5/1996"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Value.cc"

#include "Ravl/Logic/Value.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/StdHash.hh"


namespace RavlLogicN {
  
  //: Construct from a binary stream.
  
  ValueBaseBodyC::ValueBaseBodyC(std::istream &strm)
    : LiteralBodyC(strm)
  {}
  
  //: Construct from a binary stream.
  
  ValueBaseBodyC::ValueBaseBodyC(BinIStreamC &strm)
    : LiteralBodyC(strm)
  {}
    
  //: Save to binary stream 'out'.
  
  bool ValueBaseBodyC::Save(std::ostream &out) const
  { return LiteralBodyC::Save(out); }
  
  //: Save to binary stream 'out'.
  
  bool ValueBaseBodyC::Save(BinOStreamC &out) const
  { return LiteralBodyC::Save(out); }
  
  // ValueBaseC base class.
  
  bool ValueBaseBodyC::Unify(const LiteralC &other,BindSetC &SF) const {
    if(other.IsVariable() || IsVariable())
      return LiteralBodyC::Unify(other,SF);
    return IsEqual(other);
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ValueBaseBodyC,ValueBaseC,LiteralC);
  
  static VirtualConstructorInstC<RavlLogicN::ValueBodyC<RealT> > vc_function_ValueBodyC_RealT ("RavlLogicN::ValueBodyC<RealT>");
  static VirtualConstructorInstC<RavlLogicN::ValueBodyC<IntT> > vc_function_ValueBodyC_IntT ("RavlLogicN::ValueBodyC<IntT>");
  
  
}
