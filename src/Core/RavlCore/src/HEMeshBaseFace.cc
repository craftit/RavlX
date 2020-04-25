// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/HEMeshBaseFace.cc"

#include "Ravl/HEMeshBaseFace.hh"
#include "Ravl/Stream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Destructor.
  
  HEMeshBaseFaceBodyC::~HEMeshBaseFaceBodyC() {
    ONDEBUG(cerr << "HEMeshBaseFaceBodyC::~HEMeshBaseFaceBodyC(). \n");
    if(edge != 0) {
      // Remove all edges around the face.
      
      while(1) {
	HEMeshBaseEdgeBodyC *eb = &(edge->Next());
#if 0
	if(!eb->CorrectVertexEdgePtr())
	  std::cerr << "WARNING: Failed to correct vertex pointer. \n";
#else
	if(eb->VertexPtr()->edge == eb)
	  eb->vertex->edge = 0;
#endif
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
  
  void HEMeshBaseFaceBodyC::DestroyFace() {
    ONDEBUG(cerr << "HEMeshBaseFaceBodyC::DestroyFace(). \n");
    if(edge == 0)
      return;
    // Remove all edges around the face.
    while(1) {
      HEMeshBaseEdgeBodyC *eb = &(edge->Next());
      delete eb;
      if(eb == edge) // Are we finished ?
	break;
    }
    edge = 0;
  }

  //: Get number of edges on face.
  
  UIntT HEMeshBaseFaceBodyC::Sides() const {
    if(edge == 0)
      return 0;
    UIntT count = 0;
    for(HEMeshBaseFaceEdgeIterC it(const_cast<HEMeshBaseFaceBodyC &>(*this));it;it++)
      count++;
    return count;
  }

  //: Check that the vertex is connected to this face.
  
  bool HEMeshBaseFaceBodyC::HasVertex(HEMeshBaseVertexC vert) const {
    for(HEMeshBaseFaceEdgeIterC it(const_cast<HEMeshBaseFaceBodyC &>(*this));it;it++)
      if(it.Data().Vertex() == vert) 
	return true;
    return false;
  }

  //: Find the edge linking to vertex a.
  // If edge is not found an invalid handle is returned.
  
  HEMeshBaseEdgeC HEMeshBaseFaceBodyC::FindEdge(HEMeshBaseVertexC a) {
    for(HEMeshBaseFaceEdgeIterC it(const_cast<HEMeshBaseFaceBodyC &>(*this));it;it++)
      if(it.Data().Vertex() == a) 
	return it.Data();
    return HEMeshBaseEdgeC();
  }

  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseFaceC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseFaceC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseFaceC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseFaceC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream input.
  // Not implemented
  
  
}
