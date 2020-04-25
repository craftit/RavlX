// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CIRCLE2D_HEADER
#define RAVL_CIRCLE2D_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Euclidean/2D/Circle2d.hh"
//! userlevel=Normal
//! author="Charles Galambos"
//! date="25/2/1997"
//! docentry="Ravl.API.Math.Geometry.2D"
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {
  class LineABC2dC;
  
  template<class DataT> class Array1dC;
  
  //! userlevel=Normal
  //: Circle in 2-D space.
  
  class Circle2dC {
  public:
    
    inline Circle2dC()
      : centre(0,0),
	radius(1)
    {}
    //: Construct a circle with radius of 1 centered on the origin.
    
    inline Circle2dC(const Point2dC &at,RealT rad)
      : centre(at),
	radius(rad)
    {}
    //: Constructor.
    
    Circle2dC(const Point2dC &p1,const Point2dC &p2,const Point2dC &p3)
    { Fit(p1,p2,p3); }
    //: Generate circle from 3 points on its circumference
    
    inline Circle2dC(const Array1dC<Point2dC> &points) { 
      RealT tmp;
      FitLSQ(points,tmp); 
    }
    //: Fit circle to a set of points.
    
    bool FitLSQ(const Array1dC<Point2dC> &points,RealT &residual);
    //: Fit points to a circle.
    // 'residual' is from the least squares fit and can be used to assess 
    // the quality of the fit.  Returns false if fit failed.
    
    inline RealT &Radius() 
    { return radius; }
    //: Constant access to radius.
  
    inline RealT Radius() const 
    { return radius; }
    //: Constant access to radius.
  
    inline Point2dC &Centre() 
    { return centre; }
    //: Centre of circle.
    
    inline Point2dC Centre() const 
    { return centre; }
    //: Constant access to centre of circle.
    
    inline bool IsInside(const Point2dC &point) const
    { return (centre.SqrEuclidDistance(point) < (radius * radius)); }
    //: Is point inside circle ?
    
    inline Point2dC Projection(const Point2dC &point) const {
      Vector2dC dir = point - centre;
      return centre + (radius / dir.Norm()) * dir;
    }
    //: Find the closest point on the circle to 'point'.
    
    inline RealT Angle(const Point2dC & p) const
    { return Vector2dC(p - centre).Angle(); }
    //: Angle between origin and point p.
    
    inline Point2dC Value(RealT angle) const {
      return Point2dC(centre[0] + radius * Cos(angle),
		      centre[1] + radius * Sin(angle));
    }
    //: Get point on circle at given angle.
    
    inline RealT Distance(const Point2dC &p) const
    { return Abs(centre.EuclidDistance(p) - radius); }
    //: Distance to closest point on perimeter.
    
    bool Fit(const Point2dC &p1,const Point2dC &p2,const Point2dC &p3);
    //: Fit a circle from 3 points on its circumference
    // Returns false if the points are collinear.
  private:
    Point2dC centre;
    RealT radius;
  };
  

}
#endif
