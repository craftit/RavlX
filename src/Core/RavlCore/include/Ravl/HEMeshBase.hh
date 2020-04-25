// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASE_HEADER
#define RAVL_HEMESHBASE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! file="Ravl/Core/Container/Graph/HEMeshBase.hh"

#include "Ravl/HEMeshBaseFace.hh"
#include "Ravl/HEMeshBaseVertex.hh"
#include "Ravl/HEMeshBaseEdge.hh"
#include "Ravl/HEMeshBaseFaceIter.hh"
#include "Ravl/HEMeshBaseVertexIter.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Tuple2.hh"

namespace RavlN {
  
  class HEMeshBaseC;
  
  //! userlevel=Develop
  //: Half Edge Mesh Body
  
  class HEMeshBaseBodyC 
    : public RCBodyVC
  {
  public:
    HEMeshBaseBodyC()
      : allocFaceId(0),
	allocVertexId(0)
    {}
    //: Default constructor.
    // Creates an empty mesh.
    
    ~HEMeshBaseBodyC();
    //: Destructor.
    
    HEMeshBaseFaceC InsertFace(HEMeshBaseFaceC &face,
			       const SArray1dC<HEMeshBaseVertexC> &vertices,
			       HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab);
    //: Insert face defined by vertices.
    
    HEMeshBaseFaceC InsertFace(const SArray1dC<HEMeshBaseVertexC> &vertices,
			       HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab)  { 
      HEMeshBaseFaceC face = NewFace(); 
      return InsertFace(face,vertices,edgeTab); 
    }
    //: Insert face defined by vertices.
    
    bool InsertVertexInEdge(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge);
    //: Insert a vertex on an edge.
    
    bool InsertVertexInEdgeTri(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge);
    //: Insert a vertex on an edge, assuming and maintaing a triangular mesh.
    // The edge is inserted between vert and edge.Next().Vertex().
    
    bool InsertVertexInFace(HEMeshBaseVertexC vert,HEMeshBaseFaceC face);
    //: Insert a vertex into a face, link all vertexes already in the face to it.
    
    bool TwistEdge(HEMeshBaseEdgeC edge,HEMeshBaseEdgeC vertFrom,HEMeshBaseEdgeC vertTo);
    //: Twist an edge that lies between two faces.
    // Both 'from' and 'to' must be on one of the faces adjacent to 'edge'.
    
    HEMeshBaseEdgeC SplitFace(HEMeshBaseEdgeC from,HEMeshBaseEdgeC to);
    //: Split a face with a new edge.
    // Edges from and to must be on the same face. The new edge is placed
    // between the vertex's they point to.  The new edge that is adjacent to the
    // new face is returned.
    
    bool OpenEdge(HEMeshBaseEdgeC edge);
    //: Remove an edge from between two faces, merge the faces into one.
    
    UIntT NoFaces() const
    { return faces.Size(); }
    //: Get the number of faces.
    
    UIntT NoVertices() const
    { return vertices.Size(); }
    //: Get the number of vertices.
    
    bool CheckMesh(bool canBeOpen = false) const;
    //: Check mesh structure is consistant.
    // Returns false if an inconsistancy is found.
    
    HEMeshBaseVertexC InsertVertex();
    //: Insert a new vertex into the mesh.
    
    bool DeleteVertex(HEMeshBaseVertexC vert,bool leaveHole = true);
    //: Remove a vertex and assoicated faces from the mesh.
    // If 'leaveHole' is true this does not close the resulting face, 
    // so a hole will be left in the mesh.
    
    UIntT NumberOfFaceIds() const
    { return allocFaceId; }
    //: Access the number of face id's allocated.
    
    UIntT NumberOfVertexIds() const
    { return allocVertexId; }
    //: Allocate number of vertex id's allocated.
    
  protected:
    virtual HEMeshBaseFaceC NewFace();
    //: Create a new face.
    
    virtual HEMeshBaseVertexC NewVertex();
    //: Create a new vertex.
    
    virtual HEMeshBaseEdgeC NewEdge(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &face);
    //: Create a new edge.
    
    HEMeshBaseEdgeC NewEdge(const HEMeshBaseVertexC &vert,const HEMeshBaseFaceC &face)
    { return NewEdge(const_cast<HEMeshBaseVertexC &>(vert).Body(),const_cast<HEMeshBaseFaceC &>(face).Body()); }
    //: Create a new edge
    
    IntrDListC<HEMeshBaseFaceBodyC> faces;  // List of faces in the mesh.
    IntrDListC<HEMeshBaseVertexBodyC> vertices; // List of vertices.

    UIntT allocFaceId;
    UIntT allocVertexId;
    
    friend class HEMeshBaseC;
  };
  

  //! userlevel=Normal
  //: Half Edge Mesh
  // Reference counted handle to mesh.
  
  class HEMeshBaseC 
    : public RCHandleC<HEMeshBaseBodyC>
  {
  public:
    HEMeshBaseC()
    {}
    //: Default constructor.
    
    explicit HEMeshBaseC(bool)
      : RCHandleC<HEMeshBaseBodyC>(*new HEMeshBaseBodyC())
    {}
    //: Constructor.

  protected:
    HEMeshBaseC(HEMeshBaseBodyC &bod)
      : RCHandleC<HEMeshBaseBodyC>(bod)
    {}
    //: Body constructor.
    
    HEMeshBaseBodyC& Body()
    { return RCHandleC<HEMeshBaseBodyC>::Body(); }
    //: Access body.
    
    const HEMeshBaseBodyC& Body() const
    { return RCHandleC<HEMeshBaseBodyC>::Body(); }
    //: Access body.

    HEMeshBaseFaceC NewFace()
    { return Body().NewFace(); }
    //: Create a new face.
    
    HEMeshBaseVertexC NewVertex()
    { return Body().NewVertex(); }
    //: Create a new vertex.
    
    HEMeshBaseEdgeC NewEdge(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &face)
    { return Body().NewEdge(vert,face); }
    //: Create a new edge.
    
    HEMeshBaseEdgeC NewEdge(HEMeshBaseVertexC &vert,HEMeshBaseFaceC &face)
    { return Body().NewEdge(vert.Body(),face.Body()); }
    //: Create a new edge.
    
  public:    
    HEMeshBaseVertexC InsertVertex()
    { return Body().InsertVertex(); }
    //: Insert a new vertex into the mesh.
    
    bool DeleteVertex(HEMeshBaseVertexC vert,bool leaveHole = true)
    { return Body().DeleteVertex(vert,leaveHole); }
    //: Remove a vertex and assoicated faces from the mesh.
    // If 'leaveHole' is true this does not close the resulting face, 
    // so a hole will be left in the mesh.
    
    HEMeshBaseFaceC InsertFace(HEMeshBaseFaceC &face,
			       const SArray1dC<HEMeshBaseVertexC> &vertices,
			       HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab)
    { return Body().InsertFace(face,vertices,edgeTab); }
    //: Insert face defined by vertices.
    
    HEMeshBaseFaceC InsertFace(const SArray1dC<HEMeshBaseVertexC> &vertices,
			       HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> &edgeTab)
    { return Body().InsertFace(vertices,edgeTab); }
    //: Insert face defined by vertices.
    
    bool InsertVertexInEdge(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge)
    { return Body().InsertVertexInEdge(vert,edge); }
    //: Insert a vertex on an edge.

    bool InsertVertexInEdgeTri(HEMeshBaseVertexC vert,HEMeshBaseEdgeC edge)
    { return Body().InsertVertexInEdgeTri(vert,edge); }
    //: Insert a vertex on an edge, assuming and maintaing a triangular mesh.
    // The edge is inserted between vert and edge.Next().Vertex().
    
    bool InsertVertexInFace(HEMeshBaseVertexC vert,HEMeshBaseFaceC face)
    { return Body().InsertVertexInFace(vert,face); }
    //: Insert a vertex into a face, link all vertexes already in the face to it.
    // This method assumes 'vert' is not connected to a mesh.
    
    bool TwistEdge(HEMeshBaseEdgeC edge,HEMeshBaseEdgeC vertFrom,HEMeshBaseEdgeC vertTo)
    { return Body().TwistEdge(edge,vertFrom,vertTo); }
    //: Twist an edge that lies between two faces.
    // Both 'from' and 'to' must be one of the faces adjacent to 'edge'
    
    HEMeshBaseEdgeC SplitFace(HEMeshBaseEdgeC from,HEMeshBaseEdgeC to)
    { return Body().SplitFace(from,to); }
    //: Split a face with a new edge.
    // Edges from and to must be on the same face. The new edge is placed
    // between the vertex's they point to.  The new edge that is adjacent to the
    // new face is returned.
    
    bool OpenEdge(HEMeshBaseEdgeC edge)
    { return Body().OpenEdge(edge); }
    //: Remove an edge from between two faces, merge the faces into one.
    
    UIntT NoFaces() const
    { return Body().NoFaces(); }
    //: Get the number of faces.
    
    UIntT NoVertices() const
    { return Body().NoVertices(); }
    //: Get the number of vertices.
    
    bool CheckMesh(bool canBeOpen = false) const
    { return Body().CheckMesh(canBeOpen); }
    //: Check mesh structure is consistant.
    // Returns false if an inconsistancy is found.
    
    HEMeshBaseFaceIterC Faces()
    { return HEMeshBaseFaceIterC(Body().faces); }
    //: List of faces in the mesh.
    // Use to create HEMeshBaseFaceIterC.
    
    HEMeshBaseVertexIterC Vertices()
    { return HEMeshBaseVertexIterC(Body().vertices); }
    //: List of vertices.
    // Use to create HEMeshBaseVertexIterC.

    HEMeshBaseFaceC FirstFace()
    { return HEMeshBaseFaceC(Body().faces.First()); }
    //: Get the first face in the mesh.
    // Note: The mesh must NOT be empty.
    
    HEMeshBaseVertexC FirstVertex()
    { return HEMeshBaseVertexC(Body().vertices.First()); }
    //: Get the first vertex in the mesh.
    // Note: The mesh must NOT be empty.

    UIntT NumberOfFaceIds() const
    { return Body().NumberOfFaceIds(); }
    //: Access the number of face id's allocated.
    
    UIntT NumberOfVertexIds() const
    { return Body().NumberOfVertexIds(); }
    //: Allocate number of vertex id's allocated.

  };

}

#endif
