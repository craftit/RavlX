// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BHASHITER_HEADER
#define RAVL_BHASHITER_HEADER 1
/////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Branch"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Branch/BHashIter.hh"
//! author="Charles Galambos"

#include "Ravl/BHash.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  
  template<class KeyT,class DataT> 
  class BHashIterC {
  public:
    BHashIterC()
    {}
    //: Default constructor.
    
    BHashIterC(const BHashC<KeyT,DataT> &hashtab)
      : it(hashtab.table)
    { First(); }
    //: Constructor.
    
    bool First();
    //: Goto first element in table.
    // Returns false if no element was found.
    
    bool IsElm() const
    { return lit.IsElm(); }
    //: Is iterator at a valid element ?
    
    operator bool() const
    { return IsElm(); }
    //: Is iterator at a valid element ?
    
    const DataT &Data() const
    { return lit->Data(); }
    //: Access current data.
    
    const KeyT &Key() const
    { return lit->Key(); }
    //: Access current data.
    
    bool Next();
    //: Goto next element.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    
  protected:
    //: Goto next valid element.
    
    SArray1dIterC<BListC<BHashEntryC<KeyT,DataT> > > it; // Table iterator.
    BListIterC<BHashEntryC<KeyT,DataT> > lit; // list iterator.
    HSetC<KeyT> done; // Set of key;s we've seen.
  };
  
  template<class KeyT,class DataT> 
  bool BHashIterC<KeyT,DataT>::First() {
    it.First();
    if(!it)
      return false;
    lit = *it;
    done.Empty();
    while(!lit) {
      it++;
      if(!it)
	return false; // End of table.
      lit = *it;
    }
    done += lit.Data().Key();
    return true;
  }
  
  template<class KeyT,class DataT> 
  bool BHashIterC<KeyT,DataT>::Next() {
    do {
      lit++;
      while(!lit) {
	it++;
	if(!it)
	  return false; // End of table.
	lit = *it;
      }
    } while(done[lit.Data().Key()]);
    done += lit.Data().Key();
    return true;
  }
  
}

#endif
