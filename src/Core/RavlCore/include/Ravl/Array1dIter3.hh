// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY1ITER3_HEADER
#define RAVL_ARRAY1ITER3_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/Array/Array1dIter3.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/08/99"
//! userlevel=Default

#include "Ravl/Array1d.hh"
#include "Ravl/BfAccIter3.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: triple Array1dC iterator.
  // The first array in the triple controls the number of elements visited.
  
  template<class Data1T,class Data2T,class Data3T>
  class Array1dIter3C 
    : public BufferAccessIter3C<Data1T,Data2T,Data3T>
  {
  public:
    Array1dIter3C()
    {}
    //: Default constructor.
    
    Array1dIter3C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,const Array1dC<Data3T> &arr3,bool matching = true)
      : BufferAccessIter3C<Data1T,Data2T,Data3T>(arr1,arr2,arr3),
	dat1(arr1),
	dat2(arr2),
	dat3(arr3)
    {
      if(matching) {
	RavlAssertMsg(arr1.Range().Size() == arr2.Range().Size(),"Array1dIter3C, First and second array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr3.Range().Size(),"Array1dIter3C, First and third array sizes don't match.");
      } else {
	RavlAssertMsg(arr1.Range().Size() <= arr2.Range().Size(),"Array1dIter3C, Second array smaller than first.");
	RavlAssertMsg(arr1.Range().Size() <= arr3.Range().Size(),"Array1dIter3C, Third array smaller than first");
      }
    }
    //: Constructor.
    // If you don't intend to iterator over the same range for each array set the 'matching' paramiter to 
    // false.  This will disable the check.
    
    Array1dIter3C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,const Array1dC<Data3T> &arr3,const IndexRangeC &rng)
      : BufferAccessIter3C<Data1T,Data2T,Data3T>(arr1,arr2,arr3),
	dat1(arr1,rng),
	dat2(arr2,rng),
	dat3(arr3,rng)
    {}
    //: Constructor.

    Array1dIter3C(const Array1dC<Data1T> &arr1,const IndexRangeC &rng1,
		  const Array1dC<Data2T> &arr2,const IndexRangeC &rng2,
		  const Array1dC<Data3T> &arr3,const IndexRangeC &rng3)
      : BufferAccessIter3C<Data1T,Data2T,Data3T>(arr1,rng1,arr2,rng2,arr3,rng3),
	dat1(arr1,rng1),
	dat2(arr2,rng2),
	dat3(arr3,rng3)
    {}
    //: Constructor with ranges for each array.
    
    inline void First() 
    { BufferAccessIter3C<Data1T,Data2T,Data3T>::First(dat1,dat2,dat3); }
    //: Goto first element in the array.
    
    bool IsFirst() const
    { return this->at1 == &dat1[dat1.IMin()]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
    IndexC Index() const
    { return IndexC((IntT)(this->at1 - dat1.ReferenceElm())); }
    //: Calculate the index we're at (In first array.)
    
  protected:
    Array1dC<Data1T> dat1;
    Array1dC<Data2T> dat2;
    Array1dC<Data3T> dat3;
  };
  
}
#endif
