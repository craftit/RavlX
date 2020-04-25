// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/HEMeshBaseVertex.cc"

#include "Ravl/HEMeshBaseVertex.hh"
#include "Ravl/HEMeshBaseEdge.hh"
#include "Ravl/HEMeshBaseFace.hh"

namespace RavlN {

  //: Destructor.
  
  HEMeshBaseVertexBodyC::~HEMeshBaseVertexBodyC()
  {}
  
  //: Look for a connection from this vertex to oth.
  // Returns an invalid handle if ones is not found.
  
  HEMeshBaseEdgeC HEMeshBaseVertexBodyC::FindEdge(const HEMeshBaseVertexC &oth) const {
    if(edge == 0) // None ?
      return HEMeshBaseEdgeC();
    
#if 0
    HEMeshBaseEdgeC at(edge);
    while(at) {
      if(at.Vertex() == oth)
	return at;
      if(!at.HasPair())
	break;
      at = at.Pair().Next();
    }
#else
    RavlAssert(0);
#endif
    
    return HEMeshBaseEdgeC();
  }

  //: Link this vertex to newVert on indicated face
  // Both vertexes must share the face. This will effectively
  // split the face in two.
  
  HEMeshBaseEdgeC HEMeshBaseVertexBodyC::Link(HEMeshBaseVertexC vert,HEMeshBaseFaceC face) {
    HEMeshBaseEdgeC toVert = face.FindEdge(vert);
    RavlAssert(toVert.IsValid());
    HEMeshBaseEdgeC toThis = face.FindEdge(vert);
    RavlAssert(toThis.IsValid());
    
    // Construct edges.
    
    HEMeshBaseEdgeC newEdge1(vert,face);
    HEMeshBaseEdgeC newEdge2(*this,face);
    newEdge1.SetPair(newEdge2);
    newEdge2.SetPair(newEdge1);
    newEdge1.SetFace(face);
    // Split boundry into two faces.
    
    newEdge1.LinkAfter(toThis);
    newEdge2.CutPaste(newEdge1.Next(),toVert.Next());
    
    face.SetEdge(newEdge1); // Make sure face edge ptrs are correct.
    
    HEMeshBaseFaceC newFace(newEdge2);
    face.Body().LinkAft(newFace.Body()); // Put it in the face list.
    
    // Update all edges with new face.
    
    HEMeshBaseEdgeC at = newEdge2;
    newEdge2.SetFace(newFace);
    for(at = at.Next();at != newEdge2;at = at.Next()) 
      at.SetFace(newFace);
    
    return newEdge1;
  }

  //: Get the number of edges/faces linking to this vertexs.
  // This assumes this is a closed mesh.
  
  UIntT HEMeshBaseVertexBodyC::Valence() const {
    UIntT count = 0;
    for(HEMeshBaseVertexEdgeIterC it(const_cast<HEMeshBaseVertexBodyC &>(*this));it;it++)
      count++;
    return count;
  }

  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseVertexC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseVertexC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseVertexC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseVertexC &vertex) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Binary stream input.
  // Not implemented
  
}
