// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTORLINE2D_HEADER
#define RAVLMATH_STATEVECTORLINE2D_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="OrthogonalRegressionTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorLine2d.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/Matrix3d.hh"

#define DEFAULT_GAUGE_WEIGHT 1.0e3

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class representing parameters of 2D line
  class StateVectorLine2dBodyC
    : public StateVectorBodyC
  {
  public:
    StateVectorLine2dBodyC()
    {}
    //: Default constructor.

    StateVectorLine2dBodyC(RealT lx, RealT ly, RealT lz, RealT zh,
			   RealT gaugeWeight);
    //: Constructor.
    
    StateVectorLine2dBodyC(RealT lx, RealT ly, RealT lz, RealT zh,
			   RealT gaugeWeight, const VectorC &xstep);
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorLine2dBodyC(GetLx(),GetLy(),GetLz(),GetZH(),
					 gaugeWeight,xstep.Copy()); }
    //: Make a copy of the body.
    
    virtual bool IncrementLS(MatrixRSC &A, VectorC &a);
    //: Increment the linear system

    virtual bool Postprocess();
    //: Postprocess state vector after an adjustment to the parameters

    RealT GetLx() const;
    //: Get parameter lx of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetLy() const;
    //: Get parameter ly of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetLz() const;
    //: Get parameter lz of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetZH() const;
    //: Get 3rd homogeneous plane coordinate

  private:
    RealT zh; // 3rd homogeneous coordinate of plane on which line lies
    RealT gaugeWeight; // Weight attached to gauge freedom observation
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector class representing parameters of 2D line
  // Use this class when you are trying to compute a 2D line
  // <blockquote>
  //     lx*xc + ly*yc + lz*zh = 0
  // </blockquote>
  // Methods are provided to build the state vector x from the individual
  // parameters lx,ly,lz, as well as reading them back through methods GetLx(),
  // GetLy() and GetLz(). Data points to use to fit the model can be created
  // as instances of the ObservationLine2dPointC class.
  // <p>
  // Note the design decision to use the redundant representation lx,ly,lz
  // rather than reduce it to a minimal representation. Minimal representations
  // of 2D lines have singularities, as is the case with representing 3D
  // rotations, so we choose to use the "natural" representation lx,ly,lz and
  // deal with the scale freedom that results. Our approach to this
  // and other gauge freedoms is twofold. We introduce an artificial
  // observation on the state whose role is to enforce the gauge condition,
  // as well as applying post-processing to reset the gauge after an adjustment
  // to the state, using the Postprocess() virtual method. The gauge condition
  // observation, processed by the IncrementLS() virtual method, is required to
  // condition the inverse covariance matrix used in least-squares algorithms
  // such as Levenberg-Marquardt, which otherwise would be rank-deficient
  // (at least without damping).
  // The <a href="../../../LevenbergMarquardt/levmarq.html#measure_equation">measurement equation</a> for the gauge condition is
  // <blockquote>
  //   z = h(x) + w
  // </blockquote>
  // where
  // <blockquote>
  //   z = (1) and h(lx,ly,lz) = lx^2 + ly^2
  // </blockquote>
  // The inverse covariance or weight N^-1 of the noise w is set to a value
  // such as 1000, just big enough to have the desired conditioning effect.
  // Since the observations do not "pull" the solution in the gauge freedom
  // direction in the state space, there is no need to set the weight higher
  // than the minimum required. Levenberg-Marquardt will take care of any
  // conditioning problems due to deficiencies in the data.
  // <p>
  // For details on how to use this class for robust and non-robust
  // least-squares problems see the ObservationLine2dPointC and
  // LevenbergMarquardtC classes.
  class StateVectorLine2dC
    : public StateVectorC
  {
  public:    
    StateVectorLine2dC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorLine2dC(RealT lx, RealT ly, RealT lz)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,1.0,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor

    StateVectorLine2dC(RealT lx, RealT ly, RealT lz, RealT zh)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,zh,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor.
    
    StateVectorLine2dC(RealT lx, RealT ly, RealT lz, RealT zh,
		       RealT gaugeWeight)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,zh,gaugeWeight))
    {}
    //: Constructor.
    
    StateVectorLine2dC(RealT lx, RealT ly, RealT lz,
		       const VectorC &xstep)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,1.0,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor.
    
    StateVectorLine2dC(RealT lx, RealT ly, RealT lz, RealT zh,
		       const VectorC &xstep)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,zh,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor.
    
    StateVectorLine2dC(RealT lx, RealT ly, RealT lz, RealT zh,
		       RealT gaugeWeight, const VectorC &xstep)
      : StateVectorC(*new StateVectorLine2dBodyC(lx,ly,lz,zh,gaugeWeight,xstep))
    {}
    //: Constructor.
    
    StateVectorLine2dC(const StateVectorC &stateVec)
      : StateVectorC(dynamic_cast<const StateVectorLine2dBodyC *>(BodyPtr(stateVec)))
    {}
    //: Base class constructor.
    
  protected:
    StateVectorLine2dC(StateVectorLine2dBodyC &bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.

    StateVectorLine2dC(const StateVectorLine2dBodyC *bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorLine2dBodyC &Body()
    { return static_cast<StateVectorLine2dBodyC &>(StateVectorC::Body()); }
    //: Access body.

    const StateVectorLine2dBodyC &Body() const
    { return static_cast<const StateVectorLine2dBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
  public:
    RealT GetLx() const
    { return Body().GetLx(); }
    //: Get parameter lx of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetLy() const
    { return Body().GetLy(); }
    //: Get parameter ly of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetLz() const
    { return Body().GetLz(); }
    //: Get parameter lz of 2D line lx*xc + ly*yc + lz*zh = 0

    RealT GetZH() const
    { return Body().GetZH(); }
    //: Get 3rd homogeneous plane coordinate
  };
}

#undef DEFAULT_GAUGE_WEIGHT

#endif
