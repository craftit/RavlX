// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/StateOrIter.cc"

#include "Ravl/Logic/StateOrIter.hh"
#include "Ravl/Logic/LiteralIter1.hh"
#include "Ravl/Logic/Or.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  StateOrIterBodyC::StateOrIterBodyC(const StateC &nstate,const OrC &nor,const BindSetC &bs) 
    : state(nstate),
      lOr(nor.Terms()),
      binds(bs)
  {
    lOr = lOr.After(0);
    it = lOr;
    RavlAssert(it);
    if(!binds.IsValid())
      binds = BindSetC(true);
    First();
  }

  //: Goto next valid solution.
  
  bool StateOrIterBodyC::NextValid() {
    for(;it;it++) {
      solIt = state.Filter(*it,binds);
      RavlAssert(solIt.IsValid());
      if(solIt) {
	ONDEBUG(std::cerr << "StateOrIterBodyC::NextValid(), Found:" << solIt.Data() << " in or term " << it.Index() << " " << *it << "\n");
	return true;
      }
    };
    solIt = LiteralIter1C(false); // Is this the best way of making an invalid iterator ?
    ONDEBUG(std::cerr << "StateOrIterBodyC::NextValid(), No more solutions. \n");
    return false;
  }
  

  //: Goto next data element.
  // returns true if next element is valid.
  
  bool StateOrIterBodyC::Next() {
    ONDEBUG(std::cerr << "StateOrIterBodyC::Next() \n");
    solIt++;
    if(solIt)
      return true;
    it++;
    return NextValid();
  }

  //: Goto first data element.
  // returns true if next element is valid.
  
  bool StateOrIterBodyC::First() {
    ONDEBUG(std::cerr << "StateOrIterBodyC::First() \n");
    it.First();
    return NextValid();
  }
  
  //: At a valid element ?
  // returns true if iterator is at a valid
  // element.
  
  bool StateOrIterBodyC::IsElm() const {
    return it.IsElm();
  }
    
  //: Get current literal.
  
  LiteralC StateOrIterBodyC::Data() {
    return solIt.Data();
  }
    
  //: Access binds associated with solution.

  BindSetC &StateOrIterBodyC::Binds() {
    return solIt.Binds();
  }
  
  //: Access binds associated with solution.
  
  const BindSetC &StateOrIterBodyC::Binds() const {
    return solIt.Binds();
  }

}
