// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_POINT2DOBS_HEADER
#define RAVLMATH_POINT2DOBS_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Point2dObs.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {

  //! userlevel=Develop
  //: 2D point with error inverse covariance body
  
  class Point2dObsBodyC
    : public RCBodyVC
  {
  public:
    Point2dObsBodyC(const Vector2dC &nz, const MatrixRSC &nNi)
      : rz(nz),
	rNi(nNi.Copy())
    {}
    //: Constructor.

    const Vector2dC & z() const
    {
      return rz;
    }
    //: Get point.

    const MatrixRSC & Ni() const
    {
      return rNi;
    }
    //: Get point inverse covariance.

  private:
    const Vector2dC rz;
    const MatrixRSC rNi;
  };

  //! userlevel=Normal
  //! autoLink=on
  //: 2D point class with inverse covariance
  class Point2dObsC
    : public RCHandleC<Point2dObsBodyC>
  {
  public:
    Point2dObsC(const Vector2dC &z, const MatrixRSC &Ni)
      : RCHandleC<Point2dObsBodyC>(*new Point2dObsBodyC(z,Ni))
    {}
    //: Constructor

  public:
    Point2dObsC(Point2dObsBodyC &bod)
      : RCHandleC<Point2dObsBodyC>(bod)
    {}
    //: Body constructor.
    
    Point2dObsBodyC &Body()
    { return RCHandleC<Point2dObsBodyC>::Body(); }
    //: Access body.

    const Point2dObsBodyC &Body() const
    { return RCHandleC<Point2dObsBodyC>::Body(); }
    //: Access body.
    
  public:
    const Vector2dC &z() const
    { return Body().z(); }
    //: Get point.

    const MatrixRSC &Ni() const
    { return Body().Ni(); }
    //: Get point inverse covariance.
  };
}  


#endif
