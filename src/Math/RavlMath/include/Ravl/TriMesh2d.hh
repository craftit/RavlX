// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRIMESH2D_HEADER
#define RAVL_TRIMESH2D_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Geometry.2D"
//! lib=RavlMath
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/2D/TriMesh2d.hh"

#include "Ravl/Point2d.hh"
#include "Ravl/Index3d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/RealRange2d.hh"

namespace RavlN {

  //! userlevel=Normal
  //: 2D Triangular Mesh
  
  class TriMesh2dC
  {
  public:
    TriMesh2dC()
    {}
    //: Default constructor.
    // Creates an empty mesh.
    
    TriMesh2dC(const SArray1dC<Point2dC> &nVertices,const SArray1dC<Index3dC> &nFaces)
      : vertices(nVertices),
	faces(nFaces)
    {}
    //: Constructor
    
    SArray1dC<Point2dC> &Vertices()
    { return vertices; }
    //: Access array of vertices.
    
    SArray1dC<Index3dC> &Faces()
    { return faces; }
    //: Access array of faces.

    const SArray1dC<Point2dC> &Vertices() const
    { return vertices; }
    //: Access array of vertices.
    
    const SArray1dC<Index3dC> &Faces() const
    { return faces; }
    //: Access array of faces.
    
    SizeT NoFaces() const
    { return faces.Size(); }
    //: Find number of faces in the mesh.
    
    SizeT NoVertices() const
    { return vertices.Size(); }
    //: Find number of vertices in mesh
    
    IntT FindFace(const Point2dC &pnt);
    //: Find face containing point 'pnt'.
    // This assumes the edges around a face are ordered clockwise.
    // Note: This does a linear searh through the faces, it doesn't
    // use any clever indexing/search scheme at the moment <p>
    // Returns -1 if no face is found.
    
    RealRange2dC BoundingRectangle() const;
    //: Compute the bounding rectangle for the points in the mesh.
    
    bool IsFlat() const;
    //: Check that all faces in the mesh are oriented in the same direction.
    
  protected:
    SArray1dC<Point2dC> vertices;
    SArray1dC<Index3dC> faces;
  };
  
  ostream & operator<<(ostream & outS, const TriMesh2dC & mesh);
  //: Write to stream.
  
  istream & operator>>(istream & inS,TriMesh2dC & mesh);
  //: Read from stream.
  
  BinOStreamC & operator<<(BinOStreamC & outS, const TriMesh2dC & mesh);
  //: Write to binary stream.
  
  BinIStreamC & operator>>(BinIStreamC & inS,TriMesh2dC & mesh);
  //: Read from binary stream.
  
}

#endif
