// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Condition.cc"

#include "Ravl/Logic/Condition.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlLogicN {
  
  //: Construct from a binary stream.
  
  ConditionBodyC::ConditionBodyC(std::istream &strm)
    : TupleBodyC(strm)
  {}
  
  //: Construct from a binary stream.
  
  ConditionBodyC::ConditionBodyC(BinIStreamC &strm)
    : TupleBodyC(strm)
  {}
  
  //: Test if this is a condition.
  
  bool ConditionBodyC::IsCondition() const
  { return true; }
  
  //: Save to binary stream 'out'.
  
  bool ConditionBodyC::Save(std::ostream &out) const {
    return TupleBodyC::Save(out);
  }
  
  //: Save to binary stream 'out'.
  
  bool ConditionBodyC::Save(BinOStreamC &out) const {
    return TupleBodyC::Save(out);
  }
  
  //: Test if condition is true in 'state'.
  
  bool ConditionBodyC::Test(const StateC &state,BindSetC &binds) const {
    BindMarkT mark = binds.Mark();
    LiteralIterC ret = Solutions(state,binds);
    RavlAssert(ret.IsValid());
    if(!ret.IsElm()) {
      binds.Undo(mark);
      return false;
    }
    return true;
  }
  
  //: Add some terms.
  
  void ConditionBodyC::AddTerms(const SArray1dC<LiteralC> &terms) {
    SArray1dC<LiteralC> newun(args.Size() + terms.Size());
    BufferAccessIter2C<LiteralC,LiteralC> it(args,newun);
    for(;it;it++)
      it.Data2() = it.Data1();
    BufferAccessIter2C<LiteralC,LiteralC> newit(newun.From(args.Size()),terms);
    for(;newit;newit++)
      newit.Data1() = newit.Data2();
    args = newun;
  }
  
  //: Add a term.
  
  void ConditionBodyC::AddTerm(const LiteralC &term) {
    SArray1dC<LiteralC> newun(args.Size() + 1);
    for(BufferAccessIter2C<LiteralC,LiteralC> it(args,newun);it;it++)
      it.Data2() = it.Data1();
    newun[args.Size()] = term;
    args = newun;
  }
  
  //: Is this equal to another condition ?
  
  bool ConditionBodyC::IsEqual(const LiteralC &oth) const {
    ConditionC a(oth);
    if(!a.IsValid())
      return false;
    if(Arity() != a.Arity())
      return false;
    if(this == &a.Body())
      return true;
    switch(Arity()) {
    case 0: return true;
    case 1: return true; // Both AndC's must start with literalAnd.
    case 2: return args[1] == a[1];
    case 3:
      if(args[1] == a[1])
	return args[2] == a[2];
      if(args[1] != a[2]) 
	return false;
      return args[2] == a[1]; 
    default:
      // Is there a faster way ?
      SArray1dIterC<LiteralC> it(args);
      it++;
      HSetC<LiteralC> argset;
      for(;it;it++)
	argset += *it;
      SArray1dIterC<LiteralC> it2(a.Args());
      it2++;
      for(;it2;it2++)
	if(!argset[*it2]) return false;
      break;
    }
    RavlAssertMsg(0,"AndBodyC::IsEqual(), Not implemented. ");
    return false;
  }
  
  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  //: Get hash value for symbol.
  
  bool ConditionBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    RavlAlwaysAssertMsg(0,"ConditionBodyC::Substitute(), Abstract method called.");
    return false;
  }
  
  //: Generate a set of positive and negative terms used in the condition.
  
  void ConditionBodyC::ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const 
  {
    RavlAssertMsg(0,"abstract methods");
  }
  
  //: Get hash value for symbol.
  
  UIntT ConditionBodyC::Hash() const {
    UIntT hash = 123;
    // Generate an order independant hash.
    for(SArray1dIterC<LiteralC> it(Args());it;it++)
      hash += it->Hash();
    return hash;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ConditionBodyC,ConditionC,TupleC);
  
}
