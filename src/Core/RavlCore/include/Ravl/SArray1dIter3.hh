// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLSARR1ITER3_HEADER
#define RAVLSARR1ITER3_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/SArray/SArray1dIter3.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/98"
//! userlevel=Advanced

#include "Ravl/SArray1d.hh"
#include "Ravl/BfAccIter3.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Triple SArray1d iterator.
  // Fast simple array iterator.
  
  template<class Data1T,class Data2T,class Data3T>
  class SArray1dIter3C 
    : public BufferAccessIter3C<Data1T,Data2T,Data3T>
  {
  public:
    SArray1dIter3C()
      {}
    //: Default constructor.
    
    SArray1dIter3C(const SArray1dC<Data1T> &arr1,
		   const SArray1dC<Data2T> &arr2,
		   const SArray1dC<Data3T> &arr3);
    //: Constructor.
  
    inline void First()
    { BufferAccessIter3C<Data1T,Data2T,Data3T>::First(arr1,arr2,arr3); }
    // Goto first element in arrays.
    
    SArray1dC<Data1T> &Array1()
      { return arr1; }
    //: Access array.
    
    SArray1dC<Data2T> &Array2()
      { return arr2; }
    //: Access array.
    
    SArray1dC<Data3T> &Array3()
      { return arr3; }
    //: Access array.
    
    IndexC Index() const
      { return (IntT) (this->at1 - &arr1[0]); }
    //: Get current index.
    // This is a little slow.

    bool IsFirst() const
    { return this->at1 == &(arr1[0]); }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
  private:
    SArray1dC<Data1T> arr1;
    SArray1dC<Data2T> arr2;
    SArray1dC<Data3T> arr3;
  };  
  
  
  template<class Data1T,class Data2T,class Data3T>
  SArray1dIter3C<Data1T,Data2T,Data3T>::SArray1dIter3C(const SArray1dC<Data1T> &arr1,
						       const SArray1dC<Data2T> &arr2,
						       const SArray1dC<Data3T> &arr3)
    : BufferAccessIter3C<Data1T,Data2T,Data3T>(arr1,arr2,arr3),
      arr1(arr1),
      arr2(arr2),
      arr3(arr3)
  {}

}

#endif
