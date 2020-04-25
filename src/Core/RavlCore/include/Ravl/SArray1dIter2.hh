// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARR1ITER2_HEADER
#define RAVL_SARR1ITER2_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/SArray/SArray1dIter2.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/98"
//! userlevel=Advanced

#include "Ravl/SArray1d.hh"
#include "Ravl/BfAccIter2.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Dual SArray1d iterator.
  // Fast simple array iterator.
  
  template<class Data1T,class Data2T>
  class SArray1dIter2C 
    : public BufferAccessIter2C<Data1T,Data2T>
  {
  public:
    SArray1dIter2C()
      {}
    //: Default constructor.
    
    SArray1dIter2C(const SArray1dC<Data1T> &arr1,const SArray1dC<Data2T> &arr2);
    //: Constructor.
    
    inline void First()
      { BufferAccessIter2C<Data1T,Data2T>::First(arr1,arr2); }
    // Goto first element in arrays.
    
    SArray1dC<Data1T> &Array1()
    { return arr1; }
    //: Access array.
    
    SArray1dC<Data2T> &Array2()
    { return arr2; }
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
  };
  
  ///////////////////////////////////////////
  
  template<class Data1T,class Data2T>
  SArray1dIter2C<Data1T,Data2T>::SArray1dIter2C(const SArray1dC<Data1T> &narr1,const SArray1dC<Data2T> &narr2)
    : arr1(narr1),
      arr2(narr2)
  { First(); }
  
  
}
#endif
