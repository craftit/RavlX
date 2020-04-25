// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CACHE_HEADER
#define RAVL_CACHE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="25/8/2002"
//! docentry="Ravl.API.Core.Misc"
//! file="Ravl/Core/System/Cache.hh"

#include "Ravl/Hash.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/IntrDLIter.hh"

namespace RavlN {
  template<class KeyT,class DataT> class CacheIterC;
  
  //! userlevel=Advanced
  //: Entry in the CacheC.
  
  template<class DataT>
  class CacheEntryC 
  {
  public:
    CacheEntryC(const DataT &dat)
      : data(dat)
    {}
    //: Create a cache entry.
    
    CacheEntryC()
    {}
    //: Default constructor.
    
    DataT &Data()
    { return data; }
    //: Access data.

    const DataT &Data() const
    { return data; }
    //: Access data.
    
    DLinkC &Link()
    { return lnk; }
    //: Access link.
    
    const DLinkC &Link() const
    { return lnk; }
    //: Access link.
    
  protected:
    DLinkC lnk;
    DataT data;
  };

  //! userlevel=Normal
  //: Fixed size cache class.
  // This is effectively a hash table with a 'recently used' list and a maximum size.
  // When a new item is added and the table is full, the oldest item is removed from
  // the cache. <br>
  // Note: This class is not reference counted.
  
  template<class KeyT,class DataT>
  class CacheC 
    : protected HashC<KeyT,CacheEntryC<DataT> >
  {
  public:
    CacheC(UIntT nMaxSize)
      : maxSize(nMaxSize)
    { RavlAssert(maxSize > 0); }
    //: Create a cache with the given maximum size.
    
    ~CacheC()
    { HashC<KeyT,CacheEntryC<DataT> >::Empty(); }
    //: Destructor.
    
    UIntT MaxSize() const
    { return maxSize; }
    //: Get maximum size of the cache.
    
    UIntT MaxSize(UIntT size)
    { return maxSize = size; }
    //: Set maximum size of the cache.

    UIntT Size() const
    { return HashC<KeyT,CacheEntryC<DataT> >::Size(); }
    //: Returns the number of items in the cache.
    
    void Empty()
    { HashC<KeyT,CacheEntryC<DataT> >::Empty(); }
    //: Empty the cache of all entries.
    
    bool IsEmpty() const
    { return HashC<KeyT,CacheEntryC<DataT> >::IsEmpty(); }
    //: Is cache empty ?
    
  protected:
    static HashElemC<KeyT,CacheEntryC<DataT> > *DLink2Entry(DLinkC &il)
    { return (HashElemC<KeyT,CacheEntryC<DataT> > *) (((char *) &il) - (((char *) &((HashElemC<KeyT,CacheEntryC<DataT> > *) 0)->Data().Link()) - ((char *) 0))); }
    
  public:
    bool RemoveOldest() {
      if(list.IsEmpty())
	return false;
      delete DLink2Entry(list.Last());
      this->CheckDel();
      return true;
    }
    //: Remove oldest item from the cache.
    
    bool GetOldest(KeyT &key,DataT &data) {
      if(list.IsEmpty())
	return false;
      HashElemC<KeyT,CacheEntryC<DataT> > *entry = DLink2Entry(list.Last());
      key = entry->GetKey();
      data = entry->Data().Data();
      delete entry;
      this->CheckDel();
      return true;
    }
    //: Remove oldest item from the cache.
    // If there is data in the cache the key from the removed item is assigned to parameter
    // 'key', the data to 'data' and true is returned  If the cache is empty
    // false is returned and the parameters are unchanged.
    
    bool Insert(const KeyT &key,const DataT &data,bool bump = true) {
      SizeT hashVal;
      HashElemC<KeyT,CacheEntryC<DataT> > *elem = this->LookupHV(key,hashVal);
      if(elem != 0) {
        elem->Data().Data() = data;
        if(bump) {
          elem->Data().Link().Unlink();
          list.InsFirst(elem->Data().Link());
        }
        return true; // There already.
      }
      if(bump && Size() >= maxSize)
        delete DLink2Entry(list.Last()); // Remove the oldest from the cache.
      else
        this->CheckAdd();
      const UIntT ind = hashVal % this->table.Size();
      HashElemC<KeyT,CacheEntryC<DataT> > *he = new HashElemC<KeyT,CacheEntryC<DataT> >(key,hashVal,CacheEntryC<DataT>(data));
      RavlAssert(he == DLink2Entry(he->Data().Link()));
      this->table[ind].InsFirst(*he);
      list.InsFirst(he->Data().Link()); // Put it at the end of the age list.
      return false;  // Had to be added.
    }
    //: Insert a new item into the cache.
    // Returns true if item is already in the cache (which will be updated with the new value).
    // If bump is true the item is moved to the end of the age list and if the 
    // cache is full the oldest element will be remove to make new for new items.
    
    bool Remove(const KeyT &key)
    { return HashC<KeyT,CacheEntryC<DataT> >::Del(key); }
    //: Remove item with given key from cache.
    // Returns true if item was in the cache.
    
    bool Lookup(const KeyT &key,DataT &dat) { 
      CacheEntryC<DataT> *dl = HashC<KeyT,CacheEntryC<DataT> >::Lookup(key); 
      if(dl == 0) return false;
      dat = dl->Data();
      return true;
    }
    //: Lookup data associated with the key.
    // If found the data is assigned to 'dat' and
    // true is returned. If the key isn't found  'dat' is left unchanged
    // and the return value is false. <br>
    // Note: This does not change the data's position in the use list.
    
    DataT *Lookup(const KeyT &key) { 
      CacheEntryC<DataT> *dl = HashC<KeyT,CacheEntryC<DataT> >::Lookup(key); 
      if(dl == 0) return 0;
      return &dl->Data();
    }
    //: Lookup data associated with the key.
    // Return a pointer to the data element if its in the cache.
    // Note: This does not change the data's position in the use list.
    
    bool LookupR(const KeyT &key,DataT &dat) { 
      CacheEntryC<DataT> *dl = HashC<KeyT,CacheEntryC<DataT> >::Lookup(key); 
      if(dl == 0) return false;
      dl->Link().Unlink();
      list.InsFirst(dl->Link()); // Put it at the end of the use list.
      dat = dl->Data();
      return true;
    }
    //: Lookup data associated with the key. Update use list.
    // If found the data is assigned to 'dat' and
    // true is returned. If the key isn't found  'dat' is left unchanged
    // and the return value is false. <br>
    // This also puts the item to the top of the most recently used list.
    
    bool Contains(const KeyT &key) const
    { return HashC<KeyT,CacheEntryC<DataT> >::IsElm(key); }
    //: Returns true if the key is the cache.
    
  protected:
    UIntT maxSize;
    IntrDListC<DLinkC> list;
    
    friend class CacheIterC<KeyT,DataT>;
  };
  
  //! userlevel=Normal
  //: Iterator through cache entries.
  // This goes from the youngest entries to the oldest.
  
  template<class KeyT,class DataT>
  class CacheIterC
    : public IntrDLIterC<DLinkC>
  {
  public:
    CacheIterC()
    {}
    //: Default constructor.
    
    CacheIterC(const CacheC<KeyT,DataT> &entry)
      : IntrDLIterC<DLinkC>(entry.list)
    {}
    //: Constructor iterator over cache.
    
  protected:
    static HashElemC<KeyT,CacheEntryC<DataT> > *DLink2Entry(DLinkC &il)
    { return (HashElemC<KeyT,CacheEntryC<DataT> > *) (((char *) &il) - (((char *) &((HashElemC<KeyT,CacheEntryC<DataT> > *) 0)->Data().Link()) - ((char *) 0))); }
    
    static const HashElemC<KeyT,CacheEntryC<DataT> > *DLink2Entry(const DLinkC &il)
    { return DLink2Entry(const_cast<DLinkC &>(il)); }
    
  public:
    DataT &Data()
    { return DLink2Entry(IntrDLIterC<DLinkC>::Data())->Data().Data(); }
    //: Access data.
    
    DataT *operator->()
    { return &Data(); }
    //: Access data.
    
    const KeyT &Key() const
    { return DLink2Entry(IntrDLIterC<DLinkC>::Data())->GetKey(); }
    //: Access key.
    
    void Del() 
    { delete DLink2Entry(IntrDLIterC<DLinkC>::Extract()); }
    //: Delete item from cache.
  };

}

#endif
