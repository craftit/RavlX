// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSERVATIONLINE2DPOINT_HEADER
#define RAVLMATH_OBSERVATIONLINE2DPOINT_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="OrthogonalRegressionTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationLine2dPoint.hh"

#include "Ravl/ObservationImplicit.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: 2D line fitting point observation body.
  
  class ObservationLine2dPointBodyC
    : public ObservationImplicitBodyC
  {
  public:

    ObservationLine2dPointBodyC(const Vector2dC &z, const MatrixRSC &Ni);
    //: Constructor.
    
    ObservationLine2dPointBodyC(const Vector2dC &z, const MatrixRSC &Ni,
				RealT varScale, RealT chi2Thres);
    //: Constructor for robust bi-Gaussian observation.

    virtual VectorC EvaluateFunctionF(const StateVectorC &stateVec);
    //: Evaluate the observation function F(x,z) given x and z
#if 1
    virtual MatrixC EvaluateJacobianFz(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dz given x and z

    virtual MatrixC EvaluateJacobianFx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian dF/dx given x and z
    // This requires that you call the EvaluateFunctionF method first,
    // with the same stateVec parameter values, in order to set private
    // values in the class.
#endif
  private:
    RealT F; // last evaluation of F(x,z)
  };

  //! userlevel=Normal
  //! autoLink=on
  //: 2D line fitting point observation class
  // Use this class to construct 2D data point observations on a plane which
  // are to be used to fit a line through the data. If all the points have
  // the same error isotropic error in the x and y directions, you can use
  // the standard orthogonal regression algorithm. If some points have
  // different errors to others, or there are outliers in the data points,
  // this class may be useful. It allows you to specify errors separately
  // for each point, as well as use a robust error model that accounts for
  // outliers, at least if they are not TOO far away from the good data.
  // <p>
  // This class encapsulates a single data point xc,yc and its associated error
  // inverse covariance matrix estimate N^-1. It assumes that the 2D line
  // parameters are represented by an instance of the StateVectorLine2dC
  // sub-class.
  // <p>
  // To use the data points for optimisation, create one instance of an
  // ObservationLine2dPointC for each data point, place them all in a single
  // list of ObservationC (the base class) objects, and pass the list into the
  // relevant optimisation routines.
  class ObservationLine2dPointC
    : public ObservationImplicitC
  {
  public:
    ObservationLine2dPointC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationLine2dPointC(const Vector2dC &z, const MatrixRSC &Ni)
      : ObservationImplicitC(*new ObservationLine2dPointBodyC(z,Ni))
    {}
    //: Constructor.
    
    ObservationLine2dPointC(const Vector2dC &z, const MatrixRSC &Ni,
			    RealT varScale, RealT chi2Thres)
      : ObservationImplicitC(*new ObservationLine2dPointBodyC(z,Ni,varScale,chi2Thres))
    {}
    //: Constructor for robust bi-Gaussian observation.
    
    ObservationLine2dPointC(const ObservationC &obs)
      : ObservationImplicitC(dynamic_cast<const ObservationLine2dPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationLine2dPointC(ObservationLine2dPointBodyC &bod)
      : ObservationImplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationLine2dPointBodyC &Body()
    { return static_cast<ObservationLine2dPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.

    const ObservationLine2dPointBodyC &Body() const
    { return static_cast<const ObservationLine2dPointBodyC &>(ObservationImplicitC::Body()); }
    //: Access body.
    
  public:
  };
}  


#endif
