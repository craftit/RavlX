// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTORAFFINE2D_HEADER
#define RAVLMATH_STATEVECTORAFFINE2D_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="1/10/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorAffine2d.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/Affine2d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class representing parameters of 2D line
  
  class StateVectorAffine2dBodyC
    : public StateVectorBodyC
  {
  public:
    StateVectorAffine2dBodyC()
    {}
    //: Default constructor.
    
    StateVectorAffine2dBodyC(const Affine2dC &nt);
    //: Constructor from an affine transform.
    
    StateVectorAffine2dBodyC(const VectorC &sv);
    //: Construct from a state vector.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorAffine2dBodyC(t); }
    //: Make a copy of the body.
    
    virtual bool Postprocess();
    //: Postprocess state vector after an adjustment to the parameters
    
    const Affine2dC &GetAffine() const
    { return t; }
    //: Get current affine transform.
  protected:
    VectorC StateVecFromAffine2d(const Affine2dC &aff);
    //: Generate the state vector from the affine parameters.
    
  private:
    Affine2dC t; // Affine transform.
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector class representing parameters of 2D affine transform
  // <p>
  // For details on how to use this class for robust and non-robust
  // least-squares problems see the ObservationAffine2dPointC and
  // LevenbergMarquardtC classes.
  
  class StateVectorAffine2dC
    : public StateVectorC
  {
  public:    
    StateVectorAffine2dC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorAffine2dC(const Affine2dC &nt)
      : StateVectorC(*new StateVectorAffine2dBodyC(nt))
    {}
    //: Constructor
    
    StateVectorAffine2dC(const VectorC &sv)
      : StateVectorC(*new StateVectorAffine2dBodyC(sv))
    {}
    //: Construct from a state vector.
    
    StateVectorAffine2dC(const StateVectorC &sv)
      : StateVectorC(dynamic_cast<const StateVectorAffine2dBodyC *>(BodyPtr(sv)))
    {}
    //: Base constructor.
    
  protected:
    StateVectorAffine2dC(StateVectorAffine2dBodyC &bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorAffine2dBodyC &Body()
    { return static_cast<StateVectorAffine2dBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
    const StateVectorAffine2dBodyC &Body() const
    { return static_cast<const StateVectorAffine2dBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
  public:
    const Affine2dC &GetAffine() const
    { return Body().GetAffine(); }
    //: Get current affine transform.
  };
}


#endif
