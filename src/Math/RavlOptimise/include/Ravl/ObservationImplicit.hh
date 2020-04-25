// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONIMPLICIT_HEADER
#define RAVLMATH_OBSERVATIONIMPLICIT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImplicit.hh"

#include "Ravl/Observation.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Implicit observation body.
  
  class ObservationImplicitBodyC
    : public ObservationBodyC
  {
  public:
    ObservationImplicitBodyC();
    //: Default constructor.
    
    ObservationImplicitBodyC(const ObsVectorC &nobsVec, UIntT Fsize);
    //: Constructor

    RealT Residual(const StateVectorC &stateVec);
    //: Compute the residual (negative log-likelihood) of the observation

    RealT NonRobustResidual(const StateVectorC &stateVec);
    //: Compute the non-robust residual (negative log-likelihood)

    bool IncrementLS(const StateVectorC &stateVec,
		     MatrixRSC &A,
		     VectorC &a);
    //: Increment the linear system

    UIntT GetNumConstraints() const
    { return Fsize; }
    //: Returns the number of constraints imposed on the state
    
    virtual VectorC EvaluateFunctionF(const StateVectorC &stateVec);
    //: Evaluate the observation function F(x,z) given x and z

    virtual MatrixC EvaluateJacobianFz(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dz given x and z

    virtual MatrixC EvaluateJacobianFx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dx given x and z

    const MatrixRSC &GetN() const;
    //: Get observation covariance

    bool SetN(MatrixRSC &Nnew);
    //: Set observation covariance

  protected:
    MatrixRSC N; //: Observation covariance
    UIntT Fsize; //: Size of F vector
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Implicit observation class
  // This class represents a single observation on a state vector x, where
  // the observation vector z is related to x through the implicit form of
  // observation equation
  // <blockquote>
  //    F(x,z-w) = 0
  // </blockquote>
  // where the noise vector w has covariance N. To see how such observations
  // can be used in optimisation see the
  // <a href="../../../LevenbergMarquardt/node2.html">mathematical description</a>.
  // This class implements the
  // Residual() and IncrementLS() methods which are virtual in the
  // ObservationC base class, by applying the explicit
  // <a href="../../../LevenbergMarquardt/levmarq.html#chi2-def">error function</a>
  // and
  // <a href="../../../LevenbergMarquardt/levmarq.html#LM_update">state update</a>
  // formulae, substituting the terms computed from the implicit form as
  // described
  // <a href="../../../LevenbergMarquardt/node2.html">here</a>.
  // The specific form of F(.)
  // should be provided by a sub-class, which implements at least the virtual
  // function EvaluateFunctionF(). If formulae for the Jacobians dF/dx and
  // dF/dz F(.) are known then either or both can be implemented in the
  // virtual functions EvaluateJacobianFx() and EvaluateJacobianFz()
  // respectively. If one or other of dF/dx or dF/dz is not provided with
  // an implementation, the relevant Jacobian(s) are computed numerically.
  class ObservationImplicitC
    : public ObservationC
  {
  public:
    ObservationImplicitC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationImplicitC(const ObsVectorC &nobsVec, UIntT Fsize)
      : ObservationC(*new ObservationImplicitBodyC(nobsVec,Fsize))
    {}
    //: Constructor
    
    ObservationImplicitC(const ObservationC &obs)
      : ObservationC(dynamic_cast<const ObservationImplicitBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  public:
    ObservationImplicitC(ObservationImplicitBodyC &bod)
      : ObservationC(bod)
    {}
    //: Body constructor.
    
    ObservationImplicitC(const ObservationImplicitBodyC *bod)
      : ObservationC(bod)
    {}
    //: Body constructor.
    
    ObservationImplicitBodyC &Body()
    { return static_cast<ObservationImplicitBodyC &>(ObservationC::Body()); }
    //: Access body.

    const ObservationImplicitBodyC &Body() const
    { return static_cast<const ObservationImplicitBodyC &>(ObservationC::Body()); }
    //: Access body.
    
  public:

    VectorC EvaluateFunctionF(const StateVectorC &stateVec)
    { return Body().EvaluateFunctionF(stateVec); }
    //: Evaluate the observation function F(x,z) given x and z

    MatrixC EvaluateJacobianFz(const StateVectorC &stateVec)
    { return Body().EvaluateJacobianFz(stateVec); }
    //: Evaluate the Jacobian dF/dz given x and z
    // This may require that you call the EvaluateFunctionF method first,
    // with the same stateVec parameter values, in order to set private
    // values in the class. See the body class documentation of the derived
    // class for more information.

    MatrixC EvaluateJacobianFx(const StateVectorC &stateVec)
    { return Body().EvaluateJacobianFx(stateVec); }
    //: Evaluate the Jacobian dF/dx given x and z
    // This may require that you call the EvaluateFunctionF method first,
    // with the same stateVec parameter values, in order to set private
    // values in the class. See the body class documentation of the derived
    // class for more information.
    
    const MatrixRSC &GetN() const
    { return Body().GetN(); }
    //: Get observation covariance

    bool SetN(MatrixRSC &Nnew)
    { return Body().SetN(Nnew); }
    //: Set observation covariance
  };
}  


#endif
