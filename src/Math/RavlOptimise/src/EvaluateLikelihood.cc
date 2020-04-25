// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/EvaluateLikelihood.cc"

#include "Ravl/EvaluateLikelihood.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {

  //: Constructor.
  EvaluateLikelihoodBodyC::EvaluateLikelihoodBodyC(RealT nchi2Thres)
  {
    chi2Thres = nchi2Thres;
  }

  //: Returns the log likelihood for the given state parameters
  RealT EvaluateLikelihoodBodyC::SolutionScore(
                                        const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    RealT totalLogLikelihood=0.0;
    for(ConstDLIterC<ObservationC> it(obsList);it;it++)
      // only use observations that have not already been selected
      if(!it.Data().GetSelected())
	// compute the residual and decrement likelihood with it
	totalLogLikelihood -= it.Data().Residual(stateVec);

    return totalLogLikelihood;
  }

  //: Returns the observations compatible with the given state parameters
  DListC<ObservationC> EvaluateLikelihoodBodyC::CompatibleObservations(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    DListC<ObservationC> compatibleList;

    for(ConstDLIterC<ObservationC> it(obsList);it;it++) {
      // compute the residual
      RealT residual = it.Data().NonRobustResidual(stateVec);

      // add to list if residual is within threshold
      if ( residual < chi2Thres )
	compatibleList.InsLast(it.Data());
    }

    return compatibleList;
  }
}
