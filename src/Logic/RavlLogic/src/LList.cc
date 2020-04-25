// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LList.cc"

#include "Ravl/Logic/LList.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlLogicN
{
  
  //: Construct from a binary stream.
  
  LListBodyC::LListBodyC(std::istream &strm)
    : LiteralBodyC(strm)
  { RavlAssertMsg(0,"Not implemented. "); }
    
  //: Construct from a binary stream.
  
  LListBodyC::LListBodyC(BinIStreamC &strm)
    : LiteralBodyC(strm)
  {
    UIntT size;
    strm >> size;
    for(UIntT i = 0;i <size;i++) {
      LiteralC lit;
      strm >> lit;
      lst.InsLast(lit);
    }
  }
  
  //: Save to binary stream 'out'.
  
  bool LListBodyC::Save(std::ostream &out) const {
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }

  //: Save to binary stream 'out'.
  
  bool LListBodyC::Save(BinOStreamC &out) const {
    if(!LiteralBodyC::Save(out))
      return false;
    out << lst.Size();
    for(DLIterC<LiteralC> it(lst);it;it++)
      out << ObjIO(*it);
    return true; 
  }
  
  //: Unify with another variable.
  
  bool LListBodyC::Unify(const LiteralC &oth,BindSetC &bs) const {
    return false;
  }
    
  //: Unify with simple symb.
  
  bool LListBodyC::UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const {
    return false;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(LListBodyC,LListC,LiteralC);

}
