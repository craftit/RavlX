// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Optimise2dHomography.cc"

#include "Ravl/Optimise2dHomography.hh"
#include "Ravl/ObservationHomog2dPoint.hh"
#include "Ravl/Ransac.hh"
#include "Ravl/FitHomog2dPoints.hh"
#include "Ravl/EvaluateNumInliers.hh"
#include "Ravl/LevenbergMarquardt.hh"

namespace RavlN {

  // Shrink-wrap homography fitting function
  const StateVectorHomog2dC
  Optimise2dHomography ( DListC<Point2dPairObsC> &matchList,
			 RealT zh1, RealT zh2,
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
    for(DLIterC<Point2dPairObsC> it(matchList);it;it++)
      obsList.InsLast(ObservationHomog2dPointC(it.Data().z1(), it.Data().Ni1(),
					       it.Data().z2(), it.Data().Ni2(),
					       varScale, chi2Thres));

    // Build RANSAC components
    ObservationListManagerC obsManager(obsList);
    FitHomog2dPointsC fitter(zh1, zh2);
    EvaluateNumInliersC evaluator(ransacChi2Thres, compatChi2Thres);
  
    // use RANSAC to fit homography
    RansacC ransac(obsManager, fitter, evaluator);

    // select and evaluate the given number of samples
    for ( UIntT iteration=0; iteration < noRansacIterations; iteration++ )
      ransac.ProcessSample(8);

    // select observations compatible with solution
    obsList = evaluator.CompatibleObservations(ransac.GetSolution(), obsList);

    // initialise Levenberg-Marquardt algorithm with Ransac solution
    StateVectorHomog2dC stateVecInit = ransac.GetSolution();
    LevenbergMarquardtC lm = LevenbergMarquardtC(stateVecInit, obsList);

    // apply Levenberg-Marquardt iterations
    lm.NIterations ( obsList, noLevMarqIterations, lambdaStart, lambdaFactor );

    return lm.GetSolution();
  }
}
