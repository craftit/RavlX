// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DELAUNAYTRIANGULATION2D_HEADER
#define RAVL_DELAUNAYTRIANGULATION2D_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! userlevel=Normal
//! docentry="Ravl.API.Math.Geometry.2D"
//! file="Ravl/Math/Geometry/Euclidean/2D/DelaunayTriangulation2d.hh"

#include "Ravl/HEMesh2d.hh"

namespace RavlN {
  
  HEMesh2dC DelaunayTriangulation(const Array1dC<Point2dC> &points,Array1dC<IntT> &idMap);
  //: Create a delaunay triangulation of the given set of points.
  //!param: points - Array of points to triangulate.
  //!param: idMap - Array to store mapping from mesh vertices to an index into the original point array.
  //!return: Triangulated mesh.
  // The vertices in the created mesh are in same order as the
  // appear in 'points'.
  
  HEMesh2dC DelaunayTriangulation(const SArray1dC<Point2dC> &points);
  //: Create a delaunay triangulation of the given set of points.
  //!param: points - Array of points to triangulate.
  //!return: Triangulated mesh.
  // The vertices in the created mesh are in same order as the
  // appear in 'points'.
  
  HEMesh2dC DelaunayTriangulation(const Array1dC<Point2dC> &points);
  //: Create a delaunay triangulation of the given set of points.
  //!param: points - Array of points to triangulate.
  //!return: Triangulated mesh.
  // The vertices in the created mesh are in same order as the
  // appear in 'points'.
  
  bool IsDelaunayTriangulation(const HEMesh2dC &mesh);
  //: Test if mesh is a delaunay triangulation. 
}

#endif
