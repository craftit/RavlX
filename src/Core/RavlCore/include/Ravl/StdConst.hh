// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLCONST_HEADER
#define RAVLCONST_HEADER
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/StdConst.hh"
//! lib=RavlCore
//! userlevel=Basic
//! author="Radek Marik"
//! date="27/11/1995"
//! docentry="Ravl.API.Math"
//! rcsid="$Id$"

#include "Ravl/config.h"

// Information on limits of machine precision etc. can be found in one
// of the two files. 

#if RAVL_HAVE_VALUES_H
#include <values.h>
#endif

#include <limits.h>

#if RAVL_HAVE_FLOAT_H
#include <float.h>
#endif

//: Standard constants
// The class StdConstC is used for definition of useful constants.

namespace RavlConstN {
  
#if 1
  //RAVL_NEW_ANSI_CXX_DRAFT
  
  // Although the following syntax should be valid it is not accepted
  // by a lot of compilers in the full way.
  
  const double sqrt2     =  1.41421356237309504880;
  //: The value of the square root of 2.
  
  const double sqrt1_2   =  0.70710678118654752440;
  //: The value of the square root of 1/2.
  
  const double pi        =  3.1415926535897932384626433832795028;
  //: The value of PI number.
  
  const double pi_2      =  1.57079632679489661923;
  //: The value of PI/2.
  
  const double pi_4      =  0.78539816339744830962;
  //: The value of PI/4.
  
  const double o1_pi     =  0.31830988618379067154;
  //: The value of 1/PI.
  
  const double o2_pi     =  0.63661977236758134308;
  //: The value of 2/PI.
  
  const double o2_sqrtPi =  1.12837916709551257390;
  //: The value of 2/sqrt(PI).
  
  const double sqrt2Pi   =  2.50662827465;
  //: The value of sqrt(2*PI).
  
  const double deg2rad   =  pi/180.0;
  //: Conversion from degrees to radians.
  
  const double rad2deg   =  180.0/pi; 
  //: Conversion from radians to degrees.
  
  const double e         =  2.7182818284590452354;
  //: The value of e number.
  
  const double log2e     =  1.4426950408889634074;
  //: The value of log2(e).
  
  const double log10e    =  0.43429448190325182765;
  //: The value of log10(e).
  
  const double ln2       =  0.69314718055994530942;
  //: The value of ln(2).
  
  const double ln10      =  2.30258509299404568402;
  //: The value of ln(10).
  
  //:------
  // Limits
  
#ifdef DBL_EPSILON
  const double realPrecision = DBL_EPSILON;
  //: Maximum real number X for which 1.0 + X = 1.0
#else
  const double realPrecision = 1.0e-9;
  //: Maximum real number X for which 1.0 + X = 1.0
#endif

#ifdef MAXDOUBLE
  const double maxReal = MAXDOUBLE;
  //: Maximum real number.
  
  const double minReal = MINDOUBLE;
  //: Smallest +ve value a real may have.  
#else
#ifdef DBL_MAX
  const double maxReal =  DBL_MAX;
  //: Maximum real number.
  
  const double minReal =  DBL_MIN;
  //: Smallest +ve value a real may have.  
#else
#ifdef __DBL_MAX__
  const double maxReal = __DBL_MAX__;
  //: Maximum real number.
  
  const double minReal = __DBL_MIN__;
  //: Smallest +ve value a real may have.
#else
#error "DBL_MIN not defined."
  const double maxReal =  1.0e200;
  //: Maximum real number.
  
  const double minReal = 1.0e-200;
  //: Smallest +ve value a real may have.  
#endif
#endif
#endif
  
#ifdef MAXINT
  const int maxInt = MAXINT;
  //: Maximum integer used by Borland compiler.

  const int minInt = MININT;
  //: Minimum integer used by Borland compiler.
#else
  const int maxInt = INT_MAX;
  //: Maximum integer used by GNU C++ compiler.

  const int minInt = INT_MIN;
  //: Minimum integer used by GNU C++ compiler.
#endif

#if defined(LONG_LONG_MAX) && defined(LONG_LONG_MIN)
  const long long int maxInt64 = LONG_LONG_MAX;
  //: Maximum value a 64-bit integer can have.

  const long long int minInt64 = LONG_LONG_MIN;
  //: Minimum value a 64-bit integer can have.
#endif
  
#if defined(_I64_MIN) && defined(_I64_MAX)
  const long long int maxInt64 = _I64_MAX;
  //: Maximum value a 64-bit integer can have (Visual Studio).

  const long long int minInt64 = _I64_MIN;
  //: Minimum value a 64-bit integer can have (Visual Studio).
#endif

#if defined(MAXFLOAT) && defined(MINFLOAT)
  const float maxFloat = MAXFLOAT;
  //: Biggest value a float may have.
  
  const float minFloat = MINFLOAT;
  //: Smallest +ve value a float may have.  
#else 
#ifdef FLT_MAX
  const float maxFloat = FLT_MAX;
  //: Biggest value a float may have.
  
  const float minFloat = FLT_MIN;
  //: Smallest +ve value a float may have.  
#else
#ifdef __FLT_MAX__
  const float maxFloat = __FLT_MAX__;
  //: Biggest value a float may have.
  
  const float minFloat = __FLT_MIN__;
  //: Smallest +ve value a float may have.  
#else
#error "FLT_MIN not defined."
  const double maxReal =  1.0e30;
  //: Biggest value a float may have.
  
  const double minReal = 1.0e-30;
  //: Smallest +ve value a float may have.  
#endif
#endif
#endif

  //! userlevel=Develop
  const union u_nan_t { int i ; float f; } u_nan = {0x7ff80000};
  // internal nan.
  
  //! userlevel=Normal
  const float nanFloat RAVL_GNUEXT(__attribute__((unused))) = (float)(u_nan.f) ;
  //: Floating point not a number.
  
  const double nanReal RAVL_GNUEXT(__attribute__((unused))) = (double) nanFloat ;
  //: Real not a number.
  
  //! userlevel=Develop
  const union u_inf_t { int i ; float f; } u_inf = {0x7f800000};
  // internal infinity.
  
  //! userlevel=Normal
  const float infFloat  RAVL_GNUEXT(__attribute__((unused))) = (float)(u_inf.f) ;
  //: Floating point infinity.
  
  const double infReal RAVL_GNUEXT(__attribute__((unused))) = (double) infFloat ;
  //: Real infinity.
  
#else
  //:------------------------------
  // Currently valid implementation
  
  const double sqrt2;
  //: The value of the square root of 2.
  
  const double sqrt1_2;
  //: The value of the square root of 1/2.
  
  const double pi;
  //: The value of PI number.
  
  const double pi_2;
  //: The value of PI/2.
  
  const double pi_4;
  //: The value of PI/4.
  
  const double o1_pi;
  //: The value of 1/PI.
  
  const double o2_pi;
  //: The value of 2/PI.
  
  const double o2_sqrtPi;
  //: The value of 2/sqrt(PI).
  
  const double sqrt2Pi;
  //: The value of sqrt(2*PI).
  
  const double deg2rad;
  //: Conversion from degrees to radians.
  
  const double rad2deg; 
  //: Conversion from radians to degrees.
  
  const double e;
  //: The value of e number.
  
  const double log2e;
  //: The value of log2(e).
  
  const double log10e;
  //: The value of log10(e).
  
  const double ln2;
  //: The value of ln(2).
  
  const double ln10;
  //: The value of ln(10).
  
  //:------
  // Limits
  
  const double maxReal;
  //: Maximum real number.
  
  const double minReal;
  //: Minimum real number.

  const int maxInt;
  //: Maximum integer.
  
  const float maxFloat;
  //: Biggest value a float may have.
  
  const float minFloat;
  //: Snallest value a float may have.
  
  const double nanReal;
  //: Real not a number.
  
  const float nanFloat;
  //: Floating point not a number.
#endif
  
}

#endif
