// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESH2D_HEADER
#define RAVL_HEMESH2D_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Geometry.2D"
//! lib=RavlMath
//! example=exHEMesh2d.cc
#include "Ravl/THEMesh.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RealRange2d.hh"
//! file="Ravl/Math/Geometry/Euclidean/2D/HEMesh2d.hh"

namespace RavlN {
  
  class TriMesh2dC;
  
  //! userlevel=Normal
  //: 2D Half edge mesh.
  
  class HEMesh2dC
    : public THEMeshC<Point2dC>
  {
  public:
    HEMesh2dC();
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit HEMesh2dC(bool);
    //: Constructor.
    // Creates an empty mesh.
    
    Point2dC Mean() const;
    //: Compute the mean position of points in the mesh.
    
    THEMeshFaceC<Point2dC> FindFace(const Point2dC &pnt);
    //: Find face containing point 'pnt'.
    // Returns an invalid handle if none found. <p>
    // This assumes the edges around a face are ordered clockwise.
    // Note: This does a linear searh through the faces, it doesn't
    // use any clever indexing/search scheme at the moment
    
    RealRange2dC BoundingRectangle() const;
    //: Compute the bounding rectangle for the points in the mesh.
    
    TriMesh2dC TriMesh() const;
    //: Convert this mesh to a tri mesh.
    // Note: This mesh must contain only triangular faces.
  };
    
}

#endif
