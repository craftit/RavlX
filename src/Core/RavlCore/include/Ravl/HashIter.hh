// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HASHITERT_HEADER
#define RAVL_HASHITERT_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Hash/HashIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="1/9/1996"
//! example=exHash.cc
//! docentry="Ravl.API.Core.Hash Tables"
//! rcsid="$Id$"

#include "Ravl/Hash.hh"

namespace RavlN {
  
  template<class K, class T> class RCHashC ; 
  //: Hash table iterator.
  // This iterator works with both HashC and RCHashC. <p>
  // Small object !
  
  template<class K,class T>
  class HashIterC {
  public:
    HashIterC() 
      : hashtable(0)
    {}
    // Don't use this if you can help it.
    
    HashIterC(const HashC<K,T> &nTab) 
      : bIt(nTab.table),
	hashtable(&const_cast<HashC<K,T> &>(nTab))
    { First(); }
    // Normal constructor.
    
    HashIterC(const RCHashC<K,T> & nTab) 
      : bIt(nTab.Data().table),
        hashtable(&const_cast<HashC<K,T> &> (nTab.Data() ))
    { First() ; } 
    //: Constructor from RCHash

    HashIterC(const HashIterC<K,T> &oth) 
      : lIt(oth.lIt), 
        bIt(oth.bIt),
        hashtable(oth.hashtable)
    {} 
    // Copy constructor.

    bool First(void)  { 
      bIt.First(); 
      RavlAssert(bIt);
      lIt = *bIt; 
      return CheckValid(); 
    }
    // Goto first item in table.
    
    bool Next(void) { 
      lIt.Next(); 
      return CheckValid(); 
    }
    // Goto next item in table.
    // Once this returns false (or IsElm() is false) this 
    // should not be called again.
    
    void operator++(int) { 
      lIt.Next(); 
      CheckValid(); 
    }
    //: Goto next element.
    
    void operator++() { 
      lIt.Next(); 
      CheckValid(); 
    }
    //: Goto next element.
    
    bool IsElm() const 
    { return lIt.IsElm(); }
    // Pointing to valid element ?
    
    operator bool() const
    { return lIt.IsElm(); }
    //: At a valid element ?
    
    const K &Key(void) const 
    { return lIt.Data().GetKey(); }
    // Key for current item. IsElm() Must be true.
    
    const T &Data(void) const 
    { return lIt.Data().Data(); }
    //: Data for current item.IsElm() Must be true.
    
    T &Data(void) 
    { return const_cast<T &>(lIt.Data().Data()); }
    //: Data for current item.IsElm() Must be true.
    
    T &operator*() 
    { return const_cast<T &>(lIt.Data().Data()); }
    //: Access data.
    
    const T &operator*() const
    { return lIt.Data().Data(); }
    //: Access data.
    
    T *operator->() 
    { return &const_cast<T &>(lIt.Data().Data()); }
    //: Access data.
    
    const T *operator->() const
    { return &(lIt.Data().Data()); }
    //: Access data.
    
    bool Del(void);
    //: Delete current item from table; move to next.

    // Returns true if at a valid element.  If you are deleting elements in a typical
    // "for(; it; it++)" construct, use <code>DelNoInc()</code> instead to ensure the
    // element after the one you deleted isn't skipped.

    void DelNoInc(void);
    //: Delete current item from table; don't move to next element.

    // Use this method rather than <code> Del(void);</code> if using within a
    // normal "for(; it; it++)" construct, in order to avoid moving the
    // iterator on twice.<br>

    // Note: It leaves the iterator in an undefined state, so something like
    // <code>operator++</code> or <code>Next() must be used for the iterator
    // to be valid again.

    HashIterC<K,T> &operator=(const HashC<K,T> &oth) { 
      bIt = oth.table;
      hashtable = &const_cast<HashC<K,T> &>(oth);
      First();
      return *this; 
    }
    // Assign to another table.
    
    HashIterC<K,T> &operator=(const HashIterC<K,T> &oth)  { 
      bIt = oth.bIt; 
      lIt = oth.lIt; 
      hashtable = oth.hashtable;
      return *this; 
    }
    // Assign to another iterator.
    
  protected:
    bool CheckValid(void);
    
  private:
    IntrDLIterC<HashElemC<K,T> > lIt; // List iterator.
    SArray1dIterC<IntrDListC<HashElemC<K,T> > > bIt;  // Current bin.
    HashC<K,T> *hashtable;
  };
  
  //////////////////////////////////
  // Check current iterator is valid, if not
  // move to next valid one.
  
  template<class K,class T>
  bool HashIterC<K,T>::CheckValid(void) {
    while(!lIt.IsElm()) {
      RavlAssert(bIt);
      bIt.Next();
      if(!bIt)
	return false;
      lIt = *bIt;
    }
    return true;
  }
  
  ///////////////////////////////////
  // Delete current item from table, move to next.
  
  template<class K,class T>
  bool HashIterC<K,T>::Del(void) {
    lIt.Del();// Delete old member from list.
    hashtable->CheckDel(false); // Make sure element count is decremented, but don't resize the table.
    return Next(); // Goto next element.
  }
  
  ///////////////////////////////////
  //: Delete current item from table, move to last or current list header.
  // Inc must be used after use for the iterator to be valid again.

  template<class K,class T>
  void HashIterC<K,T>::DelNoInc(void) {
    lIt.Del();// Delete old member from list.
    hashtable->CheckDel(false); // Make sure element count is decremented, but don't resize the table.
  }

}

#endif

