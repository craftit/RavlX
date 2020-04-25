// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/HEMeshBaseEdge.cc"

#include "Ravl/HEMeshBaseEdge.hh"
#include "Ravl/HEMeshBaseFace.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/Stream.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Destructor.
  
  HEMeshBaseEdgeBodyC::~HEMeshBaseEdgeBodyC()
  {}
  
  //: Correct edge's vertex pointer.
  
  bool HEMeshBaseEdgeBodyC::CorrectVertexEdgePtr() {
    if(vertex == 0)
      return false;
    // Make sure the vertex doesn't link to this edge.
    if(vertex->edge != this)
      return true;
    ONDEBUG(cerr << "HEMeshBaseEdgeBodyC::CorrectVertexEdgePtr(), Correcting pointer. \n");
    HEMeshBaseEdgeBodyC *newEdge = Next().pair;
    if(newEdge == this) {
      // Looks like we're the only edge connected to the vertex.
      vertex->edge = 0; // Well we tried.
      return false;      
    }
    if(newEdge != 0) {
      vertex->edge = newEdge;
      return true;
    }
    // Must be an open mesh, try the other way.
    if(pair == 0) {
      vertex->edge = 0; // Well we tried.
      return false;
    }
    RavlAssert(&(pair->Prev()) != this);
    vertex->edge = &pair->Prev();
    return true;
  }
  
  //: Remove this half edge.
  // The use must ensure that the matching half is also removed.
  
  void HEMeshBaseEdgeBodyC::RemoveHalfEdge() {
    CorrectVertexEdgePtr();
    // Make sure the vertex doesn't link to this edge.
    if(Prev() == Next().Next()) { // Do we need to collapse the face ?
      ONDEBUG(cerr << "HEMeshBaseEdgeBodyC::RemoveHalfEdge(), Collapsing face. Edge=" << ((void *) this) << " \n");
      // Yep, remove the face.
      HEMeshBaseEdgeBodyC *np1 = &Next().Pair();
      HEMeshBaseEdgeBodyC *np2 = &Prev().Pair();
      if(face->edge == this) // Check the face ptr is ok.
	face->edge = &(face->edge->Next());
      Unlink(); // We don't want to delete this edge just yet so take it out now.
      delete face; // Remove the old face.
      // Fill in gap.
      np1->SetPair(*np2);
      np2->SetPair(*np1);
    } else {
      ONDEBUG(cerr << "HEMeshBaseEdgeBodyC::RemoveHalfEdge(), Shrinking face. Edge=" << ((void *) this) << " \n");
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
  
  HEMeshBaseVertexC HEMeshBaseEdgeBodyC::CollapseEdge() {
    HEMeshBaseVertexBodyC *deadVertex = Prev().vertex;
    HEMeshBaseVertexC ret(*vertex);
    // Redirect all vertex pointers away from dead vertex.
    for(HEMeshBaseToVertexEdgeIterC it(*deadVertex);it;it++) 
      it->vertex = vertex;
    // Remove the half edges.
    HEMeshBaseEdgeBodyC *xpair = pair;
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
    
  
  //: Constructor.
  
  HEMeshBaseEdgeC::HEMeshBaseEdgeC(HEMeshBaseVertexC vert,HEMeshBaseFaceC face)
    : body(new HEMeshBaseEdgeBodyC(vert.Body(),face.Body()))
  {}

  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseEdgeC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseEdgeC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseEdgeC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseEdgeC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream input.
  // Not implemented

}
