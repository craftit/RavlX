// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARRAY1ITERR_HEADER
#define RAVL_SARRAY1ITERR_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/SArray/SArray1dIterR.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/1998"
//! userlevel=Advanced

#include "Ravl/SArray1d.hh"
#include "Ravl/Assert.hh"
#include "Ravl/BufferAccessIterR.hh"

namespace RavlN {

  //: SArray1d reverse iterator.
  // Fast simple array iterator.
  
  template<class DataT>
  class SArray1dIterRC 
    : public BufferAccessIterRC<DataT>
  {
  public:
    SArray1dIterRC()
    {}
    //: Default constructor.
    
    SArray1dIterRC(const SArray1dC<DataT> &arr);
    //: Constructor.
    
    SArray1dIterRC(const SArray1dC<DataT> &arr,UIntT maxInd);
    //: Constructor.
    // only iterate first maxInd items, maxInd must be withing the array bounds.
    
    const SArray1dIterRC<DataT> &operator=(SArray1dC<DataT> &arr);
    //: Assignment to an array.
    
    inline void Last()
    { BufferAccessIterRC<DataT>::Last((SizeBufferAccessC<DataT> &)arr); }
    //: Goto first element in array.
    
    SArray1dC<DataT> &Array()
    { return arr; }
    //: Access array.
    
    IndexC Index() const
    { return (IntT) (&this->Data() - &arr[0]); }   //: Get current index.
    // This is a little slow.
    
    bool IsLast() const
    { return this->at == &arr[0]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
  private:
    SArray1dC<DataT> arr;
  };
  
  ///////////////////////////////////////////
  
  template<class DataT>
  SArray1dIterRC<DataT>::SArray1dIterRC(const SArray1dC<DataT> &narr)
    : arr(narr)
  { BufferAccessIterRC<DataT>::Last(narr); }
  
  template<class DataT>
  SArray1dIterRC<DataT>::SArray1dIterRC(const SArray1dC<DataT> &narr,UIntT maxInd) 
    : arr(narr)
  { BufferAccessIterRC<DataT>::Last(SizeBufferAccessC<DataT>(narr,maxInd)); }
  
  template<class DataT>
  const SArray1dIterRC<DataT> &
  SArray1dIterRC<DataT>::operator=(SArray1dC<DataT> &narr) {
    arr = narr;
    Last();
    return *this;
  }
  
}
#endif
