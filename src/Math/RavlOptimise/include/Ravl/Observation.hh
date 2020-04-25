// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATION_HEADER
#define RAVLMATH_OBSERVATION_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Observation.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector.hh"
#include "Ravl/MatrixRS.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/ObsVector.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Observation body class
  class ObservationBodyC
    : public RCBodyVC
  {
  public:
    ObservationBodyC()
    {}
    //: Default constructor.
    
    ObservationBodyC(const ObsVectorC &nobsVec);
    //: Constructor.
    
    virtual RealT Residual(const StateVectorC &stateVec);
    //: Compute the residual (negative log-likelihood) of the observation
    
    virtual RealT NonRobustResidual(const StateVectorC &stateVec);
    //: Compute the residual (negative log-likelihood) of the observation
    
    virtual bool IncrementLS(const StateVectorC &stateVec,
			     MatrixRSC &A,
			     VectorC &a);
    //: Increment the linear system

    virtual UIntT GetNumConstraints() const;
    //: Returns the number of constraints imposed on the state

    const VectorC& GetZ() const
    { return obsVec.GetZ(); }
    //: Get observation vector z

    inline const MatrixRSC& GetNi() const
    { return obsVec.GetNi(); }
    //: Get observation inverse covariance N^-1
    
    inline const ObsVectorC& GetObsVec() const
    { return obsVec; }
    //: Get observation vector/inverse covariance object
    
    inline bool GetSelected() const  
    { return selected; }
    //: Return observation "selected" flag
    
    inline void SetSelected()
    { selected = true; }
    //: Set the "selected" flag to true
    
    inline void SetUnSelected()
    { selected = false; }
    //: Set the "selected" flag to false
    
    inline UIntT ID() const
    { return id; }
    //: Access observation ID.
    
    inline void SetID(UIntT nid)
    { id = nid; }
    //: Set observation ID.
    
  protected:
    ObsVectorC obsVec; //: Observation vector

  private:
    bool selected;
    UIntT id; // Observation ID, not used internally.
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation class
  // This is the base class for observations on a state, for the purpose of
  // least-squares optimisation. It contains an observation vector z and its
  // associated inverse covariance matrix N^-1 encapsulated as an ObsVectorC
  // object, plus virtual methods implemented in subclasses that detail how
  // the observation vector relates to the state parameters x being computed,
  // either through an explicit observation equation
  // <blockquote>
  //     z = h(x) + w
  // </blockquote>
  // (w is a noise vector with covariance N) or an implicit equation
  // <blockquote>
  //     F(x,z-w) = 0
  // </blockquote>
  // ObservationC currently provides methods to support the
  // Levenberg-Marquardt algorithm, but it is envisaged that more methods
  // may be added to support other algorithms. Because there are two types
  // of observation, explicit and implicit, the specific formulae for
  // implementing the Levenberg-Marquardt updates are placed in corresponding
  // sub-classes, ObservationExplicitC and ObservationImplicitC.
  // ObservationExplicitC and ObservationImplicitC are themselves derived
  // into subclasses that directly evaluate h(x) or F(x,z) and (optionally)
  // their associated Jacobians for a specific type of observation.
  // <p>
  // The class hierarchies represented by the base classes ObsVectorC and
  // StateVectorC encapsulate respectively the observation vector/covariance
  // z,N and the state vector x. The principle is that an ObsVector (z/N)
  // sub-class object (for instance an image point) could be used as an
  // observation on a wide variety of optimisation applications, so z/N
  // "deserve" a class hierarchy in their own right. Also a given state
  // vector sub-class (for instance parameters representing a 3D rotation)
  // can be used for a variety of optimisation tasks, and even within one
  // task a number of different observation types on the state might be
  // envisaged. Finally the ObservationC class hierarchy combines the
  // observation vector and state vector together with specific observation
  // equations (explicit or implicit) relating one to the other.
  class ObservationC
    : public RCHandleC<ObservationBodyC>
  {
  public:    
    ObservationC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationC(const ObsVectorC &nobsVec)
      : RCHandleC<ObservationBodyC>(*new ObservationBodyC(nobsVec))
    {}
    //: Constructor
    
  public:
    ObservationC(ObservationBodyC &bod)
      : RCHandleC<ObservationBodyC>(bod)
    {}
    //: Body constructor.
    
    explicit ObservationC(const ObservationBodyC *bod)
      : RCHandleC<ObservationBodyC>(bod)
    {}
    //: Body constructor.
    
    ObservationBodyC &Body()
    { return RCHandleC<ObservationBodyC>::Body(); }
    //: Access body.

    const ObservationBodyC &Body() const
    { return RCHandleC<ObservationBodyC>::Body(); }
    //: Access body.
    
  public:
    RealT Residual(const StateVectorC &stateVec)
    { return Body().Residual(stateVec); }
    //: Compute the residual (negative log-likelihood) of the observation
    // This is the term in the
    // <a href="../../../LevenbergMarquardt/levmarq.html#chi2-def">error function</a>
    // for a single observation. This is a virtual method, and
    // has specific implementations for explicit and implicit observations in
    // the ObservationExplicitC and ObservationImplicitC subclasses.
    // <p>
    // Incorporates any robust aspects of the observation into the calculation.

    RealT NonRobustResidual(const StateVectorC &stateVec)
    { return Body().NonRobustResidual(stateVec); }
    //: Compute the residual (negative log-likelihood) of the observation
    // Treats the observation as a non-robust observation whether it is robust
    // or not. Use this function when using RANSAC (but use Residual() for
    // MLESAC). This is a virtual method, and has specific implementations for
    // explicit and implicit observations in the ObservationExplicitC and
    // ObservationImplicitC subclasses.

    bool IncrementLS(const StateVectorC &stateVec,
		     MatrixRSC &A,
		     VectorC &a)
    { return Body().IncrementLS(stateVec,A,a); }
    //: Increment the linear system
    // This adds the relevant terms to the inverse covariance matrix A and
    // vector a accumulated within a Levenberg-Marquardt iteration for a
    // single observation, as given by the
    // <a href="../../../LevenbergMarquardt/levmarq.html#LM_update">state update equation</a>
    // for the explicit form of observations. This is a virtual method, and
    // has specific implementations for explicit and implicit observations in
    // the ObservationExplicitC and ObservationImplicitC subclasses.

    UIntT GetNumConstraints() const
    { return Body().GetNumConstraints(); }
    //: Returns the number of constraints imposed on the state

    const VectorC &GetZ() const
    { return Body().GetZ(); }
    //: Get observation vector z

    const MatrixRSC &GetNi() const
    { return Body().GetNi(); }
    //: Get observation inverse covariance N^-1

    const ObsVectorC& GetObsVec() const
    { return Body().GetObsVec(); }
    //: Get observation vector/inverse covariance object

    bool GetSelected() const
    { return Body().GetSelected(); }
    //: Return observation "selected" flag
    // This flag can be used by RANSAC and other algorithms for flagging
    // observations which should be ignored for the purposes of a specific
    // observation.

    void SetSelected()
    { Body().SetSelected(); }
    //: Set the "selected" flag to true
    // This flag can be used by RANSAC and other algorithms for flagging
    // observations which should be ignored for the purposes of a specific
    // observation.

    void SetUnSelected()
    { Body().SetUnSelected(); }
    //: Set the "selected" flag to false
    // This flag can be used by RANSAC and other algorithms for flagging
    // observations which should be ignored for the purposes of a specific
    // observation.

    inline UIntT ID() const
    { return Body().ID(); }
    //: Access observation ID.
    // ID for observation, not used internally.
    
    inline void SetID(UIntT nid)
    { Body().SetID(nid); }
    //: Set observation ID.
    // ID for observation, not used internally.
    
  };
}


#endif
