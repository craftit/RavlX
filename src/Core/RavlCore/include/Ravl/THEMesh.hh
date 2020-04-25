// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THEMESH_HEADER
#define RAVL_THEMESH_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! file="Ravl/Core/Container/Graph/THEMesh.hh"

#include "Ravl/THEMeshFace.hh"
#include "Ravl/THEMeshVertex.hh"
#include "Ravl/THEMeshEdge.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/THEMeshVertexIter.hh"
#include "Ravl/HEMeshBase.hh"
#include "Ravl/Empty.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Half Edge Mesh Body
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshBodyC 
    : public HEMeshBaseBodyC
  {
  public:
    THEMeshBodyC()
    {}
    //: Default constructor.
    // Creates an empty mesh.
    
    THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> InsertVertex(const VertexDataT &data) {
      THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> vert(data,allocVertexId++);
      vertices.InsLast(vert.Body());
      return vert;
    }
    //: Insert a new vertex into the mesh.
    
    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> InsertFace(const FaceDataT &data,
							     const SArray1dC<HEMeshBaseVertexC> &vertices,
							     HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab) {
      THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> ret(data,allocFaceId++);
      HEMeshBaseBodyC::InsertFace(ret,vertices,edgeTab);
      return ret;
    }
    //: Insert face defined by vertices.
    
    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> InsertFace(const SArray1dC<HEMeshBaseVertexC> &vertices,
							     HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab) {
#if 1
      static FaceDataT tmp;
      THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> ret(tmp,allocFaceId++);
#else
      static FaceDataT tmp;
      //THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> ret = NewFace();
      THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> ret(*new THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT>(tmp,allocFaceId++));
#endif
      HEMeshBaseBodyC::InsertFace(ret,vertices,edgeTab);
      return ret;
    }
    //: Insert face defined by vertices.  

  protected:
    virtual THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> NewEdge(THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> &vert,
		    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> &face)
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(*new THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(vert,face)); }
    //: Create a new edge.
    
  public:
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> SplitFace( THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> from,
		    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> to ) {

      //ONDEBUG(cerr << "THEMeshC::SplitFace(). From:" << from.Hash() << " To:" << to.Hash() << "\n");
      RavlAssert(from.IsValid());
      RavlAssert(to.IsValid());
      RavlAssert(from.Face() == to.Face());
      RavlAssert(to.Next() != from);
      RavlAssert(from.Next() != to); 
      
      THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> face1 = to.Face();
      THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> face2 = NewFace();
      faces.InsLast(face2.Body()); 
      
      THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> fromVertex = from.Vertex();
      THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> toVertex = to.Vertex();
      THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> edge1 = NewEdge(fromVertex,face1);
      THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> edge2 = NewEdge(toVertex,face2);
      edge1.SetPair(edge2);
      edge2.SetPair(edge1);
      face2.SetEdge(edge2);
      face1.SetEdge(edge1); // Make sure face1 is pointing to a valid edge.
    
      edge2.Body().CutPaste(to.Body().Next(),from.Body().Next()); 
      to.LinkAfter(edge1); 
      
      for(THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> at = edge2.Next();at != edge2;at = at.Next()) {
	//ONDEBUG(cerr << "THEMeshC::SplitFace() Setting face for edge " << at.Hash() <<" to " << face2.Hash() << "\n";)
	at.SetFace(face2);
      }

      return edge2;
    }   
    //: Split a face with a new edge.
    // Edges from and to must be on the same face. The new edge is placed between
    // the vertex's they point to. The new edge is returned.
    
  protected:
    virtual HEMeshBaseFaceC NewFace()
    { return THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT>(*new THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT>(allocFaceId++)); }
    //: Create a new face.
    
    virtual HEMeshBaseVertexC NewVertex()
    { return THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT>(*new THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT>(allocVertexId++)); }
    //: Create a new face.
    
    virtual HEMeshBaseEdgeC NewEdge(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &face)
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(*new THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT>(vert,face)); }
    //: Create a new face.
    
    friend class THEMeshC<VertexDataT,FaceDataT,EdgeDataT>;
  };
  

  //! userlevel=Normal
  //: Half Edge Mesh
  // Reference counted handle to mesh.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshC 
    : public HEMeshBaseC
  {
  public:
    THEMeshC()
    {}
    //: Default constructor.

    explicit THEMeshC(bool)
      : HEMeshBaseC(*new THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT>())
    {}
    //: Constructor.
    
  protected:
    THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body()
    { return static_cast<THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseC::Body()); }
    //: Access body.
    
    const THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body() const
    { return static_cast<const THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseC::Body()); }
    //: Access body.
    
  public:
    
    THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> InsertVertex(const VertexDataT &data)
    { return Body().InsertVertex(data); }
    //: Insert a new vertex into the mesh.
    
    THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> InsertVertexOnEdge(THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> &edge)
    { return Body().InsertVertexOnEdge(edge); }
    //: Insert a vertex on an edge.
    
    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> InsertFace(const FaceDataT &data,
							     const SArray1dC<HEMeshBaseVertexC> &vertices,
							     HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab)
    { return Body().InsertFace(data,vertices,edgeTab); }
    //: Insert face defined by vertices.

    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> InsertFace(const SArray1dC<HEMeshBaseVertexC> &vertices,
							     HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab)
    { return Body().InsertFace(vertices,edgeTab); }
    //: Insert face defined by vertices.
    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> SplitFace( THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> from,
		    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> to )
    { return Body().SplitFace(from,to); }
    //: Split a face with a new edge.
    // Edges from and to must be on the same face. The new edge is placed between
    // the vertex's they point to. The new edge is returned.

    THEMeshFaceIterC<VertexDataT,FaceDataT,EdgeDataT> Faces()
    { return THEMeshFaceIterC<VertexDataT,FaceDataT,EdgeDataT>(Body().faces); }
    //: List of faces in the mesh.
    // Use to create THEMeshFaceIterC.
    
    THEMeshVertexIterC<VertexDataT,FaceDataT,EdgeDataT> Vertices()
    { return THEMeshVertexIterC<VertexDataT,FaceDataT,EdgeDataT>(Body().vertices); }
    //: List of vertices.
    // Use to create THEMeshVertexIterC.
    
    THEMeshFaceIterC<VertexDataT,FaceDataT,EdgeDataT> Faces() const
    { return THEMeshFaceIterC<VertexDataT,FaceDataT,EdgeDataT>(Body().faces); }
    //: List of faces in the mesh.
    // Use to create THEMeshFaceIterC.
    
    THEMeshVertexIterC<VertexDataT,FaceDataT,EdgeDataT> Vertices() const
    { return THEMeshVertexIterC<VertexDataT,FaceDataT,EdgeDataT>(Body().vertices); }
    //: List of vertices.
    // Use to create THEMeshVertexIterC.
    
    THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> FirstFace()
    { return THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT>(Body().faces.First()); }
    //: Get the first face in the mesh.
    // Note: The mesh must NOT be empty.
    
    THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> FirstVertex()
    { return THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT>(Body().vertices.First()); }
    //: Get the first vertex in the mesh.
    // Note: The mesh must NOT be empty.

  };

}

#endif
