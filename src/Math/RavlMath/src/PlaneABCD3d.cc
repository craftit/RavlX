// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/PlaneABCD3d.cc"

#include "Ravl/Point3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/LinePV3d.hh"
#include "Ravl/PlaneABCD3d.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  
  Point3dC PlaneABCD3dC::Intersection(const LinePV3dC & l) const {
    RealT nu = normal.Dot(l.Vector());
    if (IsAlmostZero(nu)) 
      throw ExceptionNumericalC("PlaneABCD3dC::Intersection(): the line is almost parallel to the plane:\n");
    return l.PointT(-Value(l.FirstPoint())/nu);
  }
  
  Point3dC PlaneABCD3dC::Intersection(const PlaneABCD3dC & planeB,
				      const PlaneABCD3dC & planeC) const
  {
    Vector3dC n1xn2(Normal().Cross(planeB.Normal()));
    RealT     tripleSP = n1xn2.Dot(planeC.Normal());
    if (IsAlmostZero(tripleSP))
      throw ExceptionNumericalC("PlaneABCD3dC::Intersection(): the planes are almost parallel");
    Vector3dC n2xn3(planeB.Normal().Cross(planeC.Normal()));
    Vector3dC n3xn1(planeC.Normal().Cross(Normal()));
    return Point3dC(n2xn3 * D() + n3xn1 * planeB.D() + n1xn2 * planeC.D())
      /(-tripleSP);
  }
  
  LinePV3dC PlaneABCD3dC::Intersection(const PlaneABCD3dC & plane) const {
    Vector3dC direction(Normal().Cross(plane.Normal()));
    RealT     den = direction.SumOfSqr();
    if (IsAlmostZero(den))
      throw ExceptionNumericalC("PlaneABCD3dC::Intersection(): the planes are almost parallel");
    Vector3dC n212(plane.Normal().Cross(direction));
    Vector3dC n121(direction.Cross(Normal()));
    return LinePV3dC((n212 * D() + n121 * plane.D())/(-den), direction); 
  }
  
  ostream & operator<<(ostream & outS, const PlaneABCD3dC & plane) {
    outS << plane.Normal() << ' ' << plane.D();
    return(outS);
  }
  
  istream & operator>>(istream & inS, PlaneABCD3dC & plane) {
    inS >> plane.normal >> plane.d;
    return(inS);
  }

}

