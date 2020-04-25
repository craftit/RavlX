// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PLAN3PVV_HH 
#define RAVL_PLAN3PVV_HH
////////////////////////////////////////////////////////////////////////////
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26/02/1994"
//! docentry="Ravl.API.Math.Geometry.3D"
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/3D/PlanePVV3d.hh"

#include "Ravl/Types.hh" //RealT
#include "Ravl/Point3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {

  class Point2dC;
  class LinePV3dC;
  class PlaneABCD3dC;
  
  //: Plane in 3D space
  // The PlanePVV3dC class represents the plane in 3 dimensional Euclidian
  // space. The plane is represented by one point and 2 vectors.
  
  class PlanePVV3dC
    : private Point3dC
  {
  public:
    
    inline PlanePVV3dC()
      : Point3dC(0,0,0),
	vector1(0,0,0),
	vector2(0,0,0)
    {}
    // Creates the plane P:(0,0,0),V1:[0,0,0],V2:[0,0,0].
    
    inline PlanePVV3dC(const PlanePVV3dC & plane)    
      : Point3dC(plane),
	vector1(plane.vector1),
	vector2(plane.vector2)
    {}
    
    // Copy constructor.
    
    inline PlanePVV3dC(const Point3dC  & p1,
		       const Point3dC  & p2,
		       const Point3dC  & p3)    
      : Point3dC(p1),
	vector1(p2-p1),
	vector2(p3-p1)
    {}
    
    // Creates the plane determined by three points 'p1', 'p2', and 'p3'.
    // The first vector is equal to p2-p1, the second one to p3-p1.
    
    inline PlanePVV3dC(const Point3dC  & p,
		       const Vector3dC & v1,
		       const Vector3dC & v2)
      : Point3dC(p),
	vector1(v1),
	vector2(v2)
    {}
    // Creates the plane [p; v1; v2].
    
    //:-=============================================-
    //: Access to the plane elements and conversions.
    
    inline const Point3dC & Point() const
    { return *this; }
    // Access to the point of the constant object.
    
    inline const Vector3dC & Vector1() const
    { return vector1; }
    // Access to the first vector of the constant object.
    
    inline const Vector3dC & Vector2() const
    { return vector2; }
    // Access to the second vector of the constant object.
    
    inline Point3dC & Point()
    { return *this; }
    // Access to the point.
    
    inline Vector3dC & Vector1()
    { return vector1; }
    // Access to the first vector.
    
    inline Vector3dC & Vector2()
    { return vector2; }
    // Access to the second vector.
    
    inline Vector3dC Normal() const
    { return vector1.Cross(vector2); }
    // Returns the normal of the plane.
    
    PlaneABCD3dC PlaneABCD3d() const;
    // Converts this plane representation.
    
    inline PlanePVV3dC & UnitVectors() {
      vector1.Unit();
      vector2.Unit();
      return *this;
    }  
    // Normalizes the vectors to be unit.
    
    //:-=============================================-
    //: Geometrical constructions.
    
    RealT EuclideanDistance(const Point3dC &point) const;
    // Measure the euclidean distance between 'point' and the plane.
    
    Point3dC ClosestPoint(const Point3dC &p) const;
    // Find the closest point on the plane to 'p'
    
    Point3dC Intersection(const LinePV3dC & l) const;
    // Returns the point of intersection of this plane with the line 'l'.
    
    Point2dC Projection(const Point3dC & point) const;
    //: Returns the coordinates (t1,t2) of the point projected onto
    //: the plane. 
    // The coordinate system is determined by the point of
    // the plane and its two vectors.
    
    Point2dC ProjectedIntersection(const LinePV3dC & l) const;
    // Returns the coordinates (t1,t2) of the point of intersection
    // of this plane with the line 'l'. The coordinate system of the returned
    // point is determined by the point of the plane and its two vectors.
    
    inline Point3dC Point(const RealT t1, const RealT t2) const
    { return Point() + vector1 * t1 + vector2 * t2; }
    // Returns the point of the plane: point + t1 * vector1 + t2 * vector2.
    
    inline Point3dC Point(const Point2dC &par) const
    { return Point() + vector1 * par[0] + vector2 * par[1]; }
    // Returns the point of the plane: point + t1 * vector1 + t2 * vector2.
    
  private:
    
    //:-======================-
    //: Object representation.
    Vector3dC vector1;
    Vector3dC vector2;
    
    friend istream & operator>>(istream & inS, PlanePVV3dC & plane);
  };
  
  ostream & operator<<(ostream & outS, const PlanePVV3dC & plane);
  istream & operator>>(istream & inS, PlanePVV3dC & plane);

  //: Least squares fit of a plane to a set of points in 3d
  // At least 3 points are needed.
  bool FitPlane(const SArray1dC<Point3dC> &points,PlanePVV3dC &plane);
  
  
}

#endif


