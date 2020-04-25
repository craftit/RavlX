// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImpQuadraticPoint.cc"

#include "Ravl/ObservationImpQuadraticPoint.hh"
#include "Ravl/ObsVectorBiGaussian.hh"
#include "Ravl/StateVectorQuadratic.hh"

namespace RavlN {

  //: static method to build observation vector from y-coordinate
  VectorC ObservationImpQuadraticPointBodyC::BuildZ(RealT nxc, RealT nyc)
  {
    VectorC z(2);

    z[0] = nxc;
    z[1] = nyc;
    return z;
  }

  //: static method to build observation inverse covariance
  MatrixRSC ObservationImpQuadraticPointBodyC::BuildNi(RealT nsigma)
  {
    MatrixRSC Ni(2);

    Ni.Fill(0.0);
    Ni[0][0] = Ni[1][1] = 1.0/(nsigma*nsigma);
    return Ni;
  }

  //: Constructor
  ObservationImpQuadraticPointBodyC::ObservationImpQuadraticPointBodyC(
								RealT nxc,
								RealT nyc,
								RealT nsigma)
    : ObservationImplicitBodyC(ObsVectorC(BuildZ(nxc,nyc),BuildNi(nsigma)),1)
  {
  }

  //: Constructor
  ObservationImpQuadraticPointBodyC::ObservationImpQuadraticPointBodyC(
							     RealT nxc,
							     RealT nyc,
							     RealT nsigma,
							     RealT nvarScale,
							     RealT nchi2Thres)
    : ObservationImplicitBodyC(ObsVectorBiGaussianC(BuildZ(nxc,nyc),BuildNi(nsigma),nvarScale,nchi2Thres),1)
  {
  }

  //: Evaluate the observation function F(x,z) given x and z for a single point
  VectorC ObservationImpQuadraticPointBodyC::EvaluateFunctionF(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a 2D homography
    const StateVectorQuadraticC sv(stateVec);
    RavlAssert(sv.IsValid());

    RealT xc = GetZ()[0], yc = GetZ()[1];
    VectorC F(1);

    // evaluate F(x) = F(a,b,c,y) = a*x*x + b*x + c - y
    F[0] = sv.GetA()*xc*xc + sv.GetB()*xc + sv.GetC() - yc;
#if 0
    cout << "x=" << xc << " a=" << sv.GetA() << " b=" << sv.GetB() << " c=" << sv.GetC();
    cout << " F=" << F[0] << endl;
#endif
    return F;
  }

  //: Evaluate the Jacobian dF/dz given x and z for a single point
  MatrixC ObservationImpQuadraticPointBodyC::EvaluateJacobianFz(const StateVectorC &stateVec)
  {
    // we know that the state vector actually represents a 2D homography
    const StateVectorQuadraticC sv(stateVec);
    RavlAssert(sv.IsValid());

    RealT xc = GetZ()[0];
    MatrixC Fz(1,2);

    Fz[0][0] = 2.0*sv.GetA()*xc + sv.GetB();
    Fz[0][1] = -1.0;
    return Fz;
  }

  //: Evaluate the Jacobian dF/dx given x and z  for a single point
  MatrixC ObservationImpQuadraticPointBodyC::EvaluateJacobianFx(const StateVectorC &stateVec)
  {
    RealT xc = GetZ()[0];
    MatrixC Fx(1,3);

    // fill Jacobian dF/dx with the matrix (dh/da dh/db dh/dc) = (x*x x 1)
    Fx[0][0] = xc*xc;
    Fx[0][1] = xc;
    Fx[0][2] = 1.0;
#if 0
    // we know that the state vector actually represents a 2D homography
    const StateVectorQuadraticC sv(stateVec);
    RavlAssert(sv.IsValid());
    
    cout << "x=" << xc << " a=" << sv.GetA() << " b=" << sv.GetB() << " c=" << sv.GetC();
    cout << " H=" << Fx[0][0] << " " << Fx[0][1] << " " << Fx[0][2] << endl;
#endif
    return Fx;
  }
}
