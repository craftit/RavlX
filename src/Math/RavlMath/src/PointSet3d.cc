// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/PointSet3d.cc"

#include "Ravl/PointSet3d.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {

  Point3dC PointSet3dC::Centroid() const {
    // Create return value
    Point3dC centroid(0.0,0.0,0.0);
    // Sum
    IntT size = 0;
    for (DLIterC<Point3dC> point(*this); point; point++) {
      centroid += point.Data();
      size++;
    }
    // Divide
    centroid /= (RealT) size;
    // Done
    return centroid;
  }
  
  //: Compute the bounding rectangle for the point set.
  
  RealRange3dC PointSet3dC::BoundingRectangle() const {
    RealRange3dC ret(0,0,0);
    DLIterC<Point3dC> point(*this);
    if(!point) 
      return ret; // No points in set.
    ret = RealRange3dC(*point,0);
    for (; point; point++)
      ret.Involve(*point);
    return ret;
  }
  
  //: Translate point set by vector.
  
  const PointSet3dC &PointSet3dC::operator+=(const TFVectorC<RealT,3> &offset) {
    for(DLIterC<Point3dC> it(*this);it;it++)
      *it += offset;
    return *this;
  }
  
  //: Translate point set by subracting a vector.
  
  const PointSet3dC &PointSet3dC::operator-=(const TFVectorC<RealT,3> &offset) {
    for(DLIterC<Point3dC> it(*this);it;it++)
      *it -= offset;
    return *this;
  }
  
  //: Scale the point set by multiplying the points by 'scale'.
  
  const PointSet3dC &PointSet3dC::operator*=(RealT scale) {
    for(DLIterC<Point3dC> it(*this);it;it++)
      *it *= scale;
    return *this;
  }
  
}
