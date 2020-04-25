// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_HEMESH_HEADER
#define RAVL3D_HEMESH_HEADER 1
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! docentry="Ravl.API.3D.Half Edge Mesh"
//! date="24/4/2002"
//! file="Ravl/3D/Mesh/HEMesh.hh"

#include "Ravl/3D/HEMeshFace.hh"
#include "Ravl/3D/HEMeshVertex.hh"
#include "Ravl/3D/HEMeshEdge.hh"
#include "Ravl/3D/HEMeshFaceIter.hh"
#include "Ravl/3D/HEMeshVertexIter.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Tuple2.hh"

namespace Ravl3DN {

  using namespace RavlN;
  
  class HEMeshC;
  class TriMeshC;
  
  //! userlevel=Develop
  //: Half Edge Mesh Body
  
  class HEMeshBodyC 
    : public RCBodyVC
  {
  public:
    HEMeshBodyC()
    {}
    //: Default constructor.
    // Creates an empty mesh.
    
    HEMeshBodyC(const TriMeshC &mesh);
    //: Construct from a TriMesh

    ~HEMeshBodyC();
    //: Destructor.
    
    HEMeshVertexC InsertVertex(const Vector3dC &position,const Vector3dC &norm) {
      HEMeshVertexC vert(position,norm);
      vertices.InsLast(vert.Body());
      return vert;
    }
    //: Insert a new vertex into the mesh.

    HEMeshVertexC InsertVertex(const VertexC &vert) {
      HEMeshVertexC nvert(vert);
      vertices.InsLast(nvert.Body());
      return nvert;
    }
    //: Insert a new vertex into the mesh.

    HEMeshVertexC InsertVertexOnEdge(HEMeshEdgeC edge);
    //: Insert a vertex on an edge.
    
    HEMeshFaceC InsertFace(const SArray1dC<HEMeshVertexC> &vertices,HashC<Tuple2C<HEMeshVertexC,HEMeshVertexC> , HEMeshEdgeC> &edgeTab);
    //: Insert face defined by vertices.
    
    UIntT NoFaces() const
    { return faces.Size(); }
    //: Get the number of faces.
    
    UIntT NoVertices() const
    { return vertices.Size(); }
    //: Get the number of vertices.
    
    TriMeshC TriMesh() const;
    //: Build a TriMesh from this mesh.
    
    bool CheckMesh(bool canBeOpen = false) const;
    //: Check mesh structure is consistant.
    // Returns false if an inconsistancy is found.
    
  protected:
    IntrDListC<HEMeshFaceBodyC> faces;  // List of faces in the mesh.
    IntrDListC<HEMeshVertexBodyC> vertices; // List of vertices.
    
    friend class HEMeshC;
  };
  

  //! userlevel=Normal
  //: Half Edge Mesh
  // Reference counted handle to mesh.
  
  class HEMeshC 
    : public RCHandleC<HEMeshBodyC>
  {
  public:
    HEMeshC()
    {}
    //: Default constructor.

    HEMeshC(bool)
      : RCHandleC<HEMeshBodyC>(*new HEMeshBodyC())
    {}
    //: Constructor.
    
    HEMeshC(const TriMeshC &mesh)
      : RCHandleC<HEMeshBodyC>(*new HEMeshBodyC(mesh))
    {}
    //: Construct from a TriMesh

    HEMeshVertexC InsertVertex(const Vector3dC &position,const Vector3dC &norm)
    { return Body().InsertVertex(position,norm); }
    //: Insert a new vertex into the mesh.

    HEMeshVertexC InsertVertex(const VertexC &vert)
    { return Body().InsertVertex(vert); }
    //: Insert a new vertex into the mesh.
    
    HEMeshVertexC InsertVertexOnEdge(HEMeshEdgeC edge)
    { return Body().InsertVertexOnEdge(edge); }
    //: Insert a vertex on an edge.
    
    HEMeshFaceC InsertFace(const SArray1dC<HEMeshVertexC> &vertices,HashC<Tuple2C<HEMeshVertexC,HEMeshVertexC> , HEMeshEdgeC> &edgeTab)
    { return Body().InsertFace(vertices,edgeTab); }
    //: Insert face defined by vertices.

    UIntT NoFaces() const
    { return Body().NoFaces(); }
    //: Get the number of faces.
    
    UIntT NoVertices() const
    { return Body().NoVertices(); }
    //: Get the number of vertices.
    
    TriMeshC TriMesh() const;
    //: Build a TriMesh from this mesh.
    
    bool CheckMesh(bool canBeOpen = false) const
    { return Body().CheckMesh(canBeOpen); }
    //: Check mesh structure is consistant.
    // Returns false if an inconsistancy is found.
    
    HEMeshFaceIterC Faces()
    { return HEMeshFaceIterC(Body().faces); }
    //: List of faces in the mesh.
    // Use to create HEMeshFaceIterC.
    
    HEMeshVertexIterC Vertices()
    { return HEMeshVertexIterC(Body().vertices); }
    //: List of vertices.
    // Use to create HEMeshVertexIterC.

    HEMeshFaceC FirstFace()
    { return HEMeshFaceC(Body().faces.First()); }
    //: Get the first face in the mesh.
    // Note: The mesh must NOT be empty.
    
    HEMeshVertexC FirstVertex()
    { return HEMeshVertexC(Body().vertices.First()); }
    //: Get the first vertex in the mesh.
    // Note: The mesh must NOT be empty.

  };

  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshC &obj);
  //: Write to binary stream.
  // NOT IMPLEMENTED.
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshC &obj);
  //: Read from binary stream.
  // NOT IMPLEMENTED.
  
}

#endif
