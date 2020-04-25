// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/LevenbergMarquardt.cc"

#include "Ravl/LevenbergMarquardt.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/DList.hh" // Need this for some reason to get the linker to work

namespace RavlN {

  //: Constructor for Levenberg-Marquardt.
  LevenbergMarquardtC::LevenbergMarquardtC(const StateVectorC &stateVecInit,
					   const DListC<ObservationC>& obsList)
    : StoredStateC(stateVecInit)
  {
    A = MatrixRSC(stateVec.GetX().Size());
    Ainv = MatrixRSC(stateVec.GetX().Size());
    residual = ComputeResidual(obsList);
    Aupdated = AinvUpdated = false;
  }

  // compute residual given existing state vector
  RealT LevenbergMarquardtC::ComputeResidual(const DListC<ObservationC>& obsList) const
  {
    RealT residual = 0.0;

    // go through observations one by one, adding up residual
    for(DLIterC<ObservationC> it(obsList);it;it++) {
      ObservationC obs = it.Data();

      // accumulate residual
      residual += obs.Residual(stateVec);
    }

    return residual;
  }

  // Apply an iteration, processing observations and updating the state
  // Returns true if the iteration succeeded in reducing the residual,
  // false otherwise.
  bool LevenbergMarquardtC::Iteration ( const DListC<ObservationC>& obsList,
					RealT lambda, bool rescaleDiagonal )
  {
    VectorC a = stateVec.GetX().Copy();

    // force recalculation of (inverse) covariance matrix
    Aupdated = AinvUpdated = false;

    // initialise information to zero
    A.Fill(0.0);
    a.Fill(0.0);

    // store existing state vector
    VectorC xold = stateVec.GetX().Copy();

    // go through observations one by one
    for(DLIterC<ObservationC> it(obsList);it;it++) {
      ObservationC obs = it.Data();

      // accumulate information sum
      obs.IncrementLS ( stateVec, A, a );
    }

    // add any adjustments specified by the state vector
    stateVec.IncrementLS ( A, a );

    IntT i;
    if ( rescaleDiagonal )
      // scale diagonal of A by 1+lambda
      for ( i = (IntT)A.Rows()-1; i >= 0; i-- )
	A[i][i] = (1.0+lambda)*A[i][i];
    else
      // add lambda*I to A
      for ( i = (IntT)A.Rows()-1; i >= 0; i-- )
	A[i][i] += lambda;

    // solve for increment to state vector
    if(!SolveIP(A,a))
      throw ExceptionC("LevenbergMarquardtC::Iteration(), Singular matrix detected. ");

    // add A^-1*a to state vector x
    stateVec.IncX(a);

    // compute residual after update
    RealT newResidual = ComputeResidual ( obsList );

    // reinstate state vector if the new residual is equal or higher
    if ( newResidual >= residual ) {
      stateVec.SetX(xold);

      // restore state of observations
      for(DLIterC<ObservationC> it(obsList); it; it++) {
	ObservationC obs = it.Data();
	ObsVectorC obsVec = obs.GetObsVec();
	obsVec.Restore();
      }

      return false;
    }
    else {
      // update best residual
      residual = newResidual;
      return true;
    }
  }

  // Apply a fixed number of iterations
  bool LevenbergMarquardtC::NIterations ( const DListC<ObservationC>& obsList,
					  UIntT n,
					  RealT lambda, RealT lambdaFactor,
					  bool rescaleDiagonal )
  {
    for ( UIntT i = 0; i < n; i++ ) {
      bool accepted = Iteration(obsList, lambda, rescaleDiagonal);
      if ( accepted )
	// iteration succeeded in reducing the residual
	lambda *= lambdaFactor;
      else
	// iteration failed to reduce the residual
	lambda /= lambdaFactor;
    }

    return true;
  }

  // Inverse covariance matrix of solution
  const MatrixRSC &LevenbergMarquardtC::InverseCovarianceMatrix(const DListC<ObservationC>& obsList)
  {
    // invalid vector
    VectorC a;

    // initialise information to zero
    A.Fill(0.0);

    // go through observations one by one
    for(DLIterC<ObservationC> it(obsList);it;it++) {
      ObservationC obs = it.Data();

      // accumulate information sum
      obs.IncrementLS ( stateVec, A, a );
    }

    return A;
  }
}
