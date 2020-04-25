// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALITER_HEADER
#define RAVLLOGIC_LITERALITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIter.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/BindSet.hh"

namespace RavlN {
  class RCAbstractC;
}

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Abstract iterator through a set of literals.
  
  class LiteralIterBodyC 
    : public RCBodyVC
  {
  public:
    LiteralIterBodyC()
    {}
    //: Default constructor.
    
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
  };
  
  //! userlevel=Normal
  //: Abstract iterator through a set of literals.
  
  class LiteralIterC 
    : public RCHandleC<LiteralIterBodyC>
  {
  public:
    LiteralIterC()
    {}
    //: Default constructor.
    // creates an invalid handle.

  protected:
    LiteralIterC(LiteralIterBodyC &bod)
      : RCHandleC<LiteralIterBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIterC(const LiteralIterBodyC *bod)
      : RCHandleC<LiteralIterBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIterBodyC &Body()
    { return RCHandleC<LiteralIterBodyC>::Body(); }
    //: Access body.

    const LiteralIterBodyC &Body() const
    { return RCHandleC<LiteralIterBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Next()
    { return Body().Next(); }
    //: Goto next data element.
    // returns true if next element is valid.
    
    bool operator++(int)
    { return Body().Next(); }
    //: Goto next data element.
    // returns true if next element is valid.
    
    bool First()
    { return Body().First(); }
    //: Goto first data element.
    // returns true if next element is valid.
    
    bool IsElm() const 
    { return Body().IsElm(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    operator bool() const 
    { return Body().IsElm(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    LiteralC Data()
    { return Body().Data(); }
    //: At a valid element.

    LiteralC operator*()
    { return Body().Data(); }
    //: Goto next data element.
    // returns true if next element is valid.
    
    BindSetC &Binds()
    { return Body().Binds(); }
    //: Access binds associated with solution.

    const BindSetC &Binds() const
    { return Body().Binds(); }
    //: Access binds associated with solution.

  };
}


#endif
