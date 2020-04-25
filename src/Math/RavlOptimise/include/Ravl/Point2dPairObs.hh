// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_POINT2DPAIROBS_HEADER
#define RAVLMATH_POINT2DPAIROBS_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/Point2dPairObs.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {

  //! userlevel=Develop
  //: 2D point pair body
  
  class Point2dPairObsBodyC
    : public RCBodyVC
  {
  public:
    Point2dPairObsBodyC(const Vector2dC &nz1, const MatrixRSC &nNi1,
			const Vector2dC &nz2, const MatrixRSC &nNi2)
      : rz1(nz1),
	rz2(nz2),
	rNi1(nNi1.Copy()),
	rNi2(nNi2.Copy())
    {}
    //: Constructor.

    const Vector2dC & z1() const
    {
      return rz1;
    }
    //: Get first point.

    const Vector2dC & z2() const
    {
      return rz2;
    }
    //: Get second point.

    const MatrixRSC & Ni1() const
    {
      return rNi1;
    }
    //: Get first point inverse covariance.

    const MatrixRSC & Ni2() const
    {
      return rNi2;
    }
    //: Get second point inverse covariance.

  private:
    const Vector2dC rz1, rz2;
    const MatrixRSC rNi1, rNi2;
  };

  //! userlevel=Normal
  //! autoLink=on
  //: 2D point pair class
  class Point2dPairObsC
    : public RCHandleC<Point2dPairObsBodyC>
  {
  public:
    Point2dPairObsC(const Vector2dC &z1, const MatrixRSC &Ni1,
		    const Vector2dC &z2, const MatrixRSC &Ni2)
      : RCHandleC<Point2dPairObsBodyC>(*new Point2dPairObsBodyC(z1,Ni1,z2,Ni2))
    {}
    //: Constructor
    // This sticks the individual point observation vectors and inverse
    // covariance matrices together.

  public:
    Point2dPairObsC(Point2dPairObsBodyC &bod)
      : RCHandleC<Point2dPairObsBodyC>(bod)
    {}
    //: Body constructor.
    
    Point2dPairObsBodyC &Body()
    { return RCHandleC<Point2dPairObsBodyC>::Body(); }
    //: Access body.

    const Point2dPairObsBodyC &Body() const
    { return RCHandleC<Point2dPairObsBodyC>::Body(); }
    //: Access body.
    
  public:
    const Vector2dC &z1() const
    { return Body().z1(); }
    //: Get first point.

    const Vector2dC &z2() const
    { return Body().z2(); }
    //: Get second point.

    const MatrixRSC &Ni1() const
    { return Body().Ni1(); }
    //: Get first point inverse covariance.

    const MatrixRSC &Ni2() const
    { return Body().Ni2(); }
    //: Get second point inverse covariance.
  };
}  


#endif
