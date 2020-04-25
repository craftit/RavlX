// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Literal.cc"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Construct from a binary stream.
  
  LiteralBodyC::LiteralBodyC(BinIStreamC &strm) 
    : RCBodyVC(strm)
  {}

  //: Construct from a stream.
  
  LiteralBodyC::LiteralBodyC(std::istream &strm) 
    : RCBodyVC(strm)
  {}
  
  //: Save to binary stream 'out'.
  
  bool LiteralBodyC::Save(BinOStreamC &out) const 
  { return RCBodyVC::Save(out); }
  
  //: Save to stream 'out'.
  
  bool LiteralBodyC::Save(std::ostream &out) const 
  { return RCBodyVC::Save(out); }
  
  //: Is this a variable ?
  bool LiteralBodyC::IsVariable() const
  { return false; }

  //: Is this a simple expression with no variables ?
  bool LiteralBodyC::IsGrounded() const
  { return true; }
  
  //: Is this literal a tuple ?
  bool LiteralBodyC::IsTuple() const
  { return false; }
  
  //: Test if this is a condition.
  bool LiteralBodyC::IsCondition() const
  { return false; }
  
  // Unify with another variable.
  
  bool LiteralBodyC::Unify(const LiteralC &oth,BindSetC &bs) const {
    RavlAssert(oth.IsValid());
    if(&oth.Body() == this)
      return true;
    return oth.Body().UnifyLiteral(*this,bs);
  }
  
  bool LiteralBodyC::UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const {
    return false;
  }

  //: Dump info in human readable format to stream 'out'.
  
  void LiteralBodyC::Dump(std::ostream &out) {
    out << "L:" << ((void *) this);
  }
  
  //: Is this equial to another LiteralC ?
  
  bool LiteralBodyC::IsEqual(const LiteralC &oth) const 
  { return this == &oth.Body(); }

  //: Generate a unique name.
  
  StringC LiteralBodyC::Name() const
  {
#if RAVL_CPUTYPE_64
    return StringC("L:") + StringC((UInt64T) this); 
#else
    return StringC("L:") + StringC((UIntT) this); 
#endif 
  }
  
  //: Test if condition is true in 'state'.
  
  bool LiteralBodyC::Test(const StateC &state,BindSetC &binds) const {
    LiteralC me(const_cast<LiteralBodyC &>(*this));
    return state.Ask(me);
  }

  //: Return iterator through possibile matches to this literal in 'state', if any.
  
  LiteralIterC LiteralBodyC::Solutions(const StateC &state,BindSetC &binds) const {
    LiteralC me(const_cast<LiteralBodyC &>(*this));
    return state.Filter(me,binds);
  }

  //: Get a set of all sub literals.
  
  void LiteralBodyC::SubLiterals(HSetC<LiteralC> &ret) const {
    ret += LiteralC(const_cast<LiteralBodyC &>(*this)); // Just me.
  }

  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  
  bool LiteralBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    result = LiteralC(const_cast<LiteralBodyC &>(*this));
    return false;
  }
  
  //: Replace all vars in this literal with new ones.
  // The mapping between the replacements and the new values is returned in 'subs'
  // If no new replacements where found, false is returned.
  
  bool LiteralBodyC::ReplaceVars(HashC<LiteralC,LiteralC> &subs,LiteralC &result) const {
    result = LiteralC(const_cast<LiteralBodyC &>(*this));
    return false;    
  }
  
  //: Generate a set of positive and negative terms used in the condition.
  
  void LiteralBodyC::ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const {
    posTerms += LiteralC(this);
  }
  
  ////////////////////////////////////////////////////////////////
  
  //: Return iterator through possibile solutions, if any.
  
  LiteralIterC LiteralC::Solutions(const StateC &state,BindSetC &binds) const { 
    RavlAssert(IsValid());
    return Body().Solutions(state,binds); 
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(LiteralBodyC,LiteralC,RCHandleVC<LiteralBodyC>);
  
}
