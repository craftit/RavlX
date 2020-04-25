// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RBFACC2ITER_HEADER
#define RAVL_RBFACC2ITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAcc2Iter.hh"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! lib=RavlCore
//! userlevel=Advanced
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/BfAccIter.hh"
#include "Ravl/IndexRange1d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Index2d.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: 2d buffer iterator.
  
  template <class DataT>
  class BufferAccess2dIterC {
  public:
    BufferAccess2dIterC()
    {}
    //: Default constructor.

    BufferAccess2dIterC(const BufferAccessC<BufferAccessC<DataT> > &pbuf,SizeT size1,SizeT size2)
    { First(pbuf,size1,size2); }
    //: Constructor.    
    
    BufferAccess2dIterC(const SizeBufferAccessC<BufferAccessC<DataT> > &pbuf,SizeT size)
    { First(pbuf,size); }
    //: Constructor.
    
    BufferAccess2dIterC(const RangeBufferAccessC<BufferAccessC<DataT> > &pbuf,const IndexRangeC &nrng2)
    { First(pbuf,nrng2); }
    //: Constructor.

    BufferAccess2dIterC(const BufferAccessC<BufferAccessC<DataT> > &pbuf,const IndexRangeC &nrng1,const IndexRangeC &nrng2)
    { First(pbuf,nrng1,nrng2); }
    //: Constructor.
    
    BufferAccess2dIterC(const BufferAccessC<BufferAccessC<DataT> > &pbuf,const IndexRange2dC &nrng)
    { First(pbuf,nrng.Range1(),nrng.Range2()); }
    //: Constructor.
    
    bool First(const BufferAccessC<BufferAccessC<DataT> > &pbuf,SizeT size1,SizeT size2) {
      rit.First(pbuf,size1);
      rng = IndexRangeC(size2);
      if(size2 > 0 && rit.IsElm()) {
	cit.First(*rit,rng);
	return true;
      }
      cit.Invalidate();
      return false;
    }
    //: Goto first element in the array.
    
    bool First(const SizeBufferAccessC<BufferAccessC<DataT> > &pbuf,SizeT size) {
      rit = pbuf;
      rng = IndexRangeC(size);
      if(rng.Size() > 0 && rit.IsElm()) {
	cit.First(*rit,rng);
	return true;
      }
      cit.Invalidate();
      return false;
    }
    //: Goto first element in the array.

    bool First(const BufferAccessC<BufferAccessC<DataT> > &pbuf,const IndexRangeC &nrng1,const IndexRangeC &nrng2) 
    { return First(RangeBufferAccessC<BufferAccessC<DataT> >(nrng1,pbuf),nrng2); }
    //: Goto first element in the array.
    
    bool First(const RangeBufferAccessC<BufferAccessC<DataT> > &pbuf,const IndexRangeC &nrng) {
      rit = pbuf;
      rng = nrng;
      if(rng.Size() > 0 && rit.IsElm()) {
	cit.First(*rit,rng);
	return true;
      }
      cit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    void RowStart() {
      cit.First(*rit,rng);
    }
    //: Go back to the begining of this row.
    
    void SetColEnd(int n)
    { cit.SetEnd(n); }
    //: End the iteration of the current row n elements from current place.
    // 'n' must be less than or equal to the number of elements left. 
    
    bool NextRow() {
      rit.Next();
      if(!rit.IsElm())
	return false;
      cit.First(rit.Data(),rng);
      return true;      
    }
    //: Go to the begining of the next row.
    // Returns true if the iterator is begining of a valid row, and false
    // if it is at the end of the array.
    
    bool NextRow(IndexC off) {
      rit.Next();
      if(!rit.IsElm())
	return false;
      IndexC s1 = rng.Min() + off;
      if(s1 > rng.Max())
	return false;
      RavlAssert(off >= 0);
      cit.First(rit.Data(),IndexRangeC(s1,rng.Max()));
      return true;      
    }
    //: Goto 'offset' column's in on the next row.
    // Returns true if the iterator is in a valid row, and false
    // if it is at the end of the array.
    
    bool SkipRow(IntT offset) {
      rit.Next(offset);
      if(!rit.IsElm())
	return false;
      cit.First(rit.Data(),rng);
      return true;
    }
    //: Skip 'offset' rows. 
    // Leaves the iterator 'offset' rows further down the image, in the first column. <br>
    // Returns true if the iterator is left on a valid element.
    
    void NextCol()
    { cit.Next(); }
    //: Goto next column, without checking for row change.
    // Use with care.

    void NextCol(int skip)
    { cit.Next(skip); }
    //: Go forward 'skip' columns, without checking for row change.
    // Use with care.
    
    bool IsColElm() const
    { return cit.IsElm(); }
    //: Is column element ?
    
    bool Next() { 
      cit.Next();
      if(cit.IsElm())
	return true;
      CNextRow();
      return false;
    }
    //: Goto next element.
    // Goto next element in the array. Returns true if the element
    // is on the same row of the array. It returns false if the element is on
    // the next row or at the end of the array.
    
    bool IsElm() const
    { return cit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    operator bool() const
    { return cit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    void operator++() { 
      cit++;
      if(!cit.IsElm())
	CNextRow();
    }
    //: Goto next element.
    
    void operator++(int) {
      cit++;
      if(!cit.IsElm())
	CNextRow();      
    }
    //: Goto next element.
    
    void operator+=(UIntT n) {
      cit.Next(n);
      if(!cit.IsElm())
	CNextRow(n);
    }
    //: Goto next element when subsampling by a factor of n.
    //  That is, when used to iterate through a 2D array, it will subsample rows and columns by a factor of n
    
    DataT &operator*() 
    { return *cit; }
    //: Access data of current element
    
    const DataT &operator*() const
    { return *cit; }
    //: Access data of current element
    
    DataT *operator->() 
    { return &(*cit); }
    //: Access data of current element
    
    const DataT *operator->() const
    { return &(*cit); }
    //: Access data of current element
    
    DataT &Data() 
    { return *cit; }
    //: Access data of current element

    const DataT &Data() const
    { return *cit; }
    //: Access data of current element

    DataT &Data1() 
    { return *cit; }
    //: Access data of current element
    
    const DataT &Data1() const
    { return *cit; }
    //: Access data of current element
    
    RangeBufferAccessC<DataT> Row()
    { return RangeBufferAccessC<DataT>(rng,*rit); }
    //: Access row we're currently iterating.
    
    const RangeBufferAccessC<DataT> Row() const
    { return RangeBufferAccessC<DataT>(rng,*rit); }
    //: Access row we're currently iterating.
    
    IntT RowIndex(const BufferAccessC<DataT> *row1Begin) const
    { return (IntT) (&(*rit) - row1Begin); }
    //: Work out the current row.
    
    IntT ColIndex() const
    { return (IntT) (&(*cit) - rit->ReferenceElm()); }
    //: Work out the current column.
    
    Index2dC Index(const BufferAccessC<DataT> *rowBegin) const { 
      return Index2dC((IntT) (&(*rit) - rowBegin),
		      (IntT) (&(*cit) - rit->ReferenceElm()));
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
		  
    void Invalidate()
    { cit.Invalidate(); }
    //: Invalidate this iterator.
    
  protected:
    void CNextRow();
    //: Non inlined next row method to encourage the compiler to get inlining of Next() right.

    void CNextRow(IntT n);
    //: Non inlined next row method to encourage the compiler to get inlining of Next() right.
    
    BufferAccessIterC<BufferAccessC<DataT> > rit;
    BufferAccessIterC<DataT> cit;
    IndexRangeC rng;
  };

  
  template <class DataT>
  void BufferAccess2dIterC<DataT>::CNextRow() {
    rit.Next();
    if(rit.IsElm())
      cit.First(rit.Data(),rng);
  }
  
  template <class DataT>
  void BufferAccess2dIterC<DataT>::CNextRow(IntT n) {
    rit.Next(n);
    if(rit.IsElm())
      cit.First(rit.Data(),rng);
  }
  
}


#endif
