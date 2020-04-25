// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RBFACCITER_HEADER
#define RAVL_RBFACCITER_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAccIter.hh"
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
  //: Iterator for access buffer.
  
  template <class DataT>
  class BufferAccessIterC {
  public:
    inline BufferAccessIterC();
    //: Default constructor.
    
    inline BufferAccessIterC(const BufferAccessC<DataT> &buff,const IndexRangeC &rng)
    { First(buff,rng); }
    //: Constructor.
    
    inline BufferAccessIterC(const BufferAccessC<DataT> &buff,SizeT size)
    { First(buff,size); }
    //: Constructor.

    inline BufferAccessIterC(const RangeBufferAccessC<DataT> &buff)
    { First(buff); }
    //: Construct iterator at 'element' of buffer 'buff'.
    // Element must be within buff.
    
    inline BufferAccessIterC(const SizeBufferAccessC<DataT> &buff)
    { First(buff); }
    //: Construct iterator at 'element' of buffer 'buff'.
    // Element must be within buff.
    
    inline BufferAccessIterC(DataT &element,const RangeBufferAccessC<DataT> &buff)
    { First(element,buff); }
    //: Constructor.

    inline BufferAccessIterC(DataT &element,const SizeBufferAccessC<DataT> &buff)
    { First(element,buff); }
    //: Constructor.
    
    inline BufferAccessIterC<DataT> &operator=(const RangeBufferAccessC<DataT> &buff);
    //: Assignment to a buffer.

    inline BufferAccessIterC<DataT> &operator=(const SizeBufferAccessC<DataT> &buff);
    //: Assignment to a buffer.
    
    inline bool First(const BufferAccessC<DataT> &buff,const IndexRangeC &rng);
    //: Goto fist element.
    
    inline bool First(const BufferAccessC<DataT> &buff,UIntT size);
    //: Goto fist element.
    
    inline bool First(const RangeBufferAccessC<DataT> &buff);
    //: Goto fist element.

    inline bool Last(const RangeBufferAccessC<DataT> &buff);
    //: Goto last valid element in range

    inline bool First(const SizeBufferAccessC<DataT> &buff);
    //: Goto fist element.
    
    inline bool First(DataT &element,const RangeBufferAccessC<DataT> &buff);
    //: Set iterator at 'element' of buffer 'buff'.
    
    inline bool First(DataT &element,const SizeBufferAccessC<DataT> &buff);
    //: Set iterator at 'element' of buffer 'buff'.
    
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
    { return at < endOfRow; }
    //: At valid element ?
    
    inline bool operator!() const
    { return at >= endOfRow; }
    //: Not at valid element ?
    
    inline void Next()
    { RavlAssert(at != endOfRow); at++; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.

    inline void Next(int skip)
    { at += skip; }
    //: Advance 'skip' elements.
    // WARNING: When using negative values, positions before the beginning of the array will
    // not be detected correctly by IsElm().
    
    inline BufferAccessIterC<DataT> & operator+=(int skip)
    { Next(skip); return *this; }
    //: Advance 'skip' elements.
    // WARNING: When using negative values, positions before the beginning of the array will
    // not be detected correctly by IsElm().
    
    inline BufferAccessIterC<DataT> & operator-=(int skip)
    { Next(-skip); return *this; }
    //: Go back 'skip' elements.
    // WARNING: When using positive values, positions before the beginning of the array will
    // not be detected correctly by IsElm().
    
    inline void operator++(int) 
    { RavlAssert(at != endOfRow); at++; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    inline BufferAccessIterC<DataT> &operator++()
    { RavlAssert(at != endOfRow); at++; return *this; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.

    inline void operator--(int)
    { RavlAssert(at != endOfRow); at--; }
    //: Goto previous element.
    // WARNING: When using positive values, positions before the beginning of the array will
    // not be detected correctly by IsElm().

    inline BufferAccessIterC<DataT> &operator--()
    { RavlAssert(at != endOfRow); at--; return *this; }
    //: Goto previous element.
    // WARNING: When using positive values, positions before the beginning of the array will
    // not be detected correctly by IsElm().

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

    inline DataT *DataPtr()
    { return at; }
    //: Access data.
    
    inline const DataT *DataPtr() const
    { return at; }
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
    
    inline bool operator>(const BufferAccessIterC<DataT> &oth) const
    { return at > oth.at; }
    //: Is this at a position passed 'oth' ?
    
    inline bool operator<(const BufferAccessIterC<DataT> &oth) const
    { return at < oth.at; }
    //: Is this at a position before 'oth' ?

    inline bool operator>=(const BufferAccessIterC<DataT> &oth) const
    { return at >= oth.at; }
    //: Is this at a position passed or equal to 'oth' ?
    
    inline bool operator<=(const BufferAccessIterC<DataT> &oth) const
    { return at <= oth.at; }
    //: Is this at a position before or equal to 'oth' ?

    inline bool operator==(const BufferAccessIterC<DataT> &oth) const
    { return at == oth.at; }
    //: Is this at the same position as 'oth' ?
    
    IntT operator-(const BufferAccessIterC<DataT> &oth)
    { return at - oth.at; }
    //: Return the difference in position of the iterators.

    DataT &operator[](IntT n)
    { return at[n]; }
    //: Return the value at a index relative to the iterator.
    // Note: Is it the users responsibility to ensure this is a valid element.

    DataT &operator[](IndexC n)
    { return at[n.V()]; }
    //: Return the value at a index relative to the iterator.
    // Note: Is it the users responsibility to ensure this is a valid element.
        
  protected:
    DataT *at;
    const DataT *endOfRow;
  };
  
  //////////////////////////////////////////////////////
  
  template <class DataT>
  inline 
  BufferAccessIterC<DataT>::BufferAccessIterC()
    : at(0), 
      endOfRow(0)
  {}
  
  template <class DataT>
  inline 
  bool BufferAccessIterC<DataT>::First(const BufferAccessC<DataT> &buff,const IndexRangeC &rng) {
    if(rng.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(&buff[rng.Min()]);
    endOfRow = &(at[rng.Size()]);
    return true;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterC<DataT>::First(const BufferAccessC<DataT> &buff,UIntT size) {
    if(size <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(buff.ReferenceElm());
    endOfRow = &(at[size]);
    return true;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterC<DataT>::First(const RangeBufferAccessC<DataT> &buff) {
    if(buff.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(&buff[buff.IMin()]);
    endOfRow = &(at[buff.Size()]);
    return true;
  }

  template <class DataT>
  inline
  bool BufferAccessIterC<DataT>::Last(const RangeBufferAccessC<DataT> &buff) {
    if(buff.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(&buff[buff.IMax()]);
    endOfRow = &(at[1]);
    return true;
  }

  template <class DataT>
  inline 
  bool BufferAccessIterC<DataT>::First(const SizeBufferAccessC<DataT> &buff) {
    if(buff.Size() <= 0) {
      at = 0;
      endOfRow = 0;
      return false;
    }
    at = const_cast<DataT *>(buff.ReferenceElm());
    endOfRow = &(at[buff.Size()]);
    return true;
  }

  template <class DataT>
  inline
  bool BufferAccessIterC<DataT>::First(DataT &element,const RangeBufferAccessC<DataT> &buff) {
    at = &element;
    RavlAssert(at >= &(buff.ReferenceElm()[buff.IMin().V()]));
    endOfRow = &(buff.ReferenceElm()[buff.Size()]);
    return at < endOfRow;
  }
  
  template <class DataT>
  inline 
  bool BufferAccessIterC<DataT>::First(DataT &element,const SizeBufferAccessC<DataT> &buff) {
    at = &element;
    RavlAssert(at >= &(buff.ReferenceElm()));
    endOfRow = &(buff.ReferenceElm()[buff.IMax().V()+1]);
    return at < endOfRow;
  }
  
  template <class DataT>
  inline 
  BufferAccessIterC<DataT> &
  BufferAccessIterC<DataT>::operator=(const RangeBufferAccessC<DataT> &buff) { 
    First(buff); 
    return *this; 
  }
  
  template <class DataT>
  inline 
  BufferAccessIterC<DataT> &
  BufferAccessIterC<DataT>::operator=(const SizeBufferAccessC<DataT> &buff) { 
    First(buff); 
    return *this; 
  }
  
  template <class DataT>
  inline 
  void 
  BufferAccessIterC<DataT>::Invalidate() { 
    at = 0;
    endOfRow = 0; 
  }
  
}

#endif
