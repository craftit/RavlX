// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGICLITERALMAPLISTITER_HEADER
#define RAVLLOGICLITERALMAPLISTITER_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/LiteralMapListIter.hh"

#include "Ravl/Logic/LiteralMapIter.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Tuple2.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Literal map list iterator.
  
  template<class DataT>
  class LiteralMapListIterBodyC 
    : public LiteralMapIterBodyC<DataT>
  {
  public:
    LiteralMapListIterBodyC()
    {}
    //: Default constructor.
    
    virtual bool Next() {
      it.Next();
      return it.IsElm();
    }
    //: Goto next data element.
    // returns true if next element is valid.
    
    virtual bool First() {
      it.First();
      return it.IsElm();
    }
    //: Goto first data element.
    // returns true if next element is valid.
    
    virtual bool IsElm() const
    { return it.IsElm(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    virtual LiteralC Data()
    { return it.Data1(); }
    //: Get current literal.
    
    virtual DataT &MappedData() 
    { return it.Data2(); }
    //: Access data.
    
    virtual const DataT &MappedData() const 
    { return it.Data2(); }
    //: Access data.

  protected:
    DLIterC<Tuple2C<LiteralC,DataT> > it;
  };
  
  //! userlevel=Normal
  //: Literal map list iterator.
  
  template<class DataT>
  class LiteralMapListIterC 
    : public LiteralMapIterC<DataT>
  {
  public:
    LiteralMapListIterC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
  protected:
    LiteralMapListIterC(LiteralMapListIterBodyC<DataT> &bod)
      : LiteralIterC(bod)
    {}
    //: Body constructor
    
    LiteralMapListIterBodyC<DataT> &Body()
    { return static_cast<LiteralMapListIterBodyC<DataT> &>(LiteralIterC::Body()); }
    //: Access body.
    
    const LiteralMapListIterBodyC<DataT> &Body() const
    { return static_cast<const LiteralMapListIterBodyC<DataT> &>(LiteralIterC::Body()); }
    //: Access body.
  public:
    
    inline DataT &MappedData() 
    { return Body().MappedData(); }
    //: Value literal is mapped to.
    
    inline const DataT &MappedData() const
    { return Body().MappedData(); }
    //: Value literal is mapped to.

  };
  
  
}

#endif
