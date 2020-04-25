// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Ransac.cc"

#include "Ravl/Ransac.hh"
#include "Ravl/StdConst.hh"

namespace RavlN {

  //: Constructor for RANSAC
  RansacC::RansacC(ObservationManagerC &nobsManager,
		   FitToSampleC &nmodelFitter,
		   EvaluateSolutionC &nevaluator)
    : obsManager(nobsManager),
      modelFitter(nmodelFitter),
      evaluator(nevaluator)
  {
    highestVote = RavlConstN::minReal;
  }

  //: Generate sample, compute vote and update best solution and vote
  bool RansacC::ProcessSample(UIntT minNumConstraints)
  {
    // reset "selected" flags
    obsManager.UnselectAllObservations();

    // generate sample and abort if it can't be generated
    DListC<ObservationC> sample;
    try {
      sample = obsManager.RandomSample(minNumConstraints);
    }
    catch(ExceptionC &) {
      return false;
    }

    // fit model and abort on any numerical errors found
    StateVectorC sv;
    try {
      sv = modelFitter.FitModel(sample);
    }
    catch(ExceptionNumericalC &) {
      return false;
    }

    // generate list of observations to be evaluated
    DListC<ObservationC> obsList = obsManager.ObservationList(sv);
    RealT newVote;
    try {
      newVote = evaluator.SolutionScore(sv, obsList);
    }
    catch (ExceptionNumericalC &) {
      return false;
    }
    if ( newVote > highestVote ) {
      stateVec = sv.Copy();
      highestVote = newVote;
    }
    
    return true;
  }

  //: Return the highest vote found so far 
  RealT RansacC::GetHighestVote() const
  {
    return highestVote;
  }
}
