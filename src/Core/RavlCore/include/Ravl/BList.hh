// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLIST_HEADER
#define RAVL_BLIST_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Branch"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Branch/BList.hh"

#include "Ravl/Assert.hh"
#include "Ravl/BLink.hh"

namespace RavlN {
  
  template<class DataT> class BListIterC;
  
  //! userlevel=Normal
  //: Branching list.
  
  template<class DataT>
  class BListC {
  public:
    BListC()
    {}
    //: Default constructor.
    // Creates an empty list.
    
    BListC(const DataT &newData,const BListC<DataT> &originalList)
      : list(BLinkC<DataT>(newData,originalList.list))
    {}
    //: Create a copy of 'originalList' with 'newData' prepended to the begining
    // This is a quick operation, as the old list is only
    // refrence not copied.
    
    BListC<DataT> DeepCopy(UIntT levels = ((UIntT) -1)) const
    {
      if(levels <= 1) return this->Copy();
      BLinkC<DataT> ret;
      BLinkC<DataT> lastElem;
      for(const BLinkBodyC<DataT> *place = list.BodyPtr();place != 0;place = place->Next().BodyPtr()) {
	BLinkC<DataT> newElem(StdDeepCopy(place->Data(),levels-1));
	if(lastElem.IsValid())
	  lastElem.Next() = newElem;
	else 
	  ret = newElem;
	lastElem = newElem;
      }
      return ret;
    }
    
    void InsFirst(const DataT &dat) 
    { list = BLinkC<DataT>(dat,list); }
    //: Insert data into list.
    
    DataT &First() { 
      RavlAssert(list.IsValid());
      return list.Data(); 
    }
    //: Access first in list.
    
    const DataT &First() const {
      RavlAssert(list.IsValid());
      return list.Data(); 
    }
    //: Access first in list.
    
    bool IsEmpty() const
    { return !list.IsValid(); }
    //: Is list empty ?
    
    SizeT Size() const;
    //: Count the number of elements in the list.
    
    void Empty()
    { list.Invalidate(); }
    //: Remove all elements from list.
    
    void DelFirst() { 
      RavlAssert(list.IsValid()); 
      list = list.Next(); 
    }
    //: Remove first element from the stack.
    
  protected:
    BLinkC<DataT> list;
    friend class BListIterC<DataT>;
  };
  
  //: Count the number of elements in the list.
  
  template<class DataT>
  SizeT BListC<DataT>::Size() const {
    SizeT ret = 0;
    for(const BLinkBodyC<DataT> *place = list.BodyPtr();place != 0;place = place->Next().BodyPtr())
      ret++;
    return ret;
  }
  
}
#endif
