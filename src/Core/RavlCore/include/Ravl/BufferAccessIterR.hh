// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFFERACCESSITERR_HEADER
#define RAVL_BUFFERACCESSITERR_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BufferAccessIterR.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Core.Arrays.Buffer"

#include "Ravl/BufferAccess.hh"
#include "Ravl/Assert.hh"
#include "Ravl/IndexRange1d.hh"

namespace RavlN {

  template <class DataT> class RangeBufferAccessC ;
  template <class DataT> class SizeBufferAccessC ;

  class IndexRangeC;
  
  //! userlevel=Advanced
  //: Reverse iterator for access buffer.
  
  template <class DataT>
  class BufferAccessIterRC {
  public:
    inline BufferAccessIterRC()
      : at(0), 
	endOfRow(0)
    {}
    //: Default constructor.
    
    inline BufferAccessIterRC(const BufferAccessC<DataT> &buff,const IndexRangeC &rng)
    { Last(buff,rng); }
    //: Constructor.
    
    inline BufferAccessIterRC(const BufferAccessC<DataT> &buff,UIntT size)
    { Last(buff,size); }
    //: Constructor.
    
    inline BufferAccessIterRC(const RangeBufferAccessC<DataT> &buff)
    { Last(buff); }
    //: Constructor.

    inline BufferAccessIterRC(const SizeBufferAccessC<DataT> &buff)
    { Last(buff); }
    //: Constructor.
    
    inline BufferAccessIterRC<DataT> &operator=(const RangeBufferAccessC<DataT> &buff);
    //: Assignment to a buffer.

    inline BufferAccessIterRC<DataT> &operator=(const SizeBufferAccessC<DataT> &buff);
    //: Assignment to a buffer.
    
    inline bool Last(const BufferAccessC<DataT> &buff,const IndexRangeC &rng);
    //: Goto fist element.
    
    inline bool Last(const BufferAccessC<DataT> &buff,UIntT size);
    //: Goto fist element.
    
    inline bool Last(const RangeBufferAccessC<DataT> &buff);
    //: Goto fist element.
    
    inline bool Last(const SizeBufferAccessC<DataT> &buff);
    //: Goto fist element.
    
    inline bool IsElm() const
    { return at < endOfRow; }
    //: At valid element ?
    
    inline bool IsLast() const
    { return (at+1) == endOfRow; }
    //: Test if we're at the last valid element in the range.
    // Note: This is slightly slower than IsElm().
    
    void SetEnd(int n) { 
      const DataT *newEnd = &(at[n]); 
      RavlAssert(newEnd <= endOfRow);
      endOfRow = newEnd;
    }
    //: Set the end of iteration 'n' items after this one.
    // Note: the end must be less than or equal to the old iteration
    // end point.
    
    inline operator bool() const
    { return at > endOfRow; }
    //: At valid element ?
    
    inline bool operator!() const
    { return at <= endOfRow; }
    //: Not at valid element ?
    
    inline void Prev()
    { RavlAssert(at != endOfRow); at--; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    inline void Prev(int skip)
    { at -= skip; }
    //: Advance 'skip' elements.
    // Call ONLY if you know this will not go past the end of the array.
    
    inline void operator--(int) 
    { RavlAssert(at != endOfRow); at--; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    inline void operator--() 
    { RavlAssert(at != endOfRow); at--; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    DataT &operator*() 
    { return *at; }
    //: Access data.
    
    const DataT &operator*() const
    { return *at; }
    //: Access data.
    
    DataT *operator->() 
    { return at; }
    //: Access data.
    
    const DataT *operator->() const
    { return at; }
    //: Access data.
    
    inline DataT &Data()
    { return *at; }
    //: Access data.
    
    inline const DataT &Data() const
    { return *at; }
    //: Access data.
    
    inline DataT &Data1()
    { return *at; }  
    //: Access data.
    // Equivelent to .Data(), for compatability with other iterators.
    
    inline const DataT &Data1() const
    { return *at; }
    //: Const access data.
    // Equivelent to .Data(), for compatability with other iterators.
    
    inline void Invalidate();
    //: Make IsElm() return false.
    
  protected:
    DataT *at;
    const DataT *endOfRow;
  };
  
  //////////////////////////////////////////////////////
  
  template <class DataT>
  inline 
  bool BufferAccessIterRC<DataT>::Last(const BufferAccessC<DataT> &buff,const IndexRangeC &rng) {
    if(rng.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(&buff[rng.Max()]);
    endOfRow = at-rng.Size();
    return true;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterRC<DataT>::Last(const BufferAccessC<DataT> &buff,UIntT size) {
    if(size <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    DataT *ref = const_cast<DataT *>(buff.ReferenceElm());
    at = &ref[size-1];
    endOfRow = &(ref[-1]);
    return true;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterRC<DataT>::Last(const RangeBufferAccessC<DataT> &buff) {
    if(buff.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(&buff[buff.IMax()]);
    endOfRow = at-buff.Size();
    return true;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterRC<DataT>::Last(const SizeBufferAccessC<DataT> &buff) {
    if(buff.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    DataT *ref = const_cast<DataT *>(buff.ReferenceElm());
    at = &ref[buff.Size()-1];
    endOfRow = &(ref[-1]);
    return true;
  }
  
  template <class DataT>
  inline 
  BufferAccessIterRC<DataT> &BufferAccessIterRC<DataT>::operator=(const RangeBufferAccessC<DataT> &buff) { 
    Last(buff); 
    return *this; 
  }
  
  template <class DataT>
  inline 
  BufferAccessIterRC<DataT> &BufferAccessIterRC<DataT>::operator=(const SizeBufferAccessC<DataT> &buff) { 
    Last(buff); 
    return *this; 
  }
  
  template <class DataT>
  inline 
  void BufferAccessIterRC<DataT>::Invalidate() { 
    at = 0;
    endOfRow = 0; 
  }
  
}

#endif
