// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/OptimiseQuadraticCurve.cc"

#include "Ravl/OptimiseQuadraticCurve.hh"
#include "Ravl/ObservationQuadraticPoint.hh"
#include "Ravl/Ransac.hh"
#include "Ravl/FitQuadraticPoints.hh"
#include "Ravl/EvaluateNumInliers.hh"
#include "Ravl/LevenbergMarquardt.hh"

namespace RavlN {

  // Shrink-wrap quadratic curve fitting function
  const StateVectorQuadraticC
  OptimiseQuadraticCurve ( DListC<Point2dObsC> &matchList,
			   RealT varScale,
			   RealT chi2Thres,
			   UIntT noRansacIterations,
			   RealT ransacChi2Thres,
			   RealT compatChi2Thres,
			   UIntT noLevMarqIterations,
			   RealT lambdaStart,
			   RealT lambdaFactor )
  {
    // build list of observations
    DListC<ObservationC> obsList;
    for(DLIterC<Point2dObsC> it(matchList);it;it++)
      obsList.InsLast(ObservationQuadraticPointC(it.Data().z()[0],
						 it.Data().z()[1],
						 it.Data().Ni()[1][1],
						 varScale, chi2Thres));
    // Build RANSAC components
    ObservationListManagerC obsManager(obsList);
    FitQuadraticPointsC fitter;
    EvaluateNumInliersC evaluator(ransacChi2Thres, compatChi2Thres);
  
    // use RANSAC to fit affine projection
    RansacC ransac(obsManager, fitter, evaluator);

    // select and evaluate the given number of samples
    for ( UIntT iteration=0; iteration < noRansacIterations; iteration++ )
      ransac.ProcessSample(3);

    // select observations compatible with solution
    obsList = evaluator.CompatibleObservations(ransac.GetSolution(), obsList);

    // initialise Levenberg-Marquardt algorithm with Ransac solution
    StateVectorQuadraticC stateVecInit = ransac.GetSolution();
    LevenbergMarquardtC lm = LevenbergMarquardtC(stateVecInit, obsList);

    // apply Levenberg-Marquardt iterations
    lm.NIterations ( obsList, noLevMarqIterations, lambdaStart, lambdaFactor );

    return lm.GetSolution();
  }
}
