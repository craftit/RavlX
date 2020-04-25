// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STDMATH_HEADER
#define RAVL_STDMATH_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Math/StdMath.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math"
//! rcsid="$Id: StdMath.hh 7009 2008-10-27 22:11:52Z kier $"
//! date="20/11/1995"

#include "Ravl/config.h"
#include "Ravl/Math.hh"
#include "Ravl/StdConst.hh"

#if RAVL_HAVE_FLOAT_H
#include <float.h>
#endif
#if RAVL_HAVE_NAN_H
#include <nan.h>
#endif
#if RAVL_HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#if RAVL_HAVE_ERF
#if RAVL_OS_SOLARIS
extern "C" {
  double erf __P((double)); // not in standard <math.h> file
  double erfc __P((double)); // not in standard <math.h> file
}
#endif
#if RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_IRIX || RAVL_OS_OSF
extern "C" {
  double erf(double x); // not in standard <math.h> file
  double erfc(double x); // not in standard <math.h> file
}
#endif
#else
  // Defined in Erf.cc
  double erf(double x);
  double erfc(double x);
#endif

#if RAVL_OS_CYGWIN
extern "C" {
  int isnan(double x); // not in standard <math.h> file
  int isinf(double x); // not in standard <math.h> file
}
#endif


//: The Ravl namespace.

namespace RavlN {
  
  inline RealT Cbrt(RealT r) {
#if RAVL_HAVE_CBRT
    return cbrt(r);
#else
#if RAVL_COMPILER_VISUALCPP
    RealT dem = 3.0;  // Sometimes get a compile warning unless we do this.
    return pow(r, 1.0/dem);
#else
    if(r >= 0)
      return pow(r, 1.0/3.0);
    return -pow(-r, 1.0/3.0);
#endif
#endif
  }
  //: Returns the cube root of 'x'.
  
  inline RealT Erf(RealT x)
  { return erf(x); }
  //: Returns the error function of x. Erf(x) = 2/sqrt(pi)*integral from 0 to x of exp(-t*t) dt.
  
  inline RealT Erfc(RealT x)
  { return erfc(x); }
  //: Returns 1.0 - Erf(x). (Use when x is large)
  
  inline RealT Log2(RealT r) {
    RavlAssertMsg(r > 0.0,"Log2(RealT r): Can't take log of zero or negative number.");
    return log(r)/RavlConstN::ln2;
  }
  //: Returns logarithm to base 2.
  
  inline bool IsPow2(IntT i) {
    IntT j = 1;
    while(j < i) 
      j *= 2;
    return i==j;
  }
  //: Is interger power of 2 ?
  // Returns true if 'i' is a power of 2.
    
  inline bool IsInf(RealT i) {
#if RAVL_HAVE_ISINF
    return std::isinf(i);
#elif RAVL_HAVE_FINITE
    return !finite(i);
#elif RAVL_HAVE__FINITE
    return !_finite(i);
#else
#error "No IsInf() implementation found. "
#endif
  }
  //: Is infinite ?
  
  inline bool IsInf(float i) {
#if RAVL_HAVE_ISINF
    return std::isinf(i);
#elif RAVL_HAVE_FINITE
    return !finite(i);
#elif RAVL_HAVE__FINITE
    return !_finite(i);
#else
#error "No IsInf() implementation found. "
#endif
  }
  //: Is infinite ?
  
  inline bool IsPInf(RealT i)
  { return ((bool) (i > 0)) && IsInf(i); }
  //: Is positive infinity ?
  
  inline bool IsNInf(RealT i)
  { return ((bool) (i < 0)) && IsInf(i); }
  //: Is negative infinity ?
  
  inline bool IsNan(RealT i) {
#if RAVL_HAVE_ISNAN
    return std::isnan(i);
#elif RAVL_HAVE__ISNAN
    return (_isnan(i)!=0);
#elif RAVL_HAVE_ISNAND
    return isnand(i);
#else
#error "No IsNan() implementation found. "
#endif
  }
  //: Is Not A Number ?
  
  inline bool IsNan(float i) {
#if RAVL_HAVE_ISNAN
    return std::isnan(i);
#elif RAVL_HAVE__ISNAN
    return (_isnan(i)!=0);
#elif RAVL_HAVE_ISNAND
    return isnand(i);
#else
#error "No IsNan() implementation found. "
#endif
  }
  //: Is Not A Number ?


  inline RealT Degrees2Radians(RealT angle)
  { return angle*(RavlConstN::pi / 180.0); }
  //: Convert angle from degrees to radians.

  inline RealT Radians(RealT angle)
  { return Degrees2Radians(angle); }
  //: Convert angle from degrees to radians.
  // OBSOLETE: Use Degrees2Radians

  inline RealT Radians2Degrees(RealT angle)
  { return angle*(180.0 / RavlConstN::pi);  }
  //: Convert angle from radians to degrees.

  inline RealT Degrees(RealT angle)
  { return Radians2Degrees(angle); }
  //: Convert angle from radians to degrees.
  // OBSOLETE: Use Radians2Degrees

  inline float Degrees2Radians(float angle)
  { return angle*((float) RavlConstN::pi / 180.0f); }
  //: Convert angle from degrees to radians.

  inline float  Radians(float angle)
  { return float(Radians2Degrees(angle)); }
  //: Convert angle from degrees to radians.
  // OBSOLETE: Use Degrees2Radians
  
  inline float Radians2Degrees(float angle)
  { return angle*( 180.0f/ (float) RavlConstN::pi);  }
  //: Convert angle from radians to degrees.

  inline float Degrees(float angle)
  { return float(Radians2Degrees(angle)); }
  //: Convert angle from radians to degrees.
  // OBSOLETE: Use Radians2Degrees
  
  
}  
#endif

