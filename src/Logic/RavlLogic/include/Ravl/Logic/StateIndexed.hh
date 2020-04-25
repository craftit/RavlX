// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_STATEINDEXED_HEADER
#define RAVLLOGIC_STATEINDEXED_HEADER 1
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Logic.Index"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/StateIndexed.hh"

#include "Ravl/Logic/State.hh"
#include "Ravl/Logic/LiteralIndexBase.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Indexed state
  
  class StateIndexedBodyC
    : public StateBodyC
  {
  public:
    StateIndexedBodyC();
    //: Default constructor
    
    StateIndexedBodyC(BinIStreamC &in);
    //: Binary stream constructor
    
    StateIndexedBodyC(istream &in);
    //: Stream constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual RCBodyVC &Copy() const;
    //: Make a copy of this state.
    
    virtual bool Ask(const LiteralC &lit) const;
    //: Ask if a literal is set.
    
    virtual bool Ask(const LiteralC &lit,BindSetC &binds) const;
    //: Ask if a literal is set.
    
    virtual bool Tell(const LiteralC &lit);
    //: Set a literal.
    
    virtual bool Retract(const LiteralC &lit);
    //: Retract a literal.
    
    virtual LiteralIterC List() const;
    //: List contents of state.
    
    virtual LiteralIterC Filter(const LiteralC &it,BindSetC &bnds) const;
    //: Search state for matching literals.
    
    virtual UIntT Size() const;
    //: Return the number of literals in the state.
    
    virtual void Clear();
    //: Clear state completely.
    
  protected:
    LiteralIndexBaseC index;    
  };
  
  //! userlevel=Normal
  //: Indexed state
  
  class StateIndexedC
    : public StateC
  {
  public:
    StateIndexedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateIndexedC(bool)
      : StateC(*new StateIndexedBodyC())
    {}
    //: Constructor.
    
    StateIndexedC(istream &strm);
    //: Stream constructor.

    StateIndexedC(BinIStreamC &strm);
    //: Binary stream constructor.
    
  protected:
    StateIndexedC(StateIndexedBodyC &bod)
      : StateC(bod)
    {}
    //: Body constructor.
    
    StateIndexedC(StateIndexedBodyC *bod)
      : StateC(bod)
    {}
    //: Body constructor.
    
    StateIndexedBodyC &Body()
    { return static_cast<StateIndexedBodyC &>(StateC::Body()); }
    //: Access body.
    
    const StateIndexedBodyC &Body() const
    { return static_cast<const StateIndexedBodyC &>(StateC::Body()); }
    //: Access body.
    
  };
  
}


#endif
