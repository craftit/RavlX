// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_STATESET_HEADER
#define RAVLLOGIC_STATESET_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/StateSet.hh"

#include "Ravl/Logic/State.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Set based implementation of a set.
  
  class StateSetBodyC 
    : public StateBodyC
  {
  public:
    StateSetBodyC()
    {}
    //: Default constructor.
    
    StateSetBodyC(const HSetC<LiteralC> &ndata)
      : data(ndata)
    {}
    //: Default constructor.

    StateSetBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    StateSetBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

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
    
#if 0
    virtual MinTermC Difference(const StateC &oth) const;
    //: List the differences between this and another state.
    // Positive terms are ones in this state, but not in 'oth'. <p>
    // Negative terms are those in the 'oth' state but not in this one.
#endif
    
    virtual HSetC<LiteralC> Intersection(const StateC &oth) const;
    //: List all common terms between this state and 'oth'.
    
    virtual UIntT Size() const;
    //: Return the number of literals in the state.
    
    HSetC<LiteralC> &Data()
    { return data; }
    //: Accesss set directly.
    
    virtual void Clear();
    //: Clear state completely.
  protected:
    HSetC<LiteralC> data;
  };

  //! userlevel=Normal
  //: Set based implementation of a set.
  
  class StateSetC 
    : public StateC
  {
  public:
    StateSetC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit StateSetC(bool)
      : StateC(*new StateSetBodyC())
    {}
    //: Constructor.
    
    StateSetC(const HSetC<LiteralC> &ndata)
      : StateC(*new StateSetBodyC(ndata))
    {}
    //: Constructor.
    
    StateSetC(const StateC &oth)
      : StateC(dynamic_cast<const StateSetBodyC *>(BodyPtr(oth)))
    {}
    //: Base constructor.
    // If given state is not a StateSetC an invalid handle
    // will be generated.
    
    StateSetC(std::istream &strm);
    //: Load from stream.
    
    StateSetC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    StateSetC(StateSetBodyC &bod)
      : StateC(bod)
    {}
    //: Body constructor.

    StateSetC(StateSetBodyC *bod)
      : StateC(bod)
    {}
    //: Body constructor.
    
    StateSetBodyC &Body()
    { return static_cast<StateSetBodyC &>(StateC::Body()); }
    //: Access body.
    
    const StateSetBodyC &Body() const
    { return static_cast<const StateSetBodyC &>(StateC::Body()); }
    //: Access body.
    
  public:
    HSetC<LiteralC> &Data()
    { return Body().Data(); }
    //: Accesss set directly.
    
  };
  
}

#endif
