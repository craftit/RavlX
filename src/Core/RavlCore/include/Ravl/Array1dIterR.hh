// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARRAY1DITERR_HEADER
#define RAVL_ARRAY1DITERR_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/Array/Array1dIterR.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! userlevel=Normal
//! date="17/11/2002"
//! example=exArray1.cc

#include "Ravl/Array1d.hh"
#include "Ravl/BufferAccessIterR.hh"

namespace RavlN {
  //! userlevel=Normal
  //: reverse Array1dC iterator.
  
  template<class DataT>
  class Array1dIterRC 
    : public BufferAccessIterRC<DataT>
  {
  public:
    Array1dIterRC()
    {}
    //: Default constructor.
    
    Array1dIterRC(const Array1dC<DataT> &arr)
      : BufferAccessIterRC<DataT>(arr),
        dat(arr)
    {}
    //: Constructor.
    
    Array1dIterRC(const Array1dC<DataT> &arr,const IndexRangeC & range)
      : dat(arr,range)
    { BufferAccessIterRC<DataT>::operator=(dat); }
    //: Iterate through a sub-range.
    
    inline void Last() 
    { BufferAccessIterRC<DataT>::Last(dat); }
    //: Goto first element in the array.
    
    inline void Last(Array1dC<DataT> &arr) { 
      dat = arr;
      BufferAccessIterRC<DataT>::Last(dat); 
    }
    //: Goto first element in the array.
    
    bool IsFirst() const
    { return this->at == &dat[dat.IMax()]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
    bool IsLast() const
    { return this->at == &dat[dat.IMin()]; }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
    const Array1dIterRC<DataT> &operator=(const Array1dC<DataT> &arr) {
      dat = arr;
      Last();
      return *this;
    }
    //: Assignment.
    
    IndexC Index() const
    { return IndexC((IntT)( this->at - dat.ReferenceElm())); }
    //: Calculate the index we're at.
    
  protected:
    Array1dC<DataT> dat;
  };
  
}
#endif
