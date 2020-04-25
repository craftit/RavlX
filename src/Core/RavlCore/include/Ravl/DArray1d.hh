// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1D_HEADER
#define RAVL_DARRAY1D_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1d.hh"

#include "Ravl/IntrDList.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DArray1dIter.hh"

namespace RavlN {
 
  template<class DataT> class DArray1dC;
  template<class DataT> class DArray1dIterC;
  
  //! userlevel=Develop  
  //: Chunk of a dynamic array.
  // Used in DArray1dC
  
  template<class DataT>
  class DChunkC 
    : public DLinkC
  {
  public:
    DChunkC()
    {}
    //: Default constructor.

    DChunkC(IndexC noff,const Array1dC<DataT> &dat)
      : data(dat,noff)
    {}
    //:Constructor.

    DChunkC(const Array1dC<DataT> &dat)
      : data(dat)
    {}
    //: Constructor.
        
    IndexC Offset() const
    { return data.Range().Min(); }
    //: Offset from start of list.
    
    IndexC &Offset()
    { return data.Range().Min(); }
    //: Offset from start of list.
    
    SizeT Size() const
    { return data.Size(); }
    //: Size of chunk.
    
    IndexC IMax() const
    { return data.IMax(); }
    //: Maximum offset used.

    IndexC IMin() const
    { return data.IMin(); }
    //: Maximum offset used.
    
    const Array1dC<DataT> &Data() const
    { return data; }
    //: Access data.

    Array1dC<DataT> &Data()
    { return data; }
    //: Access data.

    bool Contains(IndexC i) const 
    { return data.Contains(i); }
    //: Does this chunk contain the offset?
    
    DataT &Index(IndexC i) 
    { return data[i]; }
    //: Access data.
    
    const DataT &Index(IndexC i) const
    { return data[i]; }
    //: Access data.

    DataT &Nth(UIntT i) 
    { return data[data.IMin() + i]; }
    //: Access nth element in the array from the first in this chunk.
    
    const DataT &Nth(UIntT i) const 
    { return data[data.IMin() + i]; }
    //: Access nth element in the array from the first in this chunk
    
    DataT &First()
    { return data[data.IMin()]; }
    //: First element in the array.
    
    const DataT &First() const
    { return data[data.IMin()]; }
    //: First element in the array.
    
    DataT &Last()
    { return data[data.IMax()]; }
    //: First element in the array.
    
    const DataT &Last() const
    { return data[data.IMax()]; }
    //: First element in the array.
    
  protected:
    Array1dC<DataT> data;
  };


  //! userlevel=Develop
  //: Dynamic arrays.
  
  template<class DataT>
  class DArray1dBodyC
    : public RCBodyC
  {
  public:
    DArray1dBodyC()
      : nextFree(0),
	allocBlocksize(1024)
    {}
    //: Default constructor.
    
    DArray1dBodyC(IndexRangeC range) 
      : nextFree(0),
	allocBlocksize(1024)
    { 
      if(range.Size() > 0)
	chunks.InsLast(*new DChunkC<DataT>(range.Min(),Array1dC<DataT>(range.Size()))); 
    }
    //: Constructor an array with a range allocated.

    
    DArray1dBodyC(SizeT size) 
      : nextFree(size),
	allocBlocksize(1024)
    { 
      if(size > 0)
	chunks.InsLast(*new DChunkC<DataT>(Array1dC<DataT>(size))); 
    }
    //: Constructor an array with size elements allocated.
    
    DArray1dBodyC(SizeT size,bool preAlloc) 
      : nextFree(0),
	allocBlocksize(1024)
    { 
      if(size > 0 && preAlloc) {
	lastBlk = Array1dC<DataT>(size);
	chunks.InsLast(*new DChunkC<DataT>(Array1dC<DataT>(lastBlk,IndexRangeC(0,-1)))); 
      } else
	nextFree = size;
    }
    //: Constructor an array with an expected size.
    // This is usefull if you know you'll be appending 'size' elements.
    
    DArray1dBodyC(const Array1dC<DataT> &arr) 
      : nextFree(0),
	allocBlocksize(1024)
    { 
      if(arr.Size() > 0)
	chunks.InsLast(*new DChunkC<DataT>(arr)); 
    }
    //: Construct from a normal array.

    DArray1dBodyC(const SArray1dC<DataT> &arr) 
      : nextFree(0),
	allocBlocksize(1024)
    { 
      if(arr.Size() > 0)
	chunks.InsLast(*new DChunkC<DataT>(arr)); 
    }
    //: Construct from a normal array.
    

    DArray1dBodyC( std::istream & stream ) 
      : nextFree(0),allocBlocksize(1024)
    {
      UIntT noChunks ; 
      stream >> noChunks ;  
      for ( IndexC cnum=1 ; cnum <= noChunks ; cnum ++ )  {
        Array1dC<DataT> arr ; 
        stream >> arr ; 
        chunks.InsLast ( *new DChunkC<DataT> (arr) ) ; 
      } 
    }
    //: Construct from a stream 
    
    DArray1dBodyC(BinIStreamC &stream) 
      : nextFree(0),allocBlocksize(1024)
    {
      UIntT noChunks ; 
      stream >> noChunks ;  
      for ( IndexC cnum=1 ; cnum <= noChunks ; cnum ++ )  {
        Array1dC<DataT> arr ; 
        stream >> arr ; 
        chunks.InsLast ( *new DChunkC<DataT> (arr) ) ; 
      } 
    }
    //: Construct from a binary stream 
    
    
    bool Save(std::ostream &strm) const;
    //: Save to stream.
    
    bool Save(BinOStreamC &strm) const;
    //: Save to stream.
    
    DArray1dC<DataT> Copy() const;
    //: Make a copy of this DArray.
    
    DataT &Index(IndexC i);
    //: Find data item with that index.
    
    const DataT &Index(IndexC i) const;
    //: Find data item with that index.
    
    DataT &Nth(UIntT i);
    //: Find the n'th entry, irrespective of index values.
    
    const DataT &Nth(UIntT i) const;
    //: Find the n'th entry, irrespective of index values.
    
    IndexC Append(const Array1dC<DataT> &newData);
    //: Append data to this array.
    // Note the data is not copied! 
    // The number of items appended is returned.
    
    IndexC Append(const DArray1dC<DataT> &newData);
    //: Append data to this array.
    // Note the data is not copied!
    // The number of items appended is returned.
    
    IndexC Append(const DataT &newData);
    //: Append data to this array.
    // Returns index of new item.
    
    DataT &Append();
    //: Append an empty data element to the array.
    // This returns a pointer to the new element.
    
    bool Remove(IndexC i);
    //: Remove single entry from the array.
    
    bool Remove(IndexC min,IndexC max);
    //: Remove entries from min to max from the array.
    // This removes entries from min to max inclusively from
    // the array.
    
    bool RemoveFirst();
    //: Remove first element from the array.
    
    bool RemoveLast();
    //: Remove last element from the array.
    
    bool Contains(IndexC i) const;
    //: Test if container contains index.
    
    void Fill(const DataT &value);
    //: Fill array with given value.

    DArray1dC<DataT> CompactFrom(IndexC start,SizeT size);
    //: Get sub array from this one.
    // If size is set to zero the rest of the array will be used.
    // The new array will be indexed from zero and continuously indexed (though not necessarily in one block)
    // This does not copy the elements, it only creates a new access to existing ones.

    DArray1dC<DataT> CompactBefore(IndexC end);
    //: Make a copy of the array of all the elements before 'end'
    // The new array will be indexed from zero and continuously indexed (though not necessarily in one block)
    // This does not copy the elements, it only creates a new access to existing ones.

    SArray1dC<DataT> SArray(bool alwaysCopy = false);
    //: Access content's of DArray as single array.
    // This assumes the array starts at zero. If it does
    // not the contents of they array maybe shifted so it does.
    // This method does not preserve index's. If there are
    // holes in the DArray they will be filled. It also should
    // be noted that the data may not be copied at all unless
    // the 'alwaysCopy' flag is set.
    
    IndexRangeC Range() const { 
      if(chunks.IsEmpty())
	return IndexRangeC(0);
      return IndexRangeC(chunks.First().IMin(),chunks.Last().IMax());
    }
    //: Get range of indexes covered by array.
    // There may be holes in the range.

    IndexC IMax() const { 
      if(chunks.IsEmpty())
	return -1;// Together with IMin this should indicate an empty array.
      return chunks.Last().IMax();
    }
    //: Maximum offset used.
    // If range is empty the results are undefined.
    
    IndexC IMin() const {
      if(chunks.IsEmpty())
	return 0; // Together with IMax this should indicate an empty array.
      return chunks.First().IMin();
    }
    //: Minimum offset used.
    // If range is empty the results are undefined.
    
    bool IsEmpty() const
    {
      for(IntrDLIterC<DChunkC<DataT> > it(chunks);it;it++)
        if(it->Data().Size() > 0)
          return false;
      return true;
    }
    //: Is array empty ?
    
    void Empty() { 
      lastBlk = Array1dC<DataT>(); // Empty last block holder.
      nextFree = 0;
      chunks.Empty(); 
    }
    //: Empty this array of all its contents.
    
    SizeT Size() const;
    //: Find the number of elements in the DArray.
    // This doesn't count holes in the array. <p>
    // At the moment this value is computed, this maybe a little
    // slow for applications, and so maybe changed later.
    
    DataT &First()
    { return chunks.First().First(); }
    //: Access first element in the array.
    
    const DataT &First() const
    { return chunks.First().First(); }
    //: Access first element in the array.
    
    DataT &Last()
    { return chunks.Last().Last(); }
    //: Access last element in the array.
    
    const DataT &Last() const
    { return chunks.Last().Last(); }
    //: Access last element in the array.
    
  protected:
    bool FindChunk(int i,IntrDLIterC<DChunkC<DataT> > &it) const;
    //: Find the chunk containing the interest.
    
    bool FindNthChunk(UIntT i,UIntT &at,IntrDLIterC<DChunkC<DataT> > &it) const;
    //: Find the chunk containing the interest.
    
    IntrDListC<DChunkC<DataT> > chunks;
    
    // Some extra stuff to make appending single items faster. 
    Array1dC<DataT> lastBlk; 
    IndexC nextFree;
    UIntT allocBlocksize;
    friend class DArray1dIterC<DataT>;
    friend class DArray1dC<DataT>;
  };

  //! userlevel=Normal
  //: Dynamic array.
  // Behaves like Array1dC, together with some of the properties of a
  // list.  Thus you can increase its size after you have created it,
  // and delete elements from it.  Random access is slightly slower than
  // Array1dC. Iterating through these Arrays with DArray1dIterC is generally fast, 
  // comparable to that of a Array1dC.
  
  template<class DataT>
  class DArray1dC
    : public RCHandleC<DArray1dBodyC<DataT> >
  {
  public:
    //:----------------
    // Type definitions.
    
    typedef DataT ElementT;
    //: Allow function templates to find type of array.
    
    typedef IndexC KeyT;
    //: Allow function templates to find type of index.
    
    typedef DArray1dIterC<DataT> IteratorT;
    //: Iterator for this container.

    //:---------------------------------------------
    // Constructors, copy, assignment, and destructor

    DArray1dC()
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>())
    {}
    //: Default constructor.
    // Creates an empty array.

    DArray1dC(IndexRangeC range)
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>(range))
    {}
    //: Range constructor.
    // Creates a array with 'range' elements allocated.
    
    DArray1dC(SizeT size)
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>(size))
    {}
    //: Size constructor.
    // Creates an array of the given size starting from index 0.
    
    DArray1dC(SizeT size,bool preAlloc)
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>(size,preAlloc))
    {}
    //: Constructor an array with an expected size.
    // This is useful if you know you'll be appending 'size' elements.  The array
    // will initial appear empty, but Append operations will be fast.
    // preAlloc is used to distinguish this constructor from DArray1dC<>(SizeT) and
    // should be set to 'true'.
    
    DArray1dC(const Array1dC<DataT> &arr)
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>(arr))
    {}
    //: Array constructor.
    // Uses 'arr' to build a dynamic array.  Note, the data is NOT copied,
    // modifications to elements of 'arr' will be visible in the new array.
    
    DArray1dC(const SArray1dC<DataT> &arr)
      : RCHandleC<DArray1dBodyC<DataT> >(*new DArray1dBodyC<DataT>(arr))
    {}
    //: SArray constructor.
    // Uses 'arr' to build a dynamic array.  Note, the data is NOT copied,
    // modifications to elements of 'arr' will be visible in the new array.
    
    DArray1dC(std::istream & stream) 
      : RCHandleC<DArray1dBodyC<DataT> > (*new DArray1dBodyC<DataT> ( stream) ) 
    {}
    //: stream constructor 
    
    DArray1dC(BinIStreamC & stream) 
      : RCHandleC<DArray1dBodyC<DataT> > (*new DArray1dBodyC<DataT> ( stream) ) 
    {}
    //: stream constructor 
    
    bool Save(std::ostream &strm) const { 
      if(this->IsValid())
	return Body().Save(strm); 
      std::cerr << "0\n";
      return true;
    }
    //: Save to stream.

    bool Save(BinOStreamC &strm) const { 
      if(this->IsValid())
	return Body().Save(strm); 
      UIntT numChunks = 0;
      strm << numChunks;
      return true;
    }
    //: Save to stream.
    
    DArray1dC<DataT> Copy() const
    { return Body().Copy(); }
    //: Make a copy of this DArray.
    
    DataT &Index(IndexC i)
    { return Body().Index(i); }
    //: Find data item with that index.

    const DataT &Index(IndexC i) const
    { return Body().Index(i); }
    //: Find data item with that index.
    
    DataT &Nth(UIntT i)
    { return Body().Nth(i); }
    //: Find the n'th entry, irrespective of index values.
    
    const DataT &Nth(UIntT i) const
    { return Body().Nth(i); }
    //: Find the n'th entry, irrespective of index values.
    
    DataT &operator[](IndexC i)
    { return Index(i); }
    //: Access element.
    
    const DataT &operator[](IndexC i) const
    { return Index(i); }
    //: Access element.

    IndexC Append(const Array1dC<DataT> &newData)
    { return Body().Append(newData); }
    //: Append data to this array.
    // Note the data is not copied!
    // The number of items appended is returned.

    IndexC Append(const SArray1dC<DataT> &newData)
    { return Body().Append(Array1dC<DataT>(newData)); }
    //: Append data to this array.
    // Note the data is not copied!
    // The number of items appended is returned.

    IndexC Append(const DArray1dC<DataT> &newData)
    { return Body().Append(newData); }
    //: Append data to this array.
    // Note the data is not copied!
    // The number of items appended is returned.
    
    IndexC Append(const DataT &newData)
    { return Body().Append(newData); }
    //: Append data to this array.
    // Returns index of new item.
    
    DataT &Append()
    { return Body().Append(); }
    //: Append an empty data element to the array.
    // This returns a pointer to the new element.
    
    bool Remove(IndexC i)
    { return Body().Remove(i); }
    //: Remove single entry from the array.
    
    bool Remove(IndexC min,IndexC max)
    { return Body().Remove(min,max); }
    //: Remove entries from min to max from the array.
    // This removes entries from min to max inclusively from
    // the array.
    
    bool RemoveFirst()
    { return Body().RemoveFirst(); }
    //: Remove first element from the array.
    
    bool RemoveLast()
    { return Body().RemoveLast(); }
    //: Remove last element from the array.
    
    bool Contains(IndexC i) const
    { return Body().Contains(i); }
    //: Test if container contains index.
    
    void Fill(const DataT &value)
    { Body().Fill(value); }
    //: Fill array with given value.

    DArray1dC<DataT> CompactFrom(IndexC start,SizeT size = 0)
    { return Body().CompactFrom(start,size); }
    //: Get sub array from this one.
    // If size is set to zero the rest of the array will be used.
    // The new array will be indexed from zero and continuously indexed (though not necessarily in one block)
    // This does not copy the elements, it only creates a new access to existing ones.

    DArray1dC<DataT> CompactBefore(IndexC end)
    { return Body().CompactBefore(end); }
    //: Make a copy of the array of all the elements before 'end'
    // The new array will be indexed from zero and continuously indexed (though not necessarily in one block)
    // This does not copy the elements, it only creates a new access to existing ones.

    IndexRangeC Range() const
    { return Body().Range(); }
    //: Get range of indexes covered by array.
    // Note, there may be holes in the range.
    
    IndexC IMax() const 
    { return Body().IMax(); }
    //: Maximum offset used.
    // If range is empty the results are undefined.
    
    IndexC IMin() const 
    { return Body().IMin(); }
    //: Minimum offset used.
    // If range is empty the results are undefined.
    
    bool IsEmpty() const
    { return Body().IsEmpty(); }
    //: Is array empty ?
    
    void Empty()
    { Body().Empty(); }
    //: Empty this array of all its contents.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Find the number of elements in the DArray.
    // This doesn't count holes in the array.

    DataT &First()
    { return Body().First(); }
    //: Access first element in the array.
    
    const DataT &First() const
    { return Body().First(); }
    //: Access first element in the array.
    
    DataT &Last()
    { return Body().Last(); }
    //: Access last element in the array.
    
    const DataT &Last() const
    { return Body().Last(); }
    //: Access last element in the array.
    
    SArray1dC<DataT> SArray(bool alwaysCopy = false)
    { return Body().SArray(alwaysCopy); }
    //: Access content's of DArray as single array.
    // This assumes the array starts at zero. If it does
    // not the contents of they array maybe shifted so it does.
    // This method does not preserve index's. If there are
    // holes in the DArray they will be filled. It also should
    // be noted that the data may not be copied at all unless
    // the 'alwaysCopy' flag is set.
    
  protected:
    bool FindChunk(int i,IntrDLIterC<DChunkC<DataT> > &it) const
    { return Body().FindChunk(i,it); }
    //: Find the chunk containing the index of interest.
    
    bool FindNthChunk(UIntT i,UIntT &at,IntrDLIterC<DChunkC<DataT> > &it) const
    { return Body().FindNthChunk(i,at,it); }    
    //: Find the chunk containing the index of interest.
    
    DArray1dBodyC<DataT> &Body()
    { return RCHandleC<DArray1dBodyC<DataT> >::Body(); }
    //: Access body
    
    const DArray1dBodyC<DataT> &Body() const
    { return RCHandleC<DArray1dBodyC<DataT> >::Body(); }
    //: Access body
    
    friend class DArray1dBodyC<DataT>;
    friend class DArray1dIterC<DataT>;
  };


  //: Output operator 
  template<class DataT>
  std::ostream &operator<<(std::ostream &s,const DArray1dC<DataT> &obj) {
    obj.Save(s);
    return s;
  }
  
  //: Input operator 
  template<class DataT> 
  std::istream & operator >> (std::istream & stream, DArray1dC<DataT> & obj) { 
    obj = DArray1dC<DataT> (stream) ; 
    return stream ; 
  }

  //: Output operator 
  template<class DataT>
  BinOStreamC &operator<<(BinOStreamC &s,const DArray1dC<DataT> &obj) {
    obj.Save(s);
    return s;
  }
  
  //: Input operator 
  template<class DataT> 
  BinIStreamC & operator >> (BinIStreamC & stream, DArray1dC<DataT> & obj) { 
    obj = DArray1dC<DataT> (stream) ; 
    return stream ; 
  }



  //: Save to stream.
  template<class DataT>
  bool DArray1dBodyC<DataT>::Save(std::ostream &s) const {
    s << chunks.Size() << "\n";
    for(IntrDLIterC<DChunkC<DataT> > it(chunks);it;it++)
      s << it->Data() << "\n";
    return true;
  }

  //: Save to stream.
  template<class DataT>
  bool DArray1dBodyC<DataT>::Save(BinOStreamC &s) const {
    UIntT numChunks = chunks.Size();
    s << numChunks;
    for(IntrDLIterC<DChunkC<DataT> > it(chunks);it;it++)
      s << it->Data();
    return true;
  }
  
  
  template<class DataT>
  bool DArray1dBodyC<DataT>::FindChunk(int i,IntrDLIterC<DChunkC<DataT> > &it) const {
    it = IntrDLIterC<DChunkC<DataT> >(chunks);
    for(;it;it++) {
      if(it->Contains(i)) {
	return true;
      }
      if(it->IMax() > i)
	return false;
    }
    return false;
  }
  
  template<class DataT>
  bool DArray1dBodyC<DataT>::FindNthChunk(UIntT i,UIntT &at,IntrDLIterC<DChunkC<DataT> > &it) const {
    it = IntrDLIterC<DChunkC<DataT> >(chunks);
    at = 0;
    for(;it;it++) {
      UIntT next = at + it->Size();
      if(next > i) return true;
      at = next;
    }
    return false;
  }
  
  template<class DataT>
  DArray1dC<DataT> DArray1dBodyC<DataT>::Copy() const {
    DArray1dC<DataT> ret;
    IntrDLIterC<DChunkC<DataT> > it(chunks);
    // FIXME:- Could try and de-fragment here.
    for(;it;it++)
      ret.Body().chunks.InsLast(*new DChunkC<DataT>(it->Data().Copy()));
    return ret;
  }
  
  template<class DataT>
  SizeT DArray1dBodyC<DataT>::Size() const {
    SizeT size = 0;
    for(IntrDLIterC<DChunkC<DataT> > it(chunks);it;it++)
      size += it->Data().Size();
    return size;
  }
  
  template<class DataT>
  DataT &DArray1dBodyC<DataT>::Index(IndexC i) {
    IntrDLIterC<DChunkC<DataT> > it;
//comment x variable if assertions are not active
#if RAVL_CHECK
    bool x = 
#endif    
    FindChunk(i.V(),it);
    RavlAssertMsg(x,"Index out of range. ");
    return it->Index(i);
  }
  
  template<class DataT>
  const DataT &DArray1dBodyC<DataT>::Index(IndexC i) const {
    IntrDLIterC<DChunkC<DataT> > it;
//comment x variable if assertions are not active
#if RAVL_CHECK
    bool x = 
#endif    
    FindChunk(i.V(),it);
    RavlAssertMsg(x,"Index out of range. ");
    return it->Index(i);
  }
  
  template<class DataT>
  DataT &DArray1dBodyC<DataT>::Nth(UIntT i) {
    IntrDLIterC<DChunkC<DataT> > it;
    UIntT at;
//comment x variable if assertions are not active
#if RAVL_CHECK
    bool x = 
#endif    
    FindNthChunk(i,at,it);
    RavlAssertMsg(x,"Index out of range. ");
    return it->Nth(i - at);
  }
  
  template<class DataT>
  const DataT &DArray1dBodyC<DataT>::Nth(UIntT i) const {
    IntrDLIterC<DChunkC<DataT> > it;
    UIntT at;
//comment x variable if assertions are not active
#if RAVL_CHECK
    bool x = 
#endif    
    FindNthChunk(i,at,it);
    RavlAssertMsg(x,"Index out of range. ");
    return it->Nth(i - at);    
  }
  
  template<class DataT>
  bool DArray1dBodyC<DataT>::Contains(IndexC i) const {
    IntrDLIterC<DChunkC<DataT> > it;
    return FindChunk(i.V(),it);
  }
  
  template<class DataT>
  void DArray1dBodyC<DataT>::Fill(const DataT &value) {
    for(IntrDLIterC<DChunkC<DataT> > it(chunks);it;it++)
      it->Data().Fill(value);
  }
  
  template<class DataT>
  IndexC DArray1dBodyC<DataT>::Append(const Array1dC<DataT> &newData) {
    if(newData.Size() == 0)
      return 0;
    if(chunks.IsEmpty())
      chunks.InsLast(*new DChunkC<DataT>(0,newData));
    else {
      do {
	DChunkC<DataT> &last = chunks.Last();
	if(last.Size() == 0) { // Is last chunk empty ?
	  delete &chunks.PopLast();
	  if(chunks.IsEmpty()) {
	    chunks.InsLast(*new DChunkC<DataT>(0,newData));
	    break;
	  }
	  continue;
	}
	chunks.InsLast(*new DChunkC<DataT>(last.IMax()+1,newData));
      } while(0);
    }
    lastBlk = Array1dC<DataT>(); // Empty last block holder.
    return newData.Size();
  }
  
  template<class DataT>
  IndexC DArray1dBodyC<DataT>::Append(const DArray1dC<DataT> &newData) {
    if(!newData.IsValid())
      return 0;
    if(newData.IsEmpty())
      return 0;
    IntrDLIterC<DChunkC<DataT> > it(newData.Body().chunks);
    RavlAssert(it); // IsEmpty() passed, it should be a valid iterator.
    // Skip any empty blocks.
    for(;it;it++) 
      if(it->Size() > 0)
	break;
    if(!it) return 0;
    IndexC size = 0;
    if(chunks.IsEmpty()) // Nothing in the current set ?
      chunks.InsLast(*new DChunkC<DataT>(it->Data()));
    else {
      do {
	DChunkC<DataT> &last = chunks.Last();
	if(last.Size() == 0) { // Is last chunk empty ?
	  delete &chunks.PopLast();
	  if(chunks.IsEmpty()) {
	    chunks.InsLast(*new DChunkC<DataT>(0,it->Data()));
	    break;
	  }
	  continue;
	}
	chunks.InsLast(*new DChunkC<DataT>(last.IMax()+1,it->Data()));
      } while(0);      
    }
    size += it->Size();
    for(it++;it;it++) {
      if(it->Size() == 0)
	continue;
      size += it->Size();
      chunks.InsLast(*new DChunkC<DataT>(chunks.Last().IMax()+1,it->Data()));
    }
    lastBlk = Array1dC<DataT>(); // Empty last block holder.
    return size;
  }
  
  template<class DataT>
  IndexC DArray1dBodyC<DataT>::Append(const DataT &newData) {
    if(lastBlk.Size() == 0) {
      IndexC imax = IMax() + 1;
      Array1dC<DataT> newBlk(imax,imax + (allocBlocksize-1));
      nextFree = imax+1;
      newBlk[imax] = newData;
      Append(Array1dC<DataT>(newBlk,IndexRangeC(imax,imax)));
      lastBlk = newBlk;
      return imax;
    }
    RavlAssert(chunks.Last().Data().Buffer() == lastBlk.Buffer());
    lastBlk[nextFree] = newData;
    if(lastBlk.IMax() == nextFree)
      lastBlk = Array1dC<DataT>(); // Empty last block holder.
    chunks.Last().Data().SetIMax(nextFree); // Must be a faster way to extend the array bounds.
    return nextFree++;
  }
  
  //: Append an empty data element to the array.
  // This returns a pointer to the new element.
  
  template<class DataT>
  DataT &DArray1dBodyC<DataT>::Append() {
    DataT *ret;
    if(lastBlk.Size() == 0) {
      IndexC imax = IMax() + 1;
      Array1dC<DataT> newBlk(imax,imax + (allocBlocksize-1));
      nextFree = imax+1;
      ret = &(newBlk[imax]);
      Append(Array1dC<DataT>(newBlk,IndexRangeC(imax,imax)));
      lastBlk = newBlk;
      return *ret;
    }
    RavlAssert(chunks.Last().Data().Buffer() == lastBlk.Buffer());
    ret = &(lastBlk[nextFree]);
    if(lastBlk.IMax() == nextFree)
      lastBlk = Array1dC<DataT>(); // Empty last block holder.
    chunks.Last().Data().SetIMax(nextFree++); // Must be a faster way to extend the array bounds.
    return *ret;
  }

  
  template<class DataT>
  bool DArray1dBodyC<DataT>::Remove(IndexC i) {
    IntrDLIterC<DChunkC<DataT> > it;
    if(!FindChunk(i.V(),it))
      return false;
    if(it->IMax() == i) { // At the end of a chunk ?
      if(it->IMin() == i) { // Is this the last element in the chunk ?
        if(it->Data().Buffer() == lastBlk.Buffer())
          lastBlk = Array1dC<DataT>(); // Empty last block holder.
	it.Del(); // If so delete it.
	return true;
      }
      it->Data().SetSubRange(it->IMin(),it->IMax()-1);
      return true;
    }
    if(it->IMin() == i) { // At the beginning of a chunk ?
      // If the block sizes were equal we wouldn't get this far.
      // so we don't have to worry about deleting blocks.
      it->Data().SetSubRange(it->IMin()+1,it->IMax());
      return true;
    }
    // Cut the chunk in two removing the element.
    Array1dC<DataT> newArr = it->Data();
    newArr.SetSubRange(i+1,it->IMax());
    it->Data().SetSubRange(it->IMin(),i-1);
    it.InsertAft(*new DChunkC<DataT>(newArr));
    return true;
  }
  
  template<class DataT>  
  bool DArray1dBodyC<DataT>::RemoveFirst() {
    if(chunks.IsEmpty())
      return false;
    DChunkC<DataT> &chunk = chunks.First();
    if(chunk.IMin() != chunk.IMax())
      chunk.Data().SetSubRange(chunk.IMin()+1,chunk.IMax());
    else
      chunks.DelFirst();
    // FIXME :- We could actually be more clever about managing the lastBlk
    lastBlk = Array1dC<DataT>(); // Empty last block holder.
    return true;    
  }

  template<class DataT>  
  bool DArray1dBodyC<DataT>::RemoveLast() {
    if(chunks.IsEmpty())
      return false;
    DChunkC<DataT> &chunk = chunks.Last();
    if(chunk.IMin() != chunk.IMax())
      chunk.Data().SetSubRange(chunk.IMin(),chunk.IMax()-1);
    else
      chunks.DelLast();
    // FIXME :- We could actually be more clever about managing the lastBlk
    lastBlk = Array1dC<DataT>(); // Empty last block holder.
    return true;
  }

  template<class DataT>  
  bool DArray1dBodyC<DataT>::Remove(IndexC min,IndexC max){
    RavlAssert(min <= max);
    IntrDLIterC<DChunkC<DataT> > it(chunks);
    for(;it;it++) {
      if(it->IMax() >= min)
	break;
    }
    if(!it || (it->IMin() > max)) // Was range missed entirely ?
      return false;
    // Cut off end of chunk after min.
    if(it->IMin() < min) { // Check we don't need to delete the whole chunk.
      // Is max within the same chunk ?
      if(it->IMax() > max) {
	Array1dC<DataT> newArr = it->Data();
	newArr.SetSubRange(max+1,it->IMax());
	it->Data().SetSubRange(it->IMin(),min-1);
	it.InsertAft(*new DChunkC<DataT>(newArr));
	return true;
      }
      it->Data().SetSubRange(it->IMin(),min-1); // Cut off end of block.
      it++; // We want this block!
    }
    // Cut out whole chunks between min and max.
    for(;it && (it->IMax() <= max);it++) {
      if(it->Data().Buffer() == lastBlk.Buffer())
        lastBlk = Array1dC<DataT>(); // Empty last block holder.
      it.Del();
    }
    // Cut off beginning of chunk before max.
    if(it && it->Contains(max))
      it->Data().SetSubRange(max+1,it->IMax());
    return true;
  }

  //: Get sub array from this one.
  // This does not copy the elements, it only creates a new access to existing ones.

  template<class DataT>
  DArray1dC<DataT> DArray1dBodyC<DataT>::CompactFrom(IndexC start,SizeT size) {
    if(size == 0)
      size = (IMax() - start)+1;
    if(size == 0)
      return DArray1dC<DataT>();
    IndexC max = (start + size)-1;
    IntrDLIterC<DChunkC<DataT> > it(chunks);
    for(;it;it++) {
      if(it->IMax() >= start)
        break;
    }
    if(!it || (it->IMin() > max)) // Was range missed entirely ?
      return DArray1dC<DataT>();
    if(it->IMax() >= max) {
      // If its within a single block, just cut out the index's we want.
      Array1dC<DataT> newArr = it->Data();
      newArr.SetSubRange(start,max);
      return DArray1dC<DataT>(newArr);
    }
    // Copy part we want from first chunk.
    DArray1dC<DataT> ret;
    ret.Append(it->Data().From(start));

    for(it++;it && (it->IMax() <= max);it++)
      ret.Append(it->Data());

    // Copy last bit of last chunk.
    if(it.IsElm()) {
      if(it->IMin() <= max) {
        Array1dC<DataT> newArr = it->Data();
        newArr.SetSubRange(it->IMin(),max);
        ret.Append(newArr);
      }
    }
    return ret;
  }

  //: Make a copy of the array of all the elements before 'end'
  template<class DataT>
  DArray1dC<DataT> DArray1dBodyC<DataT>::CompactBefore(IndexC end)
  {
    if(end <= IMin())
      return DArray1dC<DataT>();
    return CompactFrom(IMin(),end - IMin());
  }

  //: Access content's of DArray as single array.
  
  template<class DataT>    
  SArray1dC<DataT> DArray1dBodyC<DataT>::SArray(bool alwaysCopy) {
    IntrDLIterC<DChunkC<DataT> > it(chunks);
    
    // Compute the number of elements in the array.
    SizeT size = 0;
    SizeT chunks = 0;
    for(;it;it++) {
      size += it->Data().Size();
      chunks++;
    }
    // Is it an empty array ?
    if(chunks == 0 || size == 0) 
      return SArray1dC<DataT>();
    // If there's 1 chunk the operation is trivial
    it.First();
    if(chunks == 1) {
      if(alwaysCopy) // Always need a copy ?
        return it->Data().SArray1d(true).Copy();  
      return it->Data().SArray1d(true);
    }
    // We have to copy the data...
    // Go through copying the elements of the array into SArray1dC
    SArray1dC<DataT> ret(size);
    BufferAccessIterC<DataT> dit(ret);
    for(;it;it++) {
      for(BufferAccessIterC<DataT> sit(it->Data());sit;sit++,dit++)
        *dit = *sit;
    }
    return ret;
  }

}

#endif
