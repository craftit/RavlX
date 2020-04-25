// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationExplicit.cc"

#include "Ravl/ObservationExplicit.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/MatrixRUT.hh"

namespace RavlN {

  //: Default constructor
  ObservationExplicitBodyC::ObservationExplicitBodyC()
  {}

  //: Constructor
  ObservationExplicitBodyC::ObservationExplicitBodyC(const ObsVectorC &nobsVec)
    : ObservationBodyC(nobsVec)
  {
  }

  //: Compute the residual (negative log-likelihood) of the observation
  RealT ObservationExplicitBodyC::Residual(const StateVectorC &stateVec) {
    // evaluate observation
    VectorC h = EvaluateFunctionH(stateVec);

    // compute innovation vector v=z-h(x)
    VectorC v = GetZ() - h;

    // compute and return v^T*N^-1*v adjusted for robust distribution, if any
    return obsVec.Residual(v, obsVec.GetNi());
  }

  //: Compute the non-robust residual (negative log-likelihood)
  RealT ObservationExplicitBodyC::NonRobustResidual(const StateVectorC &stateVec) {
    // evaluate observation
    VectorC h = EvaluateFunctionH(stateVec);

    // compute innovation vector v=z-h(x)
    VectorC v = GetZ() - h;

    // compute N^-1*v
    VectorC Niv = GetNi()*v;

    // compute and return v^T*N^-1*v
    return Niv.Dot(v);
  }

  //: Increment the linear system
  bool ObservationExplicitBodyC::IncrementLS(const StateVectorC &stateVec,
					     MatrixRSC &A,
					     VectorC &a) {
    // evaluate observation and Jacobian
    VectorC h = EvaluateFunctionH(stateVec);
    MatrixC Hx = EvaluateJacobianHx(stateVec);

    // compute Hx^T*N^-1*Hx
    MatrixC NiHx = GetNi()*Hx;
    MatrixC HxTNiHx = Hx.TMul(NiHx);
    MatrixRSC Aterm(HxTNiHx);

    if ( a.IsValid() ) {

      // compute innovation vector v=z-h(x)
      VectorC v = GetZ() - h;

      // compute Hx^T*N^-1*v
      VectorC HxTNiv = NiHx.T() * v;//NiHx.TMul(v);

      // adjust information matrix/vector for any robustness
      obsVec.AdjustInformation ( Aterm, HxTNiv );

      // increment information matrix and vector sums
      A += Aterm;
      a += HxTNiv;
    }
    else {

      // adjust information matrix for any robustness
      obsVec.AdjustInformation ( Aterm, a );

      // increment information matrix and vector sums
      A += Aterm;
    }

    return true;
  }

  //: Evaluate the observation function h(x) given x.
  VectorC ObservationExplicitBodyC::EvaluateFunctionH(const StateVectorC &stateVec)
  {
    RavlAssertMsg(0,"ObservationExplicitBodyC::EvaluateFunctionH(const StateVectorC &stateVec), Abstract method called ");
    return VectorC();
  }
  
  //: Evaluate the the Jacobian dh/dx given x.
  //: Uses numerical differentiation unless a specific symbolic implentation
  //: is supplied in a subclass
  MatrixC ObservationExplicitBodyC::EvaluateJacobianHx(const StateVectorC &stateVec)
  {
    // we want to manipulate the values of the state vector parameters locally,
    // restoring them at the end, and without making a copy of the whole state
    // vector object first. So let's cast our const reference to a non-const
    // reference.
    StateVectorC &sv = const_cast<StateVectorC &> (stateVec);
    VectorC xcopy = sv.GetX().Copy(); // copy of state vector
    VectorC xstep = sv.GetXStep(); // step sizes for differentiation
    MatrixC H = MatrixC(GetZ().Size(), xcopy.Size());

    // fill in Jacobian, one column at a time
    for ( IntT i=sv.GetX().Size()-1; i >= 0; i-- ) {
      // decrement element i of state vector
      sv.DecElX(i, xstep[i]);

      // evaluate h(x)
      VectorC hp1 = EvaluateFunctionH(sv);

      // increment element i of state vector
      sv.IncElX(i, 2.0*xstep[i]);

      // evaluate h(x) again
      VectorC hp2 = EvaluateFunctionH(sv);

      // set element back to original value
      sv.DecElX(i, xstep[i]);

      // perform subtraction and division to obtain column of Jacobian
      hp2 -= hp1;
      hp2 /= 2.0*xstep[i];

      // insert column
      for ( IntT j=hp2.Size()-1; j >= 0; j-- )
	H[j][i] = hp2[j];
    }
    
    // restore state vector to original values, so it was const really!
    sv.SetX(xcopy);

    return H;
  }
  
}
