// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Or.cc"

#include "Ravl/Logic/Or.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/Logic/StateOrIter.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  const LiteralC literalOr("or");

  //: Default constructor.
  
  OrBodyC::OrBodyC()
    : ConditionBodyC(1)
  { args[0] = literalOr; }

  //: Constructor.
  //!param: arity - Number of terms to create
  
  OrBodyC::OrBodyC(UIntT arity)
    : ConditionBodyC(arity)
  {}
  
  //:  constructor.
  
  OrBodyC::OrBodyC(const SArray1dC<LiteralC> &nterms,bool useArrayDirectly) {
    if(useArrayDirectly) {
      RavlAssert(nterms.Size() > 0);
      RavlAssert(nterms[0] == literalOr);
      args = nterms;
      return ;
    }
    if(nterms.Size() == 0) {
      args = SArray1dC<LiteralC>(1);
      args[0] = literalOr;
      return ;
    }
    args = SArray1dC<LiteralC>(nterms.Size() + 1);
    args[0] = literalOr;
    for(BufferAccessIter2C<LiteralC,LiteralC> it(nterms,args.BufferFrom(1,nterms.Size()));it;it++)
      it.Data2() = it.Data1();
    ONDEBUG(std::cerr << "OrBodyC::OrBodyC(), Name=" << Name() << "\n");    
  }
  
  //: Constructor.

  OrBodyC::OrBodyC(const LiteralC &term) 
    : ConditionBodyC(2)
  {
    args[0] = literalOr;
    args[1] = term;
  }
  
  //: Construct from a binary stream.
  
  OrBodyC::OrBodyC(std::istream &strm)
    : ConditionBodyC(strm)
  {}
  
  //: Construct from a binary stream.
  
  OrBodyC::OrBodyC(BinIStreamC &strm)
    : ConditionBodyC(strm)
  {}
  
  //: Save to binary stream 'out'.
  
  bool OrBodyC::Save(std::ostream &out) const
  { return ConditionBodyC::Save(out); }
  
  //: Save to binary stream 'out'.
  
  bool OrBodyC::Save(BinOStreamC &out) const 
  { return ConditionBodyC::Save(out); }
  
  //: Unify with another variable.
  
  bool OrBodyC::Unify(const LiteralC &oth,BindSetC &bs) const {
    BindMarkT bm = bs.Mark();
    for(SArray1dIterC<LiteralC> it(args);it;it++) {
      if(it->Unify(oth,bs))
	return true;
      bs.Undo(bm);
    }
    return false;
  }
  
  //: Return iterator through possibile matches to this literal in 'state', if any.
  
  LiteralIterC OrBodyC::Solutions(const StateC &state,BindSetC &binds) const {
    return StateOrIterC(state,OrC(const_cast<OrBodyC &>(*this)),binds);
  }
  
  //: Substitute variables in 'binds' for their bound values.
  // This builds a new literal with the substute values (if there
  // are any). The new value is assigned to 'result' <p>
  // Returns true if at least one substitution has been made,
  // false if none.
  
  bool OrBodyC::Substitute(const BindSetC &binds,LiteralC &result) const {
    SArray1dC<LiteralC> newTup(Args().Size());
    bool ret = false;
    SArray1dIter2C<LiteralC,LiteralC> it(Args(),newTup);
    it.Data2() = it.Data1(); // Skip 'or' marker.
    for(it++;it;it++)
      ret |= it.Data1().Substitute(binds,it.Data2());
    if(ret) {
      result = OrC(newTup,true);
      return true;
    }
    result = OrC(const_cast<OrBodyC &>(*this));
    return false;
  }
  
  //: Add literal.
  
  void OrBodyC::OrAdd(const LiteralC &lit) {
    // FIXME :- Try and minimize
    if(!lit.IsValid())
      return ;
    OrC on(lit);
    if(on.IsValid()) 
      OrAdd(on.Terms());
    else
      AddTerm(lit);    
  }
  
  //: Add literals.
  
  void OrBodyC::OrAdd(const SArray1dC<LiteralC> &lits) {
    // FIXME :- Try and minimize
    AddTerms(lits);
  }
  
  //: Generate a set of positive and negative terms used in the condition.
  
  void OrBodyC::ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const {
    SArray1dIterC<LiteralC> it(Args());
    RavlAssert(it);
    it++; // Skip 'And' marker
    for(;it;it++) {
      it->ListConditionTerms(posTerms,negTerms);
    }
  }
  
  //////////////////////////////////////////////////////////////////////
  
  //: Or two literals.
  // FIXME :- Do more simplification.
  
  TupleC operator+(const LiteralC &l1,const LiteralC &l2) {
    ONDEBUG(std::cerr << "operator+(LiteralC,LiteralC) Called for " << l1.Name() << " and " << l2.Name() << "\n");
    SizeT size = 0;
    OrC a1(l1);
    if(a1.IsValid())
      size += a1.Size();
    else
      size++;
    OrC a2(l2);
    if(a2.IsValid())
      size += a2.Size();
    else
      size++;
    ONDEBUG(std::cerr << "Size=" << size << "\n");
    SArray1dC<LiteralC> arr(size+1);
    arr[0] = literalOr;
    SizeT at = 1;
    if(a1.IsValid()) {
      for(SArray1dIter2C<LiteralC,LiteralC> it(a1.Terms().After(0),arr.From(at));it;it++)
	it.Data2() = it.Data1();
      at += a1.Size();
    } else
      arr[at++] = l1;
    
    if(a2.IsValid()) {
      for(SArray1dIter2C<LiteralC,LiteralC> it(a2.Terms().After(0),arr.From(at));it;it++)
	it.Data2() = it.Data1();
    } else
      arr[at] = l2;    
    ONDEBUG(std::cerr << "operator+(LiteralC,LiteralC) Result=" << arr << "\n");
    return OrC(arr,true); 
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(OrBodyC,OrC,ConditionC);

}
