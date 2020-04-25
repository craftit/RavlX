// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTORQUADRATIC_HEADER
#define RAVLMATH_STATEVECTORQUADRATIC_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorQuadratic.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/Matrix3d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class representing parameters a,b,c of quadratic curve
  class StateVectorQuadraticBodyC
    : public StateVectorBodyC
  {
  public:
    StateVectorQuadraticBodyC()
    {}
    //: Default constructor.

    StateVectorQuadraticBodyC(RealT a, RealT b, RealT c);
    //: Constructor.
    
    StateVectorQuadraticBodyC(RealT a, RealT b, RealT c,
			      const VectorC &xstep);
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorQuadraticBodyC(GetA(),GetB(),GetC(),
					    xstep.Copy()); }
    //: Make a copy of the body.
    
    RealT GetA() const;
    //: Get parameter a of quadratic curve y = a*x^2 + b*x + c

    RealT GetB() const;
    //: Get parameter b of quadratic curve y = a*x^2 + b*x + c

    RealT GetC() const;
    //: Get parameter c of quadratic curve y = a*x^2 + b*x + c

  private:
    static VectorC BuildX(RealT a, RealT b, RealT c);
    //: static method to build state vector from a,b,c parameters of quadratic
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector class representing parameters a,b,c of quadratic curve
  // Use this class when you are trying to compute a quadratic curve
  // <blockquote>
  //    yc = a*xc^2 + b*xc + c
  // </blockquote>
  // Methods are provided to build the state vector from the individual
  // parameters a,b,c, as well as reading them back through methods GetA(),
  // GetB() and GetC(). Data points to use to fit the model can be created
  // as instances of the ObservationQuadraticC class.
  class StateVectorQuadraticC
    : public StateVectorC
  {
  public:    
    StateVectorQuadraticC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorQuadraticC(RealT a, RealT b, RealT c)
      : StateVectorC(*new StateVectorQuadraticBodyC(a,b,c))
    {}
    //: Constructor

    StateVectorQuadraticC(RealT a, RealT b, RealT c, VectorC &xstep)
      : StateVectorC(*new StateVectorQuadraticBodyC(a,b,c,xstep))
    {}
    //: Constructor

    StateVectorQuadraticC(const StateVectorC &stateVec)
      : StateVectorC(dynamic_cast<const StateVectorQuadraticBodyC *>(BodyPtr(stateVec)))
    {}
    //: Base class constructor.
    
  public:
    StateVectorQuadraticC(StateVectorQuadraticBodyC &bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorQuadraticBodyC &Body()
    { return static_cast<StateVectorQuadraticBodyC &>(StateVectorC::Body()); }
    //: Access body.

    const StateVectorQuadraticBodyC &Body() const
    { return static_cast<const StateVectorQuadraticBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
  public:
    RealT GetA() const
    { return Body().GetA(); }
    //: Get parameter a of quadratic curve y = a*x^2 + b*x + c

    RealT GetB() const
    { return Body().GetB(); }
    //: Get parameter b of quadratic curve y = a*x^2 + b*x + c

    RealT GetC() const
    { return Body().GetC(); }
    //: Get parameter c of quadratic curve y = a*x^2 + b*x + c
  };
}


#endif
