// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STATISTICS_HEADER
#define RAVL_STATISTICS_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! docentry="Ravl.API.Math.Statistics"
//! author="Daniel A. Atkinson"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Statistics.hh"

#include "Ravl/Types.hh"

namespace RavlN {
  
  RealT StatNormalQ(RealT x,bool quickApprox = false);
  //: Integral from x to infinity of the standard normal distribution.
  //!param: x - Point in distribution 
  //!param: quickApprox - Use a table based approximation which makes this function 10x faster, but is not as accurate.
  // return value: Qn(x) = integral of normal density from x to infinity
  
  RealT StatNormalPercentage(RealT pc);
  //: Compute percentage points of the standard normal distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // return value: x = value for which Qn(x)=pc
  //                     x=HUGE -> pc outside limits
  
  RealT StatGamaQ(RealT x,RealT a);
  //:  Evaluate the cumulative gamma distribution function.
  // x = (0 < x), a = (0 < a)
  // Returns integral of gamma density from x to infinity
  
  RealT StatGamaPercentage(RealT pc,RealT a);
  //:  Evaluate the percentage points of the gamma distribution.
  // return value: x = value of x at which Qg(a,x)=pc, x=-1 -> pc outside limits

  RealT StatBetaQ(RealT x,RealT a,RealT b);
  //: Compute the cumulative beta, F, or Student-t distribution.
  // x = value of the argument (0 < x < 1) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // return value: Qb = integral of the beta density from 0 to x
  
  RealT StatBetaPercentage(RealT pc,RealT a,RealT b);
  //: Evaluate the percentage points of the beta distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // return value: x = value of x at which Qb(x,a,b)=pc
  //  x=-1 -> pc outside limits
  
  RealT StatNonCentralGamaQ(RealT x,RealT a,RealT d);
  //: Compute the non-central gamma cumulative distribution function.
  // x = value of argument (x > 0) <p>
  // a = distribution parameter (a > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: Qg_nc = integral of noncentral gamma density from
  //                         x to infinity
  
  RealT StatNonCentralGamaPercentage(RealT pc,RealT a,RealT d);
  //: Evaluate percentage points of the noncentral gamma distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a = distribution parameter (a > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: x = value of x at which Qg_nc(x,a,d)=pc
  // x=-1 -> pc outside limits

  RealT StatNonCentralBetaQ(RealT x,RealT a,RealT b,RealT d);
  //: Compute the cumulative non-central beta distribution function.
  // x = value of argument (0< x <1) <p>
  // a,b = distribution parameters (a,b > 0) <p> 
  // d = non-centrality parameter (d >= 0) <p>
  // return value: Qb_nc = integral of the noncentral beta density
  //                        from 0 to x

  RealT StatNonCentralBetaPercentage(RealT pc,RealT a,RealT b,RealT d);
  //: Evaluate percentage points of the noncentral beta distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: x = value of x at which Qb_nc(x,a,b,d)=pc  
}


#endif
