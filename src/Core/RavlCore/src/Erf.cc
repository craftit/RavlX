// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Math/Erf.cc"

#if !RAVL_HAVE_ERF

#include "Ravl/Assert.hh"
#include "Ravl/Types.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/StdConst.hh"

namespace RavlN {
  using namespace RavlConstN;

// *******************************************************************
//
// This code is translated from the original FORTRAN version at
//  http://www.netlib.org/specfun/erf
//  Author: W. J. Cody
//          Mathematics and Computer Science Division
//          Argonne National Laboratory
//          Argonne, IL 60439
//
// *******************************************************************
//
//   The routine  CALERF  is intended for internal packet use only,
//   all computations within the packet being concentrated in this
//   routine.  The function subprograms invoke  CALERF  with the
//   statement
//
//          CALL CALERF(ARG,RESULT,JINT)
//
//   where the parameter usage is as follows
//
//      Function                     Parameters for CALERF
//       call              ARG                  Result          JINT
//
//     ERF(ARG)      ANY REAL ARGUMENT         ERF(ARG)          0
//     ERFC(ARG)     ABS(ARG) .LT. XBIG        ERFC(ARG)         1
//     ERFCX(ARG)    XNEG .LT. ARG .LT. XMAX   ERFCX(ARG)        2
//
//   The main computation evaluates near-minimax approximations
//   from "Rational Chebyshev approximations for the error function"
//   by W. J. Cody, Math. Comp., 1969, PP. 631-638.  This
//   transportable program uses rational functions that theoretically
//   approximate  erf(x)  and  erfc(x)  to at least 18 significant
//   decimal digits.  The accuracy achieved depends on the arithmetic
//   system, the compiler, the intrinsic functions, and proper
//   selection of the machine-dependent constants.
//
// *******************************************************************

RealT calerf(RealT arg, IntT jint)
{
  // Check the arguament
  RavlAlwaysAssert( !IsNan(arg) );
  if (IsInf(arg)) return (arg < 0 ? 0.0 : 1.0);

//------------------------------------------------------------------
//  Mathematical constants
//------------------------------------------------------------------
  static const RealT four(4.0), one(1.0), half(0.5), two(2.0), zero(0.0), sixteen(16.0),
                     sqrpi(5.6418958354775628695e-01), threshold(0.46875);
//------------------------------------------------------------------
//  Machine-dependent constants
//------------------------------------------------------------------
  static const RealT XMIN = minReal;
  // the smallest positive floating-point number.
  static const RealT XINF = maxReal;
  // the largest positive finite floating-point number.
  static const RealT XNEG = -Sqrt(Log(XINF/2.0));
  // the largest negative argument acceptable to ERFCX
  // the negative of the solution to the equation 2*exp(x*x) = XINF.
  static const RealT XSMALL = realPrecision;
  // argument below which erf(x) may be represented by  2*x/sqrt(pi)
  // and above which  x*x  will not underflow.
  // a conservative value is the largest machine number X such that 1.0 + X = 1.0
  static const RealT XBIG = 26.543; // This is the solution for XMIN = 2.23e-308
  // largest argument acceptable to ERFC
  // solution to the equation:  W(x) * (1-0.5/x**2) = XMIN, where W(x) = exp(-x*x)/[x*sqrt(pi)].
  static const RealT XHUGE = 1.0/(2.0*Sqrt(XSMALL));
  // argument above which  1.0 - 1/(2*x*x) = 1.0  to machine precision
  // a conservative value is 1/[2*sqrt(XSMALL)]
  static const RealT XMAX = Min(XINF,(RealT)(1.0/(1.5707963267948966192*XMIN)));
  // largest acceptable argument to ERFCX
  // the minimum of XINF and 1/[sqrt(pi)*XMIN].
//------------------------------------------------------------------
//  Coefficients for approximation to  erf  in first interval
//------------------------------------------------------------------
  static const RealT A[] = { 3.16112374387056560e+00,1.13864154151050156e+02,
                             3.77485237685302021e+02,3.20937758913846947e+03,
                             1.85777706184603153e-01 };
  static const RealT B[] = { 2.36012909523441209e+01,2.44024637934444173e+02,
                             1.28261652607737228e+03,2.84423683343917062e+03};
//------------------------------------------------------------------
//  Coefficients for approximation to  erfc  in second interval
//------------------------------------------------------------------
  static const RealT C[] = { 5.64188496988670089e-01,8.88314979438837594e+00,
                             6.61191906371416295e+01,2.98635138197400131e+02,
                             8.81952221241769090e+02,1.71204761263407058e+03,
                             2.05107837782607147e+03,1.23033935479799725e+03,
                             2.15311535474403846e-08};
  static const RealT D[] = { 1.57449261107098347e+01,1.17693950891312499e+02,
                             5.37181101862009858e+02,1.62138957456669019e+03,
                             3.29079923573345963e+03,4.36261909014324716e+03,
                             3.43936767414372164e+03,1.23033935480374942e+03};
//------------------------------------------------------------------
//  Coefficients for approximation to  erfc  in third interval
//------------------------------------------------------------------
  static const RealT P[] = { 3.05326634961232344e-01,3.60344899949804439e-01,
                             1.25781726111229246e-01,1.60837851487422766e-02,
                             6.58749161529837803e-04,1.63153871373020978e-02};
  static const RealT Q[] = { 2.56852019228982242e+00,1.87295284992346047e+00,
                             5.27905102951428412e-01,6.05183413124413191e-02,
                             2.33520497626869185e-03};
//------------------------------------------------------------------

  RealT result = zero;
  RealT x = arg;
  RealT y = Abs(x);
  if (y <= threshold) {
//------------------------------------------------------------------
//  Evaluate  erf  for  |X| <= 0.46875
//------------------------------------------------------------------
    RealT ysq = zero;
    if (y>XSMALL) ysq = y*y;
    RealT xnum = A[4]*ysq;
    RealT xden = ysq;
    for (int i=0; i<3; i++) {
      xnum = (xnum + A[i])*ysq;
      xden = (xden + B[i])*ysq;
    }
    result = x * (xnum + A[3]) / (xden + B[3]);
    if (jint!=0) result = one - result;
    if (jint==2) result = Exp(ysq)*result;
  } else {
    if ( y <= four) {
//------------------------------------------------------------------
//  Evaluate  erfc  for 0.46875 <= |X| <= 4.0
//------------------------------------------------------------------
      RealT xnum = C[8]*y;
      RealT xden = y;
      for (int i=0; i<7; i++) {
        xnum = (xnum + C[i])*y;
        xden = (xden + D[i])*y;
      }
      result = (xnum + C[7]) / (xden + D[7]);
      if (jint!=2) {
        RealT intpart; // Round toward zero 
        modf(y*sixteen, &intpart);
        RealT ysq = intpart/sixteen;
        RealT del = (y-ysq)*(y+ysq);
        result = Exp(-ysq*ysq)*Exp(-del)*result;
      }
    } else {
//------------------------------------------------------------------
//  Evaluate  erfc  for |X| > 4.0
//------------------------------------------------------------------
      result = zero;
      if (!(y>=XBIG && (jint!=2 || y>=XMAX))) {
        if (y>=XBIG && y>=XHUGE) result = sqrpi / y;
        else {
          RealT ysq = one / (y*y);
          RealT xnum = P[5]*ysq;
          RealT xden = ysq;
          for (int i=0; i<4; i++) {
            xnum = (xnum + P[i])*ysq;
            xden = (xden + Q[i])*ysq;
          }
          result = ysq * (xnum + P[4]) / (xden + Q[4]);
          result = (sqrpi - result) / y;
          if (jint!=2) {
            RealT intpart; // Round toward zero 
            modf(y*sixteen, &intpart);
            ysq = intpart/sixteen;
            RealT del = (y-ysq)*(y+ysq);
            result = Exp(-ysq*ysq)*Exp(-del)*result;
          }
        }
      }
    }
//------------------------------------------------------------------
//  Fix up for negative argument, erf, etc.
//------------------------------------------------------------------
    if (jint==0) {
      result = (half - result) + half;
      if (x<zero) result = -result;
    } else if (jint==1) {
      if (x<zero) result = two - result;
    } else {
      if (x<zero) {
        if (x<XNEG) {
          result = XINF;
        } else {
          RealT intpart; // Round toward zero 
          modf(x*sixteen, &intpart);
          RealT ysq = intpart/sixteen;
          RealT del = (x-ysq)*(x+ysq);
          y = Exp(ysq*ysq) * Exp(del);
          result = (y+y) - result;
        }
      }
    }
  }

// Done
  return result;
}

RealT erf(RealT d) { return calerf(d, 0); }
// This computes approximate values for erfc(x).
RealT erfc(RealT d) { return calerf(d, 1); }
// This computes approximate values for erfc(x).
RealT erfcx(RealT d) { return calerf(d, 2); }
// This computes approximate values for exp(x*x) * erfc(x).

}

#endif
