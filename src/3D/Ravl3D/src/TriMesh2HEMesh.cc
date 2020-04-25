// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/TriMesh2HEMesh.cc"

#include "Ravl/3D/HEMesh.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Hash.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {
  
  //: Construct from a TriMesh
  
  HEMeshBodyC::HEMeshBodyC(const TriMeshC &mesh) {
    SArray1dC<HEMeshVertexC> verts(mesh.Vertices().Size());
    // Creat all the vertices
    for(SArray1dIter2C<HEMeshVertexC,VertexC> it(verts,mesh.Vertices());it;it++)
      it.Data1() = InsertVertex(it.Data2());
    // Creat the faces.
    HashC<Tuple2C<HEMeshVertexC,HEMeshVertexC> , HEMeshEdgeC> edgeTab;
    for(SArray1dIterC<TriC> itf(mesh.Faces());itf;itf++) {
      SArray1dC<HEMeshVertexC> face(3);
      for(int i = 0;i < 3;i++)
	face[i] = verts[mesh.Index(*itf,i)];
      InsertFace(face,edgeTab);
    }
    ONDEBUG(cerr << "EdgeTab Size=" << edgeTab.Size() << "\n");
  }

  //: Build a TriMesh from this mesh.
  
  TriMeshC HEMeshBodyC::TriMesh() const {
    UIntT noFaces = NoFaces();
    UIntT noVertices = NoVertices();
    SArray1dC<VertexC> verts(noVertices);
    // Copy vertices.
    HashC<HEMeshVertexC,UIntT> vindex;
    SArray1dIterC<VertexC> vit(verts);
    for(IntrDLIterC<HEMeshVertexBodyC> it(vertices);it;it++,vit++) {
      vindex[HEMeshVertexC(*it)] = vit.Index().V();
      ONDEBUG(cerr << "Hash=" << HEMeshVertexC(*it).Hash() << " " << HEMeshVertexC(*it).Hash() <<" Ind=" << vit.Index().V() << "\n");
      RavlAssert(vit);
      *vit = *it;
    }
    // Copy faces.
    SArray1dC<TriC> tri(noFaces);
    SArray1dIterC<TriC> fait(tri);
    for(IntrDLIterC<HEMeshFaceBodyC> fit(faces);fit;fit++,fait++) {
      ONDEBUG(cerr << "HEMeshBodyC::TriMesh(), Building face " << fait.Index() << "\n");
      RavlAssert(fait);
      HEMeshFaceC aface(*fit);
      HEMeshFaceEdgeIterC eit(aface);      
      RavlAssert(eit);
      ONDEBUG(cerr << "Vert1=" <<vindex[eit->Vertex()] << " Hash=" << eit->Vertex().Hash() << "\n");
      VertexC &v1 = verts[vindex[eit->Vertex()]];
      eit++;
      RavlAssert(eit);
      VertexC &v2 = verts[vindex[eit->Vertex()]];
      ONDEBUG(cerr << "Vert2=" <<vindex[eit->Vertex()] <<  " Hash=" << eit->Vertex().Hash() << "\n");
      eit++;
      RavlAssert(eit);
      VertexC &v3 = verts[vindex[eit->Vertex()]];
      ONDEBUG(cerr << "Vert2=" <<vindex[eit->Vertex()] <<  " Hash=" << eit->Vertex().Hash() << "\n");
#if DODEBUG
      eit++;
      RavlAssert(!eit);
#endif
      *fait = TriC(v1,v2,v3);
    }
    
    return TriMeshC(verts,tri);
  }
  
  TriMeshC HEMeshC::TriMesh() const 
  { return Body().TriMesh(); }
  //: Build a TriMesh from this mesh.

}
