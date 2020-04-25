// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/HEMesh2d.cc"

#include "Ravl/HEMesh2d.hh"
#include "Ravl/THEMeshVertexIter.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/THEMeshFace.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/TriMesh2d.hh"
#include "Ravl/Hash.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  HEMesh2dC::HEMesh2dC()
  {}
  
  //: Constructor.
  // Creates an empty mesh.
  
  HEMesh2dC::HEMesh2dC(bool)
    : THEMeshC<Point2dC>(true)
  {}
  
  //: Compute the mean position of points in the mesh.
  
  Point2dC HEMesh2dC::Mean() const {
    Point2dC ret(0,0);
    UIntT count = 0;
    for(THEMeshVertexIterC<Point2dC> it(Vertices());it;it++) {
      ret += (*it).Data();
      count++;
    }
    return ret / (RealT) count;
  }
  
  //: Find face containing point 'pnt'.
  // Returns an invalid handle if none found.
  
  // TODO :- Implement a faster method for finding a face.
  
  THEMeshFaceC<Point2dC> HEMesh2dC::FindFace(const Point2dC &pnt) {
    ONDEBUG(cerr << "HEMesh2dC::FindFace(), Point= " << pnt << "\n");
    for(THEMeshFaceIterC<Point2dC> it(Faces());it;it++) {
      RavlAssert((*it).IsValid());
      THEMeshFaceEdgeIterC<Point2dC> eit(*it);
      if(!eit) continue; // Faces has no edges!
      Point2dC last = eit->Vertex().Data();
      Point2dC first = last;
      bool found = true;
      for(eit++;eit;eit++) {
	Point2dC x = eit->Vertex().Data();
	if(!LinePP2dC(last,x).IsPointToRight(pnt)) {
	  found = false;
	  break;
	}
	last = x;
      }
      if(found && LinePP2dC(last,first).IsPointToRight(pnt))
	return *it; // Found it !
      // Keep trying....
    }
    ONDEBUG(cerr << "HEMesh2dC::FindFace(), No face found.\n");
    return THEMeshFaceC<Point2dC>();
  }
  
  //: Compute the bounding rectangle for the points in the mesh.
  
  RealRange2dC HEMesh2dC::BoundingRectangle() const {
    RealRange2dC ret(0,0);
    THEMeshVertexIterC<Point2dC> it(Vertices());
    if(!it)
      return ret;
    ret = RealRange2dC((*it).Data(),0);
    for(;it;it++)
      ret.Involve((*it).Data());
    return ret;
  }

  //: Convert this mesh to a tri mesh.
  // Note: This mesh must contain only triangular faces.
  
  TriMesh2dC HEMesh2dC::TriMesh() const {
    SArray1dC<Point2dC> vertices(NoVertices());
    SArray1dC<Index3dC> faces(NoFaces());
    
    // Extract points.
    
    HashC<HEMeshBaseVertexC,IntT> vertexIndex;
    SArray1dIterC<Point2dC> pit(vertices);
    for(THEMeshVertexIterC<Point2dC> vit(Vertices());vit;vit++,pit++) {
      *pit = (*vit).Data();
      vertexIndex[(*vit)] = pit.Index().V();
    }
    
    // Extract faces.
    SArray1dIterC<Index3dC> nfit(faces);
    for(THEMeshFaceIterC<Point2dC> fit(Faces());fit;fit++,nfit++) {
      THEMeshFaceEdgeIterC<Point2dC> feit(*fit);
      RavlAssert(feit);
      (*nfit)[0] = vertexIndex[(*feit).Vertex()]; feit++;
      RavlAssert(feit);
      (*nfit)[1] = vertexIndex[(*feit).Vertex()]; feit++;
      RavlAssert(feit);
      (*nfit)[2] = vertexIndex[(*feit).Vertex()]; 
#ifndef NDEBUG
      // Check its only got 3 edges to the face.
      feit++;
      RavlAssert(!feit);
#endif
    }
      
    return TriMesh2dC(vertices,faces);
  }

  
}
