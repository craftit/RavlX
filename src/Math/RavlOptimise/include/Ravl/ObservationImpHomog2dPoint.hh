// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONIMPHOMOG2DPOINT_HEADER
#define RAVLMATH_OBSERVATIONIMPHOMOG2DPOINT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImpHomog2dPoint.hh"

#include "Ravl/ObservationImplicit.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: 2D homography fitting implicit 2D point observation body.
  
  class ObservationImpHomog2dPointBodyC
    : public ObservationImplicitBodyC
  {
  public:

    ObservationImpHomog2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				    const Vector2dC &nz2, const MatrixRSC &nNi2);
    //: Constructor.
    //!param: nz1, nz2 - pair of corresponding points
    //!param: nNi1, nNi2 - covariance matrices for nz1 and nz2
    
    ObservationImpHomog2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				    const Vector2dC &nz2, const MatrixRSC &nNi2,
				    RealT varScale, RealT chi2Thres);
    //: Constructor for robust bi-Gaussian observation.
    //!param: nz1, nz2 - pair of corresponding points
    //!param: nNi1, nNi2 - covariance matrices for nz1 and nz2
    //!param: varScale - ratio of <i>variances</i> of outliers to inliers
    //!param: chi2Thres - chi-squared threshold

    virtual VectorC EvaluateFunctionF(const StateVectorC &stateVec);
    //: Evaluate the observation function F(x,z) given x and z
#if 1
    virtual MatrixC EvaluateJacobianFz(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dz given x and z

    virtual MatrixC EvaluateJacobianFx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dx given x and z
#endif
  private:
    Vector2dC p2; // last evaluation of projected position on plane 2
    RealT z2; // last evaluation of z-coordinate of plane 2 projection
  };

  //! userlevel=Normal
  //! autoLink=on
  //: 2D homography fitting implicit 2D point observation class
  // Use this class to construct an observation consisting of a pair of 2D
  // points, each point of which lies on a separate plane. The two planes are
  // related by a 2D projective transformation represented by a 3x3 homography
  // matrix. This class applies the functional model constraining the points
  // in in planes 1 and 2 through the homography constraint:
  // <blockquote>
  //    F(P,p1-w1,p2-w2) = 0
  // </blockquote>
  // where p1,p2 are the x/y coordinates of the point on planes 1,2, P is
  // the 3x3 homography matrix and w1,w2 are the errors in p1,p2.
  // Comparing this with the implicit form of
  // observation
  // <blockquote>
  //    F(x,z-w) = 0
  // </blockquote>
  // (see the
  // <a href="../../../LevenbergMarquardt/node2.html">mathematical description</a>),
  // we can identify:-
  // <ul>
  //   <li> The observation vector z as the points p1,p2 stacked on top of each
  //        other into a 4-vector: z^T = (p1^T p2^T).
  //   <li> The state vector x contains the nine elements of the homography
  //        matrix P;
  //   <li> The noise vector w models the errors in p1 and p2:
  //        w^T = (w1^T w2^T).
  // </ul>
  // We can write the functional model in full, given p1=(x1 x2)^T,
  // p2=(x2 y2)^T and w1=(w1x w1y)^T, w2=(w2x w2y)^T, as
  // <blockquote>
  //   zh2(Pxx*(x1-w1x) + Pxy*(y1-w1y) + Pxz*zh1)/(Pzx*(x1-w1x) + Pzy*(y1-w1y) + Pzz*zh1) - (x2-w2x) = 0
  // </blockquote>
  // <blockquote>
  //   zh2(Pyx*(x1-w1x) + Pyy*(y1-w1y) + Pyz*zh1)/(Pzx*(x1-w1x) + Pzy*(y1-w1y) + Pzz*zh1) - (y2-w2y) = 0
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
  // If you want to model the error only in p2, use the simpler and more
  // efficient explicit form of functional model provided by the
  // ObservationHomog2dPointC class.
  // <p>
  // This class encapsulates a single data point pair p1,p2 and the
  // associated error inverse covariance matrices N1^-1, N2^-1 for p1,p2.
  // It assumes that the 2D homography parameters are represented by an
  // instance of the StateVectorHomog2dC sub-class.
  // <p>
  // To use the data point pairs for optimisation, create one instance of an
  // ObservationImpHomog2dPointC for each pair of points, place them all in a
  // single list of ObservationC (the base class) objects, and pass the list
  // into the relevant optimisation routines.
  class ObservationImpHomog2dPointC
    : public ObservationImplicitC
  {
  public:
    ObservationImpHomog2dPointC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationImpHomog2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				const Vector2dC &nz2, const MatrixRSC &nNi2)
      : ObservationImplicitC(*new ObservationImpHomog2dPointBodyC(nz1,nNi1,nz2,nNi2))
    {}
    //: Constructor.
    //!param: nz1, nz2 - pair of corresponding points
    //!param: nNi1, nNi2 - covariance matrices for nz1 and nz2
    
    ObservationImpHomog2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				const Vector2dC &nz2, const MatrixRSC &nNi2,
				RealT varScale, RealT chi2Thres=5.0)
      : ObservationImplicitC(*new ObservationImpHomog2dPointBodyC(nz1,nNi1,nz2,nNi2,varScale,chi2Thres))
    {}
    //: Constructor for robust bi-Gaussian observation.
    //!param: nz1, nz2 - pair of corresponding points
    //!param: nNi1, nNi2 - covariance matrices for nz1 and nz2
    //!param: varScale - ratio of variances of outliers to inliers
    //!param: chi2Thres - chi-squared threshold

    ObservationImpHomog2dPointC(const ObservationC &obs)
      : ObservationImplicitC(dynamic_cast<const ObservationImpHomog2dPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationImpHomog2dPointC(ObservationImpHomog2dPointBodyC &bod)
      : ObservationImplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationImpHomog2dPointBodyC &Body()
    { return static_cast<ObservationImpHomog2dPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.

    const ObservationImpHomog2dPointBodyC &Body() const
    { return static_cast<const ObservationImpHomog2dPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.
    
  public:
  };
}  


#endif
