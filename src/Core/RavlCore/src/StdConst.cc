// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore
//! file="Ravl/Core/Base/StdConst.cc"

#include "Ravl/StdConst.hh"

namespace RavlConstN {
#if 0
  //!RAVL_NEW_ANSI_CXX_DRAFT
  
  const double sqrt2     =  1.41421356237309504880;
  const double sqrt1_2   =  0.70710678118654752440;
  const double pi        =  3.1415926535897932384626433832795028;
  const double pi_2      =  1.57079632679489661923;
  const double pi_4      =  0.78539816339744830962;
  const double o1_pi     =  0.31830988618379067154;
  const double o2_pi     =  0.63661977236758134308;
  const double o2_sqrtPi =  1.12837916709551257390;
  const double sqrt2Pi   =  2.50662827465;
  const double deg2rad   =  pi/180.0;
  const double rad2deg   =  180.0/pi; 
  const double e         =  2.7182818284590452354;
  const double log2e     =  1.4426950408889634074;
  const double log10e    =  0.43429448190325182765;
  const double ln2       =  0.69314718055994530942;
  const double ln10      =  2.30258509299404568402;
  
#ifdef MAXDOUBLE
  const double maxReal   = MAXDOUBLE;
  const double minReal   = MINDOUBLE;
#else
#ifdef DBL_MAX
  const double maxReal   =  DBL_MAX;
  const double minReal   =  DBL_MIN;
#else
  const double maxReal   =  1.0e200;
  const double minReal   = -1.0e200;
#endif
#endif
  
#ifdef MAXINT
  const int maxInt = MAXINT;
#else
  const int maxInt = INT_MAX;
#endif
  
#ifdef MAXFLOAT
  const float maxFloat = MAXFLOAT;
  const float minFloat = MINFLOAT;
#else
  const float maxFloat = FLT_MAX;
  const float minFloat = FLT_MIN;
#endif
  
  static  const union { int i ; float f; } StdConst_nan = {0x7ff80000};
  // internal nan.
  
  const float nanFloat = (StdConst_nan.f);

  const double nanReal = (double)(nanFloat);
#endif
  
}


