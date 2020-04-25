// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXFILTER_HEADER
#define RAVLLOGIC_LITERALINDEXFILTER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexFilter.hh"

#include "Ravl/Logic/LiteralIndexFilterBase.hh"
#include "Ravl/Logic/LiteralIndex.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Index filter.
  
  template<class DataT>
  class LiteralIndexFilterBodyC 
    : public LiteralMapIterBodyC<DataT>,
      public LiteralIndexFilterBaseBodyC 
  {
  public:
    LiteralIndexFilterBodyC(const LiteralIndexC<DataT> &ind,const LiteralC &filter)
      : LiteralIndexFilterBaseBodyC(ind,filter)
    {}
    //: Constructor from an index and a filter.
    
    LiteralIndexFilterBodyC(const LiteralIndexC<DataT> &ind,const LiteralC &filter,BindSetC &nbinds)
      : LiteralIndexFilterBaseBodyC(ind,filter,nbinds)
    {}
    //: Constructor from an index and a filter with bindings.
    
    virtual bool Next()
    { return LiteralIndexFilterBaseBodyC::Next(); }
    //: Goto next data element.
    // returns true if next element is valid.
    
    virtual bool First()
    { return LiteralIndexFilterBaseBodyC::First(); }
    //: Goto first data element.
    // returns true if next element is valid.
    
    virtual bool IsElm() const
    { return LiteralIndexFilterBaseBodyC::IsElm(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    virtual LiteralC Data()
    { return LiteralIndexFilterBaseBodyC::Data().Key(); }
    //: At a valid element.

    virtual BindSetC &Binds()
      { return binds; }
    //: Access binds associated with solution.
    
    virtual const BindSetC &Binds() const
      { return binds; }
    //: Access binds associated with solution.
    
    virtual DataT &MappedData()
    { return LiteralIndexLeafDataC<DataT>(LiteralIndexFilterBaseBodyC::Data()).Data(); }
    //: Access data.
    
    virtual const DataT &MappedData() const
    { return LiteralIndexLeafDataC<DataT>(LiteralIndexFilterBaseBodyC::Data()).Data(); }
    //: Access data.
    
  protected:
    
  };
  
  //! userlevel=Normal
  //: Index filter
  
  template<class DataT>
  class LiteralIndexFilterC 
    : public LiteralMapIterC<DataT>
  {
  public:
    LiteralIndexFilterC()
    {}
    //: Default constructor.
    
    LiteralIndexFilterC(const LiteralIndexC<DataT> &ind,const LiteralC &filter)
      : LiteralMapIterC<DataT>(*new LiteralIndexFilterBodyC<DataT>(ind,filter))
    {}
    //: Construct from and index and a filter.
    
    LiteralIndexFilterC(const LiteralIndexC<DataT> &ind,const LiteralC &filter,BindSetC &newBinds)
      : LiteralMapIterC<DataT>(*new LiteralIndexFilterBodyC<DataT>(ind,filter,newBinds))
    {}
    //: Construct from and index, filter and bindings.
    
  protected:
    LiteralIndexFilterC(LiteralMapIterBodyC<DataT> &bod)
      : LiteralMapIterC<DataT>(bod)
    {}
    //: Body constructor.
    
    LiteralMapIterBodyC<DataT> &Body()
    { return static_cast<LiteralMapIterBodyC<DataT> &>(LiteralIterC::Body()); }
    //: Access body.
    
    const LiteralMapIterBodyC<DataT> &Body() const
    { return static_cast<const LiteralMapIterBodyC<DataT> &>(LiteralIterC::Body()); }
    //: Access body.
    
  public:
    
  };
  
}


#endif
