// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/FitLineABC2d.cc"

#include "Ravl/LineABC2d.hh"
#include "Ravl/Moments2d2.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array1dIter.hh"

namespace RavlN {
  
#if 0
  static RealT LineResidual(const Moments2d2C &sums,RealT a,RealT b) {
    RealT n = sums.M00();    
    RealT sx2 = sums.M20();
    RealT sy2 = sums.M02();
    RealT sx1 = sums.M10();
    RealT sy1 = sums.M01();
    RealT sxy = sums.M11();
    RealT b2 = Sqr(b);
    return (sy2 - 2*a*sy1 + 2*b*sxy + a*a*n + 2*b * sx1 + b2*sx2) 
      / (1 + b2);
  }
#endif
  
  //: Fit points to a circle.
  // 'residual' is from the least squares fit and can be used to assess 
  // the quality of the fit.  Returns false if fit failed.
  
  bool LineABC2dC::FitLSQ(const Array1dC<Point2dC> &points,RealT &residual) {
    if(points.Size() < 2)
      return false; // Not enough points.
    Moments2d2C sums;
    for(Array1dIterC<Point2dC> it(points);it;it++)
      sums += *it;
#if 0
    // Minimise perpendicular distance.
    RealT n = sums.M00();
    RealT sx2 = sums.M20();
    RealT sy2 = sums.M02();
    RealT sx1 = sums.M10();
    RealT sy1 = sums.M01();
    
    RealT B = ((sy2 - Sqr(sy1) / n) - (sx2 - Sqr(sx1) / n)) / 
      ((sx1 * sy1 / n) - sums.M11());
    
    RealT sr = Sqrt(Sqr(B) + 1);
    
    RealT b1 = B+sr;
    RealT b2 = B-sr;
    RealT a1 = (sy1 - b1 * sx1) / n;
    RealT a2 = (sy1 - b2 * sx1) / n;
    
    RealT r1 = LineResidual(sums,b1,a1);
    RealT r2 = LineResidual(sums,b2,a2);
    cerr << "Residual1=" << r1 << "\n";
    cerr << "Residual2=" << r2 << "\n";
    if(r1 < r2) 
      (*this) = LineABC2dC(1.0,-b1,-a1);
    else
      (*this) = LineABC2dC(1.0,-b2,-a2);
#else
    RealT sxy = sums.M11()  - sums.M00() * sums.CentroidX() * sums.CentroidY();
    RealT sxx = sums.M20() - sums.M00() * Sqr(sums.CentroidX());
    RealT syy = sums.M02() - sums.M00() * Sqr(sums.CentroidY());
    RealT prod = (sxx * syy);
    if(prod == 0) residual = 0;
    else residual = Sqrt(Sqr(sxy) / prod);
    
    if(sums.VarX() > sums.VarY()) {
      RealT b = sxy/sxx;
      RealT a = (sums.M01() - b * sums.M10())/sums.M00();
      (*this) = LineABC2dC(-b, 1.0,-a);
    } else {
      RealT b = sxy/syy;
      RealT a = (sums.M10() - b * sums.M01())/sums.M00();
      (*this) = LineABC2dC(1.0,-b,-a);
    }
#endif
    return true;
  }
  
  
}
