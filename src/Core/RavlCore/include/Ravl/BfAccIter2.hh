// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SBFACCITER2_HEADER
#define RAVL_SBFACCITER2_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAccIter2.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! date="10/09/1998"

#include "Ravl/RBfAcc.hh"
#include "Ravl/SBfAcc.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Iterate 2 buffers at the same time.
  
  template <class Data1T,class Data2T>
  class BufferAccessIter2C {
  public:
    inline BufferAccessIter2C()
      : at1(0), 
        at2(0), // Avoid warning
        endOfRow(0)
    {}
    //: Default constructor.
    
    inline BufferAccessIter2C(const RangeBufferAccessC<Data1T> &buff,const RangeBufferAccessC<Data2T> &buff2)
    { First(buff,buff2); }
    
    inline BufferAccessIter2C(const BufferAccessC<Data1T> &buff,const BufferAccessC<Data2T> &buff2,SizeT size)
    { First(buff,buff2,size); }
    
    inline BufferAccessIter2C(const SizeBufferAccessC<Data1T> &buff,const SizeBufferAccessC<Data2T> &buff2)
    { First(buff,buff2); }
    
    inline BufferAccessIter2C(const SizeBufferAccessC<Data1T> &buff,const SizeBufferAccessC<Data2T> &buff2,SizeT off1,SizeT off2)
    { First(buff,buff2,off1,off2); }
    //: Constructor.
    // start from off1 in the first array and off2 in the second.
    
    inline BufferAccessIter2C(const RangeBufferAccessC<Data1T> &buff,const RangeBufferAccessC<Data2T> &buff2,const IndexRangeC & range)
    { First(buff,buff2,range); }
    //: Constructor.
    // Only iterate through 'range' in both buffers.

    inline BufferAccessIter2C(const RangeBufferAccessC<Data1T> &buff,const RangeBufferAccessC<Data2T> &buff2,SizeT off1,SizeT off2)
    { First(buff,buff2,off1,off2); }
    //: Constructor.
    // Iterate through buffers starting at the given offsets off1 and off2 from the beginning of the ranges.
    
    inline bool First(const RangeBufferAccessC<Data1T> &buff,const RangeBufferAccessC<Data2T> &buff2);
    //: Goto first elements.

    inline bool First(const RangeBufferAccessC<Data1T> &buff,const RangeBufferAccessC<Data2T> &buff2,
                        const IndexRangeC &rng);
    //: Goto first elements.
    
    inline bool First(const SizeBufferAccessC<Data1T> &buff,const SizeBufferAccessC<Data2T> &buff2);
    //: Goto first elements.
    
    inline bool First(const SizeBufferAccessC<Data1T> &buff,const SizeBufferAccessC<Data2T> &buff2,
                        SizeT off1,SizeT off2);
    //: Goto first elements.
    
    inline bool First(const RangeBufferAccessC<Data1T> &buff,
                      const RangeBufferAccessC<Data2T> &buff2,SizeT off1,SizeT off2 = 0);
    //: Goto first elements.
    
    inline bool First(const BufferAccessC<Data1T> &buff,
                        const BufferAccessC<Data2T> &buff2,SizeT size);
    //: Goto first elements.


    inline bool First(const BufferAccessC<Data1T> &buff,const IndexRangeC &rng1,
                        const BufferAccessC<Data2T> &buff2,const IndexRangeC &rng2);
    //: Goto first elements.

    inline bool First(const BufferAccessC<Data1T> &buff,SizeT size1,
                        const BufferAccessC<Data2T> &buff2,SizeT size2)
    { return First(buff,IndexRangeC(size1),buff2,IndexRangeC(size2)); }
    //: Goto first elements.

    inline bool IsElm() const
    { return at1 < endOfRow; }
    //: At valid element ?
    
    inline bool IsLast() const
    { return (at1+1) == endOfRow; }
    //: Test if we're at the last valid element in the range.
    // Note: This is slightly slower than IsElm().

    inline operator bool() 
    { return at1 < endOfRow; }
    //: At valid element ?
    
    inline void Next();
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    inline void Next(int skip) {
      at1 += skip;
      at2 += skip;
    }
    //: Advance 'skip' elements.
    // Call ONLY if you know this will not go past the end of the array.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.

    BufferAccessIter2C<Data1T,Data2T> &operator++()
    { Next(); return *this; }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    inline BufferAccessIter2C<Data1T,Data2T> & operator+=(int skip)
    { Next(skip); return *this; }
    //: Advance 'skip' elements.
    // WARNING: When using negative values, positions before the beginning of the array will
    // not be detected correctly by IsElm().
    
    inline BufferAccessIter2C<Data1T,Data2T> & operator-=(int skip)
    { Next(-skip); return *this; }
    //: Go back 'skip' elements.
    // WARNING: When using positive values, positions before the beginning of the array will
    // not be detected correctly by IsElm().
    
    inline Data1T &Data1()
    { return *at1; }
    //: Access data.
    
    inline const Data1T &Data1() const
    { return *at1; }
    //: Access data.
    
    inline Data2T &Data2()
    { return *at2; }
    //: Access data.
    
    inline const Data2T &Data2() const
    { return *at2; }
    //: Access data.
    
    inline void Invalidate();
    //: Make IsElm() return false.
    
    inline bool operator>(const BufferAccessIter2C<Data1T,Data2T> &oth) const
    { return at1 > oth.at1; }
    //: Is this at a position passed 'oth' ?
    // Using position in first buffer as a reference.
    
    inline bool operator<(const BufferAccessIter2C<Data1T,Data2T> &oth) const
    { return at1 < oth.at1; }
    //: Is this at a position before 'oth' ?
    // Using position in first buffer as a reference.

    inline bool operator>=(const BufferAccessIter2C<Data1T,Data2T> &oth) const
    { return at1 >= oth.at1; }
    //: Is this at a position passed or equal to 'oth' ?
    // Using position in first buffer as a reference.
    
    inline bool operator<=(const BufferAccessIter2C<Data1T,Data2T> &oth) const
    { return at1 <= oth.at1; }
    //: Is this at a position before or equal to 'oth' ?
    // Using position in first buffer as a reference.

    inline bool operator==(const BufferAccessIter2C<Data1T,Data2T> &oth) const
    { return at1 == oth.at1; }
    //: Is this at the same position as 'oth' ?
    // Using position in first buffer as a reference.
    
    IntT operator-(const BufferAccessIter2C<Data1T,Data2T> &oth)
    { return at1 - oth.at1; }
    //: Return the difference in position of the iterators.
    // Using position in first buffer as a reference.
    
  protected:
    Data1T *at1;
    Data2T *at2;
    const Data1T *endOfRow;
  };
  
  //////////////////////////////////////////////////////
  
  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const BufferAccessC<Data1T> &buff1,
                                                     const BufferAccessC<Data2T> &buff2,
                                                     SizeT size)
  {
    if(size <= 0) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    at1 = const_cast<Data1T *>(buff1.ReferenceElm());
    at2 = const_cast<Data2T *>(buff2.ReferenceElm());
    endOfRow = &(at1[size]);
    return true;
  }

  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const BufferAccessC<Data1T> &buff1,const IndexRangeC &rng1,
                                                     const BufferAccessC<Data2T> &buff2,const IndexRangeC &rng2)
  {
    if(rng1.Size() <= 0) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(rng1.Size() <= rng2.Size());
    at1 = const_cast<Data1T *>(&buff1[rng1.Min()]);
    at2 = const_cast<Data2T *>(&buff2[rng2.Min()]);
    endOfRow = &(at1[rng1.Size()]);
    return true;
  }
  
  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const RangeBufferAccessC<Data1T> &buff1,const RangeBufferAccessC<Data2T> &buff2) {
    if(buff1.Size() <= 0) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(buff1.Size() <= buff2.Size()); 
    at1 = const_cast<Data1T *>(&buff1[buff1.IMin()]);
    at2 = const_cast<Data2T *>(&buff2[buff2.IMin()]);
    endOfRow = &(at1[buff1.Size()]);
    return true;
  }

  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const SizeBufferAccessC<Data1T> &buff1,const SizeBufferAccessC<Data2T> &buff2) {
    if(buff1.Size() <= 0) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(buff1.Size() <= buff2.Size()); 
    at1 = const_cast<Data1T *>(buff1.ReferenceElm());
    at2 = const_cast<Data2T *>(buff2.ReferenceElm());
    endOfRow = &(at1[buff1.Size()]);
    return true;
  }
  
  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const SizeBufferAccessC<Data1T> &buff1,const SizeBufferAccessC<Data2T> &buff2,SizeT off1,SizeT off2) {
    if(buff1.Size() <= off1) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(buff2.Size() > off2); 
    RavlAssert(((int) buff2.Size() - off2) >= ((int) buff1.Size() - off1));
    at1 = const_cast<Data1T *>(&buff1[off1]);
    at2 = const_cast<Data2T *>(&buff2[off2]);
    endOfRow = &(at1[buff1.Size() - off1]);
    return true;
  }

  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const RangeBufferAccessC<Data1T> &buff1,const RangeBufferAccessC<Data2T> &buff2,SizeT off1,SizeT off2) {
    if(buff1.Size() <= off1) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(buff2.Size() > off2); 
    RavlAssert(((int) buff2.Size() - off2) >= ((int) buff1.Size() - off1));
    at1 = const_cast<Data1T *>(&buff1[buff1.IMin() + off1]);
    at2 = const_cast<Data2T *>(&buff2[buff2.IMin() + off2]);
    endOfRow = &(at1[buff1.Size() - off1]);
    return true;
  }

  
  template<class Data1T,class Data2T>
  inline 
  bool BufferAccessIter2C<Data1T,Data2T>::First(const RangeBufferAccessC<Data1T> &buff1,const RangeBufferAccessC<Data2T> &buff2,const IndexRangeC & range) {
    if(range.Size() <= 0) {
      at1 = 0;
      at2 = 0; // Avoid a warning.
      endOfRow = 0;
      return false;
    }
    RavlAssert(buff1.Range().Contains(range));
    RavlAssert(buff2.Range().Contains(range));
    at1 = const_cast<Data1T *>(&buff1[range.Min()]);
    at2 = const_cast<Data2T *>(&buff2[range.Min()]);
    endOfRow = &(at1[range.Size()]);
    return true;
  }
  
  template<class Data1T,class Data2T>
  inline 
  void 
  BufferAccessIter2C<Data1T,Data2T>::Next() {
    RavlAssert(at1 < endOfRow);
    at1++;
    at2++;
  }
  
  template<class Data1T,class Data2T>
  inline 
  void 
  BufferAccessIter2C<Data1T,Data2T>::Invalidate() { 
    at1 = 0;
    endOfRow = 0; 
  }
  
}
#endif
