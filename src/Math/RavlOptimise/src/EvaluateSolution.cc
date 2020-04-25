// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/EvaluateSolution.cc"

#include "Ravl/EvaluateSolution.hh"

namespace RavlN {

  //: Returns the score/vote/likelihood for the given state parameters
  RealT EvaluateSolutionBodyC::SolutionScore(
                                        const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    RavlAssertMsg(0,"EvaluateSolutionBodyC::SolutionScore(const StateVectorC &, DListC<ObservationC> &) const, Abstract method called ");
    return 0.0;
  }

  //: Returns the observations compatible with the given state parameters
  DListC<ObservationC> EvaluateSolutionBodyC::CompatibleObservations(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    DListC<ObservationC> emptyObsList;
    RavlAssertMsg(0,"EvaluateSolutionBodyC::CompatibleObservations(const StateVectorC &, DListC<ObservationC> &) const, Abstract method called ");
    return emptyObsList;
  }

  //: Returns the observations compatible with the given state parameters
  DListC<bool> EvaluateSolutionBodyC::ObservationCompatibility(
					const StateVectorC &stateVec,
					const DListC<ObservationC> &obsList) const
  {
    DListC<bool> emptyObsList;
    RavlAssertMsg(0,"EvaluateSolutionBodyC::ObservationCompatibility(const StateVectorC &, DListC<ObservationC> &) const, Abstract method called ");
    return emptyObsList;
  }
}
