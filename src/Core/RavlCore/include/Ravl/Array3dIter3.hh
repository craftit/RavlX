// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY2ITER3_HEADER
#define RAVL_ARRAY2ITER3_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.3D"
//! file="Ravl/Core/Container/Array/Array3dIter3.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/08/99"
//! userlevel=Default

#include "Ravl/Array3d.hh"
#include "Ravl/BfAcc3Iter3.hh"

namespace RavlN {
  //! userlevel=Normal
  //: triple Array3dC iterator.
  
  template<class Data1T,class Data2T,class Data3T>
  class Array3dIter3C 
    : public BufferAccess3dIter3C<Data1T,Data2T,Data3T>
  {
  public:
    Array3dIter3C()
    {}
    //: Default constructor.
    
    Array3dIter3C(const Array3dC<Data1T> &arr1,
		  const Array3dC<Data2T> &arr2,
		  const Array3dC<Data3T> &arr3,
		  bool matching = true)
      : BufferAccess3dIter3C<Data1T,Data2T,Data3T>(arr1,arr1.Range2(),arr1.Range3(),
						   arr2,arr2.Range2(),arr2.Range3(),
						   arr3,arr3.Range2(),arr3.Range3()),
        dat1(arr1),
        dat2(arr2),
        dat3(arr3)
    { 
      if(matching) {
	RavlAssert(dat1.Frame() == dat2.Frame());
	RavlAssert(dat1.Frame() == dat3.Frame());
      }
      First();
    }
    //: Constructor.
    
    Array3dIter3C(const Array3dC<Data1T> &arr1,
		  const Array3dC<Data2T> &arr2,
		  const Array3dC<Data3T> &arr3,
		  const IndexRange3dC &rect)
      : dat1(arr1,rect),
        dat2(arr2,rect),
        dat3(arr3,rect)
    { BufferAccess3dIter3C<Data1T,Data2T,Data3T>::First(dat1,dat1.Range2(),dat1.Range3(),
							dat2,dat2.Range2(),dat2.Range3(),
							dat3,dat3.Range2(),dat3.Range3()); 
    }
    //: Constructor.
    
    inline bool First() {
      return BufferAccess3dIter3C<Data1T,Data2T,Data3T>::First(dat1,dat1.Range2(),dat1.Range3(),
							       dat2,dat2.Range2(),dat2.Range3(),
							       dat3,dat3.Range2(),dat3.Range3()); 
    }
    //: Goto first element in the array.
    // Return TRUE if there actually is one.
    
    Index3dC Index() const { 
      RavlAssert(dat1.IsValid());
      Index2dC i2 = this->sit.Index(this->rit.Data1().ReferenceElm());
      return Index3dC((IndexC) (&(this->rit.Data1()) - dat1.ReferenceElm()),
		      (IndexC) i2.Row(),
		      (IndexC) i2.Col()); 
    }
    //: Get index of current location.
    // Has to be calculate, and so is slightly slow.
    
  protected:
    Array3dC<Data1T> dat1;
    Array3dC<Data2T> dat2;
    Array3dC<Data3T> dat3;
  };
}
#endif
