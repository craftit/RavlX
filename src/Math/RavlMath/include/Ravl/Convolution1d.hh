// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CONVOLUTION1D_HEADER
#define RAVL_CONVOLUTION1D_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Math.Signals.1D"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/Convolution1d.hh"

#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"

namespace RavlN {
  
  
  template<class DataT>
  Array1dC<DataT> Convolve1d(const Array1dC<DataT> &arr1,const Array1dC<DataT> &arr2) {
    IndexRangeC rng(arr1.IMin() - arr2.IMin(),arr1.IMax() - arr2.IMax());
    if(rng.Size() < 1)
      return Array1dC<DataT>();
    Array1dC<DataT> ret(rng);
    const DataT *start = &(arr1[arr1.IMin()]);
    for(Array1dIterC<DataT> itr(ret);itr;itr++,start++) {      
      DataT sum = 0;
      const DataT *at = start;
      const DataT *at2 = &(arr2[arr2.IMax()]);
      const DataT *end2 = at2 - arr2.Size();
      for(;at2 != end2;at++,at2--)
        sum += *at2 * *at;
      *itr = sum;
    }
    return ret;
  }
  //! userlevel=Normal
  //: Convolve arr1 and arr2 and return the result.
  
  
}


#endif
