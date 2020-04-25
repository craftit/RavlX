// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONHOMOG2DPOINT_HEADER
#define RAVLMATH_OBSERVATIONHOMOG2DPOINT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationHomog2dPoint.hh"

#include "Ravl/ObservationExplicit.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Body observation class for fitting 2D homography to 2D points
  
  class ObservationHomog2dPointBodyC
    : public ObservationExplicitBodyC
  {
  public:

    ObservationHomog2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				 const Vector2dC &nz2, const MatrixRSC &nNi2);
    //: Constructor.
    ObservationHomog2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				 const Vector2dC &nz2, const MatrixRSC &nNi2,
				 RealT varScale, RealT chi2Thres);
    //: Constructor for robust bi-Gaussian observation.

    virtual VectorC EvaluateFunctionH(const StateVectorC &stateVec);
    //: Evaluate the observation function h(x) given x

    virtual MatrixC EvaluateJacobianHx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian of an observation for a single point
    // This requires that you call the EvaluateFunctionH method first,
    // with the same stateVec parameter values, in order to set private
    // values in the class.

    inline const Vector2dC& GetZ1() const
    { return z1; }
    //: Get point position on first plane

    inline const MatrixRSC& GetNi1() const
    { return Ni1; }
    //: Get point position inverse covariance on first plane

  private:
    Vector2dC z1; // point position on first plane
    MatrixRSC Ni1; // point position inverse covariance on first plane

    Vector2dC p2; // last evaluation of observation function
    RealT z2; // last evaluation of z-coordinate of plane 2 projection
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation class for fitting 2D homography to 2D points
  // Use this class to construct an observation consisting of a pair of 2D
  // points, each point of which lies on a separate plane. The two planes are
  // related by a 2D projective transformation represented by a 3x3 homography
  // matrix. This class applies the functional model relating the point on
  // plane 2 to that on plane 1 in the explicit form
  // <blockquote>
  //    p2 = h(P,p1) + w
  // </blockquote>
  // where p1,p2 are the x/y coordinates of the point on planes 1,2, P is
  // the 3x3 homography matrix and w is a noise vector. Comparing this with
  // the explicit form of observation
  // <blockquote>
  //    z = h(x) + w
  // </blockquote>
  // (see the
  // <a href="../../../LevenbergMarquardt/levmarq.html#measure_equation">mathematical description</a>),
  // we can identify:-
  // <ul>
  //   <li> The observation vector z as the point p2 in the second plane;
  //   <li> The state vector x contains the nine elements of the homography
  //        matrix P;
  //   <li> The noise vector w models the error in p2 only;
  //   <li> p1 is treated as having known "independent" coordinates.
  // </ul>
  // We can write the functional model in full, given p1=(x1 x2)^T and
  // p2=(x2 y2)^T as
  // <blockquote>
  //   x2 = zh2(Pxx*x1 + Pxy*y1 + Pxz*zh1)/(Pzx*x1 + Pzy*y1 + Pzz*zh1)
  // </blockquote>
  // <blockquote>
  //   y2 = zh2(Pyx*x1 + Pyy*y1 + Pyz*zh1)/(Pzx*x1 + Pzy*y1 + Pzz*zh1)
  // </blockquote>
  // where Pxx, Pxy, ..., Pzz are the elements of P and zh1,zh2 are the chosen
  // fixed values for the third (z) homogenous plane coordinate
  // (see StateVectorHomog2dC).
  // <p>
  // If there are potentially outliers in the data use the robust form of
  // constructor, which applies a bi-Gaussian inlier/outlier error model.
  // This should be fine if the outliers are not TOO far away from the
  // good data.
  // <p>
  // If you want to model errors in both p1 and p2, use the implicit form
  // of functional model provided by the ObservationImpHomog2dPointC class.
  // <p>
  // This class encapsulates a single data point pair p1,p2 and the
  // associated error inverse covariance matrix N^-1 for p2.
  // It assumes that the 2D homography parameters are represented by an
  // instance of the StateVectorHomog2dC sub-class.
  // <p>
  // To use the data point pairs for optimisation, create one instance of an
  // ObservationHomog2dPointC for each pair of points, place them all in a
  // single list of ObservationC (the base class) objects, and pass the list
  // into the relevant optimisation routines.
  class ObservationHomog2dPointC
    : public ObservationExplicitC
  {
  public:
    ObservationHomog2dPointC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationHomog2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
			     const Vector2dC &nz2, const MatrixRSC &nNi2)
      : ObservationExplicitC(*new ObservationHomog2dPointBodyC(nz1,nNi1,nz2,nNi2))
    {}
    //: Constructor.
    //!param: nz1, nz2 - pair of corresponding vectors
    //!param: nNi1, nNi2 -  Inverse covariance matrix N^-1 of noise vector added to nz1, nz2
    
    ObservationHomog2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
			     const Vector2dC &nz2, const MatrixRSC &nNi2,
			     RealT varScale, RealT chi2Thres)
      : ObservationExplicitC(*new ObservationHomog2dPointBodyC(nz1,nNi1,nz2,nNi2,varScale,chi2Thres))
    {}
    //: Constructor for robust bi-Gaussian observation.
    //!param: nz1, nz2 - pair of corresponding vectors
    //!param: nNi1, nNi2 -  Inverse covariance matrix N^-1 of noise vector added to nz1, nz2
    
    ObservationHomog2dPointC(const ObservationC &obs)
      : ObservationExplicitC(dynamic_cast<const ObservationHomog2dPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationHomog2dPointC(ObservationHomog2dPointBodyC &bod)
      : ObservationExplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationHomog2dPointBodyC &Body()
    { return static_cast<ObservationHomog2dPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.

    const ObservationHomog2dPointBodyC &Body() const
    { return static_cast<const ObservationHomog2dPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.
    
  public:
    const Vector2dC& GetZ1() const
    { return Body().GetZ1(); }
    //: Get point position on first plane

    const MatrixRSC& GetNi1() const
    { return Body().GetNi1(); }
    //: Get point position inverse covariance on first plane
    
    friend class ObservationHomog2dPointBodyC;
  };
}  


#endif
