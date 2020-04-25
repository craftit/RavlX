// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Math/ScalMath.cc"

#include "Ravl/ScalMath.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
#if !RAVL_NEW_ANSI_CXX_DRAFT
  const IntT RavlPreComputedFactorialSize = 64; 
#endif
  
#if RAVL_NEW_ANSI_CXX_DRAFT
  RealT RavlPreComputedFactorial[RavlPreComputedFactorialSize]
  ={1.0, 1.0, 2.0, 6.0, 24.0, 120.0};
  // The table of precomputed factorials.   
#else
  // FIXME :- Do better ?? 
  RealT RavlPreComputedFactorial[64]
  ={1.0, 1.0, 2.0, 6.0, 24.0, 120.0};
#endif

  
  static IntT Init() {
    // Initialisation of the table of precomputed factorials.
    RavlPreComputedFactorial[0] = 1.0;
    for (IntT i = 1; i < RavlPreComputedFactorialSize; i++)
      RavlPreComputedFactorial[i] = RavlPreComputedFactorial[i-1] * RealT(i);  
    return 0;
  }
  
  static IntT init = Init();
  
  //: Double Factorial, returns n!!
  // Product of:
  //  n.(n-2) ...  5 . 3 . 1 for n > 0 is odd <br>
  //  n.(n-2) ...  6 . 4 . 2 for n > 0 is even <br>
  //  1 for n = 0 or -1
  
  RealT DoubleFactorial(IntT n) {
    if(n < 1) return 1;
    RealT fac;
    if(n & 1) fac = 1;
    else fac = 2;
    for (IntT i = 1; i <= n; i += 2)
      fac *= (RealT) i;
    return fac;
  }
  
  //: Compute the surface area of a hyper sphere in n dimensions with radius r
  
  RealT HypersphereSurfaceArea(IntT n,RealT r) {
    RealT rn = (RealT) n;
    if(n & 1) // Is n Odd?
      return Pow(2,(rn + 1.0)/2.0) * Pow(RavlConstN::pi,(rn-1)/2.0) / DoubleFactorial(n-2);
    return 2 *  Pow(RavlConstN::pi,rn/2.0) / Factorial((n / 2) - 1);
  }
  
  //: Compute the volume of a hyper sphere in n dimensions with radius r
  
  RealT HypersphereVolume(IntT n,RealT r) 
  { return (HypersphereSurfaceArea(n,r) * Pow(r,n)) / (RealT) n; }
  
}



