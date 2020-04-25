// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BFACC3ITER3_HEADER
#define RAVL_BFACC3ITER3_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/BfAcc3Iter3.hh"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! lib=RavlCore
//! userlevel=Advanced
//! author="Charles Galambos"

#include "Ravl/BfAccIter3.hh"
#include "Ravl/BfAcc2Iter3.hh"
#include "Ravl/IndexRange1d.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: 3D buffer iterator.
  
  template <class Data1T,class Data2T,class Data3T>
  class BufferAccess3dIter3C {
  public:
    BufferAccess3dIter3C()
    {}
    //: Default constructor.
    
    BufferAccess3dIter3C(const BufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size1a,SizeT size2a,SizeT size3a,
			 const BufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size1b,SizeT size2b,SizeT size3b,
			 const BufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,SizeT size1c,SizeT size2c,SizeT size3c)
    { First(pbufa,size1a,size2a,size3a,
	    pbufb,size1b,size2b,size3b,
	    pbufc,size1c,size2c,size3c); 
    }
    //: Constructor.
    
    BufferAccess3dIter3C(const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size2a,SizeT size3a,
			 const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size2b,SizeT size3b,
			 const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,SizeT size2c,SizeT size3c)
    { First(pbufa,size2a,size3a,
	    pbufb,size2b,size3b,
	    pbufc,size2c,size3c);
    }
    //: Constructor.
    
    BufferAccess3dIter3C(const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,IndexRangeC nrng2a,IndexRangeC nrng3a,
			 const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,IndexRangeC nrng2b,IndexRangeC nrng3b,
			 const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,IndexRangeC nrng2c,IndexRangeC nrng3c)
    { First(pbufa,nrng2a,nrng3a,
	    pbufb,nrng2b,nrng3b,
	    pbufc,nrng2c,nrng3c); 
    }
    //: Constructor.
    
    bool First(const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,IndexRangeC nrng2a,IndexRangeC nrng3a,
	       const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,IndexRangeC nrng2b,IndexRangeC nrng3b,
	       const RangeBufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,IndexRangeC nrng2c,IndexRangeC nrng3c
	       ) {
      rng2a = nrng2a;
      rng3a = nrng3a;
      rng2b = nrng2b;
      rng3b = nrng3b;
      rng2c = nrng2c;
      rng3c = nrng3c;
      rit.First(pbufa,pbufa.Range(),
		pbufb,pbufb.Range(),
		pbufc,pbufc.Range());
      if(rit.IsElm())
	return sit.First(rit.Data1(),rng2a,rng3a,
			 rit.Data2(),rng2b,rng3b,
			 rit.Data3(),rng2c,rng3c);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    bool First(const BufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size1a,SizeT size2a,SizeT size3a,
	       const BufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size1b,SizeT size2b,SizeT size3b,
	       const BufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,SizeT size1c,SizeT size2c,SizeT size3c
	       ) {
      rng2a = IndexRangeC(size2a);
      rng3a = IndexRangeC(size3a);
      rng2b = IndexRangeC(size2b);
      rng3b = IndexRangeC(size3b);
      rng2c = IndexRangeC(size2c);
      rng3c = IndexRangeC(size3c);
      rit.First(pbufa,size1a,
		pbufb,size1b,
		pbufc,size1c);
      if(rit.IsElm())
	return sit.First(rit.Data1(),size2a,size3a,
			 rit.Data2(),size2b,size3b,
			 rit.Data3(),size2c,size3c);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    bool First(const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data1T> > > &pbufa,SizeT size2a,SizeT size3a,
	       const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data2T> > > &pbufb,SizeT size2b,SizeT size3b,
	       const SizeBufferAccessC<BufferAccessC<BufferAccessC<Data3T> > > &pbufc,SizeT size2c,SizeT size3c) {
      rng2a = IndexRangeC(0,size2a-1);
      rng3a = IndexRangeC(0,size3a-1);
      rng2b = IndexRangeC(0,size2b-1);
      rng3b = IndexRangeC(0,size3b-1);
      rng2c = IndexRangeC(0,size2c-1);
      rng3c = IndexRangeC(0,size3c-1);
      rit.First(pbufa,pbufa.Size(),
		pbufb,pbufb.Size(),
		pbufc,pbufc.Size());
      if(rit.IsElm())
	return sit.First(rit.Data1(),size2a,size3a,
			 rit.Data2(),size2b,size3b,
			 rit.Data3(),size2c,size3c);
      sit.Invalidate();
      return false;
    }
    //: Goto first element in the array
    
    void SliceStart() 
    { sit.First(rit.Data1(),rng2a,rng3a,
		rit.Data2(),rng2b,rng3b,
		rit.Data3(),rng2c,rng3c); 
    }
    //: Go back to the begining of this row.
    
    bool NextSlice() {
      rit.Next();
      if(!rit.IsElm())
	return false;
      sit.First(rit.Data1(),rng2a,rng3a,
		rit.Data2(),rng2b,rng3b,
		rit.Data3(),rng2c,rng3c);
      return true;      
    }
    //: Go to the beginning of the next row.
    // returns true if iterator is left at the beginning of a valid row or
    // false if the end of the array has been reached.
    
    bool Next() { 
      sit.Next();
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
      sit.Next();
      if(!sit.IsElm())
	NextRow();
    }
    //: Goto next element.
    
    void operator++(int) {  
      sit.Next();
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

    Data3T &Data3() 
    { return sit.Data3(); }
    //: Access data of current element
    
    const Data3T &Data3() const
    { return sit.Data3(); }
    //: Access data of current element
    
  protected:
    bool NextRow();
    
    BufferAccessIter3C<BufferAccessC<BufferAccessC<Data1T> >, BufferAccessC<BufferAccessC<Data2T> >, BufferAccessC<BufferAccessC<Data3T> > > rit;
    BufferAccess2dIter3C<Data1T,Data2T,Data3T> sit;
    IndexRangeC rng2a;
    IndexRangeC rng3a;
    IndexRangeC rng2b;
    IndexRangeC rng3b;
    IndexRangeC rng2c;
    IndexRangeC rng3c;
  };

  template <class Data1T,class Data2T,class Data3T>
  bool BufferAccess3dIter3C<Data1T,Data2T,Data3T>::NextRow() {
    rit.Next();
    if(!rit.IsElm())
      return false;
    sit.First(rit.Data1(),rng2a,rng3a,
	      rit.Data2(),rng2b,rng3b,
	      rit.Data3(),rng2c,rng3c);
    return true; 
  }

}


#endif
