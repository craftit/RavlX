// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/PlanePVV3d.cc"

#include "Ravl/PlanePVV3d.hh"
#include "Ravl/LinePV3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/PlaneABCD3d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/FMatrix.hh"
#include "Ravl/CCMath.hh"

extern "C" {  double qrlsq(double *a,double *b,int m,int n,int *f) ; }

namespace RavlN {

  PlaneABCD3dC PlanePVV3dC::PlaneABCD3d() const
  { return PlaneABCD3dC(Normal(), Point()); }
  
  
  // Returns the point of intersection of this plane with the line 'l'.
  
  Point3dC PlanePVV3dC::Intersection(const LinePV3dC & l) const 
  { return PlaneABCD3d().Intersection(l); }
  
  // Find the closest point on the plane to 'p'
  
  Point3dC PlanePVV3dC::ClosestPoint(const Point3dC &p) const 
  { return PlaneABCD3d().ClosestPoint(p); }
  
  
  // Measure the euclidean distance to the plane.
  
  RealT PlanePVV3dC::EuclideanDistance(const Point3dC &point) const 
  { return PlaneABCD3d().EuclideanDistance(point); }
  
  //: Project a point into the coordinate system of the plane
  
  Point2dC PlanePVV3dC::Projection(const Point3dC &pointOnPlane) const {
    FMatrixC<3,2> a;
    a[0][0] = vector1[0];
    a[1][0] = vector1[1];
    a[2][0] = vector1[2];
    a[0][1] = vector2[0];
    a[1][1] = vector2[1];
    a[2][1] = vector2[2];
    Point3dC tmp = pointOnPlane;
    tmp -= Point();
    int f = 0;
    ::qrlsq(&(a[0][0]),&(tmp[0]),3,2,&f);
    return Point2dC(tmp[0],tmp[1]);
  }

  // Returns the coordinates (t1,t2) of the point projected onto
  // the plane. The coordinate system is determined by the point of
  // the plane and its two vectors.
  
  Point2dC PlanePVV3dC::ProjectedIntersection(const LinePV3dC & l) const 
  { return Projection(PlanePVV3dC::Intersection(l)); }
  
  ostream & operator<<(ostream & outS, const PlanePVV3dC & plane) {
    const Point3dC  & p  = plane.Point();
    const Vector3dC & v1 = plane.Vector1();
    const Vector3dC & v2 = plane.Vector2();
    outS << p << ' ' << v1 << ' ' << v2;
    return(outS);
  }
  
  istream & operator>>(istream & inS, PlanePVV3dC & plane) {
    Point3dC  & p  = plane.Point();
    Vector3dC & v1 = plane.Vector1();
    Vector3dC & v2 = plane.Vector2();
    inS >> p >> v1 >> v2;
    return(inS);
  }
  
}



