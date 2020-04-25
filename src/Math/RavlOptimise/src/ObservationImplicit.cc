// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImplicit.cc"

#include "Ravl/ObservationImplicit.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/MatrixRUT.hh"

#define DEFAULT_STEP_SIZE 1.0e-6

namespace RavlN {

  //: Default constructor
  ObservationImplicitBodyC::ObservationImplicitBodyC()
    : ObservationBodyC()
  {
  }

  //: Constructor
  ObservationImplicitBodyC::ObservationImplicitBodyC(const ObsVectorC &nobsVec, UIntT nFsize)
    : ObservationBodyC(nobsVec)
  {
    // compute covariance matrix from inverse
    N = nobsVec.GetNi().Inverse();

    // store size of F vector
    Fsize = nFsize;
  }

  //: Compute the residual (negative log-likelihood) of the observation
  RealT ObservationImplicitBodyC::Residual(const StateVectorC &stateVec) {
    // evaluate observation F(x,z) identified with the negation of innovation v
    // and the Jacobian dF/dz
    VectorC F = EvaluateFunctionF(stateVec);
    RavlAssertMsg(F.Size()==Fsize,"ObservationImplicitBodyC::Residual(const StateVectorC &stateVec), Inconsistent size of F ");
    MatrixC Fz = EvaluateJacobianFz(stateVec);

    // compute Fz*N*Fz^T
    MatrixC NFzT = GetN()*Fz.T();
    MatrixC FzNFzT = Fz*NFzT;
    MatrixRSC FNFT(FzNFzT);

    // invert observation covariance
    MatrixRSC Nip = FNFT.Inverse();

    // compute and return F^T*N^-1*F adjusted for robust distribution, if any
    return obsVec.Residual(F, Nip);
  }

  //: Compute the non-robust residual (negative log-likelihood)
  RealT ObservationImplicitBodyC::NonRobustResidual(const StateVectorC &stateVec) {
    // evaluate observation F(x,z) identified with the negation of innovation v
    // and the Jacobian dF/dz
    VectorC F = EvaluateFunctionF(stateVec);
    RavlAssertMsg(F.Size()==Fsize,"ObservationImplicitBodyC::NonRobustResidual(const StateVectorC &stateVec), Inconsistent size of F ");
    MatrixC Fz = EvaluateJacobianFz(stateVec);

    // compute Fz*N*Fz^T
    MatrixC NFzT = GetN()*Fz.T();
    MatrixC FzNFzT = Fz*NFzT;
    MatrixRSC FNFT(FzNFzT);

    // invert observation covariance
    MatrixRSC Nip = FNFT.Inverse();

    // compute Np^-1*F
    VectorC NipF = Nip*F;

    // compute and return F^T*Np^-1*F
    return NipF.Dot(F);
  }

  //: Increment the linear system
  bool ObservationImplicitBodyC::IncrementLS(const StateVectorC &stateVec,
					     MatrixRSC &A,
					     VectorC &a) {

    // evaluate observation F(x,z) identified with the negation of innovation v
    // and the Jacobians Fz=dF/dz and Fx=dF/dx
    VectorC F = EvaluateFunctionF(stateVec);
    RavlAssertMsg(F.Size()==Fsize,"ObservationImplicitBodyC::IncrementLS(), Inconsistent size of F ");
    MatrixC Fz = EvaluateJacobianFz(stateVec);
    MatrixC Fx = EvaluateJacobianFx(stateVec);

    // compute Fz*N*Fz^T
    MatrixC NFzT = GetN()*Fz.T();
    MatrixC FzNFzT = Fz*NFzT;
    MatrixRSC FNFT(FzNFzT);

    // invert observation covariance
    MatrixRSC Nip = FNFT.Inverse();

    // compute Fx^T*N^-1*Fx
    MatrixC NiFx = Nip*Fx;
    MatrixC FxTNiFx = Fx.TMul(NiFx);
    MatrixRSC Aterm(FxTNiFx);

    if ( a.IsValid() ) {

      // compute Fx^T*N^-1*F
      VectorC FxTNiF = NiFx.T() * F;//NiFx.TMul(v);

      // adjust information matrix/vector for any robustness
      obsVec.AdjustInformation ( Aterm, FxTNiF );

      // increment information matrix and vector sums
      A += Aterm;
      a -= FxTNiF;
    }
    else {

      // adjust information matrix for any robustness
      obsVec.AdjustInformation ( Aterm, a );

      // increment information matrix and vector sums
      A += Aterm;
    }

    return true;
  }

  //: Evaluate the observation function F(x,z) given x and z
  VectorC ObservationImplicitBodyC::EvaluateFunctionF(const StateVectorC &stateVec)
  {
    RavlAssertMsg(0,"ObservationImplicitBodyC::EvaluateFunctionF(const StateVectorC &stateVec), Abstract method called ");
    return VectorC();
  }

  //: Evaluate the Jacobian dF/dz given x and z
  MatrixC ObservationImplicitBodyC::EvaluateJacobianFz(const StateVectorC &stateVec)
  {
    // we want to manipulate the values of the observation vector parameters
    // locally, restoring them at the end. So let's cast our const reference
    // to a non-const reference.
    VectorC &z = const_cast<VectorC &> (GetZ());
    VectorC zcopy = z.Copy(); // so that we can restore the observation vector
    VectorC zstep = obsVec.GetZStep(); // step sizes for differentiation
    VectorC F = EvaluateFunctionF(stateVec); // evaluate to get size
    MatrixC Fz = MatrixC(F.Size(), GetZ().Size());

    // fill in Jacobian, one column at a time
    for ( IntT i=z.Size()-1; i >= 0; i-- ) {
      // decrement element i of observation vector
      z[i] -= zstep[i];

      // evaluate h(x)
      VectorC Fp1 = EvaluateFunctionF(stateVec);

      // increment element i of state vector
      z[i] += 2.0*zstep[i];

      // evaluate h(x)
      VectorC Fp2 = EvaluateFunctionF(stateVec);

      // set element back to original value
      z[i] -= zstep[i];

      // perform subtraction and division to obtain column of Jacobian
      Fp2 -= Fp1;
      Fp2 /= 2.0*zstep[i];

      // insert column
      for ( IntT j=F.Size()-1; j >= 0; j-- )
	Fz[j][i] = Fp2[j];
    }

    // restore observation vector to original values, so it was const really!
    z = zcopy;

    return Fz;
  }

  //: Evaluate the Jacobian dF/dx given x and z
  MatrixC ObservationImplicitBodyC::EvaluateJacobianFx(const StateVectorC &stateVec)
  {
    // we want to manipulate the values of the state vector parameters locally,
    // restoring them at the end, and without making a copy of the whole state
    // vector object first. So let's cast our const reference to a non-const
    // reference.
    StateVectorC &sv = const_cast<StateVectorC &> (stateVec);
    VectorC xcopy = sv.GetX().Copy(); // copy of state vector
    VectorC xstep = sv.GetXStep(); // step sizes for differentiation
    VectorC F = EvaluateFunctionF(stateVec); // evaluate to get size
    MatrixC Fx = MatrixC(F.Size(), xcopy.Size());

    // fill in Jacobian, one column at a time
    for ( IntT i=sv.GetX().Size()-1; i >= 0; i-- ) {
      // decrement element i of state vector
      sv.DecElX(i, xstep[i]);

      // evaluate h(x)
      VectorC Fp1 = EvaluateFunctionF(sv);

      // increment element i of state vector
      sv.IncElX(i, 2.0*xstep[i]);

      // evaluate h(x)
      VectorC Fp2 = EvaluateFunctionF(sv);

      // set element back to original value
      sv.DecElX(i, xstep[i]);

      // perform subtraction and division to obtain column of Jacobian
      Fp2 -= Fp1;
      Fp2 /= 2.0*xstep[i];

      // insert column
      for ( IntT j=Fp2.Size()-1; j >= 0; j-- )
	Fx[j][i] = Fp2[j];
    }

    // restore state vector to original values, so it was const really!
    sv.SetX(xcopy);

    return Fx;
  }

  //: Get implicit observation covariance
  const MatrixRSC& ObservationImplicitBodyC::GetN() const
  {
    return N;
  }

  //: Set implicit observation covariance
  bool ObservationImplicitBodyC::SetN(MatrixRSC &Nnew)
  {
    N = Nnew;
    return true;
  }

}
