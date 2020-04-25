// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitQuadraticPoints.cc"

#include "Ravl/FitQuadraticPoints.hh"
#include "Ravl/StateVectorQuadratic.hh"
#include "Ravl/ObservationQuadraticPoint.hh"
#include "Ravl/ObservationImpQuadraticPoint.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {

  //: Constructor.
  FitQuadraticPointsBodyC::FitQuadraticPointsBodyC()
  {
  }

  //: Fit parameters to sample of observations
  StateVectorC FitQuadraticPointsBodyC::FitModel(DListC<ObservationC> sample)
  {
    // we need at least two points to fit a 2D line
    if ( sample.Size() < 3 )
      throw ExceptionC("Sample size too small in FitQuadraticPointsBodyC::FitModel(). ");

    if ( sample.Size() == 3 ) {
      // initialise quadratic curve y = a*x^2 + b*x + c using three points.
      // We construct equations
      //
      //     (y1)   (x1*x1 x1 1) (a)
      //     (y2) = (x2*x2 x2 1) (b) = A * b for 3x3 matrix A and 3-vector b
      //     (y3)   (x3*x3 x3 1) (c)
      //
      // and solve the equations by direct matrix inversion (not pretty...) to
      // obtain our estimate of a, b, c given points (x1,y1), (x2,y2)
      // and (x3,y3).

      Matrix3dC Am;
      Vector3dC bv;
      IntT i;
      RealT xc, yc;

      for ( i = 0; i < 3; i++ ) {
	const ObservationC &obs = sample.Nth(i);
	if ( dynamic_cast<const ObservationExplicitBodyC *>(&obs.Body()) != 0 ) {
	  // explicit form of point observation
	  const ObservationQuadraticPointC &eobs = obs;
	  xc = eobs.GetXC();
	  yc = eobs.GetZ()[0];
	}
	else {
	  // implicit form of point observation
	  const ObservationImpQuadraticPointC &iobs = obs;
	  xc = iobs.GetZ()[0];
	  yc = iobs.GetZ()[1];
	}

	Am[i][0] = xc*xc; Am[i][1] = xc; Am[i][2] = 1.0; // row of A matrix
	bv[i] = yc; // element of b vector
      }

      // solve linear equations
      if(!SolveIP(Am,bv))
	return StateVectorC();

      return StateVectorQuadraticC(bv[0], bv[1], bv[2]);
    }

    Matrix3dC A;
    Vector3dC b;

    A.Fill(0.0);
    b.Fill(0.0);

    // compute solution by least squares
    RealT xc, yc;
    for(DLIterC<ObservationC> it(sample);it;it++) {
      const ObservationC &obs = it.Data();
      if ( dynamic_cast<const ObservationExplicitBodyC *>(&obs.Body()) != 0 ) {
	// explicit form of point observation
	const ObservationQuadraticPointC &eobs = obs;
	xc = eobs.GetXC();
	yc = eobs.GetZ()[0];
      }
      else {
	// implicit form of point observation
	const ObservationImpQuadraticPointC &iobs = obs;
	xc = iobs.GetZ()[0];
	yc = iobs.GetZ()[1];
      }

      A[0][0] += xc*xc*xc*xc;
      A[0][1] += xc*xc*xc;
      A[0][2] += xc*xc;
      A[1][0] += xc*xc*xc;
      A[1][1] += xc*xc;
      A[1][2] += xc;
      A[2][0] += xc*xc;
      A[2][1] += xc;
      A[2][2] += 1.0;
      b[0] += xc*xc*yc;
      b[1] += xc*yc;
      b[2] += yc;
    }

    // solve linear equations
    if(!SolveIP(A,b))
	return StateVectorC();

    return StateVectorQuadraticC(b[0], b[1], b[2]);
  }
}
