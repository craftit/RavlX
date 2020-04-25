// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARR1ITER_HEADER
#define RAVL_SARR1ITER_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/SArray/SArray1dIter.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/1998"
//! userlevel=Advanced

#include "Ravl/SArray1d.hh"
#include "Ravl/Assert.hh"
#include "Ravl/BfAccIter.hh"

namespace RavlN {

  //: SArray1d iterator.
  // Fast simple array iterator.
  
  template<class DataT>
  class SArray1dIterC 
    : public BufferAccessIterC<DataT>
  {
  public:
    SArray1dIterC()
    {}
    //: Default constructor.
    
    SArray1dIterC(const SArray1dC<DataT> &arr);
    //: Constructor.
    
    SArray1dIterC(const SArray1dC<DataT> &arr,UIntT maxInd);
    //: Constructor.
    // only iterate first maxInd items, maxInd must be withing the array bounds.
    
    const SArray1dIterC<DataT> &operator=(SArray1dC<DataT> &arr);
    //: Assignment to an array.
    
    inline void First()
    { BufferAccessIterC<DataT>::First((SizeBufferAccessC<DataT> &)arr); }
    //: Goto first element in array.
    
    SArray1dC<DataT> &Array()
    { return arr; }
    //: Access array.
    
    IndexC Index() const
    { return (IntT) (&this->Data() - &arr[0]); }   //: Get current index.
    // This is a little slow.
    
    bool IsFirst() const
    { return this->at == &arr[0]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().

    void Goto(UIntT off) { 
      this->at = &(arr[off]); 
      RavlAssert(this->at <= this->endOfRow);
    }
    //: Goto specific entry in array.
    // off must be within current array
    
  private:
    SArray1dC<DataT> arr;
  };
  
  ///////////////////////////////////////////
  
  template<class DataT>
  SArray1dIterC<DataT>::SArray1dIterC(const SArray1dC<DataT> &narr)
    : arr(narr)
  { BufferAccessIterC<DataT>::First( arr); }
  
  template<class DataT>
  SArray1dIterC<DataT>::SArray1dIterC(const SArray1dC<DataT> &narr,UIntT maxInd) 
    : arr(narr)
  { BufferAccessIterC<DataT>::First(SizeBufferAccessC<DataT>(arr,maxInd)); }
  
  template<class DataT>
  const SArray1dIterC<DataT> &
  SArray1dIterC<DataT>::operator=(SArray1dC<DataT> &narr) {
    arr = narr;
    First();
    return *this;
  }
  
}
#endif
