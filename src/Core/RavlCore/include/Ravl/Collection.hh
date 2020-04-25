// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_COLLECTION_HEADER
#define RAVL_COLLECTION_HEADER 1
//////////////////////////////////////////////////////
//! rcsid=$Id$
//! docentry="Ravl.API.Core.Misc"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/System/Collection.hh"

#include "Ravl/SArray1d.hh"
#include "Ravl/Random.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  template<class DataT> class CollectionC;
  template<class DataT> class CollectionIterC;
  template<class DataT> class DListC;
  template<class DataT> class DLIterC;
  
  //! userlevel=Develop
  //: Collection of data.
  
  template<class DataT>
  class CollectionBodyC 
    : public RCBodyC
  {
  public:
    CollectionBodyC(SizeT maxSize = 10,SizeT used = 0)
      : data(maxSize+1),
	n(used)
    {}
    //: Create an empty collection.
    // maxSize should be set to maximum number of 
    // elements the collection will contain. 'used' is the number
    // of elements to be in the collection at the time of creation.
    
    CollectionBodyC(const DListC<DataT> &list);
    //: Construct collection from a list.
    
    CollectionBodyC(const SArray1dC<DataT> &dat)
      : data(dat),
	n(dat.Size())
    {}
    //: Create a collection from an array of data.
    
    CollectionBodyC(const SArray1dC<DataT> &dat,UIntT used)
      : data(dat),
	n(used)
    { RavlAssert(used <= data.Size()); }
    //: Create a collection from an array of data, including 'used' elements.
    
    CollectionC<DataT> Copy() const;
    //: Create a copy of this collection.
    
    inline
    UIntT Insert(const DataT &dat);
    //: Add a data item to the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The index at which the item was placed is returned.
    
    UIntT Insert(const Array1dC<DataT> &dat);
    //: Add an array of data items to the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The first index at which the items were placed is returned.
    
    inline
    UIntT Append(const DataT &dat)
    { return Insert(dat); }
    //: Add a data item to the end of the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The index at which the item was placed is returned.
    
    inline UIntT Append(const Array1dC<DataT> &dat)
    { return Insert(dat); }
    //: Add an array of data items to the end of the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The first index at which the items were placed is returned.
    
    inline
    UIntT InsertRandom(const DataT &dat);
    //: Add a data item to the collection in a random place.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The index at which the item was placed is returned.
    
    inline void Delete(IndexC ind)
    { data[ind] = data[--n]; }
    //: Remove item at 'ind' from the collection.
    // Note the last item in the collection will be moved
    // to replace element at 'ind'
    
    inline void Remove(IndexC ind)
    { data[ind] = data[--n]; }
    //: Remove item at 'ind' from the collection.
    // Note the last item in the collection will be moved
    // to replace element at 'ind'.
    // Same as Delete, added for consistency with DArray1dC.
    
    DataT Pick();
    //: Pick a random item from the collection.
    // the element will be removed from the set.
    // It is the users responsibility to ensure the
    // set is not empty when this method is called.
    // See 'IsEmpty()'
    
    CollectionC<DataT> Shuffle() const;
    //: Create a shuffled version of this collection.
    
    void ShuffleIP();
    //: Shuffle collection in place.
    
    void Merge(const CollectionC<DataT> &x);
    //: Merge collection 'x' into this one.
    
    void Merge(const SArray1dC<DataT> &x);
    //: Merge of array 'x' into this collection.
    
    inline bool IsEmpty() const
    { return n == 0; }
    //: Test if collection is empty.
    
    void Empty()
    { n = 0; }
    //: Empty the collection of all its contents.
    
    SizeT Size() const
    { return n; }
    //: Returns the number of data elements in the collection.
    
    SArray1dC<DataT> Array()
    { return SArray1dC<DataT>(data,n); }
    //: Access data as array.
    // Obsolete, use SArray1d() instead. <br>
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use.  
    
    
    const SArray1dC<DataT> Array() const
    { return SArray1dC<DataT>(data,n); }
    //: Access data as array.
    // Obsolete, use SArray1d() instead. <br>
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use. 

    SArray1dC<DataT> SArray1d()
    { return SArray1dC<DataT>(data,n); }
    //: Access data as array.
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use. 
    
    const SArray1dC<DataT> SArray1d() const
    { return SArray1dC<DataT>(data,n); }
    //: Access data as array.
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use. 
    
    CollectionC<DataT> Split(SizeT ne);
    //: Split the collection in two
    // return a random set of 'ne' elements from this one.
    
    CollectionC<DataT> Sample(SizeT ne) const;
    //: Take a random sample from the collection.
    // This collection is not modified. NOTE: Items
    // may be taken more than once.

    DataT &KthHighest(UIntT k);
    //: k'th highest element of a collection
    // k should be between 0 and Size()-1. For the median use k = Size()/2.
    // The DataT type class must have the < and > operators defined on them. <p>
    // Note: This method re-orders the contents of the collection.
    
    DataT &operator[](IndexC ind) { 
      RavlAssertMsg(ind < n,"Index out of range.");
      return data[ind];
    }
    //: Array style access.
    
    const DataT &operator[](IndexC ind) const { 
      RavlAssertMsg(ind < n,"Index out of range.");
      return data[ind];
    }
    //: Array style access.
    
    DataT &Last()
    { return data[Size()-1]; }
    //: Access last element in the collection.

    const DataT &Last() const
    { return data[Size()-1]; }
    //: Access last element in the collection.

    DataT GetLast() {
      RavlAssert(n > 0);
      return data[--n];
    }
    //: Get the last item from the collection and remove it.

    DataT &First()
    { return data[0]; }
    //: Access first element in the collection.
    
    const DataT &First() const
    { return data[0]; }
    //: Access first element in the collection.
    
    DataT *begin()
    { return data.begin(); }
    //: Access start.

    DataT *end()
    { return begin() + n; }
    //: Access end

    const DataT *begin() const
    { return data.begin(); }
    //: Access start.

    const DataT *end() const
    { return begin() + n; }
    //: Access end

  protected:
    SArray1dC<DataT> data;
    SizeT n;
  };
  
  //! userlevel=Normal
  //: Collection of data
  // Simple extensible container, with properties similar to std::vector 
  
  template<class DataT>
  class CollectionC 
    : public RCHandleC<CollectionBodyC<DataT> >
  {
  public:
    CollectionC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    CollectionC(SizeT maxSize,SizeT used = 0)
      : RCHandleC<CollectionBodyC<DataT> >(*new CollectionBodyC<DataT>(maxSize,used))
    {}
    //: Create an empty collection.
    // maxSize should be set to maximum number of elements the collection 
    // will contain.  'used' is the number of elements to be in the collection 
    // at the time of creation.
    
    CollectionC(const SArray1dC<DataT> &dat)
      : RCHandleC<CollectionBodyC<DataT> >(*new CollectionBodyC<DataT>(dat))
    {}
    //: Create a collection from an array of data.
    
    CollectionC(const DListC<DataT> &list)
      : RCHandleC<CollectionBodyC<DataT> >(*new CollectionBodyC<DataT>(list))
    {}
    //: Construct collection from a list.
    
    CollectionC<DataT> Copy() const
    { return Body().Copy(); }
    //: Create a copy of this collection.
    
    CollectionC<DataT> DeepCopy(UIntT levels = ((UIntT) -1)) const {
      if(levels <= 1)
        return Copy();
      CollectionC<DataT> ret(Size());
      for(int i = 0;i < Size();i++)
        ret[i] = StdDeepCopy((*this)[i],levels-1);
      return ret;
    }
    //: Creates a new deep copy of the collection.

    inline
    UIntT Insert(const DataT &dat)
    { return Body().Insert(dat); }
    //: Add a data item to the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <br>
    // The index at which the item was placed is returned.
    
    UIntT Insert(const Array1dC<DataT> &dat)
    { return Body().Insert(dat); }
    //: Add an array of data items to the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The first index at which the items were placed is returned.
    
    inline
    UIntT Append(const DataT &dat)
    { return Body().Append(dat); }
    //: Add a data item to the end of the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The index at which the item was placed is returned.
    
    inline UIntT Append(const Array1dC<DataT> &dat)
    { return Body().Append(dat); }
    //: Add an array of data items to the end of the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection. <p>
    // The first index at which the items were placed is returned.
    
    inline
    UIntT InsertRandom(const DataT &dat)
    { return Body().InsertRandom(dat); }
    //: Add a data item to the collection in a random place.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection.
    
    void Delete(IndexC ind)
    { Body().Delete(ind); }
    //: Remove item at 'ind' from the collection.
    // Note the last item in the collection will be moved
    // to replace element at 'ind'
    
    void operator+=(const DataT &dat)
    { Body().Insert(dat); }
    //: Add data item to the collection.
    //  NB. This may cause the storage array to 
    // be reallocated which will invalidate any iterators
    // held on the collection.
    
    DataT Pick()
    { return Body().Pick(); }
    //: Pick a random item from the collection.
    // the element will be removed from the set.
    // It is the users responsibility to ensure the
    // set is not empty when this method is called.
    // See 'IsEmpty()'
    
    CollectionC<DataT> Shuffle() const
    { return Body().Shuffle(); }
    //: Create a shuffled version of this collection.
    
    void ShuffleIP()
    { Body().ShuffleIP(); }
    //: Shuffle collection in place.
    
    void Merge(const CollectionC<DataT> &x)
    { Body().Merge(x); }
    //: Merge collection 'x' into this one.
    
    void Merge(const SArray1dC<DataT> &x)
    { Body().Merge(x); }
    //: Merge of array 'x' into this collection.
    
    inline bool IsEmpty() const
    { return Body().IsEmpty(); }
    //: Test if collection is empty.
    
    void Empty()
    { Body().Empty(); }
    //: Empty the collection of all its contents.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Returns the number of data elements in the collection.
    
    SArray1dC<DataT> Array()
    { return Body().Array(); }
    //: Access data as array.
    // Obsolete, use SArray1d() instead. <br>
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use. 
    
    const SArray1dC<DataT> Array() const
    { return Body().Array(); }
    //: Access data as array.
    // Obsolete, use SArray1d() instead. <br>
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed 
    // while its in use. 
    
    SArray1dC<DataT> SArray1d()
    { return Body().SArray1d(); }
    //: Access data as array.
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed
    // while its in use.

    const SArray1dC<DataT> SArray1d() const
    { return Body().SArray1d(); }
    //: Access data as array.
    // Note: The returned array is a direct access
    // to the internal data structure, no operations
    // that modify the collection should be performed
    // while its in use.

    CollectionC<DataT> Split(SizeT ne)
    { return Body().Split(ne); }
    //: Split the collection in two
    // return a random set of 'ne' elements from this one.
    // ne must be smaller than the size of the collection.
    
    CollectionC<DataT> Sample(SizeT ne) const
    { return Body().Sample(ne); }
    //: Take a random sample from the collection.
    // This collection is not modified.  There is no
    // garantee that an element will be picked only once.
    // 'ne' may be bigger than the size of this collection.
    
    DataT &KthHighest(UIntT k)
    { return Body().KthHighest(k); }
    //: k'th highest element of a collection
    // k should be between 0 and Size()-1. For the median use k = Size()/2.
    // The DataT type class must have the < and > operators defined on them.
    // This function will reorder the collection so that elements 0...k-1
    // are <= element k and elements k+1...Size()-1 are >= element k. 
    // Based on algorithm in Numerical Recipes in C, second edition.
    // <p>
    // Note: This method re-orders the contents of the collection.

    DataT &operator[](IndexC ind)
    { return Body().operator[](ind); }
    //: Array style access.
    
    const DataT &operator[](IndexC ind) const
    { return Body().operator[](ind); }
    //: Array style access.

    DataT &Last()
    { return Body().Last(); }
    //: Access last element in the collection.

    DataT GetLast()
    { return Body().GetLast(); }
    //: Get the last item from the collection and remove it.

    const DataT &Last() const
    { return Body().Last(); }
    //: Access last element in the collection.
    
    DataT &First()
    { return Body().First(); }
    //: Access first element in the collection.
    
    const DataT &First() const
    { return Body().First(); }
    //: Access first element in the collection.
    
    template<typename FuncT>
    void ForEach(FuncT func) {
      for(int i = 0;i < (int) Size();i++)
        func((*this)[i]);
    }
    //: Apply to each element in the array

    template<typename FuncT>
    void ForEach(FuncT func) const {
      for(int i = 0;i < (int) Size();i++)
        func((*this)[i]);
    }
    //: Apply to each element in the array

    DataT *begin()
    {
      if(!this->IsValid()) return 0;
      return Body().begin();
    }
    //: Access start.

    DataT *end()
    {
      if(!this->IsValid()) return 0;
      return Body().end();
    }
    //: Access end

    const DataT *begin() const
    {
      if(!this->IsValid()) return 0;
      return Body().begin();
    }
    //: Access start.

    const DataT *end() const
    {
      if(!this->IsValid()) return 0;
      return Body().end();
    }
    //: Access end


    typedef CollectionIterC<DataT> IteratorT;
    //: Type of iterator.

    typedef DataT CollectionDataT;
  protected:
    CollectionBodyC<DataT> &Body()
    { return RCHandleC<CollectionBodyC<DataT> >::Body(); }
    //: Access body

    const CollectionBodyC<DataT> &Body() const
    { return RCHandleC<CollectionBodyC<DataT> >::Body(); }
    //: Access body
    
  };

  
  template <class DataT>
  BinOStreamC & operator<<(BinOStreamC & s, const CollectionC<DataT> &col)
  {
    if(!col.IsValid())
      s << SArray1dC<DataT>();
    else
      s << col.Array();
    return s;
  }
  
  template <class DataT>
  BinIStreamC & operator>>(BinIStreamC& s, CollectionC<DataT> &col)
  {
    SArray1dC<DataT> arr;
    s >> arr;
    col = CollectionC<DataT>(arr);
    return s;
  }
  
  template<class DataT>
  std::ostream &operator<<(std::ostream &s,const CollectionC<DataT> &d) {
    if(!d.IsValid())
      s << SArray1dC<DataT>();
    else
      s << d.Array();
    return s;
  }

  template<class DataT>
  std::istream &operator>>(std::istream &s,CollectionC<DataT> &d) {
    SArray1dC<DataT> ad;
    s >> ad;
    d = CollectionC<DataT>(ad);
    return s;
  }

  template<class DataT>
  CollectionBodyC<DataT>::CollectionBodyC(const DListC<DataT> &list)
    : data(list.Size()),
      n(0)
  {
    DataT *at = &(data[0]);
    for(DLIterC<DataT> it(list);it;it++,at++)
      *at = *it;
    n = data.Size();
  }
  
  template<class DataT>
  CollectionC<DataT> CollectionBodyC<DataT>::Copy() const {
    return CollectionC<DataT>(Array().Copy());
  }
  
  template<class DataT>
  inline
  UIntT CollectionBodyC<DataT>::Insert(const DataT &dat) {
    if(n >= data.Size())
      data = data.Copy(RavlN::Max((UIntT) data.Size(),(UIntT) 2) * 2); // Double the size of the collection.
    int i = n;
    data[n++] = dat;
    return i;
  } 

  template<class DataT>
  UIntT CollectionBodyC<DataT>::Insert(const Array1dC<DataT> &dat) {
    if( (n + dat.Size()) > data.Size() ) {
      UIntT ns = RavlN::Max((UIntT) data.Size(),(UIntT) 2) * 2;
      while(ns < (n + dat.Size()))
	ns *= 2;
      data = data.Copy(ns); // Double the size of the collection.
    }
    int i = n;
    // Copy in elements.
    for(BufferAccessIter2C<DataT,DataT> it(dat,data,0,n);it;it++)
      it.Data2() = it.Data1();
    n += dat.Size();
    return i;
  }
  

  template<class DataT>
  inline
  UIntT CollectionBodyC<DataT>::InsertRandom(const DataT &dat) {
    if(n >= data.Size())
      data = data.Copy(RavlN::Max((UIntT) data.Size(),(UIntT) 2) * 2); // Double the size of the collection.
    SizeT p = (SizeT)((RealT) Random1() * n);
    if(p > n-1)
      p = n-1; // Incase of rounding errors.
    data[n++] = data[p];
    data[p] = dat;
    return p;
  }
  
  template<class DataT>
  DataT CollectionBodyC<DataT>::Pick() {
    RavlAssert(n != 0);
    SizeT i = static_cast<SizeT>((RealT) Random1() * n);
    if(i >= n-1)
      i = n-1; // Incase of rounding errors.
    DataT ret = data[i];
    Delete(i);
    return ret;
  }

  template<class DataT>
  void CollectionBodyC<DataT>::Merge(const CollectionC<DataT> &x) {
    Merge(x.Array());
  }
  
  template<class DataT>
  void CollectionBodyC<DataT>::Merge(const SArray1dC<DataT> &x) {
    SizeT spair = data.Size() - n;
    if(spair >= x.Size()) { // Is there space in existing array ?
      // Copy in data.
      SArray1dC<DataT> rest(data,spair,n);
      for(BufferAccessIter2C<DataT,DataT> it(x,rest);it;it++)
	it.Data2() = it.Data1();
      n+= x.Size();
      return ;
    }
    data = Array().Join(x);
    n += x.Size();
  }
  
  template<class DataT>
  CollectionC<DataT> CollectionBodyC<DataT>::Split(SizeT ne) {
    RavlAssert(ne <= Size());
    CollectionC<DataT> ret(ne);
    for(SizeT i = 0;i < ne;i++)
      ret.Insert(Pick());
    return ret;
  }
  
  template<class DataT>
  CollectionC<DataT>  CollectionBodyC<DataT>::Shuffle() const {
    CollectionC<DataT> ret(n);
    BufferAccessIterC<DataT> it(data);
    for(SizeT i=0;i<n;i++,it++) {
      ret.InsertRandom(*it);
    }
    return ret;
  }

  template<class DataT>
  void CollectionBodyC<DataT>::ShuffleIP() {
    BufferAccessIterC<DataT> it(data);
    for (SizeT i = 0; i < n; i++, it++) {
      SizeT p = (SizeT) (Random1() * (RealT) n);
      if (p > n - 1)
        p = n - 1; // Incase of rounding errors.
      DataT tmp = *it;
      *it = data[p];
      data[p] = tmp;
    }
  }
  
  template<class DataT>
  CollectionC<DataT> CollectionBodyC<DataT>::Sample(SizeT ne) const {
    CollectionC<DataT> ret(ne);
    for(SizeT i = 0;i < ne;i++) {
      SizeT p = (SizeT)((RealT) Random1() * n);
      if(p > n-1)
	p = n-1; // Incase of rounding errors.
      ret.Insert(data[p]);
    }
    return ret;
  }

  template<class DataT>
  DataT& CollectionBodyC<DataT>::KthHighest(UIntT ik) {
    DataT *k = &data[ik];
    DataT *low = &data[0];
    DataT *high = &data[Size()-1];
    
    for(;;) {
      // if low and high element are the same, we're done
      if ( low == high ) {
	RavlAssert(low == k);
	return *k;
      }
      
      // if low and high are consecutive, choose one of them
      if ( low == high-1 ) {
	if ( *low > *high ) 
	  Swap(*low,*high);
	RavlAssert(low == k || high == k);
	return *k;
      }
      
      // choose a comparison element as median of low, high and middle
      // element of current sub-array
      DataT *middle = low + ((high - low) / 2);
      
      // move median element to position low+1, and swap around low and high
      // elements if necessary to maintain their order
      if ( *low < *high ) {
	if ( *high < *middle )
	  // high is the median of the three; swap with middle element
	  Swap(*high,*middle);
	else if ( *low > *middle )
	  // low is the median of the three
	  Swap(*low,*middle);
      }
      else {
	// first swap low and high to put them in order
	Swap(*low,*high);
	if ( *high < *middle )
	  // high is the median of the three; swap with middle element
	  Swap(*high,*middle);
	else if ( *low > *middle )
	  /* low is the median of the three */
	  Swap(*low,*middle);
      }

      // move median element into position low+1
      if ( middle != low+1 ) {
	Swap(*(low+1),*middle);
	middle = low+1;
      }
      
      // initialise indices of lower and upper elements
      DataT *x = middle;
      DataT *y = high;
      for(;;) {
	do x++; while (*x < *middle);
	do y--; while (*y > *middle);
	if (x>y) break; // terminate when indices cross
	Swap(*x,*y);
      }
      
      if (y != middle)
	Swap(*middle,*y); // place partitioning element into position
      
      if (y == k) return *k;
      if (y > k) high=y-1;
      if (y < k) low = (x==y+2) ? x-1 : x;
      RavlAssert(low <= k && high >= k);
    }
  }
  
  template<class DataT>
  bool operator==(const CollectionC<DataT> &c1,const CollectionC<DataT> &c2)
  {
    if(c1.VoidPtr() == c2.VoidPtr())
      return true; // They're identical.
    if(c1.IsValid() != c2.IsValid())
      return false; // They're not both valid.
    if(c1.Size() != c2.Size())
      return false;
    for(unsigned i = 0;i < c1.Size();i++) {
      if(!(c1[i] == c2[i]))
        return false;
    }
    return true;
  }

  template<class DataT>
  inline bool operator!=(const CollectionC<DataT> &c1,const CollectionC<DataT> &c2)
  { return !operator==(c1,c2); }

}

#endif
