// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="3/5/1997"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/FitCircle2d.cc"

#include "Ravl/Circle2d.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Assert.hh"
#include "Ravl/LeastSquares.hh"
#include "Ravl/Array1d.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  ////////////////////
  // Fit to some points.
  
  bool Circle2dC::FitLSQ(const Array1dC<Point2dC> &points,RealT &residual) {
    IntT N = points.Size();
    if(N < 3) // Under determined.
      return false;  
    MatrixC A(N,3);
    VectorC B(N);
    for(IntT i =0 ;i < N;i++) {
      const RealT X = points[i][0];
      const RealT Y = points[i][1];
      A[i][0] = X;
      A[i][1] = Y;
      A[i][2] = 1;
      B[i] = - X * X - Y * Y ;
    }
    if(!LeastSquaresQR_IP(A,B,residual))
      return false; // Fit failed.
    
    const RealT X = B[0] / -2;
    const RealT Y = B[1] / -2;
    Centre() = Point2dC(X,Y);
    Radius() = Sqrt(((X * X) + (Y * Y)) - B[2]);
    ONDEBUG(cerr << "Circle2dC::FitLSQ() Center=" << Centre() << " Radius=" << Radius() << "\n");
    return true;
  }

}
