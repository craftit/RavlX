// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/HEMeshVertex.cc"

#include "Ravl/3D/HEMeshVertex.hh"
#include "Ravl/3D/HEMeshEdge.hh"
#include "Ravl/3D/HEMeshFace.hh"

namespace Ravl3DN {
    
  //: Look for a connection from this vertex to oth.
  // Returns an invalid handle if ones is not found.
  
  HEMeshEdgeC HEMeshVertexBodyC::FindEdge(const HEMeshVertexC &oth) const {
    if(edge == 0) // None ?
      return HEMeshEdgeC();
    
#if 0
    HEMeshEdgeC at(edge);
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
    
    return HEMeshEdgeC();
  }

  //: Link this vertex to newVert on indicated face
  // Both vertexes must share the face. This will effectively
  // split the face in two.
  
  HEMeshEdgeC HEMeshVertexBodyC::Link(HEMeshVertexC vert,HEMeshFaceC face) {
    HEMeshEdgeC toVert = face.FindEdge(vert);
    RavlAssert(toVert.IsValid());
    HEMeshEdgeC toThis = face.FindEdge(vert);
    RavlAssert(toThis.IsValid());
    
    // Construct edges.
    
    HEMeshEdgeC newEdge1(vert,face);
    HEMeshEdgeC newEdge2(*this,face);
    newEdge1.SetPair(newEdge2);
    newEdge2.SetPair(newEdge1);
    newEdge1.SetFace(face);
    // Split boundry into two faces.
    
    newEdge1.LinkAfter(toThis);
    newEdge2.CutPaste(newEdge1.Next(),toVert.Next());
    
    face.SetEdge(newEdge1); // Make sure face edge ptrs are correct.
    
    HEMeshFaceC newFace(newEdge2);
    face.Body().LinkAft(newFace.Body()); // Put it in the face list.
    
    // Update all edges with new face.
    
    HEMeshEdgeC at = newEdge2;
    newEdge2.SetFace(newFace);
    for(at = at.Next();at != newEdge2;at = at.Next()) 
      at.SetFace(newFace);
    
    return newEdge1;
  }

  //: Get the number of edges/faces linking to this vertexs.
  // This assumes this is a closed mesh.
  
  UIntT HEMeshVertexBodyC::Valence() const {
    UIntT count = 0;
    for(HEMeshVertexEdgeIterC it(const_cast<HEMeshVertexBodyC &>(*this));it;it++)
      count++;
    return count;
  }
  
}
