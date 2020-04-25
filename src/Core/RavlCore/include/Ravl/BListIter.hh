// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLISTITER_HEADER
#define RAVL_BLISTITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Branch"
//! file="Ravl/Core/Container/Branch/BListIter.hh"

#include "Ravl/BList.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: BList iterator.
  
  template<class DataT>
  class BListIterC {
  public:
    BListIterC()
      : place(0)
    {}
    //: Default constructor.
    
    BListIterC(const BListC<DataT> &list)
      : place(0)
    {
      if(list.list.IsValid())
	place = const_cast<BLinkBodyC<DataT> *>(list.list.BodyPtr());
    }
    //: Construct from list.

    BListIterC(const BLinkC<DataT> &link)
      : place(0)
    {
      if(link.IsValid())
	place = const_cast<BLinkBodyC<DataT> *>(link.BodyPtr());
    }
    //: Construct from a link.
    
    const BListIterC<DataT> &operator=(const BListC<DataT> &l) {
      if(l.list.IsValid())
	place = const_cast<BLinkBodyC<DataT> *>(l.list.BodyPtr());
      else
	place = 0;
      return *this;
    }
    //: Assign to a list.

    const BListIterC<DataT> &operator=(const BLinkC<DataT> &link) {
      if(link.IsValid())
	place = const_cast<BLinkBodyC<DataT> *>(link.BodyPtr());
      else
	place = 0;
      return *this;
    }
    //: Assign to a link.

    bool IsElm() const
    { return place != 0; }
    //: Are we at a valid element in the list ?
    
    operator bool() const
    { return place != 0; }
    //: Are we at a valid element in the list ?
    
    DataT &Data()
    { return place->Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return place->Data(); }
    //: Access data.
    
    const DataT &operator*() const
    { return place->Data(); }
    //: Access data.
    
    DataT &operator*()
    { return place->Data(); }
    //: Access data.
    
    DataT *operator->()
    { return &place->Data(); }
    //: Access data.
    
    const DataT *operator->() const
    { return &place->Data(); }
    //: Access data.
    
    void Next() 
    { place = place->Next().BodyPtr(); }
    //: Goto next element in the list.
    
    bool IsLast() const
    { return place->Next().BodyPtr() == 0; }
    //: Is this the last element in the list?
    
    void operator++(int)
    { Next(); }
    //: Goto next element in the list.
  protected:
    BLinkBodyC<DataT> *place;
  };
  
}
#endif
