// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASEVERTEX_HEADER 
#define RAVL_HEMESHBASEVERTEX_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Graph/HEMeshBaseVertex.hh"

#include "Ravl/DLink.hh"
#include "Ravl/StdHash.hh"

namespace RavlN {
  
  class HEMeshBaseVertexC;
  class HEMeshBaseEdgeC;
  class HEMeshBaseFaceC;
  class HEMeshBaseC;
  class HEMeshBaseFaceEdgeIterC;
  class HEMeshBaseEdgeBodyC;
  class HEMeshBaseBodyC;
  class HEMeshBaseVertexIterC;
  
  //! userlevel=Develop
  //: Vertex body.
  
  class HEMeshBaseVertexBodyC
    : public DLinkC // Link in list of all vertexes
  {
  public:    
    virtual ~HEMeshBaseVertexBodyC();
    //: Destructor.
    
    HEMeshBaseEdgeC FindEdge(const HEMeshBaseVertexC &oth) const;
    //: Look for a connection from this vertex to oth.
    // Returns an invalid handle if ones is not found.
    
    HEMeshBaseEdgeBodyC &FirstEdge()
    { return *edge; }
    //: Access edge.
    // This is always an edge going to this vertex.
    
    const HEMeshBaseEdgeBodyC &FirstEdge() const
    { return *edge; }
    //: Access edge.
    // This is always an edge going to this vertex.
    
    HEMeshBaseEdgeC Link(HEMeshBaseVertexC newVert,HEMeshBaseFaceC face);
    //: Link this vertex to newVert on indicated face
    // Both vertexes must share the face. This will effectively
    // split the face in two.
    
    UIntT Valence() const;
    //: Get the number of edges/faces linking to this vertexs.
    // This assumes this is a closed mesh.
    
    void SetEdge(HEMeshBaseEdgeBodyC &nedge)
    { edge = &nedge; }
    //: Set edge associated with this vertex.
    // This must always be an edge going to
    // this vertex.
    
    bool HasEdge() const
    { return edge != 0; }
    //: Is this vertex associated with any faces/edges ?
    
    UIntT Id() const
    { return id; }
    //: Access vertex id.
    
  protected:
    HEMeshBaseVertexBodyC(UIntT nid = 0)
      : edge(0),
	id(nid)
    {}
    //: Construct from a vertex position.
    
    HEMeshBaseEdgeBodyC *edge; 
    // Ptr to one of the edges on the vertex.
    // This must always point to an edge going to
    // this vertex.
    
    UIntT id;
    //: Identifier for vertex.
    
    friend class HEMeshBaseVertexC;
    friend class HEMeshBaseEdgeBodyC;
    friend class HEMeshBaseFaceBodyC;
  };
  
  //! userlevel=Normal
  //: Vertex in half edge mesh
  
  class HEMeshBaseVertexC {
  public:
    HEMeshBaseVertexC()
      : body(0)
    {}
    //: Default constructor.
    
    bool IsValid()
    { return body != 0; }
    //: Is this a valid Vertex ?
    
    HEMeshBaseEdgeC FindEdge(const HEMeshBaseVertexC &oth) const;
    //: Look for a connection from this vertex to oth.
    // Returns an invalid handle if ones is not found.
    
    inline HEMeshBaseEdgeC FirstEdge() const;
    //: Access edge.
    // This is always an edge going to this vertex.
    
    void Invalidate()
    { body = 0; }
    //: Invalidate this handle to the vertex body.
    
  protected:
    explicit HEMeshBaseVertexC(bool)
      : body(new HEMeshBaseVertexBodyC())
    {}
    //: Constructor.
    
    HEMeshBaseVertexC(HEMeshBaseVertexBodyC &nbody)
      : body(&nbody)
    {}
    //: Construct from reference
    
    HEMeshBaseVertexC(const HEMeshBaseVertexBodyC *nbody)
      : body(const_cast<HEMeshBaseVertexBodyC *>(nbody))
    {}
    //: Construct from pointer.
    
    HEMeshBaseVertexBodyC &Body()
    { return *body; }
    //: Access body.
    
    const HEMeshBaseVertexBodyC &Body() const
    { return *body; }
    //: Access body.
    
    static const HEMeshBaseVertexBodyC *BodyPtr(const HEMeshBaseVertexC &handle)
    { return handle.body; }
    //: Access handle ptr.
    
    static HEMeshBaseVertexBodyC *BodyPtr(HEMeshBaseVertexC &handle)
    { return handle.body; }
    //: Access handle ptr.
    
  public:
    bool operator==(const HEMeshBaseVertexBodyC &vert) const
    { return body == &vert; }
    //: Is this a handle to the vertex.
    
    bool operator!=(const HEMeshBaseVertexBodyC &vert) const
    { return body != &vert; }
    //: Is this a handle to the vertex.
    
    UIntT Hash() const 
    { return StdHash(static_cast<const void *>(body)); }
    //: Hash value for handle.
    
    bool operator==(const HEMeshBaseVertexC &oth) const
    { return body == oth.body; }
    //: Is this a handle to the same object ?

    bool operator!=(const HEMeshBaseVertexC &oth) const
    { return body != oth.body; }
    //: Is this a handle to a different object ?
    
    UIntT Valence() const
    { return Body().Valence(); }
    //: Get the number of edges/faces linking to this vertexs.
    // This assumes this is a closed mesh.
    
    void SetEdge(HEMeshBaseEdgeBodyC &nedge)
    { Body().SetEdge(nedge); }
    //: Set edge associated with this vertex.
    // This must always be an edge going to
    // this vertex.
    
    bool HasEdge() const
    { return Body().HasEdge(); }
    //: Is this vertex associated with any faces/edges ?
    
    UIntT Id() const
    { return Body().Id(); }
    //: Access vertex id.
    
    bool operator>(const HEMeshBaseVertexC &oth) const
    { return body > oth.body; }
    //: This provides an arbitary ordering on the vertex's.
    
    bool operator<(const HEMeshBaseVertexC &oth) const
    { return body < oth.body; }
    //: This provides an arbitary ordering on the vertex's.
    
    bool operator>=(const HEMeshBaseVertexC &oth) const
    { return body >= oth.body; }
    //: This provides an arbitary ordering on the vertex's.
    
    bool operator<=(const HEMeshBaseVertexC &oth) const
    { return body <= oth.body; }
    //: This provides an arbitary ordering on the vertex's.
    
  private:
    HEMeshBaseVertexBodyC *body;    
    friend class HEMeshBaseBodyC;
    friend class HEMeshBaseVertexBodyC;
    friend class HEMeshBaseEdgeC;
    friend class HEMeshBaseEdgeBodyC;
    friend class HEMeshBaseVertexIterC;
    friend class HEMeshBaseC;
  };
  
  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseVertexC &vertex);
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseVertexC &vertex);
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseVertexC &vertex);
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseVertexC &vertex);
  //: Binary stream input.
  // Not implemented
  
}
#endif
