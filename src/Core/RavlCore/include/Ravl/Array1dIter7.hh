// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY1ITER7_HEADER
#define RAVL_ARRAY1ITER7_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/Array/Array1dIter7.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="07/12/2002"
//! userlevel=Default

#include "Ravl/Array1d.hh"
#include "Ravl/BfAccIter7.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //:  Array1dC iterator for 7 arrays.
  // The first array controls the number of elements visited.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class Data6T,class Data7T>
  class Array1dIter7C 
    : public BufferAccessIter7C<Data1T,Data2T,Data3T,Data4T,Data5T,Data6T,Data7T>
  {
  public:
    Array1dIter7C()
    {}
    //: Default constructor.
    
    Array1dIter7C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,
		  const Array1dC<Data3T> &arr3,const Array1dC<Data4T> &arr4,
		  const Array1dC<Data5T> &arr5,const Array1dC<Data6T> &arr6,const Array1dC<Data7T> &arr7,
		  bool matching = true)
      : BufferAccessIter7C<Data1T,Data2T,Data3T,Data4T,Data5T,Data6T,Data7T>(arr1,arr2,arr3,arr4,arr5,arr6,arr7),
	dat1(arr1),
	dat2(arr2),
	dat3(arr3),
	dat4(arr4),
	dat5(arr5),
	dat6(arr6),
	dat7(arr7)
    {
      if(matching) {
	RavlAssertMsg(arr1.Range().Size() == arr2.Range().Size(),"Array1dIter7C, First and second array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr3.Range().Size(),"Array1dIter7C, First and third array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr4.Range().Size(),"Array1dIter7C, First and fourth array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr5.Range().Size(),"Array1dIter7C, First and fifth array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr6.Range().Size(),"Array1dIter7C, First and sixth array sizes don't match.");
	RavlAssertMsg(arr1.Range().Size() == arr7.Range().Size(),"Array1dIter7C, First and seventh array sizes don't match.");
      } else {
	RavlAssertMsg(arr1.Range().Size() <= arr2.Range().Size(),"Array1dIter7C, Second array smaller than first.");
	RavlAssertMsg(arr1.Range().Size() <= arr3.Range().Size(),"Array1dIter7C, Third array smaller than first");
	RavlAssertMsg(arr1.Range().Size() <= arr4.Range().Size(),"Array1dIter7C, Forth array smaller than first");
	RavlAssertMsg(arr1.Range().Size() <= arr5.Range().Size(),"Array1dIter7C, Fifth array smaller than first");
	RavlAssertMsg(arr1.Range().Size() <= arr6.Range().Size(),"Array1dIter7C, Sixth array smaller than first");
	RavlAssertMsg(arr1.Range().Size() <= arr7.Range().Size(),"Array1dIter7C, Seventh array smaller than first");
      }
    }
    //: Constructor.
    // If you don't intend to iterator over the same range for each array set the 'matching' paramiter to 
    // false.  This will disable the check.
    
    Array1dIter7C(const Array1dC<Data1T> &arr1,const Array1dC<Data2T> &arr2,
		  const Array1dC<Data3T> &arr3,const Array1dC<Data4T> &arr4,
		  const Array1dC<Data5T> &arr5,const Array1dC<Data6T> &arr6,const Array1dC<Data7T> &arr7,
		  const IndexRangeC &rng)
      : BufferAccessIter7C<Data1T,Data2T,Data3T,Data4T,Data5T,Data6T,Data7T>(arr1,arr2,arr3,arr4,arr5,arr6,arr7),
	dat1(arr1,rng),
	dat2(arr2,rng),
	dat3(arr3,rng),
	dat4(arr4,rng),
	dat5(arr5,rng),
	dat6(arr6,rng),
	dat7(arr7,rng)
    {}
    //: Constructor.
    
    inline void First() 
    { BufferAccessIter7C<Data1T,Data2T,Data3T,Data4T,Data5T,Data6T,Data7T>::First(dat1,dat2,dat3,dat4,dat5,dat6,dat7); }
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
    Array1dC<Data4T> dat4;
    Array1dC<Data5T> dat5;
    Array1dC<Data6T> dat6;
    Array1dC<Data7T> dat7;
  };
  
}
#endif
