// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BFACC3ITER2_HEADER
#define RAVL_BFACC3ITER2_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAcc3Iter2.hh"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! lib=RavlCore
//! userlevel=Advanced
//! author="Charles Galambos"

#include "Ravl/BfAccIter2.hh"
#include "Ravl/BfAcc2Iter2.hh"
#include "Ravl/IndexRange1d.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: 3D buffer iterator.
  
  template <class Data1T,class Data2T>
  class BufferAccess3dIter2C {
  public:
    BufferAccess3dIter2C()
    {}
    //: Default constructor.
    
    BufferAccess3dIter2C(const BufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size1a,SizeT size2a,SizeT size3a,
			 const BufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size1b,SizeT size2b,SizeT size3b)
    { First(pbufa,size1a,size2a,size3a,
	    pbufb,size1b,size2b,size3b);
    }
    //: Constructor.
    
    BufferAccess3dIter2C(const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size2a,SizeT size3a,
			 const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size2b,SizeT size3b)
    { First(pbufa,size2a,size3a,
	    pbufb,size2b,size3b);
    }
    //: Constructor.
    
    BufferAccess3dIter2C(const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,IndexRangeC nrng2a,IndexRangeC nrng3a,
			 const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,IndexRangeC nrng2b,IndexRangeC nrng3b)
    { First(pbufa,nrng2a,nrng3a,
	    pbufb,nrng2b,nrng3b); 
    }
    //: Constructor.
    
    bool First(const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,IndexRangeC nrng2a,IndexRangeC nrng3a,
	       const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,IndexRangeC nrng2b,IndexRangeC nrng3b
	       ) {
      rng2a = nrng2a;
      rng3a = nrng3a;
      rng2b = nrng2b;
      rng3b = nrng3b;
      rit.First(pbufa,pbufa.Range(),
		pbufb,pbufb.Range());
      if(rit.IsElm())
	return sit.First(rit.Data1(),rng2a,rng3a,
			 rit.Data2(),rng2b,rng3b);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    bool First(const BufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size1a,SizeT size2a,SizeT size3a,
	       const BufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size1b,SizeT size2b,SizeT size3b
	       ) {
      rng2a = IndexRangeC(size2a);
      rng3a = IndexRangeC(size3a);
      rng2b = IndexRangeC(size2b);
      rng3b = IndexRangeC(size3b);
      rit.First(pbufa,size1a,
                pbufb,size1b);
      if(rit.IsElm())
	return sit.First(rit.Data1(),size2a,size3a,
	                  rit.Data2(),size2b,size3b);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array

    bool First(const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size2a,SizeT size3a,
	       const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size2b,SizeT size3b) {
      rng2a = IndexRangeC(size2a);
      rng3a = IndexRangeC(size3a);
      rng2b = IndexRangeC(size2b);
      rng3b = IndexRangeC(size3b);
      rit.First(pbufa,pbufa.Size(),
		pbufb,pbufb.Size());
      if(rit.IsElm())
	return sit.First(rit.Data1(),size2a,size3a,
			 rit.Data2(),size2b,size3b);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    void SliceStart() 
    { sit.First(rit.Data1(),rng2a,rng3a,
		rit.Data2(),rng2b,rng3b); 
    }
    //: Go back to the beginning of this row.
    
    bool NextSlice() {
      rit.Next();
      if(!rit.IsElm())
	return false;
      sit.First(rit.Data1(),rng2a,rng3a,
		rit.Data2(),rng2b,rng3b);
      return true;      
    }
    //: Go to the beginning of the next row.
    // returns true if iterator is left at the beginning of a valid row or
    // false if the end of the array has been reached.
    
    bool Next() { 
      sit++;
      if(!sit.IsElm()) {
	NextRow();
	return false;
      }
      return true;
    }
    //: Goto next element.
    // Goto next element in the array. Returns true if the element
    // is on the same row of the array. It returns false if the element is on
    // the next row or at the end of the array.
    
    bool IsElm() const
    { return sit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    operator bool() const
    { return sit.IsElm(); }
    //: Test if iterator is at a valid element.
    
    void operator++() {  
      sit++;
      if(!sit.IsElm())
	NextRow();
    }
    //: Goto next element.
    
    void operator++(int) {  
      sit++;
      if(!sit.IsElm())
	NextRow();
    }
    //: Goto next element.
    
    Data1T &Data() 
    { return sit.Data1(); }
    //: Access data of current element

    const Data1T &Data() const
    { return sit.Data1(); }
    //: Access data of current element
    
    Data1T &Data1() 
    { return sit.Data1(); }
    //: Access data of current element
    
    const Data1T &Data1() const
    { return sit.Data1(); }
    //: Access data of current element
    
    Data2T &Data2() 
    { return sit.Data2(); }
    //: Access data of current element
    
    const Data2T &Data2() const
    { return sit.Data2(); }
    //: Access data of current element
    
  protected:
    bool NextRow();
    //: Goto next row.
    
    BufferAccessIter2C<BufferAccessC<BufferAccessC<Data1T> >, BufferAccessC<BufferAccessC<Data2T> > > rit;
    BufferAccess2dIter2C<Data1T,Data2T> sit;
    IndexRangeC rng2a;
    IndexRangeC rng3a;
    IndexRangeC rng2b;
    IndexRangeC rng3b;
  };
  
  template <class Data1T,class Data2T>
  bool BufferAccess3dIter2C<Data1T,Data2T>::NextRow() {
    rit.Next();
    if(!rit.IsElm())
      return false;
    sit.First(rit.Data1(),rng2a,rng3a,
	      rit.Data2(),rng2b,rng3b);
    return true;
  }
  //: Goto next row.
  
}


#endif
