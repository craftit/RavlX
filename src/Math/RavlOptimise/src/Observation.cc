// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Observation.cc"

#include "Ravl/Observation.hh"
#include "Ravl/ObsVectorBiGaussian.hh"

namespace RavlN {
  
  //: Constructor.
  ObservationBodyC::ObservationBodyC(const ObsVectorC &nobsVec)
  {
    obsVec = nobsVec;
    selected = false;
  }

  //: Compute the residual (negative log-likelihood) of the observation
  RealT ObservationBodyC::Residual(const StateVectorC &stateVec) {
    RavlAssertMsg(0,"ObservationBodyC::GetResidual(const VectorC &), Abstract method called ");
    return 0;
  }
  
  //: Compute the residual (negative log-likelihood) of the observation
  RealT ObservationBodyC::NonRobustResidual(const StateVectorC &stateVec) {
    RavlAssertMsg(0,"ObservationBodyC::GetNonRobustResidual(const VectorC &), Abstract method called ");
    return 0;
  }
  
  //: Accumulate the linear system
  bool ObservationBodyC::IncrementLS(const StateVectorC &stateVec,
				     MatrixRSC &A,
				     VectorC &a) {
    RavlAssertMsg(0,"ObservationBodyC::IncrementLS(const VectorC &,MatrixRSC &,VectorC &) const, Abstract method called ");
    return false;
  }
  
  //: Get the number of constraints imposed on the state vector
  UIntT ObservationBodyC::GetNumConstraints() const
  {
    RavlAssertMsg(0,"ObservationBodyC::GetNumConstraints() const, Abstract method called ");
    return 0;
  }
}
