// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONIMPQUADRATICPOINT_HEADER
#define RAVLMATH_OBSERVATIONIMPQUADRATICPOINT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationImpQuadraticPoint.hh"

#include "Ravl/ObservationImplicit.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Quadratic fitting observation body
  // This is the implicit version of ObservationQuadraticBodyC with errors
  // in both x and y directions.
  
  class ObservationImpQuadraticPointBodyC
    : public ObservationImplicitBodyC
  {
  public:
    ObservationImpQuadraticPointBodyC(RealT nxc, RealT nyc, RealT nsigma);
    //: Constructor.
    
    ObservationImpQuadraticPointBodyC(RealT nxc, RealT nyc, RealT nsigma,
				      RealT nvarScale, RealT nchi2Thres);
    //: Constructor.

    virtual VectorC EvaluateFunctionF(const StateVectorC &stateVec);
    //: Evaluate the observation function F(x,z) given x and z

    virtual MatrixC EvaluateJacobianFz(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dz given x and z

    virtual MatrixC EvaluateJacobianFx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dx given x and z

  private:
    static VectorC BuildZ(RealT nxc, RealT nyc);
    //: static method to build observation vector from y-coordinate

    static MatrixRSC BuildNi(RealT nsigma);
    //: static method to build observation inverse covariance
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Implicit quadratic fitting data point observation class
  // The problem addressed by this class is that of fitting a quadratic curve
  // <blockquote>
  //    yc = a*xc^2 + b*xc + c
  // </blockquote>
  // to points xc,yc on a plane. We assume here that errors are present in
  // both x and y directions, and are zero-mean Gaussians with a given 2x2
  // covariance matrix N (which can vary between points). Thus using this
  // type of observation will allow least-squares minimisation of the true
  // squared distance of the data points to the curve.
  // This class is an example of how to build a Ravl implicit observation
  // sub-class for a specific problem.
  // <p>
  // This class encapsulates a single data point xc,yc and its associated error
  // covariance. It assumes that the quadratic parameters are represented
  // by an instance of the StateVectorQuadraticC sub-class.
  // The quadratic curve equation above is interpreted as an implicit
  // observation function
  // <blockquote>
  //    F(x,z-w) = 0
  // </blockquote>
  // so the observation vector z is the 2-vector (xc yc)^T, the state vector
  // x is/ formed from the quadratic parameters as (a b c)^T and
  // <blockquote>
  //    F(a,b,c;xc,yc) = a*xc^2 + b*xc + c - yc
  // </blockquote>
  // The noise vector w has two elements and has covariance matrix N, an
  // estimate of which is provided with the point coordinates xc,yc.
  // <p>
  // There is a constructor for both robust and non-robust data point
  // observations, the former of which should be used if there are outliers
  // in the data.
  // <p>
  // To use the data
  // points for optimisation, create one instance of an
  // ObservationImpQuadraticPointC class for each data point, place them all in a
  // single list of ObservationC (the base class) objects, and pass the list
  // into the relevant optimisation routines.
  class ObservationImpQuadraticPointC
    : public ObservationImplicitC
  {
  public:
    ObservationImpQuadraticPointC(RealT nxc, RealT nyc, RealT nsigma)
      : ObservationImplicitC(*new ObservationImpQuadraticPointBodyC(nxc,nyc,nsigma))
    {}
    //: Constructor.
    
    ObservationImpQuadraticPointC(RealT nxc, RealT nyc, RealT nsigma,
			     RealT nvarScale, RealT nchi2Thres)
      : ObservationImplicitC(*new ObservationImpQuadraticPointBodyC(nxc,nyc,nsigma,nvarScale,nchi2Thres))
    {}
    //: Constructor.
    
    ObservationImpQuadraticPointC(const ObservationC &obs)
      : ObservationImplicitC(dynamic_cast<const ObservationImpQuadraticPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationImpQuadraticPointC(ObservationImpQuadraticPointBodyC &bod)
      : ObservationImplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationImpQuadraticPointC(const ObservationImpQuadraticPointBodyC *bod)
      : ObservationImplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationImpQuadraticPointBodyC &Body()
    { return static_cast<ObservationImpQuadraticPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.

    const ObservationImpQuadraticPointBodyC &Body() const
    { return static_cast<const ObservationImpQuadraticPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.
    
  };
}  

#endif
