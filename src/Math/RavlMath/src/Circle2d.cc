// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////
//! date="25/2/1997"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Circle2d.cc"

#include "Ravl/Circle2d.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  // Fit a circle through 3 points.
  
  bool Circle2dC::Fit(const Point2dC &p0,const Point2dC &p1,const Point2dC &p2) {
    Vector2dC a1(p1[1] - p0[1],p0[0] - p1[0]);
    Vector2dC a2(p2[1] - p1[1],p1[0] - p2[0]);
    RealT d = a2[0]*a1[1] - a2[1]*a1[0];
    if(Abs(d) < 0.000001)
      return false;
    Vector2dC np1 = (p0 + p1) / 2;
    Vector2dC np2 = (p1 + p2) / 2;
    RealT m = (a1[0] * (np2[1] - np1[1]) - a1[1] *(np2[0] - np1[0]))/d;
    a2 *= m;
    centre = np2 + a2;
    radius = centre.EuclidDistance(p0);
    return true;
  }
}
