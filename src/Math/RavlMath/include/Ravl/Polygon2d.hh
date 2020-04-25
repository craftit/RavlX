// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POLYGON2D_HEADER
#define RAVL_POLYGON2D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/2D/Polygon2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/9/2002"
//! docentry="Ravl.API.Math.Geometry.2D"

#include "Ravl/PointSet2d.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  
  class Moments2d2C;

  //: A polygon in 2d space
  // The Polygon2dC class is a representation of a polygon in 2 dimensional
  // space. A polygon is the region of a plane bounded by a finite collection
  // of line segments forming a simple closed curve. <p>
  // Ref.: -  O'Rourke,J.: Computational geometry in C;
  //          Cambridge University Press, 1994, p. 1 <p>
  
  class Polygon2dC
    : public PointSet2dC
  {
  public:

    Polygon2dC() {}
    // Empty list of points.
      
    Polygon2dC(const DListC<Point2dC>& points)
      : PointSet2dC(points)
    {}
    // Construct from list of points

    Polygon2dC(const RealRange2dC &range);
    //: Constructor creates a rectangular polygon of the range
    // The corners of the range are inserted into the polygon in clockwise order

    RealT Area2() const;
    // Returns twice the signed area of this polygon.
    // Ref.: -  O'Rourke,J.: Computational geometry in C;
    //          Cambridge University Press, 1994, pp. 20-26
    //!deprecated: Use Area instead

    RealT Area() const;
    //!return: the signed area of this polygon

    Point2dC Centroid() const;
    //!return: the centre of mass of the polygon

    Moments2d2C Moments() const;
    //!return: the moments of the polygon
  
    bool IsDiagonal(const DLIterC<Point2dC> & a, const DLIterC<Point2dC> & b, bool allowExternal = false) const;
    // Returns true if (a, b) is a proper internal or external (if allowExternal is true)
    // diagonal of this polygon. The edges incident to 'a' and 'b'
    // are ignored.
    // Ref.: -  O'Rourke,J.: Computational geometry in C;
    //          Cambridge University Press, 1994, pp. 35-36
  
    bool IsInCone(const DLIterC<Point2dC> & a, const DLIterC<Point2dC> & b) const;
    // Returns true iff the diagonal (a,b) is strictly internal
    // to this polygon in the neighbourhood of the 'a' endpoint.
    // Ref.: -  O'Rourke,J.: Computational geometry in C;
    //          Cambridge University Press, 1994, pp. 37-38

    Polygon2dC ClipByConvex(const Polygon2dC &oth) const;
    //: Clips this polygon by another convex polygon
    //!param: oth - a convex clipping polygon
    //!return: the intersection of the two polygons
    // Note that this only works if the other polygon is convex.
    // Ref.: -  Foley. van Dam. Feiner. Hughes: Computer Graphics Principles and Practice
    //          Addison Wesley Publishing Company, 1996, pp. 123-129

    Polygon2dC ClipByLine(const LinePP2dC &line) const;
    //: Clips this polygon by the line
    //!param: line - a line
    //!return: the clipped polygon so that only the part on the right side of the
    //!return: line remains.

    Polygon2dC ClipByAxis(RealT threshold, UIntT axis, bool isGreater) const;
    //: Clips this polygon by the specified axis line through the given point
    //!param: threshold - the threshold for the specified axis
    //!param: axis - we will clip by point[axis]
    //!param: isGreater - determines which side of the axis is accepted
    //!return: the remains of the polygon after clipping
    
    Polygon2dC ClipByRange(const RealRange2dC &range) const;
    //: Clip polygon so it lies entirely within 'range'
    // If adjacent points on the polygon map to the same place, 
    // one of the points will be removed.
    
    bool Contains(const Point2dC & p) const;
    // Returns true iff the point 'p' is an internal point of this polygon.
    
    RealT Perimeter() const;
    //: Returns the perimeter length of this polygon.
    
    RealT Length() const
    { return Perimeter(); }
    //: Return the length of the curve.
    
    bool IsSelfIntersecting() const;
    //: Returns true if the polygon is self intersecting, ie do any sides cross
  
    RealT Overlap(const Polygon2dC &poly) const;
    //: Measure the fraction of the polygons overlapping as a fraction of the area of 'poly' 
    //!return: 0= Not overlapping 1=This polygon is completely covered by 'poly'.
    
    RealT CommonOverlap(const Polygon2dC &poly) const;
    //: Measure the fraction of the polygons overlapping as a fraction of the larger of the two polygons.
    //!return: 0= Not overlapping 1=If the two polygons are identical.
    
    Polygon2dC Approx(RealT distLimit) const;
    //: Generate an approximation to the given polygon within the given Euclidean distance limit.
    // The approximation is computed by finding the furthest point from the start, and then
    // the furthest point from that point. The two line segments are then approximated by searching for the
    // furthest point from the line defined by the two end points and if it is further than the distance limit
    // adding it to the approximation. The procedure is then repeated for each of the segments either side
    // of furthest point.
    
  };
  
  Polygon2dC ConvexHull(const SArray1dC<Point2dC>& points);
  //! author="Charles Galambos"
  //: Generate a convex hull from a set of points.
  
  Polygon2dC ConvexHull(DListC<Point2dC>& points);
  //! author="Charles Galambos"
  //: Generate a convex hull from a set of points
  // The list 'points' is destroyed.
  
  
}

#endif
