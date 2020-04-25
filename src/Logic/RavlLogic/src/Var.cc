// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Var.cc"

#include "Ravl/Logic/Var.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlLogicN {
  
  //: Construct from a stream.
  
  VarBodyC::VarBodyC(std::istream &strm)
    : LiteralBodyC(strm)
  {}
    
  //: Construct from a binary stream.
  
  VarBodyC::VarBodyC(BinIStreamC &strm) 
    : LiteralBodyC(strm)
  {}
    
  //: Save to stream 'out'.
  
  bool VarBodyC::Save(std::ostream &out) const 
  { return LiteralBodyC::Save(out); }
    
  //: Save to binary stream 'out'.
  
  bool VarBodyC::Save(BinOStreamC &out) const 
  { return LiteralBodyC::Save(out); }

  //: Is this a variable ?
  
  bool VarBodyC::IsVariable() const
  { return true; }
  
  //: Is this a simple object with no variables ?
  
  bool VarBodyC::IsGrounded() const
  { return false; }
  
  //: Unify with another variable.
  
  bool VarBodyC::Unify(const LiteralC &x,BindSetC &bs) const {
    //: Bind value 'oth' to this.
    LiteralC var(const_cast<VarBodyC &>(*this));
    if(var == x) 
      return true;
    LiteralC val;
    if(bs.Resolve(var,val)) // Is x bound to something ?
      return val.Unify(x,bs);
    if(bs.Resolve(x,val))
      return var.Unify(val,bs);
    return bs.Bind(var,x);
  }
  
  //: Unify 

  bool VarBodyC::UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const {
    //: Bind this to 'oth'
    LiteralC val(const_cast<LiteralBodyC &>(oth));
    return VarBodyC::Unify(val,bs);
  }

  //: Dump info in human readable format to stream 'out'.
  void VarBodyC::Dump(std::ostream &out) {
    out << "V:" << ((void *) this);
  }

  //: Get the name of symbol.
  StringC VarBodyC::Name() const { 
#if RAVL_CPUTYPE_64
    return StringC("V:") + StringC((UInt64T) this); 
#else
    return StringC("V:") + StringC((UIntT) this); 
#endif 
  }
  
  
  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.

  bool VarBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    VarC me(const_cast<VarBodyC &>(*this));
    if(!binds.Resolve(me,result)) {
      result = me;
      return false;
    }
    return true;
  }
  
  //: Replace all vars in this literal with new ones.
  // The mapping between the replacements and the new values is returned in 'subs'
  // If no new replacements where found, false is returned.
  
  bool VarBodyC::ReplaceVars(HashC<LiteralC,LiteralC> &subs,LiteralC &result) const {
    VarC me(const_cast<VarBodyC &>(*this));
    if(subs.Lookup(me,result))
      return true;
    result = Var();
    subs.Insert(me,result);
    return true;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(VarBodyC,VarC,LiteralC);
  
}
