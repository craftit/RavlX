// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PLANE3ABCD_HEADER 
#define RAVL_PLANE3ABCD_HEADER
/////////////////////////////////////////////////////////////////////////
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Geometry.3D"
//! rcsid="$Id$"
//! date="06/08/1995"
//! file="Ravl/Math/Geometry/Euclidean/3D/PlaneABCD3d.hh"

#include "Ravl/Types.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Point3d.hh"

namespace RavlN {
  template<class DataT> class SArray1dC;
  class MeanCovariance3dC;
  class Point3dC;
  class LinePV3dC;
  class Point2dC;
  
  //: Plane in 3D space - equation Ax+By+Cz+D = 0
  
  class PlaneABCD3dC
  {
  public:
    PlaneABCD3dC()
      : normal(0.0, 0.0, 0.0), 
	d(0.0)
    {}
    //: The non-existing plane (0,0,0,0).
    
    PlaneABCD3dC(RealT aa, RealT bb, RealT cc, RealT dd)
      : normal(aa, bb, cc), 
	d(dd)
    {}
    //: The plane determined by the equation aa*x+bb*y+cc*z+dd = 0.
    
    PlaneABCD3dC(const Vector3dC & n, RealT p)
      : normal(n), 
	d(-p)
    {}
    //: Creates the plane determined by the vector equation n*x = p.
    
    PlaneABCD3dC(const Vector3dC & n, const Point3dC & p)
      : normal(n), 
	d(-(n.Dot(p)))
    {}
    //: Creates the plane with normal 'n' passing through the point 'p'.
    
    inline PlaneABCD3dC(const Point3dC  & p,
			const Vector3dC & v1,
			const Vector3dC & v2)
      : normal(v1.Cross(v2))
    { d = - normal.Dot(p); }
    //: The plane [p; v1; v2].
    
    inline PlaneABCD3dC(const Point3dC  & p1,
			const Point3dC  & p2,
			const Point3dC  & p3)
      : normal(Vector3dC(p2 - p1).Cross(Vector3dC(p3 - p1)))
    { d = - normal.Dot(p1); }
    //: The plane passing through three points 'p1', 'p2', and 'p3'.
    
    inline Vector3dC Normal() const
    { return(normal); }
    //: Returns the normal of the plane.
    
    inline RealT A() const
    { return(normal[0]); }
    //: Returns parameter a.
    
    inline RealT B() const
    { return(normal[1]); }
    //: Returns parameter b.
    
    inline RealT C() const
    { return(normal[2]); }
    //: returns parameter c.
    
    inline RealT D() const
    { return(d); }
    //: Returns parameter d.
    
    inline PlaneABCD3dC & UnitNormal() {
      RealT size = normal.Magnitude();
      normal /= size;
      d      /= size;
      return(*this);
    }
    //: Normalizes the normal vector to be unit.
    
    inline RealT Value(const Point3dC & p) const
    { return normal.Dot(p) + d; }
    //: Returns the value of the expression 'normal.Dot(p) + d'
    // This is often used in analytical geometry.
    
    Point3dC ClosestPoint(const Point3dC & p) const 
    { return p + normal * (-(normal.Dot(p) + d)/normal.SumOfSqr()); }
    //: Find closest point to 'p' that lies on the plane.
    
    inline RealT DirectedEuclideanDistance(const Point3dC & p) const
    { return Value(p)/normal.Magnitude(); }
    //: Returns the directed Euclidian distance of the point 'p' from
    //: this plane. 
    // If returned value is positive the point is in the direction of the
    // plane normal.
    
    inline RealT DirEuclidDistance(const Point3dC & p) const
    { return DirectedEuclideanDistance(p); }
    //: Returns the directed Euclidian distance of the point 'p' from
    //: this plane. 
    // If returned value is positive the point is in the direction of the
    // plane normal.
    // Obsolete, use DirectedEuclideanDistance()
    
    inline RealT EuclideanDistance(const Point3dC & p) const
    { return RavlN::Abs(DirEuclidDistance(p)); }
    //: Returns the Euclidean distance of the point 'p' from this plane.
    
    inline RealT EuclidDistance(const Point3dC & p) const
    { return EuclideanDistance(p); }
    //: Returns the Euclidean distance of the point 'p' from this plane.
    
    inline PlaneABCD3dC ParallelPlane(const Point3dC & p) const
    { return PlaneABCD3dC(Normal(), p); }
    //: Returns the plane parallel to this plane and passing through
    //: the point 'p'.
    
    Point3dC Intersection(const LinePV3dC & l) const;
    //: Returns the point which is the intersection of this plane with
    //: the line 'l'. 
    // If the intersection does not exist the function throw an ExceptionNumericalC
    
    Point3dC Intersection(const PlaneABCD3dC & planeB,
			  const PlaneABCD3dC & planeC) const;
    //: Returns the point which is the intersection of three planes.
    // If the intersection does not exist the function throw an ExceptionNumericalC
    
    LinePV3dC Intersection(const PlaneABCD3dC & plane) const;
    //: Returns the line which is the intersection of this plane with
    // the plane 'plane'. 
    // If the intersection does not exist the function throw an ExceptionNumericalC
    
  private:
    Vector3dC normal;
    RealT     d;
    
    friend istream & operator>>(istream & inS, PlaneABCD3dC & plane);
  };
  
  //: Least squares fit of a plane to a set of points in 3d
  // At least 3 points are needed.
  bool FitPlane(const SArray1dC<Point3dC> &points,PlaneABCD3dC &plane);
  
  ostream & operator<<(ostream & outS, const PlaneABCD3dC & plane);
  istream & operator>>(istream & inS, PlaneABCD3dC & plane);
  
}

#endif


