// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralFilterIter.cc"

#include "Ravl/Logic/LiteralFilterIter.hh"
#include "Ravl/Logic/Unify.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {
  
  //: Constructor.
  LiteralFilterIterBodyC::LiteralFilterIterBodyC(const LiteralIterC &niter,const LiteralC &nfilter) 
    : iter(niter),
      filter(nfilter)
  { First(); }
  
  //: Destructor.
  
  LiteralFilterIterBodyC::~LiteralFilterIterBodyC() {
    if(binds.IsValid()) 
      binds.Undo(mark);
  }
  
  //: Goto next valid solution.
  
  bool LiteralFilterIterBodyC::NextValid() {
    do {
      RavlAssert(iter.IsElm()); // Check .Next() isn't lying.
      binds = iter.Binds();
      mark = binds.Mark();
      if(Unify(filter,iter.Data(),binds)) {
	ONDEBUG(std::cerr << "Unify of " << filter << " and " << iter.Data() << "  Binds:" << binds << " =true\n");
	return true;
      }
    } while(iter.Next()) ;
    binds.Invalidate();
    return false;
  }
  
  //: Goto next data element.
  // returns true if next element is valid.
  
  bool LiteralFilterIterBodyC::Next() {
    ONDEBUG(std::cerr << "LiteralFilterIterBodyC::Next(), Called \n");
    if(binds.IsValid())
      binds.Undo(mark);
    if(!iter.Next()) {
      binds.Invalidate();
      return false;
    }
    return NextValid();
  }
  
  //: Goto first data element.
  // returns true if next element is valid.
  
  bool LiteralFilterIterBodyC::First() {
    ONDEBUG(std::cerr << "LiteralFilterIterBodyC::First(), Called \n");
    if(binds.IsValid())
      binds.Undo(mark);
    if(!iter.First()) {
      binds.Invalidate();
      return false;
    }
    return NextValid();
  }
  
  //: At a valid element ?
  // returns true if iterator is at a valid
  // element.
  
  bool LiteralFilterIterBodyC::IsElm() const {
    return iter.IsElm();
  }
  
  //: Get current literal.
  
  LiteralC LiteralFilterIterBodyC::Data() {
    return iter.Data();
  }
  
  //: Access binds associated with solution.
  
  BindSetC &LiteralFilterIterBodyC::Binds() {
    return iter.Binds();
  }
  
  //: Access binds associated with solution.
  
  const BindSetC &LiteralFilterIterBodyC::Binds() const {
    return iter.Binds();
  }

}
