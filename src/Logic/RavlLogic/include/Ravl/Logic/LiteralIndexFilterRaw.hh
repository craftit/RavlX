// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXFILTERRAW_HEADER
#define RAVLLOGIC_LITERALINDEXFILTERRAW_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexFilterRaw.hh"

#include "Ravl/Logic/LiteralIndexFilterBase.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Index filter.
  
  class LiteralIndexFilterRawBodyC 
    : public LiteralIterBodyC,
      public LiteralIndexFilterBaseBodyC 
  {
  public:
    LiteralIndexFilterRawBodyC(const LiteralIndexBaseC &ind,const LiteralC &filter)
      : LiteralIndexFilterBaseBodyC(ind,filter)
    {}
    //: Constructor from an index and a filter.
    
    LiteralIndexFilterRawBodyC(const LiteralIndexBaseC &ind,const LiteralC &filter,BindSetC &nbinds)
      : LiteralIndexFilterBaseBodyC(ind,filter,nbinds)
    {}
    //: Constructor from an index and a filter with bindings.
    
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
    //: At a valid element.
    
    virtual BindSetC &Binds();
    //: Access binds associated with solution.
    
    virtual const BindSetC &Binds() const;
    //: Access binds associated with solution.
    
  };
  
  //! userlevel=Normal
  //: Index filter
  
  class LiteralIndexFilterRawC 
    : public LiteralIterC
  {
  public:
    LiteralIndexFilterRawC()
    {}
    //: Default constructor.
    
    LiteralIndexFilterRawC(const LiteralIndexBaseC &ind,const LiteralC &filter)
      : LiteralIterC(*new LiteralIndexFilterRawBodyC(ind,filter))
    {}
    //: Construct from and index and a filter.
    
    LiteralIndexFilterRawC(const LiteralIndexBaseC &ind,const LiteralC &filter,BindSetC &newBinds)
      : LiteralIterC(*new LiteralIndexFilterRawBodyC(ind,filter,newBinds))
    {}
    //: Construct from and index, filter and bindings.
    
  protected:
    LiteralIndexFilterRawC(LiteralIndexFilterRawBodyC &bod)
      : LiteralIterC(bod)
    {}
    //: Body constructor.
    
    LiteralIndexFilterRawBodyC &Body()
    { return static_cast<LiteralIndexFilterRawBodyC &>(LiteralIterC::Body()); }
    //: Access body.
    
    const LiteralIndexFilterRawBodyC &Body() const
    { return static_cast<const LiteralIndexFilterRawBodyC &>(LiteralIterC::Body()); }
    //: Access body.
    
  public:
    
  };  
}


#endif
