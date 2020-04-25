// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Not.cc"

#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/Logic/LiteralFilterIter.hh"
#include "Ravl/Logic/State.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlLogicN {
  
  LiteralC literalNot("not");

  //: Default constructor.
  
  NotBodyC::NotBodyC()
    : ConditionBodyC(2)
  { args[0] = literalNot; }
  
  //: Constructor.
  
  NotBodyC::NotBodyC(const LiteralC &nterm)
    : ConditionBodyC(2)
  {
    args[0] = literalNot;
    args[1] = nterm;
  }

  //: Construct from a binary stream.
  
  NotBodyC::NotBodyC(std::istream &strm)
    : ConditionBodyC(strm)
  {}
  
  //: Construct from a binary stream.
  
  NotBodyC::NotBodyC(BinIStreamC &strm)
    : ConditionBodyC(strm)
  {}
  
  //: Save to binary stream 'out'.
  
  bool NotBodyC::Save(std::ostream &out) const
  { return ConditionBodyC::Save(out); }
  
  //: Save to binary stream 'out'.
  
  bool NotBodyC::Save(BinOStreamC &out) const 
  { return ConditionBodyC::Save(out); }
  
  //: Is this equal to another condition ?
  
  bool NotBodyC::IsEqual(const LiteralC &oth) const {
    NotC isNot(oth);
    if(!isNot.IsValid())
      return false;
    return Term().IsEqual(isNot.Term());
  }
  
  //: Is this a simple expression with no variables ?
  
  bool NotBodyC::IsGrounded() const {
    if(!Term().IsValid())
      return true;
    return Term().IsGrounded();
  }
  
  //: Unify with another variable.
  
  bool NotBodyC::Unify(const LiteralC &oth,BindSetC &bs) const {
    BindMarkT bm = bs.Mark();
    if(Term().Unify(oth,bs)) {
      bs.Undo(bm);
      return false;
    }
    return true;
  }
  
  //: Get the name of symbol.
  
  StringC NotBodyC::Name() const 
  { return StringC("!") + Term().Name(); }

  //: Test if condition is true in 'state'.
  
  bool NotBodyC::Test(const StateC &state,BindSetC &binds) const {
    RavlAssert(Term().IsValid());
    return !Term().Test(state,binds);
  }
  
  //: Return iterator through possibile matches to this literal in 'state', if any.
  
  LiteralIterC NotBodyC::Solutions(const StateC &state,BindSetC &binds) const {
    NotC me(const_cast<NotBodyC &>(*this));
    return LiteralFilterIterC(state.List(),me);
  }
  
  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  
  bool NotBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    LiteralC lit;
    if(Term().Substitute(binds,lit)) {
      result = NotC(lit);
      return true;
    }
    result = NotC(const_cast<NotBodyC &>(*this));
    return false;
  }

  //: Generate a set of positive and negative terms used in the condition.
  
  void NotBodyC::ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const {
    Term().ListConditionTerms(posTerms,negTerms);
  }
  
  //: Not operator.
  // FIXME :- Do more simplification.

  LiteralC operator!(const LiteralC &lit) {
    const NotBodyC *ptr = dynamic_cast<const NotBodyC *>(&lit.Body());
    if(ptr != 0)
      return ptr->Term(); // Double not simplifies.
    return NotC(true,lit);
  }
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(NotBodyC,NotC,ConditionC);
  
}
