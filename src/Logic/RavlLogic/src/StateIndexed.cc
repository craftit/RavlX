// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/StateIndexed.cc"

#include "Ravl/Logic/StateIndexed.hh"
#include "Ravl/Logic/LiteralIndexIter.hh"
#include "Ravl/Logic/LiteralIndexFilterBase.hh"
#include "Ravl/Logic/LiteralIndexFilterRaw.hh"
#include "Ravl/Logic/LiteralIndexLeafIter.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlLogicN {
  
  //: Default constructor
  
  StateIndexedBodyC::StateIndexedBodyC()
    : index(true)
  {}
  
  //: Stream constructor
  
  StateIndexedBodyC::StateIndexedBodyC(BinIStreamC &in)
    : StateBodyC(in),
      index(in)
  {}
  
  //: Stream constructor
  
  StateIndexedBodyC::StateIndexedBodyC(istream &in)
    : StateBodyC(in)
  { RavlAssertMsg(0,"Not implemented. "); }
  
  //: Save to binary stream 'out'.
  
  bool StateIndexedBodyC::Save(BinOStreamC &out) const { 
    if(!StateBodyC::Save(out))
      return false;
    return index.Save(out); 
  }
  
  //: Make a copy of this state.
  
  RCBodyVC &StateIndexedBodyC::Copy() const 
  { return *new StateIndexedBodyC(*this); }
  
  //: Ask if a literal is set.
  
  bool StateIndexedBodyC::Ask(const LiteralC &lit) const {
    LiteralIndexFilterBaseBodyC it(index,lit);
    return it.IsElm();  
  }
  
  //: Ask if a literal is set.
  
  bool StateIndexedBodyC::Ask(const LiteralC &lit,BindSetC &binds) const {
    LiteralIndexFilterBaseBodyC it(index,lit,binds);
    return it.IsElm();
  }
  
  //: Set a literal.
  
  bool StateIndexedBodyC::Tell(const LiteralC &lit) 
  { return index.Insert(lit).IsValid(); }
  
  //: Retract a literal.
  
  bool StateIndexedBodyC::Retract(const LiteralC &lit) 
  { return index.Del(lit); }
  
  //: List contents of state.
  
  LiteralIterC StateIndexedBodyC::List() const  
  { return LiteralIndexLeafIterC(index); }
  
  //: Search state for matching literals.
  
  LiteralIterC StateIndexedBodyC::Filter(const LiteralC &lit,BindSetC &bnds) const 
  { return LiteralIndexFilterRawC(index,lit,bnds); }
  
  //: Return the number of literals in the state.
  
  UIntT StateIndexedBodyC::Size() const 
  { return index.Size(); }
  
  //: Clear state completely.
  
  void StateIndexedBodyC::Clear() {
    // Clear state by replacing with an empty index.
    index = LiteralIndexBaseC(true);
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(StateIndexedBodyC,StateIndexedC,StateC);
  
}
