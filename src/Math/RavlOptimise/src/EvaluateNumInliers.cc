// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/EvaluateNumInliers.cc"

#include "Ravl/EvaluateNumInliers.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {

  //: Constructor.
  EvaluateNumInliersBodyC::EvaluateNumInliersBodyC(RealT nchi2Thres,
						   RealT ncompatChi2Thres)
  {
    chi2Thres = nchi2Thres;
    compatChi2Thres = (ncompatChi2Thres == 0.0)
                      ? chi2Thres : ncompatChi2Thres;
  }

  //: Returns the number of inliers for the given state parameters
  RealT EvaluateNumInliersBodyC::SolutionScore(
                                        const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    RavlAssert(stateVec.IsValid());
    UIntT totalVote=0;
    for(ConstDLIterC<ObservationC> it(obsList);it;it++)
      // only use observations that have not already been selected
      if(!it.Data().GetSelected()) {
	// compute the residual
	RealT residual = it.Data().NonRobustResidual(stateVec);

	// increment counter if residual is within threshold
	if ( residual < chi2Thres )
	  totalVote++;
      }

    return (RealT)totalVote;
  }

  //: Returns the observations compatible with the given state parameters
  DListC<ObservationC> EvaluateNumInliersBodyC::CompatibleObservations(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    RavlAssert(stateVec.IsValid());
    DListC<ObservationC> compatibleList;

    for(ConstDLIterC<ObservationC> it(obsList);it;it++) {
      // compute the residual
      RealT residual = it.Data().NonRobustResidual(stateVec);

      // add to list if residual is within threshold
      if ( residual < compatChi2Thres )
	compatibleList.InsLast(it.Data());
    }

    return compatibleList;
  }

  //: Returns  list of booleans indicating which observations are compatible with the given state parameters
  DListC<bool> EvaluateNumInliersBodyC::ObservationCompatibility(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    RavlAssert(stateVec.IsValid());
    DListC<bool> compatibleList;

    for(ConstDLIterC<ObservationC> it(obsList);it;it++) {
      // compute the residual
      RealT residual = it.Data().NonRobustResidual(stateVec);

      // add true/false to list if residual is within / not within threshold
      compatibleList.InsLast( residual < compatChi2Thres );
    }

    return compatibleList;
  }
}
