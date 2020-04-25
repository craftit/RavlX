// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/2D/AffineTransformPolygon2d.cc"

#include "Ravl/Affine2d.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  
  //: Apply a affine transform to a point set
  
  PointSet2dC operator*(const FAffineC<2> &trans,const PointSet2dC &points) {
    PointSet2dC ret;
    for(DLIterC<Point2dC> it(points);it;it++)
      ret.InsLast(trans * (*it));
    return ret;
  }
  
  //: Apply a affine transform to a polygon
  
  Polygon2dC operator*(const FAffineC<2> &trans,const Polygon2dC &points) 
  { return Polygon2dC(trans * static_cast<const PointSet2dC &>(points)); }
  
}
