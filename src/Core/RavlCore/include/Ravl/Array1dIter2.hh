// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY1ITER2_HEADER
#define RAVL_ARRAY1ITER2_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/Array/Array1dIter2.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/08/99"
//! userlevel=Default

#include "Ravl/Array1d.hh"
#include "Ravl/BfAccIter2.hh"

namespace RavlN {
  //! userlevel=Normal
  //: dual Array1dC iterator.
  // The first array in the pair controls the number of elements visited.
  
  template<class Data1T,class Data2T>
  class Array1dIter2C 
    : public BufferAccessIter2C<Data1T,Data2T>
  {
  public:
    Array1dIter2C()
    {}
    //: Default constructor.
    
    Array1dIter2C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,bool matching = true)
      : BufferAccessIter2C<Data1T,Data2T>(arr1,arr2),
        dat1(arr1),
        dat2(arr2)
    {
      if(matching) {
	RavlAssertMsg(arr1.Range().Size() == arr2.Range().Size(),"Array1dIter2C, First and second array sizes don't match.");
      } else {
	RavlAssertMsg(arr1.Range().Size() <= arr2.Range().Size(),"Array1dIter2C, Second array smaller than first.");
	RavlAssert(arr1.Range().Size() <= arr2.Range().Size());
      }
    }
    //: Constructor.
    // If you don't intend to iterator over the same range for each array set the 'matching' paramiter to 
    // false.  This will disable the check.
    
    Array1dIter2C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,const IndexRangeC &rng)
      : dat1(arr1,rng),
        dat2(arr2,rng)
    { BufferAccessIter2C<Data1T,Data2T>::First(dat1,dat2); }
    //: Constructor.
    
    Array1dIter2C(const Array1dC<Data1T> &arr1,const IndexRangeC &rng1,
		  const Array1dC<Data2T> &arr2,const IndexRangeC &rng2)
      : dat1(arr1,rng1),
        dat2(arr2,rng2)
    { BufferAccessIter2C<Data1T,Data2T>::First(dat1,dat2); }
    //: Constructor with ranges for each array.
    
    Array1dIter2C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,IntT offset1,IntT offset2)
      : dat1(arr1),
        dat2(arr2)
    { BufferAccessIter2C<Data1T,Data2T>::First(dat1,dat2,offset1,offset2); }
    //: Constructor from two arrays.
    // Iterate through the length of the first array starting from offet1 in the first, and
    // offset2 in the second.
    
    inline void First() 
    { BufferAccessIter2C<Data1T,Data2T>::First(dat1,dat2); }
    //: Goto first element in the array.
    
    inline void First(IntT offset1,IntT offset2) 
    { BufferAccessIter2C<Data1T,Data2T>::First(dat1,dat2,offset1,offset2); }
    //: Reset to position offset1 in the first array, and offset2 in the second.
    
    bool IsFirst() const
    { return this->at1 == &dat1[dat1.IMin()]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
    IndexC Index() const
    { return IndexC((IntT)(this->at1 - dat1.ReferenceElm())); }
    //: Calculate the index we're at (In first array.)
    
    IndexC Index2() const 
      { return IndexC( (IntT) (this->at2 - dat2.ReferenceElm()) ) ; } 
    //: Calcualte the index we're at (In second array.) 

  protected:
    Array1dC<Data1T> dat1;
    Array1dC<Data2T> dat2;
  };
}

#endif
