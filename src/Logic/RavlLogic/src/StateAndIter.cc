// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/StateAndIter.cc"

#include "Ravl/Logic/StateAndIter.hh"
#include "Ravl/Logic/LiteralIter1.hh"
#include "Ravl/Logic/And.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Constructor.
  
  StateAndIterBodyC::StateAndIterBodyC(const StateC &nstate,const AndC &nand,const BindSetC &bs) 
    : state(nstate),
      lAnd(nand.Terms()),
      binds(bs)
  {
    lAnd = lAnd.After(0);
    initalBm = binds.Mark();
    First();
  }

  //: Constructor.
  
  StateAndIterBodyC::StateAndIterBodyC(const StateC &nstate,const SArray1dC<LiteralC> &nand,const BindSetC &bs)
    : state(nstate),
      lAnd(nand),
      binds(bs)
  {
    initalBm = binds.Mark();
    First();
  }

  bool StateAndIterBodyC::NextValid() {
    ONDEBUG(std::cerr << "StateAndIterBodyC::NextValid(), Called. MaxTerms:" << lAnd.Size() << "\n");
    
    RavlAssert(lAnd.Size() != 0); // What to do with zero terms ?
    int maxTerm = lAnd.Size() - 1;
    while(!stack.IsEmpty()) {
      int nextTerm;
      while(stack.Top().termNo < maxTerm) {
	nextTerm = stack.Top().termNo + 1;
	BindMarkT mark = binds.Mark();
	ONDEBUG(std::cerr << "StateAndIterBodyC::NextValid(), Iterating term " << nextTerm <<" " << lAnd.Terms()[nextTerm] << " \n");
	LiteralIterC lit = state.Filter(lAnd[nextTerm],binds);
	ONDEBUG(std::cerr << "Binds= " << binds << "\n");
	if(!lit.IsElm()) {
	  stack.DelTop(); // Back track.
	  break;
	}
	stack.Push(StateAndBackTrackPointC(mark,nextTerm,lit));
	if(stack.Top().termNo == maxTerm)
	  return true;
      }
      if(stack.Top().termNo == maxTerm && stack.Top().iter.IsElm())
	return true;
      for(;!stack.IsEmpty();) {
	if(stack.Top().iter.IsElm()) {
	  if(stack.Top().iter.Next()) {
	    ONDEBUG(std::cerr << "StateAndIterBodyC::NextValid(), Back tracking. \n");
	    break;
	  }
	}
	stack.DelTop();
      }
    } 
    ONDEBUG(std::cerr << "StateAndIterBodyC::NextValid(), No more solutions. \n");
    binds.Undo(initalBm); // Make sure all binds are cleared.
    return false;
  }
  
  //: Goto next data element.
  // returns true if next element is valid.
  
  bool StateAndIterBodyC::Next() {
    ONDEBUG(std::cerr << "StateAndIterBodyC::Next(), Called. \n");
    RavlAssert(!stack.IsEmpty());
    RavlAssert(stack.Top().iter.IsElm());
    if(stack.Top().iter.Next())
      return true;
    return NextValid();
  }
    
  //: Goto first data element.
  // returns true if next element is valid.
  
  bool StateAndIterBodyC::First() {
    ONDEBUG(std::cerr << "StateAndIterBodyC::First(), Called. \n");
    stack.Empty();
    binds.Undo(initalBm);
    ONDEBUG(std::cerr << "StateAndIterBodyC::First(), Iterating term " << 0 <<" " << lAnd.Terms()[0] << " \n");
    stack.Push(StateAndBackTrackPointC(initalBm,0,state.Filter(lAnd[0],binds)));
    ONDEBUG(std::cerr << "Binds= " << binds << "\n");
    return NextValid();
  }
  
  //: At a valid element ?
  // returns true if iterator is at a valid
  // element.
  
  bool StateAndIterBodyC::IsElm() const {
    return !stack.IsEmpty();
  }
  
  //: Get current literal.
  
  LiteralC StateAndIterBodyC::Data() {
    RavlAssert(!stack.IsEmpty());
    return stack.Top().iter.Data();
  }
  
  //: Access binds associated with solution.

  BindSetC &StateAndIterBodyC::Binds() {
    return stack.Top().iter.Binds();
  }
  
  //: Access binds associated with solution.
  const BindSetC &StateAndIterBodyC::Binds() const {
    return stack.Top().iter.Binds();
  }

}
