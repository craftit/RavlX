// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_STATEORITER_HEADER
#define RAVLLOGIC_STATEORITER_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/StateOrIter.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/State.hh"

namespace RavlLogicN {
  
  class OrC;
  
  //! userlevel=Develop
  //: Iterate through possible solutions in a state for a 'or' condition.
  // NB. Solutions may be repeated if two or more terms in the or condtion
  // unify with the same literal..
  
  class StateOrIterBodyC
    : public LiteralIterBodyC 
  {
  public:
    StateOrIterBodyC(const StateC &nstate,const OrC &nor,const BindSetC &bs);
    //: Constructor.
    
    virtual bool Next();
    //: Goto next data element.
    // returns true if next element is valid.
    
    virtual bool First();
    //: Goto first data element.
    // returns true if next element is valid.
    
    virtual bool IsElm() const;
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    virtual LiteralC Data();
    //: Get current literal.
    
    virtual BindSetC &Binds();
    //: Access binds associated with solution.
    
    virtual const BindSetC &Binds() const;
    //: Access binds associated with solution.
    
  protected:    
    bool NextValid();
    //: Goto next valid solution.
    
    StateC state;
    SArray1dC<LiteralC> lOr;
    BindSetC binds;
    SArray1dIterC<LiteralC> it;
    LiteralIterC solIt; // Solutions within the state.
  };
  
  //! userlevel=Advanced
  //: Iterate through possible solutions in a state for a 'or' condition.
  // NB. Solutions may be repeated if two or more terms in the or condtion
  // unify with the same literal..
  
  class StateOrIterC
    : public LiteralIterC 
  {
  public:
    StateOrIterC()
      {}
    //: Default constructor.
    // creates an invalid handle.
    
    StateOrIterC(const StateC &nstate,const OrC &nor,const BindSetC &bs)
      : LiteralIterC(*new StateOrIterBodyC(nstate,nor,bs))
      {}
    //: 
    
  };
}


#endif


