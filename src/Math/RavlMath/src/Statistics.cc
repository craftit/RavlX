// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Statistics.cc"

#include "Ravl/Statistics.hh"
#include "Ravl/SArray1d.hh"
#include "ccmath/ccmath.h"

namespace RavlN {
  
  static const UIntT statNormalQSampleSize = 1024;
  static const RealT statNormalQSampleRange = 6;
  
  static SArray1dC<RealT> BuildStatNormalQ() {
    SArray1dC<RealT> tmp(statNormalQSampleSize);
    for(UIntT i = 0;i < tmp.Size();i++)
      tmp[i] = qnorm((RealT) (i) * statNormalQSampleRange / (RealT) statNormalQSampleSize);
    return tmp;
  }
  
  inline static RealT PosStatNormalQ(RealT val) {
    static const SArray1dC<RealT> table = BuildStatNormalQ();
    val *= (statNormalQSampleSize / statNormalQSampleRange);
    IntT at = (IntT) val;
    if((UIntT) (at+1) >= table.Size()) {
      // FIXME :- The following approximation could be better.
      return table[table.Size()-1] * (10.0 / (10.0 + (val - statNormalQSampleSize))); // Fill with a value tending to 1
    }
    RealT f = val - (RealT) at;
    RavlAssert(f >= 0 && f <= 1.0);
    return (1-f) * table[at] + f * table[at+1];
  }
  //: Integral from x to infinity of the standard normal distribution.
  // return value: Qn(x) = integral of normal density from x to infinity
  
  RealT StatNormalQ(RealT val,bool quickApprox) { 
    if(!quickApprox) 
      return qnorm(val); 
    if(val < 0) return 1 - PosStatNormalQ(-1 * val);
    return PosStatNormalQ(val);
  }
  
  //: Compute percentage points of the standard normal distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // return value: x = value for which Qn(x)=pc
  //                     x=HUGE -> pc outside limits
  RealT StatNormalPercentage(RealT pc) 
  { return pctn(pc); }
  
  //:  Evaluate the cumulative gamma distribution function.
  // x = (0 < x), a = (0 < a)
  // Returns integral of gamma density from x to infinity
  RealT StatGamaQ(RealT x,RealT a) 
  { return qgama(x,a); }
  
  //:  Evaluate the percentage points of the gamma distribution.
  // return value: x = value of x at which Qg(a,x)=pc, x=-1 -> pc outside limits
  RealT StatGamaPercentage(RealT pc,RealT a) 
  { return pctg(pc,a); }
  
  //: Compute the cumulative beta, F, or Student-t distribution.
  // x = value of the argument (0 < x < 1) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // return value: Qb = integral of the beta density from 0 to x
  RealT StatBetaQ(RealT x,RealT a,RealT b)
  { return qbeta(x,a,b); }
  
  //: Evaluate the percentage points of the beta distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // return value: x = value of x at which Qb(x,a,b)=pc
  //  x=-1 -> pc outside limits
  RealT StatBetaPercentage(RealT pc,RealT a,RealT b) 
  { return pctb(pc,a,b); }
  
  //: Compute the non-central gamma cumulative distribution function.
  // x = value of argument (x > 0) <p>
  // a = distribution parameter (a > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: Qg_nc = integral of noncentral gamma density from
  //                         x to infinity
  RealT StatNonCentralGamaQ(RealT x,RealT a,RealT d)
  { return qgnc(x,a,d); }
  
  //: Evaluate percentage points of the noncentral gamma distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a = distribution parameter (a > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: x = value of x at which Qg_nc(x,a,d)=pc
  // x=-1 -> pc outside limits
  RealT StatNonCentralGamaPercentage(RealT pc,RealT a,RealT d)
  { return pctgn(pc,a,d); }
  
  //: Compute the cumulative non-central beta distribution function.
  // x = value of argument (0< x <1) <p>
  // a,b = distribution parameters (a,b > 0) <p> 
  // d = non-centrality parameter (d >= 0) <p>
  // return value: Qb_nc = integral of the noncentral beta density
  //                        from 0 to x
  RealT StatNonCentralBetaQ(RealT x,RealT a,RealT b,RealT d)
  { return qbnc(x,a,b,d); }

  //: Evaluate percentage points of the noncentral beta distribution.
  // pc = probability argument (te< pc <1-te, with te=1.e-9) <p>
  // a,b = distribution parameters (a,b > 0) <p>
  // d = non-centrality parameter (d >= 0) <p>
  // return value: x = value of x at which Qb_nc(x,a,b,d)=pc
  RealT StatNonCentralBetaPercentage(RealT pc,RealT a,RealT b,RealT d)
  { return pctbn(pc,a,b,d); }
  
  
  
}
