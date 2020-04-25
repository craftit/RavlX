// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationQuadraticPoint.cc"

#include "Ravl/ObservationQuadraticPoint.hh"
#include "Ravl/ObsVectorBiGaussian.hh"
#include "Ravl/StateVectorQuadratic.hh"

namespace RavlN {

  //: static method to build observation vector from y-coordinate
  VectorC ObservationQuadraticPointBodyC::BuildZ(RealT nyc)
  {
    VectorC z(1);

    z[0] = nyc;
    return z;
  }

  //: static method to build observation inverse covariance
  MatrixRSC ObservationQuadraticPointBodyC::BuildNi(RealT nsigma)
  {
    MatrixRSC Ni(1);

    Ni[0][0] = 1.0/(nsigma*nsigma);
    return Ni;
  }

  //: Constructor
  ObservationQuadraticPointBodyC::ObservationQuadraticPointBodyC(RealT nxc,
								 RealT nyc,
								 RealT nsigma)
    : ObservationExplicitBodyC(ObsVectorC(BuildZ(nyc),BuildNi(nsigma)))
  {
    xc = nxc;
  }

  //: Constructor
  ObservationQuadraticPointBodyC::ObservationQuadraticPointBodyC(
						RealT nxc,
						RealT nyc,
						RealT nsigma,
						RealT nvarScale,
						RealT nchi2Thres)
    : ObservationExplicitBodyC(ObsVectorBiGaussianC(BuildZ(nyc),BuildNi(nsigma),nvarScale,nchi2Thres))
  {
    xc = nxc;
  }

  //: Return x-coordinate of point
  RealT ObservationQuadraticPointBodyC::GetXC() const
  {
    return xc;
  }

  //: Evaluate an observation for a single point
  VectorC ObservationQuadraticPointBodyC::EvaluateFunctionH(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a quadratic curve
    const StateVectorQuadraticC sv(stateVec);
    RavlAssert(sv.IsValid());
    
    VectorC h(1);

    // evaluate h(x) = h(a,b,c) = y = a*x*x + b*x + c
    h[0] = sv.GetA()*xc*xc + sv.GetB()*xc + sv.GetC();
#if 0
    cout << "x=" << xc << " a=" << x[0] << " b=" << x[1] << " c=" << x[2];
    cout << " h=" << h[0] << endl;
#endif

    return h;
  }

  //: Evaluate the Jacobian for a single point
  MatrixC ObservationQuadraticPointBodyC::EvaluateJacobianHx(const StateVectorC &stateVec)
  {
    MatrixC H(1,3);

    // fill Jacobian with the matrix (dh/da dh/db dh/dc) = (x*x x 1)
    H[0][0] = xc*xc;
    H[0][1] = xc;
    H[0][2] = 1.0;
#if 0
    // we know that the state vector actually represents a quadratic curve
    const StateVectorQuadraticC sv(stateVec);
    RavlAssert(sv.IsValid());

    cout << "x=" << xc << " a=" << sv.GetA() << " b=" << sv.GetB() << " c=" << sv.GetC();
    cout << " H=" << H[0][0] << " " << H[0][1] << " " << H[0][2] << endl;
#endif
    return H;
  }
}
