// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImpHomog2dPoint.cc"

#include "Ravl/ObservationImpHomog2dPoint.hh"
#include "Ravl/StateVectorHomog2d.hh"
#include "Ravl/ObsVectorPoint2dPair.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {

  //: Constructor.
  ObservationImpHomog2dPointBodyC::ObservationImpHomog2dPointBodyC(
			     const Vector2dC &nz1, const MatrixRSC &nNi1,
			     const Vector2dC &nz2, const MatrixRSC &nNi2)
    : ObservationImplicitBodyC(ObsVectorPoint2dPairC(nz1,nNi1,nz2,nNi2),2)
  {
  }

  //: Constructor for robust bi-Gaussian observation.
  ObservationImpHomog2dPointBodyC::ObservationImpHomog2dPointBodyC(
			     const Vector2dC &nz1, const MatrixRSC &nNi1,
			     const Vector2dC &nz2, const MatrixRSC &nNi2,
			     RealT nvarScale, RealT nchi2Thres)
    : ObservationImplicitBodyC(ObsVectorBGPoint2dPairC(nz1,nNi1,nz2,nNi2,
						       nvarScale,nchi2Thres),
			       2)
  {
  }

  //: Evaluate an observation for a single point
  VectorC ObservationImpHomog2dPointBodyC::EvaluateFunctionF(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a 2D homography
    const StateVectorHomog2dC sv = stateVec;
    RavlAssert(sv.IsValid());
    
    const Matrix3dC &P = sv.GetHomog(); // homography matrix
    const VectorC &z = GetZ(); // 2D point pair
    RealT zh1 = sv.GetZH1(), zh2 = sv.GetZH2();

    // compute p = P*x1
    Vector3dC p(P[0][0]*z[0] + P[0][1]*z[1] + P[0][2]*zh1,
		P[1][0]*z[0] + P[1][1]*z[1] + P[1][2]*zh1,
		P[2][0]*z[0] + P[2][1]*z[1] + P[2][2]*zh1);
    
    if ( fabs(p[2]) < 1.0e-20 )
      throw ExceptionNumericalC("Divide by near-zero in ObservationImpHomog2dPointBodyC::EvaluateFunctionF(). ");

    // evaluate projection store in observation object
    p2[0] = p[0]*zh2/p[2];
    p2[1] = p[1]*zh2/p[2];

    // store computed z-coordinate as well
    z2 = p[2];
    
    // subtract observed point on plane 2
    return VectorC(p2[0]-z[2], p2[1]-z[3]);
  }
#if 1
  //: Evaluate the Jacobian of an observation for a single point w.r.t. the
  //: 2D point pair observation
  MatrixC ObservationImpHomog2dPointBodyC::EvaluateJacobianFz(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a 2D homography
    const StateVectorHomog2dC& sv = stateVec;
    RavlAssert(sv.IsValid());
    
    const Matrix3dC &P = sv.GetHomog(); // homography matrix
    RealT zh2 = sv.GetZH2();
    MatrixC Fz(2,4); // Jacobian matrix

    // evaluate Jacobian
    Fz[0][0] = (P[0][0]*zh2 - P[2][0]*p2[0])/z2;
    Fz[0][1] = (P[0][1]*zh2 - P[2][1]*p2[0])/z2;
    Fz[0][2] = -1.0;
    Fz[0][3] = 0.0;
    Fz[1][0] = (P[1][0]*zh2 - P[2][0]*p2[1])/z2;
    Fz[1][1] = (P[1][1]*zh2 - P[2][1]*p2[1])/z2;
    Fz[1][2] = 0.0;
    Fz[1][3] = -1.0;
    return Fz;
  }

  //: Evaluate the Jacobian of an observation for a single point w.r.t. the
  //: homography parameters
  MatrixC ObservationImpHomog2dPointBodyC::EvaluateJacobianFx(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a 2D homography
    const StateVectorHomog2dC& sv = stateVec;
    RavlAssert(sv.IsValid());
    
    const VectorC &z = GetZ(); // 2D point pair
    RealT zh1 = sv.GetZH1(), zh2 = sv.GetZH2();
    MatrixC Fx(2,9); // Jacobian matrix

    // evaluate Jacobian
    Fx[0][0] = z[0]*zh2/z2;     Fx[1][0] = 0.0;
    Fx[0][1] = z[1]*zh2/z2;     Fx[1][1] = 0.0;
    Fx[0][2] = zh1*zh2/z2;      Fx[1][2] = 0.0;
    Fx[0][3] = 0.0;             Fx[1][3] = z[0]*zh2/z2;    
    Fx[0][4] = 0.0;             Fx[1][4] = z[1]*zh2/z2;    
    Fx[0][5] = 0.0;             Fx[1][5] = zh1*zh2/z2;    
    Fx[0][6] = -z[0]*p2[0]/z2;  Fx[1][6] = -z[0]*p2[1]/z2;
    Fx[0][7] = -z[1]*p2[0]/z2;  Fx[1][7] = -z[1]*p2[1]/z2;
    Fx[0][8] = -zh1*p2[0]/z2;   Fx[1][8] = -zh1*p2[1]/z2;
    return Fx;
  }
#endif
}
