// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HSETCEXTRAS_HEADER
#define RAVL_HSETCEXTRAS_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! file="Ravl/Core/Container/Hash/HSetExtra.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Hash Tables"
//! date="31/07/1998"
//! rcsid="$Id$"

#include "Ravl/HSet.hh"
#include "Ravl/DList.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {

  //: Put every thing in a list into a set.
  
  template<class DataT>
  HSetC<DataT> DList2HSet(const DListC<DataT> &list) { 
    HSetC<DataT> set;
    for(ConstDLIterC<DataT> it(list);it.IsElm();it.Next())
      set += it.Data();
    return set;
  }
  
  //: Put every thing in a set into a list.
  
  template<class DataT>
  DListC<DataT> HSet2DList(const HSetC<DataT> &set) { 
    DListC<DataT> list;
    // The Set isn't modified by this function, hence the const.
    for(HSetIterC<DataT> it(const_cast<HSetC<DataT> &>(set));
	it.IsElm();it.Next())
      list.InsLast(it.Data());
    return list;
  }
  
  
  //: Add a list of items to a set.
  
  template<class DataT>
  const HSetC<DataT> operator+=(HSetC<DataT> &set,const DListC<DataT> &list) { 
    for(ConstDLIterC<DataT> it(list);it.IsElm();it.Next())
      set += it.Data();
    return set;
  }
  
  //: Remove a list of items from a set.
  
  template<class DataT>
  const HSetC<DataT> operator-=(HSetC<DataT> &set,const DListC<DataT> &list) { 
    for(ConstDLIterC<DataT> it(list);it.IsElm();it.Next())
      set -= it.Data();
    return set;
  }
  
  //: Add a set to a list .
  
  template<class DataT>
  const DListC<DataT> operator+=(DListC<DataT> &list,const HSetC<DataT> &set) { 
    // The Set isn't modified by this function, hence the const.
    for(HSetIterC<DataT> it(const_cast<HSetC<DataT> &>(set));
	it.IsElm();it.Next())
      list.InsLast(it.Data());
    return list;
  }
  
}
#endif
