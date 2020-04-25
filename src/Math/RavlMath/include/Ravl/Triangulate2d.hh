// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRIANGULATE2D_HEADER
#define RAVL_TRIANGULATE2D_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos, based on code by John W. Ratcliff"
//! date="22/2/2008"
//! userlevel=Normal
//! docentry="Ravl.API.Math.Geometry.2D"
//! lib=RavlMath

#include "Ravl/Polygon2d.hh"
#include "Ravl/Index3d.hh"

namespace RavlN {
  
  RealT PolygonArea(const SArray1dC<Point2dC> &polygon);
  //: Compute area of polygon defined by an array of points.
  
  bool PolygonTriangulation(const SArray1dC<Point2dC> &polygon,SArray1dC<Index3dC> &result);
  //: Triangulate a polygon given by the set of points in 'polygon'
  //: Returned are sets of 3 indexs into the original array.
  
  
}

#endif
