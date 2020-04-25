// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SIZEBUFFERACCESS_HEADER 
#define RAVL_SIZEBUFFERACCESS_HEADER 1
/////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Buffer/SBfAcc.hh"
//! lib=RavlCore
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! date="14/02/1997"
//! userlevel=Develop

#include "Ravl/IndexRange1d.hh"
#include "Ravl/BufferAccess.hh"
#include "Ravl/Types.hh"
#include "Ravl/TFVector.hh"

namespace RavlN {
  template <class DataT> class SizeBufferAccessC;
  template <class DataT> class Slice1dC;
  
  class BinOStreamC;
  class BinIStreamC;
  
  //: Basic access to buffer with limited size
  // The class SizeBufferAccessC enables to random indexed access to
  // a sequentially organised continous part of memory called buffer.
  // The access functions check if an accessed element is valid only in
  // debug mode.
  
  template <class DataT>
  class SizeBufferAccessC 
    : public BufferAccessC<DataT>
  {
  public:
    typedef DataT ElementT;
    
    // Constructors, copies, assigment, and destructor
    // -----------------------------------------------
    
    inline SizeBufferAccessC()
      : sz(0)
    {}
    //: Default constructor.
    
    inline SizeBufferAccessC(DataT * bp, SizeT size = 0);
    // Creates an access to a buffer pointed by the pointer 'bp'. If
    // 'bp' is 0, the access is not valid.
    
    inline SizeBufferAccessC(const BufferAccessC<DataT> & bp,SizeT s);
    // Creates an access to a buffer referenced by 'bp' with size 's'.
    
    inline SizeBufferAccessC(const SizeBufferAccessC<DataT> & ba);
    // Creates a physical copy of the access 'ba'.
    
    inline SizeBufferAccessC(const SizeBufferAccessC<DataT> & ba,SizeT s);
    // Creates a physical copy of the access 'ba' limited by new size 's'.
    
    inline const SizeBufferAccessC<DataT> & operator=(DataT * bp);
    // Changes the reference element to the element pointed by 'bp'.
    
    // Access to the object
    // --------------------

    inline DataT * ReferenceElm() const
    { return this->buff; }
    // Returns the pointer to the reference element of the attached buffer.
    // The reference element need not to be the valid element of the buffer.
    
    inline void * ReferenceVoid() const
    { return (void *) this->buff; }
    // Returns the pointer to the reference element of the attached buffer.
    // The reference element need not to be the valid element of the buffer.
    // The function is intended to be used in printing.
    
    inline DataT * DataStart() const
    { return this->buff; }
    // Returns the address of the first element of the buffer.
    
    inline SizeT Size() const
    { return sz; }
    // Returns the number of elements of the array.
    
    inline IndexRangeC Limits() const
    { return IndexRangeC(0,sz-1); }
    // Returns the usable range of indeces expressed by this object.
    
    inline IndexRangeC Range() const
    { return IndexRangeC(0,sz-1); }
    // Returns the usable range of indeces expressed by this object.
    
    inline IndexC IMin() const
    { return 0; }
    // Returns the minimum index of the range of this access.
    
    inline IndexC IMax() const
    { return sz-1; }
    // Returns the maximum index of the range of this access.
    
    inline const DataT  & operator[](IntT i) const {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %d out of  range 0 - %zu  ",i,static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-only access to the 'i'-th element of the buffer.     
    
    inline DataT & operator[](IntT i) {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %d out of range 0 - %zu  ",i,static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-write access  to the 'i'-th element of the buffer. 

    inline const DataT  & operator[](const SizeC &i) const {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %u out of range 0 - %zu  ",i.V(),static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-only access to the 'i'-th element of the buffer.
    
    inline DataT & operator[](const SizeC &i) {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %u out of range 0 - %zu  ",i.V(),static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-write access  to the 'i'-th element of the buffer.

    inline const DataT  & operator[](const IndexC &i) const {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %d out of range 0 - %zu  ",i.V(),static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-only access to the 'i'-th element of the buffer.     
    
    inline DataT & operator[](const IndexC &i) {
#if RAVL_CHECK
      if (!Contains(i))
        IssueError(__FILE__,__LINE__,"Index %d out of range 0 - %zu  ",i.V(),static_cast<size_t>(Size()-1));
#endif
      return BufferAccessC<DataT>::operator[](i);
    }
    // Read-write access  to the 'i'-th element of the buffer. 
    
    inline const SizeBufferAccessC<DataT> & SAccess(void) const
    { return *this; }
    // Returns this object.
    
    // Logical functions
    // -----------------
    
    inline bool IsEmpty() const
    { return sz == 0; }
    // Returns TRUE if the size of the array is zero.
    
    inline bool Contains(IndexC i) const
    { return ((SizeT) i.V()) < sz; }
    // Returns TRUE if the array contains an item with the index 'i'.
    
    // Modifications of the access
    // ---------------------------
    
    inline SizeT ShrinkHigh(SizeT k) { 
      RavlAssert(k <= sz);
      sz -= k;
      return sz;
    }
    // Changes the number of elements by subtracting the last 'k' elements.
    
    inline 
    const SizeBufferAccessC<DataT> & Swap(SizeBufferAccessC<DataT> & a);
    // Exchanges the contents of this buffer with buffer 'a'.
    
    inline void Attach(const SizeBufferAccessC<DataT> & b);
    // Changes this buffer access to have the same access rights as 'b'.
    
    inline void Attach(const BufferAccessC<DataT> & b,
		       SizeT size);
    // Changes this buffer access to have the access rights as 'b' limited
    // for 'size' elements.
    
    inline SizeBufferAccessC<DataT> operator+(SizeT i) const;
    // Creates the new access object shifted 'i' elements to the right
    // (towards next elements). The size is descreased to fit the 
    // the original range of this access.
    
    // Modifications of the buffer contents
    // ------------------------------------
    
    void Fill(const DataT & d);
    // 'd' value is assigned to all elements of the buffer.
    
    void CopyFrom(const SizeBufferAccessC<DataT> &oth);
    //: Copy contents of another buffer into this one.
    // NB. Buffers MUST be the same length.
    
    void CopyFrom(const Slice1dC<DataT> &slice);
    //: Copy slice into this array.
    // slice must have the same length as this buffer. <br>
    // Implementation can be found in Slice1d.hh 
    
    template<unsigned int N>
    void CopyFrom(const TFVectorC<DataT, N> &vec) {
      RavlAssert(Size() == N);
      DataT *to = ReferenceElm();
      const DataT *from = &(vec[0]);
      const DataT *endOfRow = &to[Size()];
      for(;to != endOfRow;to++,from++)
	*to = *from;
    }
    //: Copy values from vec into this array.
    // Vector must have the same length as this buffer.
    
    void Reverse();
    //: Reverse the order of elements in this array in place.
    
    SizeBufferAccessC<DataT> BufferFrom(SizeT first);
    //: Get an access for this buffer starting from the 'first' element to the end of the buffer.
    
    SizeBufferAccessC<DataT> BufferFrom(SizeT first,SizeT len);
    //: Get an access for this buffer starting from the 'first' element for 'len' elements.
    // An error will be generated if the requested buffer isn't contains within this one.

    bool operator==(const SizeBufferAccessC<DataT> &ba) const
    { return (this->buff == ba.buff) && (this->sz == ba.sz); }
    //: Are two accesses the same ?

    template<typename FuncT>
    void ForEach(FuncT func) {
      for(SizeT i = 0;i < sz;i++)
        func((*this)[i]);
    }
    //: Apply to each element in the array

    template<typename FuncT>
    void ForEach(FuncT func) const {
      for(SizeT i = 0;i < sz;i++)
        func((*this)[i]);
    }
    //: Apply to each element in the array

    DataT *begin()
    { return DataStart(); }
    //: Access start.

    DataT *end()
    { return DataStart() + sz; }
    //: Access end

    const DataT *begin() const
    { return DataStart(); }
    //: Access start.

    const DataT *end() const
    { return DataStart() + sz; }
    //: Access end

    inline SizeT size() const
    { return sz; }
    //: Returns the number of elements of the array. c++ std compatible

  protected:
    
    // Copy
    // ----
    
    SizeBufferAccessC<DataT> Copy(void) const;
    // Returns a physical copy of this access pointing to the physical 
    // copy of the accessed buffer in the range accessible by this access.
    // The new copy is necessary to attach to reference counted buffer
    // or to delete at the end of the life of this object.
    
    SizeT sz; // size of the buffer in elements.
  };
  
  /////////////////////////////////////////////////////////////////////////////
  
  template <class DataT>
  void 
  SizeBufferAccessC<DataT>::CopyFrom(const SizeBufferAccessC<DataT> &oth) {
    RavlAssert(oth.Size() == Size());
    DataT *to = ReferenceElm();
    const DataT *from = oth.ReferenceElm();
    DataT *endOfRow = &to[Size()];
    for(;to != endOfRow;to++,from++)
      *to = *from;
  }
  
  template <class DataT>
  inline 
  SizeBufferAccessC<DataT>::SizeBufferAccessC(DataT * bp,SizeT size)
    : BufferAccessC<DataT>(bp), 
      sz(size)
  {}
  
  template <class DataT>
  inline 
  SizeBufferAccessC<DataT>::SizeBufferAccessC(const BufferAccessC<DataT> & bp,SizeT s)
    : BufferAccessC<DataT>(bp), 
      sz(s)
  {}
  
  template <class DataT>
  inline 
  SizeBufferAccessC<DataT>::
  SizeBufferAccessC(const SizeBufferAccessC<DataT> & ba)
    : BufferAccessC<DataT>(ba), 
      sz(ba.sz)
  {}
  
  template <class DataT>
  inline 
  SizeBufferAccessC<DataT>::
  SizeBufferAccessC(const SizeBufferAccessC<DataT> & ba,SizeT s)
    : BufferAccessC<DataT>(ba), 
      sz(s)
  {
#if RAVL_CHECK
    if (s > ba.Size())
      IssueError(__FILE__,__LINE__,"Size %zu out of index range 0-%zu  ",static_cast<size_t>(s) ,static_cast<size_t>(ba.Size()-1));
#endif
  }
  
  template <class DataT>
  inline 
  const SizeBufferAccessC<DataT> & 
  SizeBufferAccessC<DataT>::operator=(DataT * bp) {
    ((BufferAccessC<DataT> &) *this) = bp;
    return *this;
  }
  
  template <class DataT>
  inline 
  const SizeBufferAccessC<DataT> & 
  SizeBufferAccessC<DataT>::Swap(SizeBufferAccessC<DataT> & a) {
    BufferAccessC<DataT>::Swap(a);
    RavlN::Swap(sz,a.sz);
    return *this;
  }
  
  template <class DataT>
  inline 
  void 
  SizeBufferAccessC<DataT>::Attach(const SizeBufferAccessC<DataT> & b)
  { *this=b; }

  template <class DataT>
  inline 
  void 
  SizeBufferAccessC<DataT>::Attach(const BufferAccessC<DataT> & b,
				   SizeT size) {
    ((BufferAccessC<DataT> &)(*this)) = b;
    sz=size;
  }
  
  template <class DataT>
  inline 
  SizeBufferAccessC<DataT> 
  SizeBufferAccessC<DataT>::operator+(SizeT i) const { 
    RavlAssert(i <= sz);
    return SizeBufferAccessC<DataT>(this->buff + i, sz - i); 
  }
  
  template <class DataT>
  SizeBufferAccessC<DataT> 
  SizeBufferAccessC<DataT>::Copy(void) const {
    if (IsEmpty()) 
      return SizeBufferAccessC<DataT>();
    DataT * bp = new DataT[Size().V()];
    SizeBufferAccessC<DataT> b(bp, Size());
    DataT *at = DataStart();
    DataT *at2 = b.DataStart();
    DataT *endOfRow = &at[sz];
    for(;at != endOfRow;at++,at2++)
      *at2 = *at;
    return b;
  }
  
  template <class DataT>
  void SizeBufferAccessC<DataT>::Fill(const DataT & d) {
    DataT *at = DataStart();
    DataT *endOfRow = &at[sz.V()];
    for(;at != endOfRow;at++)
      *at = d;
  }
  
  template<class DataT>
  void SizeBufferAccessC<DataT>::Reverse() {
    // If there's zero or one element, nothing to do.
    if(sz < 2)
      return ;
    DataT *at = &((*this)[IMin()]);
    DataT *end = &((*this)[IMax()]);
    DataT tmp;
    for(;at < end;at++,end--) {
      tmp = *at;
      *at = *end;
      *end = tmp;
    }
  }

  template<class DataT>
  SizeBufferAccessC<DataT> SizeBufferAccessC<DataT>::BufferFrom(SizeT first) {
    RavlAssert(first < sz);
    return SizeBufferAccessC<DataT>(&(ReferenceElm()[first]),sz - first);
  }
  
  template<class DataT>
  SizeBufferAccessC<DataT> SizeBufferAccessC<DataT>::BufferFrom(SizeT first,SizeT len) {
    RavlAssert((first + len) <= sz);
    return SizeBufferAccessC<DataT>(&(ReferenceElm()[first]),len);
  }
  
  template <class DataT>
  std::ostream &operator<<(std::ostream &out,const SizeBufferAccessC<DataT> &dat) {
    const DataT *at = dat.DataStart();
    const DataT *endOfRow = &at[dat.Size()];
    if(dat.Size() == 0)
      return out;
    out << *at;
    at++;
    for(;at != endOfRow;at++)
      out << ' ' << *at;
    return out;
  }
  
  //: Read buffer from stream.
  // NB. The buffer must be pre-allocated.

  template <class DataT>
  std::istream &operator>>(std::istream &strm,SizeBufferAccessC<DataT> &dat) {
    DataT *at = dat.DataStart();
    DataT *endOfRow = &at[dat.Size()];
    for(;at != endOfRow;at++)
      strm >> *at;
    return strm;
  }
  
  
  //: Wrtie buffer to stream.
  // NB. This size of the buffer is NOT written.
  
  template <class DataT>
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccessC<DataT> &dat) {
    const DataT *at = dat.DataStart();
    const DataT *endOfRow = &at[dat.Size()];
    if(dat.Size() == 0)
      return out;
    for(;at != endOfRow;at++)
      out << *at;
    return out;
  }
  
  //: Read buffer from stream.
  // NB. The buffer must be pre-allocated.
  
  template <class DataT>
  BinIStreamC &operator>>(BinIStreamC &strm,SizeBufferAccessC<DataT> &dat) {
    DataT *at = dat.DataStart();
    DataT *endOfRow = &at[dat.Size()];
    for(;at != endOfRow;at++)
      strm >> *at;
    return strm;
  }
  
  //: Save real array to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccessC<RealT> &img);
  
  //: Load real array image from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccessC<RealT> &img);  
  
  //: Save float array image to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccessC<FloatT> &img);
  
  //: Load float array image from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccessC<FloatT> &img);  
  
  //: Save byte array to binary stream 
  BinOStreamC &operator<<(BinOStreamC &out,const SizeBufferAccessC<ByteT> &img);
  
  //: Load byte array from binary stream 
  BinIStreamC &operator>>(BinIStreamC &in,SizeBufferAccessC<ByteT> &img);  

}
#endif


