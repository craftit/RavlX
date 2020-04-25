// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/HEMeshEdge.cc"

#include "Ravl/3D/HEMeshEdge.hh"
#include "Ravl/3D/HEMeshFace.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {
  
  //: Correct edge's vertex pointer.
  
  void HEMeshEdgeBodyC::CorrectVertexEdgePtr() {
    if(vertex == 0)
      return ;
    // Make sure the vertex doesn't link to this edge.
    if(vertex->edge != this)
      return ;
    HEMeshEdgeBodyC *newEdge = Next().pair;
    if(newEdge == this) {
      // Looks like we're the only edge connected to the vertex.
      vertex->edge = 0; // Well we tried.
      return ;      
    }
    if(newEdge != 0) {
      vertex->edge = newEdge;
      return ;
    }
    // Must be an open mesh, try the other way.
    if(pair == 0) {
      vertex->edge = 0; // Well we tried.
      return ;
    }
    vertex->edge = &pair->Prev();
  }
  
  //: Remove this half edge.
  // The use must ensure that the matching half is also removed.
  
  void HEMeshEdgeBodyC::RemoveHalfEdge() {
    CorrectVertexEdgePtr();
    // Make sure the vertex doesn't link to this edge.
    if(Prev() == Next().Next()) { // Do we need to collapse the face ?
      ONDEBUG(cerr << "HEMeshEdgeBodyC::RemoveHalfEdge(), Collapsing face. Edge=" << ((void *) this) << " \n");
      // Yep, remove the face.
      HEMeshEdgeBodyC *np1 = &Next().Pair();
      HEMeshEdgeBodyC *np2 = &Prev().Pair();
      if(face->edge == this) // Check the face ptr is ok.
	face->edge = &(face->edge->Next());
      Unlink(); // We don't want to delete this edge just yet so take it out now.
      delete face; // Remove the old face.
      // Fill in gap.
      np1->SetPair(*np2);
      np2->SetPair(*np1);
    } else {
      ONDEBUG(err << "HEMeshEdgeBodyC::RemoveHalfEdge(), Shrinking face. Edge=" << ((void *) this) << " \n");
      // Just take a side out of the face.
      if(face->Edge() == this)
	face->edge = &Prev();
      Unlink();
    }
    SetSelfPointing(); // To avoid problems in destructor.
    // Clean up pointers, for debugging mainly.
    pair = 0;
    vertex = 0;
    face = 0;
  }


  //: Collapse edge to zero length.
  // This deletes the edge from the mesh and merges the vertex's
  // at either end.
  
  HEMeshVertexC HEMeshEdgeBodyC::CollapseEdge() {
    HEMeshVertexBodyC *deadVertex = Prev().vertex;
    HEMeshVertexC ret(*vertex);
    // Redirect all vertex pointers away from dead vertex.
    for(HEMeshToVertexEdgeIterC it(*deadVertex);it;it++) 
      it->vertex = vertex;
    // Remove the half edges.
    HEMeshEdgeBodyC *xpair = pair;
    RemoveHalfEdge();
    if(xpair != 0) {
      xpair->RemoveHalfEdge();
      delete xpair;
      pair = 0;
    }
    // Dispose of old vertex.
    delete deadVertex;
    return ret;
  }
  
  //: Open an edge, merging the faces on either side.
  // This deletes the edge from the mesh and returns
  // a handle to the new face.
  
  HEMeshFaceC HEMeshEdgeBodyC::OpenEdge() {
    RavlAssertMsg(0,"HEMeshEdgeBodyC::OpenEdge(), Not implemented. ");
#if 0
#endif
    return HEMeshFaceC();
  }
  
  //: Constructor.
  
  HEMeshEdgeC::HEMeshEdgeC(HEMeshVertexC vert,HEMeshFaceC face)
    : body(new HEMeshEdgeBodyC(vert.Body(),face.Body()))
  {}

}
