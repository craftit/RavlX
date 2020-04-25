// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STATEVECTORHOMOG2D_HEADER
#define RAVLMATH_STATEVECTORHOMOG2D_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorHomog2d.hh"

#include "Ravl/StateVector.hh"
#include "Ravl/Matrix3d.hh"

#define DEFAULT_GAUGE_WEIGHT 1.0e3

namespace RavlN {
  
  //! userlevel=Develop
  //: State vector body class representing 2D homography
  class StateVectorHomog2dBodyC
    : public StateVectorBodyC
  {
  public:
    StateVectorHomog2dBodyC()
    {}
    //: Default constructor.

    StateVectorHomog2dBodyC(const VectorC &x, RealT zh1, RealT zh2,
			    RealT gaugeWeight);
    //: Constructor.
    
    StateVectorHomog2dBodyC(const VectorC &x, RealT zh1, RealT zh2,
			    RealT gaugeWeight,
			    const VectorC &xstep);
    //: Constructor.
    
    StateVectorHomog2dBodyC(const Matrix3dC &P, RealT zh1, RealT zh2,
			    RealT gaugeWeight);
    //: Constructor.
    
    StateVectorHomog2dBodyC(const Matrix3dC &P, RealT zh1, RealT zh2,
			    RealT gaugeWeight,
			    const VectorC &xstep);
    //: Constructor.
    
    StateVectorHomog2dBodyC(const VectorC &x, RealT zh1, RealT zh2,
			    RealT gaugeWeight, const VectorC &xstep,
			    const Matrix3dC &Pnew);
    //: Constructor.
    
    virtual RCBodyVC &Copy() const
    { return *new StateVectorHomog2dBodyC(x.Copy(),GetZH1(),GetZH2(),gaugeWeight,xstep.Copy(),P); }
    //: Make a copy of the body.
    
    virtual bool IncrementLS(MatrixRSC &A, VectorC &a);
    //: Increment the linear system

    virtual bool Postprocess();
    //: Postprocess state vector after an adjustment to the parameters

    const Matrix3dC GetHomog() const
    { return P; }
    //: Get 2D homography represented by state vector

    RealT GetZH1() const
    { return zh1; }
    //: Return 3rd homogeneous coordinate of first plane

    RealT GetZH2() const
    { return zh2; }
    //: Return 3rd homogeneous coordinate of second plane

  protected:
    RealT zh1, zh2; // 3rd homogeneous coordinates in planes 1 & 2
    Matrix3dC P; // 2D homography
    RealT gaugeWeight; // weight of gauge condition observation

  private:
    static VectorC StateVecFromHomog(const Matrix3dC &P);
    //: static method to build state vector from homography matrix
  };

  //! userlevel=Normal
  //! autoLink=on
  //: State vector handle class representing 2D homography
  // Use this class when you are trying to compute a 2D homography relating
  // two planes. The relevant aspects of a 2D homography for optimisation
  // purposes are:
  // <ul>
  //   <li> The state vector x has nine elements representing a 3x3 homography
  //        matrix P. We identify the nine elements x[0..8] of x as P[0][0],
  //        P[0][1], P[0][2], P[1][0], P[1][1], P[1][2], P[2][0], P[2][1]
  //        and P[2][2] respectively, i.e. "raster scan" order.
  //   <li> P relates the coordinates of two planes under projective geometry:
  //        p2 = l*P*p1 where p1, p2 are the homogeneous coordinates of points
  //        in planes 1, 2, and l is a scale factor as is normal for equations
  //        involving projective quantities.
  //   <li> The scaling of P is irrelevant, so s*P represents the same
  //        homography as P for any scale factor s.
  // </ul>
  // The main use of this class will be to estimate the motion between images,
  // typically using point features. However we want to be able to estimate
  // a 2D homography from different types of features, e.g. lines, or maybe
  // using image-based methods. As a result nothing in this class is specific
  // to point features. The ObsVectorPoint2dPairC and
  // ObservationHomog2dPointC classes are relevant for estimating a 2D
  // homography using point features.
  // <p>
  // The scale ambiguity in P requires special attention. Our approach to this
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
  //   z = (1) and h(x) = ||x||^2
  // </blockquote>
  // which is equivalent to h(P) = ||P||^2, the square Frobenius norm of P.
  // The inverse covariance or weight N^-1 of the noise w is set to a value
  // such as 1000, just big enough to have the desired conditioning effect.
  // Since the observations do not "pull" the solution in the gauge freedom
  // direction in the state space, there is no need to set the weight higher
  // than the minimum required. Levenberg-Marquardt will take care of any
  // conditioning problems due to deficiencies in the data.
  // <p>
  // The third (z) homogeneous coordinate for each plane (zh1 and zh2 in the
  // constructors) are assumed to
  // be one if they are not passed to the constructor, but convergence CAN
  // depend on an appropriate choice of the coordinates. You should typically
  // choose zh1 and zh2 to be "similar" to the x and y
  // coordinates of the data used in
  // the fitting. For instance, if image coordinates are used, say from the
  // centre of a 720x568 pixel video image, the homogeneous z coordinate should
  // be 300 or thereabouts.
  class StateVectorHomog2dC
    : public StateVectorC
  {
  public:    
    StateVectorHomog2dC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StateVectorHomog2dC(const VectorC &x)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,1.0,1.0,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the default
    // gauge condition weighting.

    StateVectorHomog2dC(const VectorC &x, RealT zh1, RealT zh2)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,zh1,zh2,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates with the default
    // gauge condition weighting.

    StateVectorHomog2dC(const VectorC &x, RealT gaugeWeight)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,1.0,1.0,gaugeWeight))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the provided
    // gauge condition weighting.

    StateVectorHomog2dC(const VectorC &x, RealT zh1, RealT zh2,
			RealT gaugeWeight)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,zh1,zh2,gaugeWeight))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates and gauge condition
    // weighting.

    StateVectorHomog2dC(const VectorC &x, const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,1.0,1.0,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the default
    // gauge condition weighting. The vector xstep, which should have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const VectorC &x, RealT zh1, RealT zh2,
			const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,zh1,zh2,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates with the default
    // gauge condition weighting. The vector xstep, which should have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const VectorC &x, RealT gaugeWeight,
			const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,1.0,1.0,gaugeWeight,xstep))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the provided
    // gauge condition weighting. The vector xstep, which should be have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const VectorC &x, RealT zh1, RealT zh2,
			RealT gaugeWeight, const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(x,zh1,zh2,gaugeWeight,xstep))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates and gauge condition
    // weighting. The vector xstep, which should be have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.
    StateVectorHomog2dC(const Matrix3dC &P)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,1.0,1.0,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the default
    // gauge condition weighting.

    StateVectorHomog2dC(const Matrix3dC &P, RealT zh1, RealT zh2)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,zh1,zh2,DEFAULT_GAUGE_WEIGHT))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates with the default
    // gauge condition weighting.

    StateVectorHomog2dC(const Matrix3dC &P, RealT gaugeWeight)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,1.0,1.0,gaugeWeight))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the provided
    // gauge condition weighting.

    StateVectorHomog2dC(const Matrix3dC &P, RealT zh1, RealT zh2,
			RealT gaugeWeight)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,zh1,zh2,gaugeWeight))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates and gauge condition
    // weighting.

    StateVectorHomog2dC(const Matrix3dC &P, const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,1.0,1.0,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the default
    // gauge condition weighting. The vector xstep, which should be have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const Matrix3dC &P, RealT zh1, RealT zh2,
			const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,zh1,zh2,DEFAULT_GAUGE_WEIGHT,xstep))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates with the default
    // gauge condition weighting. The vector xstep, which should be have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const Matrix3dC &P, RealT gaugeWeight,
			const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,1.0,1.0,gaugeWeight,xstep))
    {}
    //: Constructor
    // Sets the 3rd homogeneous image coordinates to 1 and uses the provided
    // gauge condition weighting. The vector xstep, which should be have nine
    // elements, specifies the step sizes to use when calculating
    // numerical derivatives with respect to the elements of x, overriding
    // the default step size.

    StateVectorHomog2dC(const Matrix3dC &P, RealT zh1, RealT zh2,
			RealT gaugeWeight, const VectorC &xstep)
      : StateVectorC(*new StateVectorHomog2dBodyC(P,zh1,zh2,gaugeWeight,xstep))
    {}
    //: Constructor
    // Uses the provided 3rd homogeneous image coordinates and gauge condition
    // weighting. The vector xstep, which should be have nine elements,
    // specifies the step sizes to use when calculating numerical derivatives
    // with respect to the elements of x, overriding the default step size.

    StateVectorHomog2dC(const StateVectorC &stateVec)
      : StateVectorC(dynamic_cast<const StateVectorHomog2dBodyC *>(BodyPtr(stateVec)))
    {}
    //: Base class constructor.
    
  protected:
    StateVectorHomog2dC(StateVectorHomog2dBodyC &bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorHomog2dC(const StateVectorHomog2dBodyC *bod)
      : StateVectorC(bod)
    {}
    //: Body constructor.
    
    StateVectorHomog2dBodyC &Body()
    { return static_cast<StateVectorHomog2dBodyC &>(StateVectorC::Body()); }
    //: Access body.

    const StateVectorHomog2dBodyC &Body() const
    { return static_cast<const StateVectorHomog2dBodyC &>(StateVectorC::Body()); }
    //: Access body.
    
  public:
    const Matrix3dC GetHomog() const
    { return Body().GetHomog(); }
    //: Get 2D homography represented by state vector

    RealT GetZH1() const
    { return Body().GetZH1(); }
    //: Return 3rd homogeneous coordinate of first plane

    RealT GetZH2() const
    { return Body().GetZH2(); }
    //: Return 3rd homogeneous coordinate of second plane
  };
}

#undef DEFAULT_GAUGE_WEIGHT

#endif
