// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENERATESIGNAL1D_HEADER
#define RAVL_GENERATESIGNAL1D_HEADER 1
///////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Signals.1D"
//! userlevel=Normal
//! file="Ravl/Math/Signals/1D/GenerateSignal1d.hh"

#include "Ravl/Array1d.hh"

namespace RavlN {
  
  
  Array1dC<RealT> GenerateRandomGauss1d(UIntT size,RealT stdDeviation,RealT mean);
  //: Generate an array containing Gaussian noise with specified properties.
  
  Array1dC<RealT> GenerateGauss1d(RealT sigma,RealT mean = 0,RealT scale = 1,RealT size = 2);
  //: Generate a truncated 1-D Gaussian curve.
    
  template<class DataT>
  Array1dC<DataT> GenerateBinomial(UIntT size, bool doNorm = false,bool centre = false, UIntT derivative = 0) {
    Array1dC<DataT> fnc;
    if(size == 0)
      return Array1dC<DataT>();
    if(!centre)
      fnc = Array1dC<DataT>(size);
    else {
      fnc = Array1dC<DataT>(-(static_cast<IntT>(size)/2),((static_cast<IntT>(size)-1)/2));
      RavlAssertMsg(size == fnc.Size(),"GenerateBinomial(), size must be odd to centre. ");
    }
    BufferAccessIterC<DataT> it(fnc);
    DataT i = 1;
    DataT last,rsize = size-derivative;
    *it = 1; // i.e. 1st coeff = 1
    last = *it;
    it++;
    for (;it;it++,i++) {
      *it = last * (rsize-i) / i;
      last = *it;
    }
    for (UIntT j(derivative); j>0; --j) {
      for (IndexC k(fnc.IMax()); k>fnc.IMin(); --k) fnc[k] -= fnc[k-1];
    }
    if(doNorm) {
      DataT norm = (DataT)1 / 2;
      for (it.First(fnc);it;it++) norm *= 2;
      for (it.First(fnc);it;it++) *it /= norm;
    }
    return fnc;
  }
  //: Creates an array of binomial coefficients 

  // <p>It generates row "size" from Pascal's
  // triangle. E.g. GenerateBinomial(5) would create the array: 1,4,6,4,1.</p>

  // <p>It can be used as a finite approximation of a Gaussian, but with the property that the spectral content at &frac12; sampling frequency = 0.  The
  // "variance" of this Gaussian is (size-1)/4.
  // The rationale is this:
  // <ul>
  // <li> The simplest possible non-trivial Gaussian filter is a 2nd-order one,
  // which from symmetry considerations will have two equal coefficients.
  // <li> Higher-order filters can be viewed as convolved 2nd-order filters,
  // i.e. an <i>n</i>th order filter is a convolution of <i>n</i>-1 2nd-order filters.
  // By the Central Limit Theorem these will approximate a Gaussian profile
  // more and more closely as the order increases.
  // <li> At the same time because they are of finite width they avoid the
  // problem of truncating the Gaussian tails.
  // </ul>
  
  //!param: size - size of the array that is created
  //!param: doNorm - if true, the array is normalised so that the sum of the terms = 1.
  //!param: centre - if true, the array index is centred so that array[0] is the middle of the array. (If size is even, the centre of the array is at -&frac12;.)
  //!param: derivative - if > 0, the Gaussian is differentiated this number of times

  template<class DataT>
  Array1dC<DataT> GenerateBinomial(DataT seed,UIntT size, bool doNorm = false,bool centre = false)
  { return GenerateBinomial<DataT>(size, doNorm, centre); }
  //! userlevel=Obsolete
  //Use this function instead:<br>
  //<code>  template<class DataT>  Array1dC<DataT> GenerateBinomial(SizeT size, bool doNorm = false,bool center = false, UIntT derivative = 0)</code>

}

#endif
