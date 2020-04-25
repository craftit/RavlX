// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OPTIMISEQUADRATICCURVE_HEADER
#define RAVLMATH_OPTIMISEQUADRATICCURVE_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/OptimiseQuadraticCurve.hh"

#include "Ravl/StateVectorQuadratic.hh"
#include "Ravl/Point2dObs.hh"
#include "Ravl/DList.hh"

namespace RavlN {

  //: Shrink-wrap quadratic curve fitting function
  // <ul>
  //  <li> matchList: A list of 2D points.
  //  <li> varScale: The ratio of outlier and inlier error standard deviations.
  //  <li> noRansacIterations: The number of RANSAC iterations to perform.
  //  <li> ransacChi2Thres: The RANSAC inlier error threshold.
  //  <li> compatChi2Thres: The threshold for compatibility used to select
  //                        the final inlier matches
  //  <li> noLevMarqIterations: The number of Levenberg-Marquardt iterations.
  //  <li> lambdaStart: Starting value for the damping factor lambda.
  //  <li> lambdaFactor: Multiplication factor for damping factor lambda.
  // </ul>
  const StateVectorQuadraticC
  OptimiseQuadraticCurve ( DListC<Point2dObsC> &matchList,
			   RealT varScale=10.0,
			   RealT chi2Thres=5.0,
			   UIntT noRansacIterations=100,
			   RealT ransacChi2Thres=3.0,
			   RealT compatChi2Thres=5.0,
			   UIntT noLevMarqIterations=10,
			   RealT lambdaStart=0.1,
			   RealT lambdaFactor=0.1 );

}  


#endif
