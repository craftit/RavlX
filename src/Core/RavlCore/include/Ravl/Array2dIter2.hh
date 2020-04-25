// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2DITER2_HEADER
#define RAVL_ARRAY2DITER2_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! file="Ravl/Core/Container/Array/Array2dIter2.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/08/1999"
//! userlevel=Default

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter2.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: dual Array2dC iterator.
  // This will iterate through two rectangles.  The rectangles need not
  // have the same origin, or be the same size, depending on the constructor.
  // The first array controls the number of elements visited.
  
  template<class Data1T,class Data2T>
  class Array2dIter2C 
    : public BufferAccess2dIter2C<Data1T,Data2T>
  {
  public:
    Array2dIter2C()
    {}
    //: Default constructor.
    
    Array2dIter2C(const Array2dC<Data1T> &arr1,const Array2dC<Data2T> &arr2,bool matching = true)
      : BufferAccess2dIter2C<Data1T,Data2T>(arr1,arr1.Range2(),
					    arr2,arr2.Range2()),
        dat1(arr1),
        dat2(arr2)
    { 
      if(matching) {
	RavlAssertMsg(dat1.Range().Size() == dat2.Range().Size(),"Array2dIter2C, Array size mismatch");
      } else {
	RavlAssertMsg(dat1.Range().Rows() <= dat2.Range().Rows(),"Array2dIter2C, Second array is has fewer rows than the first. ");
	RavlAssertMsg(dat1.Range().Cols() <= dat2.Range().Cols(),"Array2dIter2C, Second array is has fewer cols than the first. ");
      }	
      First();
    }
    //: Constructor for which the rectangles can be offset from each other.  
    // The iterator iterates relative to the top left corner of each rectangle, and always iterates through the whole of 'arr1'.<br>
    // If 'matching' is true, the rectangles MUST be of the same size. Hence in this case, the iterator will iterate through the whole of both rectangles.<br>
    // If 'matching' is false, 
    // 'arr2' MUST be as big or bigger than 'arr1' in both dimensions (but need not contain 'arr1').
    
    Array2dIter2C(const Array2dC<Data1T> &arr1,const Array2dC<Data2T> &arr2,const IndexRange2dC &rect)
      : dat1(arr1,rect),
        dat2(arr2,rect)
    { BufferAccess2dIter2C<Data1T,Data2T>::First(dat1,dat1.Range2(),
						 dat2,dat2.Range2()); 
    }
    //: Constructor that iterates through the same subrange 'rect' in both arrays. 
    // Therefore 'rect' MUST be within both arrays.

    Array2dIter2C(const Array2dC<Data1T> &arr1,const IndexRange2dC &irng1,
		  const Array2dC<Data2T> &arr2,const IndexRange2dC &irng2)
      : dat1(arr1,irng1),
        dat2(arr2,irng2)
    { BufferAccess2dIter2C<Data1T,Data2T>::First(dat1,dat1.Range2(),
						 dat2,dat2.Range2()); 
    }
    //: Constructor. Iterates through indicated subranges in both arrays.
    // 'irng2' defines the starting point for iterating through 'arr2'.  Hence care must be taken that 'irng1' does not cause the iterator to go outside 'arr2'.
    
    inline bool First() {
      return BufferAccess2dIter2C<Data1T,Data2T>::First(dat1,dat1.Range2(),
							dat2,dat2.Range2()); 
    }
    //: Goto first elements in the arrays.
    // Return TRUE if there actually is one.

    Index2dC Index() const { 
      RavlAssert(dat1.IsValid());
      return Index2dC((IntT) (&(this->rit.Data1()) - dat1.ReferenceElm()),
		      (IntT) (&(this->cit.Data1()) - this->rit.Data1().ReferenceElm()));
    }
    //: Get index of current location in 'arr1'.
    // Has to be calculated, and so is slightly slow.
        
  protected:
    Array2dC<Data1T> dat1;
    Array2dC<Data2T> dat2;
  };
}

#endif
