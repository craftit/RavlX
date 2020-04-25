// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2ITER4_HEADER
#define RAVL_ARRAY2ITER4_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.2D"
//! file="Ravl/Core/Container/Array/Array2dIter4.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/08/1999"
//! userlevel=Default

#include "Ravl/Array2d.hh"
#include "Ravl/BfAcc2Iter4.hh"

namespace RavlN {
  //! userlevel=Normal
  //: quadruple Array2dC iterator.
  // This will iterate through four rectangles.  The rectangles need not
  // have the same origin, or be the same size, depending on the constructor.  
  // The first array controls the number of elements visited.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  class Array2dIter4C 
    : public BufferAccess2dIter4C<Data1T,Data2T,Data3T,Data4T>
  {
  public:
    Array2dIter4C()
    {}
    //: Default constructor.
    
    Array2dIter4C(const Array2dC<Data1T> &arr1,
		  const Array2dC<Data2T> &arr2,
		  const Array2dC<Data3T> &arr3,
		  const Array2dC<Data4T> &arr4,
		  bool matching = true)
      : BufferAccess2dIter4C<Data1T,Data2T,Data3T,Data4T>(arr1,arr1.Range2(),
							  arr2,arr2.Range2(),
							  arr3,arr3.Range2(),
							  arr4,arr4.Range2()),
        dat1(arr1),
        dat2(arr2),
        dat3(arr3),
        dat4(arr4)
    { 
      if(matching) {
	RavlAssert(dat1.Range().Size() == dat2.Range().Size());
	RavlAssert(dat1.Range().Size() == dat3.Range().Size());
	RavlAssert(dat1.Range().Size() == dat4.Range().Size());
      } else {
	RavlAssertMsg(dat1.Range().Rows() <= dat2.Range().Rows(),"Array2dIter3C, Second array is has fewer rows than the first. ");
	RavlAssertMsg(dat1.Range().Cols() <= dat2.Range().Cols(),"Array2dIter3C, Second array is has fewer cols than the first. ");
	RavlAssertMsg(dat1.Range().Rows() <= dat3.Range().Rows(),"Array2dIter3C, Third array is has fewer rows than the first. ");
	RavlAssertMsg(dat1.Range().Cols() <= dat3.Range().Cols(),"Array2dIter3C, Third array is has fewer cols than the first. ");
	RavlAssertMsg(dat1.Range().Rows() <= dat4.Range().Rows(),"Array2dIter3C, Fourth array is has fewer rows than the first. ");
	RavlAssertMsg(dat1.Range().Cols() <= dat4.Range().Cols(),"Array2dIter3C, Fourth array is has fewer cols than the first. ");
      }
      First();
    }
    //: Constructor for which the rectangles can be offset from each other.  
    // The iterator iterates relative to the top left corner of each rectangle,
    // and always iterates through the whole of 'arr1'.<br>
    // If 'matching' is true, the rectangles MUST be of the same size. Hence
    // in this case, the iterator will iterate through the whole of all of the
    // rectangles.<br>
    // If 'matching' is false, the other arrays MUST be as big or bigger than
    // 'arr1' in both dimensions (but need not contain 'arr1').
    
    Array2dIter4C(const Array2dC<Data1T> &arr1,
		  const Array2dC<Data2T> &arr2,
		  const Array2dC<Data3T> &arr3,
		  const Array2dC<Data4T> &arr4,
		  const IndexRange2dC &rect)
      : dat1(arr1,rect),
        dat2(arr2,rect),
        dat3(arr3,rect),
        dat4(arr4,rect)
    { BufferAccess2dIter4C<Data1T,Data2T,Data3T,Data4T>::First(dat1,dat1.Range2(),
							       dat2,dat2.Range2(),
							       dat3,dat3.Range2(),
							       dat4,dat4.Range2()); 
    }
    //: Constructor that iterates through the same subrange 'rect' in each arrays. 
    // Therefore 'rect' MUST be within all of the arrays.

    Array2dIter4C(const Array2dC<Data1T> &arr1,const IndexRange2dC &irng1,
		  const Array2dC<Data2T> &arr2,const IndexRange2dC &irng2,
		  const Array2dC<Data3T> &arr3,const IndexRange2dC &irng3,
		  const Array2dC<Data4T> &arr4,const IndexRange2dC &irng4)
      : dat1(arr1,irng1),
        dat2(arr2,irng2),
        dat3(arr3,irng3),
        dat4(arr4,irng4)
    { BufferAccess2dIter4C<Data1T,Data2T,Data3T,Data4T>::First(dat1,dat1.Range2(),
							       dat2,dat2.Range2(),
							       dat3,dat3.Range2(),
							       dat4,dat4.Range2()); 
    }
    //: Constructor. Iterates through indicated subranges in each array.
    // 'irng2' defines the starting point for iterating through 'arr2', etc.
    // Hence care must be taken that 'irng1' does not cause the iterator to go
    // outside the other arrays.
    
    inline bool First() {
      return BufferAccess2dIter4C<Data1T,Data2T,Data3T,Data4T>::First(dat1,dat1.Range2(),
								      dat2,dat2.Range2(),
								      dat3,dat3.Range2(),
								      dat4,dat4.Range2()); 
    }
    //: Goto first element in the array.
    // Return TRUE if there actually is one.
    
    Index2dC Index() const { 
      RavlAssert(dat1.IsValid());
      return Index2dC((IntT) (&(this->rit.Data1()) - dat1.ReferenceElm()),
		      (IntT) (&(this->cit.Data1()) - this->rit.Data1().ReferenceElm())); 
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
        
  protected:
    Array2dC<Data1T> dat1;
    Array2dC<Data2T> dat2;
    Array2dC<Data3T> dat3;
    Array2dC<Data4T> dat4;
  };
}
#endif
