// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/NamedVar.cc"

#include "Ravl/Logic/NamedVar.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

namespace RavlLogicN {
  
  //: Construct from a stream.
  
  NamedVarBodyC::NamedVarBodyC(std::istream &strm)
    : VarBodyC(strm)
  { strm >> name; }
    
  //: Construct from a binary stream.
  
  NamedVarBodyC::NamedVarBodyC(BinIStreamC &strm) 
    : VarBodyC(strm)
  { strm >> name; }
    
  //: Save to stream 'out'.
  
  bool NamedVarBodyC::Save(std::ostream &out) const { 
    if(!VarBodyC::Save(out)) return false;
    out << ' ' << name;
    return true;
  }
  
  //: Save to binary stream 'out'.
  
  bool NamedVarBodyC::Save(BinOStreamC &out) const { 
    if(!VarBodyC::Save(out))
      return false;
    out << name;
    return true; 
  }

  //: Is this a variable ?
  
  bool NamedVarBodyC::IsVariable() const
  { return true; }
  
  //: Is this a simple object with no variables ?
  
  bool NamedVarBodyC::IsGrounded() const
  { return false; }
  
  //: Get the name of symbol.
  
  StringC NamedVarBodyC::Name() const {
    return name;
  }

  //: Get hash value for symbol.
  UIntT NamedVarBodyC::Hash() const {
    return name.Hash();
  }
  
  //: Is this equial to another LiteralC ?
  
  bool NamedVarBodyC::IsEqual(const LiteralC &oth) const {
    return oth.Name() == name;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(NamedVarBodyC,NamedVarC,VarC);
  
}
