// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RBFACC2ITER4_HEADER
#define RAVL_RBFACC2ITER4_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAcc2Iter4.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! userlevel=Default
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/BfAccIter4.hh"
#include "Ravl/Index2d.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Iterate through a 2d buffer.
  
  template <class Data1T,class Data2T,class Data3T,class Data4T>
  class BufferAccess2dIter4C {
  public:
    BufferAccess2dIter4C()
    {}
    //: Default constructor.
    
    BufferAccess2dIter4C(const SizeBufferAccessC<BufferAccessC<Data1T> > &pbuf1,SizeT size1,
			 const SizeBufferAccessC<BufferAccessC<Data2T> > &pbuf2,SizeT size2,
			 const SizeBufferAccessC<BufferAccessC<Data3T> > &pbuf3,SizeT size3,
			 const SizeBufferAccessC<BufferAccessC<Data4T> > &pbuf4,SizeT size4
			 )
    { First(pbuf1,size1,pbuf2,size2,pbuf3,size3,pbuf4,size4); }
    //: Constructor.
    
    BufferAccess2dIter4C(const RangeBufferAccessC<BufferAccessC<Data1T> > &pbuf1,const IndexRangeC &nrng1,
			 const RangeBufferAccessC<BufferAccessC<Data2T> > &pbuf2,const IndexRangeC &nrng2,
			 const RangeBufferAccessC<BufferAccessC<Data3T> > &pbuf3,const IndexRangeC &nrng3,
			 const RangeBufferAccessC<BufferAccessC<Data4T> > &pbuf4,const IndexRangeC &nrng4
			 )
    { First(pbuf1,nrng1,pbuf2,nrng2,pbuf3,nrng3,pbuf4,nrng4); }
    //: Constructor.


    bool First(const RangeBufferAccessC<BufferAccessC<Data1T> > &pbuf1,const IndexRangeC &nrng1,
	       const RangeBufferAccessC<BufferAccessC<Data2T> > &pbuf2,const IndexRangeC &nrng2,
	       const RangeBufferAccessC<BufferAccessC<Data3T> > &pbuf3,const IndexRangeC &nrng3,
	       const RangeBufferAccessC<BufferAccessC<Data4T> > &pbuf4,const IndexRangeC &nrng4
	       ) {
      rit.First(pbuf1,pbuf2,pbuf3,pbuf4);
      rng1 = nrng1;
      rng2 = nrng2;
      rng3 = nrng3;
      rng4 = nrng4;
      if(rng1.Size() > 0 && rit.IsElm())
	return cit.First(rit.Data1(),rng1,rit.Data2(),rng2,rit.Data3(),rng3,rit.Data4(),rng4); 
      cit.Invalidate();
      return false;
    }
    //: Goto first element.
    // returns true if there is one.
    
    bool First(const SizeBufferAccessC<BufferAccessC<Data1T> > &pbuf1,SizeT size1,
	       const SizeBufferAccessC<BufferAccessC<Data2T> > &pbuf2,SizeT size2,
	       const SizeBufferAccessC<BufferAccessC<Data3T> > &pbuf3,SizeT size3,
	       const SizeBufferAccessC<BufferAccessC<Data4T> > &pbuf4,SizeT size4
	       ) {
      rit.First(pbuf1,pbuf2,pbuf3,pbuf4);
      rng1 = IndexRangeC(size1);
      rng2 = IndexRangeC(size2);
      rng3 = IndexRangeC(size3);
      rng4 = IndexRangeC(size4);
      if(rng1.Size() > 0 && rit.IsElm())
	return cit.First(rit.Data1(),rng1,
			 rit.Data2(),rng2,
			 rit.Data3(),rng3,
			 rit.Data4(),rng4); 
      cit.Invalidate();
      return false;
    }
    //: Goto first element.
    // returns true if there is one.
    
    bool Next() { 
      cit.Next();
      if(cit.IsElm())
	return true;
      rit.Next();
      if(!rit.IsElm())
	return false;
      cit.First(rit.Data1(),rng1,
		rit.Data2(),rng2,
		rit.Data3(),rng3,
		rit.Data4(),rng4);
      return false;
    }
    //: Goto next element.
    // returns true if on the same row.
    
    void NextCol(int skip)
    { cit.Next(skip); }
    //: Go forward 'skip' columns, without checking for row change.
    // Use with care.
    
    bool SkipRow(IntT offset) {
      rit.Next(offset);
      if(!rit.IsElm())
	return false;
      cit.First(rit.Data1(),rng1,
		rit.Data2(),rng2,
		rit.Data3(),rng3,
		rit.Data4(),rng4);
      return true;
    }
    //: Skip 'offset' rows. 
    // Leaves the iterator 'offset' rows further down the image, in the first column. <br>
    // Returns true if the iterator is left on a valid element.
    
    bool NextRow() {
      rit.Next();
      if(!rit.IsElm())
	return false;
      cit.First(rit.Data1(),rng1,
		rit.Data2(),rng2,
		rit.Data3(),rng3,
		rit.Data4(),rng4);
      return true;      
    }
    //: Go to the begining of the next row.
    // Returns true if the iterator is begining of a valid row, and false
    // if it is at the end of the array.
    
    bool IsElm() const
    { return cit.IsElm(); }
    //: At a valid element ?
    
    operator bool() const
    { return cit.IsElm(); }
    //: At a valid element ?
    
    void operator++() 
    { Next(); }
    //: Goto next element.

    void operator++(int) 
    { Next(); }
    //: Goto next element.
    
    Data1T &Data1() 
    { return cit.Data1(); }
    //: Access data from array 1.

    const Data1T &Data1() const
    { return cit.Data1(); }
    //: Access data from array 1.

    Data2T &Data2() 
    { return cit.Data2(); }
    //: Access data from array 2.

    const Data2T &Data2() const
    { return cit.Data2(); }
    //: Access data from array 2.
    
    Data3T &Data3()
    { return cit.Data3(); }
    //: Access data from array 3.

    const Data3T &Data3() const
    { return cit.Data3(); }
    //: Access data from array 3.

    Data4T &Data4()
    { return cit.Data4(); }
    //: Access data from array 4.

    const Data4T &Data4() const
    { return cit.Data4(); }
    //: Access data from array 4.
    
    IntT RowIndex(const BufferAccessC<Data1T> *row1Begin) const
    { return (IntT) (&(rit.Data1()) - row1Begin); }
    //: Work out the current row number
    
    IntT ColIndex() const
    { return (IntT) (&(cit.Data1()) - rit.Data1().ReferenceElm()); }
    //: Work out the current column number
    
    Index2dC Index(const BufferAccessC<Data1T> *row1Begin) const { 
      return Index2dC((IntT) (&(rit.Data1()) - row1Begin),
		      (IntT) (&(cit.Data1()) - rit.Data1().ReferenceElm()));
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
#if 0
#endif
    
  protected:
    BufferAccessIter4C<BufferAccessC<Data1T>,BufferAccessC<Data2T>,BufferAccessC<Data3T>,BufferAccessC<Data4T> > rit;
    BufferAccessIter4C<Data1T,Data2T,Data3T,Data4T> cit;
    IndexRangeC rng1;
    IndexRangeC rng2;
    IndexRangeC rng3;
    IndexRangeC rng4;
  };
}

#endif

