// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_LEVENBERGMARQUARDT_HEADER
#define RAVLMATH_LEVENBERGMARQUARDT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! example="OrthogonalRegressionTest.cc;QuadraticFitTest.cc;Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/LevenbergMarquardt.hh"

#include "Ravl/StoredState.hh"
#include "Ravl/Observation.hh"
#include "Ravl/DList.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //! autoLink=on
  //: Levenberg-Marquardt algorithm class
  // This is a generic implementation of Levenberg-Marquardt least-squares
  // minimisation. The algorithm is described mathematically in a
  // <a href="../../../html/levmarq.pdf">separate document</a>.
  // <p>
  // This class contains a constructor for initialising the algorithm, a method
  // for a applying a damped Levenberg-Marquardt iteration, which should be
  // successively applied until convergence, and methods for returning the
  // state and covariance results.
  // <p>
  // Initialisation of the state parameters needs to be implemented separately
  // in the user program. This class only deals with optimisation of the
  // parameters after reasonably good values are available.
  class LevenbergMarquardtC
    : public StoredStateC
  {
  public:
    LevenbergMarquardtC()
    {}
    //: Default constructor.
    
    LevenbergMarquardtC(const StateVectorC &stateVecInit,
			const DListC<ObservationC>& obsList);
    //: Constructor.
    // Initialises the Levenberg-Marquardt algorithm with the given state
    // parameter vector stateVecInit. The observation list obsList is used
    // to initialise the least-squares residual. stateVecInit should contain
    // a good enough estimate of the optimal state vector parameters for
    // direct minimisation to achieve the global minimum. Levenberg-Marquardt
    // always goes downhill!

    bool Iteration ( const DListC<ObservationC>& obsList, RealT lambda=0.1,
		     bool rescaleDiagonal=false );
    //: Apply a single iteration
    // Process observations in the provided list (obsList) to build the
    // inverse covariance matrix A and vector a in the
    // <a href="../../../LevenbergMarquardt/levmarq.html#LM_update">state update equation</a>,
    // apply damping by adding lambda*identity to A, and update the state
    // vector x.
    // <p>
    // Returns true if the iteration succeeded in reducing the residual,
    // false otherwise. In the latter case, the state vector is not adjusted,
    // and the user program should reduce the value of lambda before calling
    // this method again.
    // <p>
    // If rescaleDiagonal is passed as false, the damping factor is added
    // to the diagonal elements of the inverse covariance matrix in order to
    // damp it. This is the algorithm as given by Marquardt. If it is passed as
    // true, the diagonal elements are instead multiplied by 1+lambda, as
    // given by Numerical Recipes. The former should be preferred if the
    // parameters are scaled reasonable homogeneously, and is the default
    // if the parameter is excluded.

    bool NIterations ( const DListC<ObservationC>& obsList, UIntT n=10,
		       RealT lambdaStart=0.1, RealT lambdaFactor=0.1,
		       bool rescaleDiagonal=false );
    //: Apply a fixed number of iterations
    // Apply n Levenberg-Marquardt optimisation iterations. At each iteration,
    // if the updated state gives a smaller residual, the damping factor lambda
    // is multiplied by lambdaFactor, otherwise the original state vector is
    // reinstated and lambda is divided by lambdaFactor.
    // <p>
    // If rescaleDiagonal is passed as false, the damping factor is added
    // to the diagonal elements of the inverse covariance matrix in order to
    // damp it. This is the algorithm as given by Marquardt. If it is passed as
    // true, the diagonal elements are instead multiplied by 1+lambda, as
    // given by Numerical Recipes. The former should be preferred if the
    // parameters are scaled reasonable homogeneously, and is the default
    // if the parameter is excluded.

    inline const VectorC &SolutionVector() const
    { return stateVec.GetX(); }
    // Latest estimate of solution parameters

    const MatrixRSC &InverseCovarianceMatrix(const DListC<ObservationC>& obsList);
    //: Inverse covariance matrix of solution
    // This is the matrix A in the <a href="../../../LevenbergMarquardt/levmarq.html#LM_update">state update equation</a>,
    // but computed without any damping, i.e. lambda set to zero.
    // The observation list is reprocessed to produce the matrix.
    // This will normally be called once, after convergence has been achieved.
    // <p>
    // If all gauge freedoms have been dealt with using gauge condition
    // observations (see e.g. the class StateVectorLine2dC), then this matrix
    // should be invertible should the covariance matrix A^-1 be required.
    // However there may be data-dependent conditioning issues, and it is
    // safer to leave the inversion for the user program.

    inline RealT GetResidual() const
    { return residual; }
    //: Get stored Chi-squared residual for latest state estimate
    // This is the
    // <a href="../../../LevenbergMarquardt/levmarq.html#chi2-def">error function</a>
    // evaluated for the latest state estimate x.

  private:
    MatrixRSC A; // Inverse covariance matrix
    bool Aupdated; // Whether A is up to date
    MatrixRSC Ainv; // Covariance matrix
    bool AinvUpdated; // Whether Ainv is up to date
    RealT residual; // current minimum value of Chi-squared residual

    RealT ComputeResidual(const DListC<ObservationC>& obsList) const;
    //: Compute least-squares residual
  };
}


#endif
