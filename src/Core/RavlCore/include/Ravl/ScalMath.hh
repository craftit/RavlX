// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SCALMATH_HEADER
#define RAVL_SCALMATH_HEADER 1
//////////////////////////////////////////////////////////
//! file="Ravl/Core/Math/ScalMath.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math"
//! date="02/11/1995"

#include "Ravl/StdMath.hh"
#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
#if RAVL_NEW_ANSI_CXX_DRAFT
  static const IntT RavlPreComputedFactorialSize = 64; 
  // The number of precomputed factorials.
  
  extern RealT RavlPreComputedFactorial[RavlPreComputedFactorialSize]; 
  // The table of precomputed factorials.   
#else
  extern const IntT RavlPreComputedFactorialSize; 
  // The number of precomputed factorials.
  
  extern RealT RavlPreComputedFactorial[64]; // NB. !!! Size must be same as 'factorialSize' !!!
  // The table of precomputed factorials.     
#endif
  
  //! userlevel=Normal
  
  inline RealT Factorial(UIntT n)
  {
    if(n < ((UIntT) RavlPreComputedFactorialSize))
      return RavlPreComputedFactorial[n];
    RealT fac = RavlPreComputedFactorial[RavlPreComputedFactorialSize-1];
    UIntT i = (RavlPreComputedFactorialSize);
    for (; i <= n; i++)
      fac *= (RealT) i;
    return fac;
  }
  //: Returns the factorial of the integer 'n'. 
  
  RealT DoubleFactorial(IntT n)  RAVL_GNUEXT(__attribute__ ((const)));
  //: Double Factorial, returns n!!
  // Product of: <br>
  //  n.(n-2) ...  5 . 3 . 1 for n > 0 is odd <br>
  //  n.(n-2) ...  6 . 4 . 2 for n > 0 is even <br>
  //  1 for n = 0 or -1
  
  inline RealT BinomalCoeff(IntT n, IntT k) {
    double numerator = 1.0;
    for (IntT i = n; i > n-k; i--)
      numerator *= i;
    return numerator/Factorial(k);
  }
  //: Returns the binomial coefficient (n over k) as a real number.
  
  RealT HypersphereSurfaceArea(IntT n,RealT r)  RAVL_GNUEXT(__attribute__ ((const)));
  //: Compute the surface area of a hyper sphere in n dimensions with radius r
  
  RealT HypersphereVolume(IntT n,RealT r)  RAVL_GNUEXT(__attribute__ ((const)));
  //: Compute the volume of a hyper sphere in n dimensions with radius r
}
#endif

