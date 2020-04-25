// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTORROT3D_HEADER
#define RAVLMATH_STATEVECTORROT3D_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorRot3d.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/Matrix3d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class representing 3D rotation
  class StateVectorRot3dBodyC
    : public StateVectorBodyC
  {
  public:
    StateVectorRot3dBodyC()
    {}
    //: Default constructor.

    StateVectorRot3dBodyC(const VectorC &x);
    //: Constructor.
    
    StateVectorRot3dBodyC(const VectorC &x, const VectorC &xstep);
    //: Constructor.

    StateVectorRot3dBodyC(const VectorC &x, const VectorC &xstep, const Matrix3dC &Pnew);
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorRot3dBodyC(x.Copy(),xstep.Copy(),R); }
    //: Make a copy of the body.
    
    virtual const Matrix3dC GetRotMatrix() const;
    //: Get 3D rotation matrix represented by state vector

  protected:
    Matrix3dC R; // 3D rotation matrix
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector handle class representing 3D rotation
  // Straightforward representations of 3D rotation for optimisation all have
  // drawbacks. Angle representations (e.g. Euler angles) all have unpleasant
  // singularities and involve complex and computationally expensive
  // trigonometric formulae, especially when derivatives are involved.
  // Quaternion and angle/axis representations are redundant, i.e. they
  // involve four parameters to represent a 3-dimensional state space.
  // While introducing such gauge freedoms into a state representation is
  // not necessarily bad (see the related class StateVectorHomog2DC),
  // in this case there is an alternative which avoids all these problems,
  // while using an efficient minimal state space representation.
  // <p>
  // We represent a rotation using local rotation parameters. Thus a
  // rotation is R = Rs*R0 where R0 is a reference rotation and Rs is a small
  // adjustment. For the purposes of optimisation, R0 is always set to the
  // latest estimate of R, and Rs set to zero rotation, just before an
  // iterative adjustment. R0 is then treated as constant for the purpose of
  // iterative adjustment, and only Rs is computed. R0 is then reset to Rs*R0,
  // Rs reset to zero, and the process continues. In this way we can use a
  // minimal representation of the estimated Rs, safe in the knowlege that the
  // singularities of such representations will be avoided. We actually use
  // the exponential or angle*axis representation for Rs. A quaternion is
  // used to represent R0, which has the important feature that a product
  // of quaternions only needs to be rescaled to become another valid
  // quaternion. If matrices were used there would be the potential for
  // drift away from orthogonality. Another reason for using quaternions for
  // the reference rotation is that quaternion products are faster to compute
  // than matrix products.
  // <p>
  // Some extra bookkeeping is associated with this representation.
  // The Postprocess() virtual method is overridden to update R0 when the
  // rotation state vector (represented by Rs) changes. The Reset() virtual
  // method is also overridden, and is used to backtrack to the previous
  // R0 before the last adjustment, in the case that the adjustment is
  // rejected by the least-squares algorithm.
  class StateVectorRot3dC
    : public StateVectorC
  {
  public:    
    StateVectorRot3dC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorRot3dC(const VectorC &x)
      : StateVectorC(*new StateVectorRot3dBodyC(x))
    {}
    //: Constructor

    StateVectorRot3dC(const VectorC &x, const VectorC &xstep)
      : StateVectorC(*new StateVectorRot3dBodyC(x,xstep))
    {}
    //: Constructor
    
    StateVectorRot3dC(const StateVectorC &stateVec)
      : StateVectorC(dynamic_cast<const StateVectorRot3dBodyC *>(BodyPtr(stateVec)))
    {}
    //: Base class constructor.
    
  public:
    StateVectorRot3dC(StateVectorRot3dBodyC &bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorRot3dBodyC &Body()
    { return static_cast<StateVectorRot3dBodyC &>(StateVectorC::Body()); }
    //: Access body.

    const StateVectorRot3dBodyC &Body() const
    { return static_cast<const StateVectorRot3dBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
  public:
    const Matrix3dC GetRotMatrix() const
    { return Body().GetRotMatrix(); }
    //: Get 3D rotation matrix represented by state vector
  };
}


#endif
