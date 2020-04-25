// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ARC2D_HEADER
#define RAVL_ARC2D_HEADER 1
////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! date="10/3/1997"
//! docentry="Ravl.API.Math.Geometry.2D"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Arc2d.hh"

#include "Ravl/Circle2d.hh"

namespace RavlN {

  //: This class implements 2d circular arcs.
  
  class Arc2dC : public Circle2dC {
  public:
    inline Arc2dC() {
      ends[0] = 0; 
      ends[1] = 0; 
    }
    //: Default constructor.
    
    bool FitLSQ(const Array1dC<Point2dC> &points,RealT &residual);
    //: Fit points to a circle.
    // 'residual' is from the least squares fit and can be used to assess the quality of the
    // fit. Assumes the points are ordered around the arc.
    // Returns false if fit failed.
    
    inline RealT &StartAngle()
    { return ends[0]; }
    //: Start angle of arc, which proccedes clockwise.
    
    inline RealT StartAngle() const
    { return ends[0]; }
    //: Start angle of arc, which proccedes clockwise.
    
    inline RealT &EndAngle()
    { return ends[1]; }
    //: End angle of arc, which proccedes clockwise.
    
    inline RealT EndAngle() const
    { return ends[1]; }
    //: End angle of arc, which proccedes clockwise.
    
    bool Fit(const Point2dC &p1,const Point2dC &p2,const Point2dC &p3);
    //: Fit a circle through 3 points.
    // Returns false if the points are collinear.
    
  private:
    RealT ends[2];
  };

}
#endif
