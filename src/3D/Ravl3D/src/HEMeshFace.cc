// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/HEMeshFace.cc"

#include "Ravl/3D/HEMeshFace.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  //: Destructor.
  
  HEMeshFaceBodyC::~HEMeshFaceBodyC() {
    ONDEBUG(cerr << "HEMeshFaceBodyC::~HEMeshFaceBodyC(). \n");
    if(edge != 0) {
      // Remove all edges around the face.
      while(1) {
	HEMeshEdgeBodyC *eb = &(edge->Next());
	eb->CorrectVertexEdgePtr();
	if(eb->HasPair())
	  eb->Pair().pair = 0; // Clear pair pointers.
	delete eb;
	if(eb == edge) // Are we finished ?
	  break;
      }
      edge = 0;
    }
  }
  
  //: Destroy the face without worrying about mesh consistancy.
  
  void HEMeshFaceBodyC::DestroyFace() {
    ONDEBUG(cerr << "HEMeshFaceBodyC::DestroyFace(). \n");
    if(edge == 0)
      return;
    // Remove all edges around the face.
    while(1) {
      HEMeshEdgeBodyC *eb = &(edge->Next());
      delete eb;
      if(eb == edge) // Are we finished ?
	break;
    }
    edge = 0;
  }

  //: Get number of edges on face.
  
  UIntT HEMeshFaceBodyC::Sides() const {
    if(edge == 0)
      return 0;
    UIntT count = 0;
    for(HEMeshFaceEdgeIterC it(const_cast<HEMeshFaceBodyC &>(*this));it;it++)
      count++;
    return count;
  }

  //: Check that the vertex is connected to this face.
  
  bool HEMeshFaceBodyC::HasVertex(HEMeshVertexC vert) const {
    for(HEMeshFaceEdgeIterC it(const_cast<HEMeshFaceBodyC &>(*this));it;it++)
      if(it.Data().Vertex() == vert) 
	return true;
    return false;
  }

  //: Find the edge linking to vertex a.
  // If edge is not found an invalid handle is returned.
  
  HEMeshEdgeC HEMeshFaceBodyC::FindEdge(HEMeshVertexC a) {
    for(HEMeshFaceEdgeIterC it(const_cast<HEMeshFaceBodyC &>(*this));it;it++)
      if(it.Data().Vertex() == a) 
	return it.Data();
    return HEMeshEdgeC();
  }
  
  
}
