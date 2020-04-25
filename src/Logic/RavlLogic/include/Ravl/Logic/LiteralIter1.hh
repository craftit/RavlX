// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALITER1_HEADER
#define RAVLLOGIC_LITERALITER1_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIter1.hh"

#include "Ravl/Logic/LiteralIter.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Dummy iterator for cases where there are only 1 or zero solutions.
  
  class LiteralIter1BodyC 
    : public LiteralIterBodyC 
  {
  public:
    LiteralIter1BodyC(const LiteralC &nlit)
      : ready(nlit.IsValid()),
        lit(nlit)
      {}
    //: Constructor.
    // Construct with one literal.
    
    LiteralIter1BodyC(bool)
      : ready(false)
      {}
    //: Constructor.
    // Used to construct a null iterator.
    
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
    bool ready;
    LiteralC lit;  
    BindSetC binds;
  };
  
  //! userlevel=Normal
  //: Dummy iterator for cases where there are only 1 or zero solutions.
  
  class LiteralIter1C 
    : public LiteralIterC
  {
  public:
    LiteralIter1C()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    LiteralIter1C(const LiteralC &nlit)
      : LiteralIterC(*new LiteralIter1BodyC(nlit))
      {}
    //: Constructor.
    // Create a iterator giving 'nlit' as the one element in the list.
    
    LiteralIter1C(bool v)
      : LiteralIterC(*new LiteralIter1BodyC(v))
      { RavlAssert(!v); }
    //: Null iterator constructor.
    // You should pass false to this constructor.
    
  protected:
    LiteralIter1C(LiteralIter1BodyC &bod)
      : LiteralIterC(bod)
      {}
    //: Body constructor.
    
    LiteralIter1BodyC &Body()
      { return static_cast<LiteralIter1BodyC &>(LiteralIterC::Body()); }
    //: Access body.

    const LiteralIter1BodyC &Body() const
      { return static_cast<const LiteralIter1BodyC &>(LiteralIterC::Body()); }
    //: Access body.
    
  public:
  };
}


#endif
