// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_OBSERVATIONAFFINE2DPOINT_HEADER
#define RAVL_OBSERVATIONAFFINE2DPOINT_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="1/10/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObservationAffine2dPoint.hh"

#include "Ravl/ObservationExplicit.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {
    //! userlevel=Develop
  //: Body observation class for fitting 2D affine projection to 2D points
  
  class ObservationAffine2dPointBodyC
    : public ObservationExplicitBodyC
  {
  public:
    ObservationAffine2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				  const Vector2dC &nz2, const MatrixRSC &nNi2);
    //: Constructor.
    
    ObservationAffine2dPointBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
				  const Vector2dC &nz2, const MatrixRSC &nNi2,
				  RealT varScale, RealT chi2Thres);
    //: Constructor for robust bi-Gaussian observation.

    virtual VectorC EvaluateFunctionH(const StateVectorC &stateVec);
    //: Evaluate the observation function h(x) given x
    
    virtual MatrixC EvaluateJacobianHx(const StateVectorC &stateVec);
    //: Evaluate the Jacobian of an observation for a single point
    
    inline const Vector2dC& GetZ1() const
    { return z1; }
    //: Get point position on first plane

    inline const MatrixRSC& GetNi1() const
    { return Ni1; }
    //: Get point position inverse covariance on first plane

  private:
    Vector2dC z1; // point position on first plane
    MatrixRSC Ni1; // point position inverse covariance on first plane    
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Observation class for fitting 2D affine projection to 2D points 
  
  class ObservationAffine2dPointC
    : public ObservationExplicitC
  {
  public:
    ObservationAffine2dPointC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ObservationAffine2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
			      const Vector2dC &nz2, const MatrixRSC &nNi2)
      : ObservationExplicitC(*new ObservationAffine2dPointBodyC(nz1,nNi1,nz2,nNi2))
    {}
    //: Constructor.
    
    ObservationAffine2dPointC(const Vector2dC &nz1, const MatrixRSC &nNi1,
			      const Vector2dC &nz2, const MatrixRSC &nNi2,
			      RealT varScale, RealT chi2Thres)
      : ObservationExplicitC(*new ObservationAffine2dPointBodyC(nz1,nNi1,nz2,nNi2,varScale,chi2Thres))
    {}
    //: Constructor for robust bi-Gaussian observation.

    ObservationAffine2dPointC(const ObservationC &obs)
      : ObservationExplicitC(dynamic_cast<const ObservationAffine2dPointBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
  protected:
    ObservationAffine2dPointC(ObservationAffine2dPointBodyC &bod)
      : ObservationExplicitC(bod)
    {}
    //: Body constructor.
    
    ObservationAffine2dPointBodyC &Body()
    { return static_cast<ObservationAffine2dPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.

    const ObservationAffine2dPointBodyC &Body() const
    { return static_cast<const ObservationAffine2dPointBodyC &>(ObservationExplicitC::Body()); }
    //: Access body.
    
  public:
    const Vector2dC& GetZ1() const
    { return Body().GetZ1(); }
    //: Get point position on first plane

    const MatrixRSC& GetNi1() const
    { return Body().GetNi1(); }
    //: Get point position inverse covariance on first plane
  };

  
}


#endif
