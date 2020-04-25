// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationAffine2dPoint.cc"

#include "Ravl/ObservationAffine2dPoint.hh"
#include "Ravl/StateVectorAffine2d.hh"
#include "Ravl/ObsVectorBiGaussian.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Point2d.hh"

namespace RavlN {

  //: Constructor.
  
  ObservationAffine2dPointBodyC::ObservationAffine2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
							       const Vector2dC &nz2, const MatrixRSC &nNi2)
    : ObservationExplicitBodyC(ObsVectorC(nz2,nNi2)),
      z1(nz1),
      Ni1(nNi1.Copy())
  {
  }
  
  //: Constructor for robust bi-Gaussian observation.

  ObservationAffine2dPointBodyC::ObservationAffine2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
							       const Vector2dC &nz2, const MatrixRSC &nNi2,
							       RealT varScale, RealT chi2Thres)
    : ObservationExplicitBodyC(ObsVectorBiGaussianC(nz2,nNi2,varScale,chi2Thres)),
      z1(nz1),
      Ni1(nNi1.Copy())
  {
  }

  //: Evaluate an observation for a single point
  
  VectorC ObservationAffine2dPointBodyC::EvaluateFunctionH(const StateVectorC &stateVec) {
    // we know that the state vector actually represents a 2D affine projection
    const StateVectorAffine2dC sv(stateVec);
    RavlAssert(sv.IsValid());
    Vector2dC p = sv.GetAffine() * z1;
    return VectorC(p);
  }
  
  //: Evaluate the Jacobian of an observation for a single point
  
  MatrixC ObservationAffine2dPointBodyC::EvaluateJacobianHx(const StateVectorC &stateVec) {
    // we know that the state vector actually represents a 2D affine projection
    const StateVectorAffine2dC sv(stateVec);
    RavlAssert(sv.IsValid());
    Vector2dC p = sv.GetAffine() * z1;
    
    MatrixC H(2,6); // Jacobian matrix
    
    H[0][0] = p[0];   H[1][0] = 0;
    H[0][1] = p[1];   H[1][1] = 0;
    H[0][2] = 1;      H[1][2] = 0;
    H[0][3] = 0;      H[1][3] = p[0];
    H[0][4] = 0;      H[1][4] = p[1];
    H[0][5] = 0;      H[1][5] = 1;
    
    return H;
  }
}
