// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXITER_HEADER
#define RAVLLOGIC_LITERALINDEXITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexIter.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/LiteralMapIter.hh"
#include "Ravl/HashIter.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Iterate through the entire contents of LiteralIndexC.
  
  class LiteralIndexBaseIterC 
  {
  public:
    LiteralIndexBaseIterC()
    {}
    //: Default constructor
    
    LiteralIndexBaseIterC(const LiteralIndexBaseC &ind)
      : hit(ind.Body().map)
    {}
    //: Constructor
    
    bool Next()
    { return hit.Next(); }
    //: Goto next element.
    // returns true if its valid.
    
    void operator++(int)
    { hit.Next(); }
    //: Goto next element

    bool IsElm() const 
    { return hit.IsElm(); }
    //: At a valid element ?
    
    operator bool() const
    { return hit.IsElm(); }
    //: At a valid element ?
    
    const LiteralC &Key(void) const 
    { return hit.Key(); }
    //: Get key for current item.
    
    LiteralIndexLeafC &Data()
    { return hit.Data(); }
    //: Access data.

    const LiteralIndexLeafC &Data() const
    { return hit.Data(); }
    //: Access data.

    bool First()
      { return hit.First(); }
    //: Goto first element.
    
  protected:
    void Dummy();
    // Dummy function to give the compiler
    // a hint about where it can generate code.
    
    HashIterC<LiteralC,LiteralIndexLeafC>  hit;
  };

  //! userlevel=Normal
  //: Iterate through the entire contents of LiteralIndexC.
  
  template<class DataT>
  class LiteralIndexIterC 
    : public LiteralMapIterBodyC<DataT>,
      public LiteralIndexBaseIterC
  {
  public:
    LiteralIndexIterC()
    {}
    //: Default constructor.

    LiteralIndexIterC(const LiteralIndexC<DataT> &ind)
      : LiteralIndexBaseIterC (ind)
    {}
    //: Constructor.
    
    virtual LiteralC Data()
      { return Key(); }
    //: At a valid element.
    
    virtual bool Next()
      { return LiteralIndexBaseIterC::Next(); }
    //: Goto next item.

    virtual bool IsElm() const
      { return LiteralIndexBaseIterC::IsElm(); }
    //: At valid element ?
    
    virtual bool First()
      { return LiteralIndexBaseIterC::First(); }
    //: Goto first element in map
    
    DataT *operator->() 
    { return &Data(); }
    //: Access data.
    
    virtual DataT &MappedData() {
      LiteralIndexLeafDataC<DataT> t(LiteralIndexBaseIterC::Data());
      return t.Data();
    }
    //: Access data.
    
    virtual const DataT &MappedData() const {
      LiteralIndexLeafDataC<DataT> t(LiteralIndexBaseIterC::Data());
      return t.Data();
    }
    //: Access data.
    
  protected:
  };

  
}

#endif
