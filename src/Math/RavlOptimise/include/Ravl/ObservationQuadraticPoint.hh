// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONQUADRATICPOINT_HEADER
#define RAVLMATH_OBSERVATIONQUADRATICPOINT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationQuadraticPoint.hh"

#include "Ravl/ObservationExplicit.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Quadratic fitting point observation body.
  
  class ObservationQuadraticPointBodyC
    : public ObservationExplicitBodyC
  {
  public:
    ObservationQuadraticPointBodyC(RealT nxc, RealT nyc, RealT nsigma);
    //: Constructor.
    
    ObservationQuadraticPointBodyC(RealT nxc, RealT nyc, RealT nsigma,
				   RealT nvarScale, RealT nchi2Thres);
    //: Constructor.

    virtual VectorC EvaluateFunctionH(const StateVectorC &stateVec);
    //: Evaluate the observation function h(x) given x

    virtual MatrixC EvaluateJacobianHx(const StateVectorC &x);
    //: Evaluate the Jacobian dh/dx given x

    RealT GetXC() const;
    //: Return x-coordinate of point

  private:
    RealT xc; //: x-coordinate of point

    static VectorC BuildZ(RealT nyc);
    //: static method to build observation vector from y-coordinate

    static MatrixRSC BuildNi(RealT nsigma);
    //: static method to build observation inverse covariance
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Explicit quadratic fitting data point observation class
  // The problem addressed by this class is that of fitting a quadratic curve
  // <blockquote>
  //    yc = a*xc^2 + b*xc + c
  // </blockquote>
  // to points xc,yc on a plane. We assume that all the errors in the point
  // coordinates are in the y-direction, and are zero-mean Gaussians with
  // standard deviation sigma (which can vary between points).
  // This problem can be solved by linear
  // least squares, unless outliers are present in the data.
  // If there are errors in both x and y directions, use the implicit form
  // ObservationImpQuadraticPointC of observation instead.
  // This class is an example of how to build a Ravl observation sub-class
  // for a specific problem. It also serves as a test for the Ravl optimisation
  // routines, since the result can be checked against the linear least squares
  // solution.
  // <p>
  // This class encapsulates a single data point xc,yc and its associated error
  // estimate sigma. It assumes that the quadratic parameters are represented
  // by an instance of the StateVectorQuadraticC sub-class.
  // The quadratic curve equation above is interpreted directly as an
  // explicit observation function
  // <blockquote>
  //    z = h(x) + w
  // </blockquote>
  // so the observation vector z is the 1-vector (yc), the state vector x is
  // formed from the quadratic parameters as (a b c)^T and
  // <blockquote>
  //    h(a,b,c) = a*xc^2 + b*xc + c
  // </blockquote>
  // The x-coordinate xc is in a sense outside the standard observation
  // model and is in fact stored directly in the ObservationQuadraticPointBodyC
  // class. The noise vector w has one element and has variance sigma^2.
  // <p>
  // There is a constructor for both robust and non-robust data point
  // observations, the former of which should be used if there are outliers
  // in the data.
  // <p>
  // To use the data
  // points for optimisation, create one instance of an ObservationQuadraticPointC
  // for each data point, place them all in a single list of ObservationC
  // (the base class) objects, and pass the list into the relevant optimisation
  // routines.
  class ObservationQuadraticPointC
    : public ObservationExplicitC
  {
  public:
    ObservationQuadraticPointC(RealT xc, RealT yc, RealT sigma)
      : ObservationExplicitC(*new ObservationQuadraticPointBodyC(xc,yc,sigma))
    {}
    //: Constructor.
    // xc, yc are the x, y coordinates of a point on the curve, and sigma is
    // the estimated standard deviation of the error in yc.
    
    ObservationQuadraticPointC(RealT xc, RealT yc, RealT sigma,
			       RealT varScale, RealT chi2Thres)
      : ObservationExplicitC(*new ObservationQuadraticPointBodyC(xc,yc,sigma,varScale,chi2Thres))
    {}
    //: Constructor.
    // xc, yc are the x, y coordinates of a point on the curve, and sigma is
    // the estimated standard deviation of the error in yc. This constructs
    // a robust bi-gaussian observation using the ObsVectorBiGaussianC class.
    // The varScale parameter is the size of the outlier standard deviation
    // relative to the inlier distribution (as given by sigma). If you estimate
    // the outlier distrubition standard deviation as sigma_o, then varScale
    // should be set to (sigma_o/sigma)^2. The chi2Thres value specifies the
    // switching point between the inlier and outlier distributions, as a
    // threshold on the normalised chi-squared error using the inlier
    // distribution.
    
    ObservationQuadraticPointC(const ObservationC &obs)
      : ObservationExplicitC(dynamic_cast<const ObservationQuadraticPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationQuadraticPointC(ObservationQuadraticPointBodyC &bod)
      : ObservationExplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationQuadraticPointC(const ObservationQuadraticPointBodyC *bod)
      : ObservationExplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationQuadraticPointBodyC &Body()
    { return static_cast<ObservationQuadraticPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.

    const ObservationQuadraticPointBodyC &Body() const
    { return static_cast<const ObservationQuadraticPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.

  public:
    RealT GetXC() const
    { return Body().GetXC(); }
    //: Return x-coordinate of point

  };
}  


#endif
