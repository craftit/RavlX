// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASEFACE_HEADER
#define RAVL_HEMESHBASEFACE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Graph/HEMeshBaseFace.hh"

#include "Ravl/DLink.hh"
#include "Ravl/HEMeshBaseEdge.hh"

namespace RavlN {
  
  class HEMeshBaseVertexC;
  class HEMeshBaseFaceC;
  class HEMeshBaseFaceBodyC;
  class HEMeshBaseC;
  class HEMeshBaseFaceEdgeIterC;
  class HEMeshBaseEdgeC;
  class HEMeshBaseEdgeBodyC;
  class HEMeshBaseBodyC;
  class HEMeshBaseFaceIterC;
  
  //! userlevel=Develop
  //: Face in mesh.
  
  class HEMeshBaseFaceBodyC 
    : public DLinkC // Link in list of all faces.
  {
  public:
    HEMeshBaseFaceBodyC(size_t nid = 0)
      : edge(0),
	id(nid)
    {}
    //: Default constructor.
    
    virtual ~HEMeshBaseFaceBodyC();
    //: Destructor.
    
    HEMeshBaseFaceBodyC(HEMeshBaseEdgeBodyC &nedge)
      : edge(&nedge)
    {}
    //: Default constructor.
    
  protected:
    HEMeshBaseEdgeBodyC *FirstEdge()
    { return edge; }
    //: Access first edge.

    const HEMeshBaseEdgeBodyC *FirstEdge() const
    { return edge; }
    //: Access first edge.
    
    void DestroyFace();
    //: Destroy the face without worrying about mesh consistancy.
    
  public:
    void SetEdge(HEMeshBaseEdgeBodyC *nedge)
    { edge = nedge; }
    //: Set first edge.

    void SetEdge(HEMeshBaseEdgeBodyC &nedge)
    { edge = &nedge; }
    //: Set first edge.
    
    UIntT Sides() const;
    //: Get number of sides on face.
    
    bool HasVertex(HEMeshBaseVertexC vert) const;
    //: Check that the vertex is connected to this face.
    
    HEMeshBaseEdgeC Edge() 
    { return HEMeshBaseEdgeC(*edge); }
    //: Get an edge leading to the face.
    
    HEMeshBaseEdgeC FindEdge(HEMeshBaseVertexC a);
    //: Find the edge linking to vertex a.
    // If edge is not found an invalid handle is returned.
    
    bool operator==(const HEMeshBaseFaceBodyC &bod) const
    { return this == &bod; }
    //: Is this the same body ?

    bool operator!=(const HEMeshBaseFaceBodyC &bod) const
    { return this != &bod; }
    //: Is this the same body ?
    
    UIntT Id() const
    { return id; }
    //: Access face id.
    
  protected:
    HEMeshBaseEdgeBodyC *edge; // Ptr to one of the edges adjacent to the face.
    UIntT id;
    
    friend class HEMeshBaseFaceEdgeIterC;
    friend class HEMeshBaseFaceC;
    friend class HEMeshBaseEdgeBodyC;
    friend class HEMeshBaseBodyC;
  };

  //! userlevel=Normal
  //: Face in mesh.
  
  class HEMeshBaseFaceC 
  {
  public:
    HEMeshBaseFaceC()
      : body(0)
    {}
    //: Default constructor
    
    bool IsValid() const
    { return body != 0; }
    //: Is this a valid handle ?
    
  protected:
    HEMeshBaseFaceC(HEMeshBaseFaceBodyC &bod)
      : body(&bod)
    {}
    //: Body constructor.

    HEMeshBaseFaceC(const HEMeshBaseFaceBodyC *bod)
      : body(const_cast<HEMeshBaseFaceBodyC *>(bod))
    {}
    //: Body constructor.
    
    explicit HEMeshBaseFaceC(bool)
      : body(new HEMeshBaseFaceBodyC())
    {}
    //: Construct a new face.

    HEMeshBaseFaceC(HEMeshBaseEdgeC edge)
      : body(new HEMeshBaseFaceBodyC(edge.Body()))
    {}
    //: Construct a new face.
    
    HEMeshBaseFaceBodyC &Body()
    { return *body; }
    //: Access body.

    const HEMeshBaseFaceBodyC &Body() const
    { return *body; }
    //: Access body.
    
    static const HEMeshBaseFaceBodyC *BodyPtr(const HEMeshBaseFaceC &handle)
    { return handle.body; }
    //: Access body.
    
    static HEMeshBaseFaceBodyC *BodyPtr(HEMeshBaseFaceC &handle)
    { return handle.body; }
    //: Access body.
    
    HEMeshBaseEdgeBodyC *FirstEdge()
    { return Body().FirstEdge(); }
    //: Access first edge.

    const HEMeshBaseEdgeBodyC *FirstEdge() const
    { return Body().FirstEdge(); }
    //: Access first edge.
    
  public:    
    void SetEdge(HEMeshBaseEdgeC nedge)
    { Body().SetEdge(&nedge.Body()); }
    //: Set first edge.
    
    void SetEdge(HEMeshBaseEdgeBodyC &nedge)
    { Body().SetEdge(nedge); }
    //: Set first edge.
    
    SizeT Hash() const
    { return StdHash(static_cast<const void *>(body)); }
    //: Hash value for handle.
    
    bool operator==(const HEMeshBaseFaceC &oth) const
    { return body == oth.body; }
    //: Is this a handle to the same object ?
    
    UIntT Sides() const
    { return Body().Sides(); }
    //: Get number of edges on face.
    
    bool HasVertex(HEMeshBaseVertexC vert) const
    { return Body().HasVertex(vert); }
    //: Check that the vertex is connected to this face.
    
    HEMeshBaseEdgeC Edge() 
    { return Body().Edge(); }
    //: Get an edge leading to the face.
    
    HEMeshBaseEdgeC FindEdge(HEMeshBaseVertexC a)
    { return Body().FindEdge(a); }
    //: Find the edge linking to vertex a.
    // If edge is not found an invalid handle is returned.
    
    bool Remove() { 
      if(body == 0) return false;
      delete body; 
      body = 0;
      return true;
    }
    //: Delete face from mesh.
    // This operation leaves the mesh open.
    
    void Invalidate()
    { body = 0; }
    //: Invalidate handle to edge.
    
    UIntT Id() const
    { return Body().Id(); }
    //: Access face id.
    
  protected:
    HEMeshBaseFaceBodyC *body;
    friend class HEMeshBaseFaceEdgeIterC;
    friend class HEMeshBaseBodyC;
    friend class HEMeshBaseEdgeC;
    friend class HEMeshBaseVertexBodyC;
    friend class HEMeshBaseFaceIterC;
    friend class HEMeshBaseC;
  };

  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseFaceC &vertex);
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseFaceC &vertex);
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseFaceC &vertex);
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseFaceC &vertex);
  //: Binary stream input.
  // Not implemented
  
  //! userlevel=Normal
  //: Iterate through edges around a face.
  
  class HEMeshBaseFaceEdgeIterC {
  public:
    HEMeshBaseFaceEdgeIterC()
      : first(0),
	at(0)
    {}
    //: Default constructor.
    
    HEMeshBaseFaceEdgeIterC(const HEMeshBaseFaceC &face)
      : first(const_cast<HEMeshBaseEdgeBodyC *>(face.FirstEdge())),
	at(const_cast<HEMeshBaseEdgeBodyC *>(face.FirstEdge()))
    {}
    //: Construct from a face
    
    HEMeshBaseFaceEdgeIterC(HEMeshBaseFaceBodyC &face)
      : first(face.FirstEdge()),
	at(face.FirstEdge())
    {}
    //: Construct from a face
    
    bool First() { 
      at = first; 
      return at != 0;
    }
    //: Goto first element.
    // Returns true if the new element is valid.
    
    bool IsFirst()
    { return at == first && at != 0; }
    //: Is this the first element in list.
    
    bool Next() { 
      at = &(at->Next());
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    bool Prev() { 
      at = &(at->Prev());
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    bool operator--(int)
    { return Prev(); }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    operator bool() const
    { return at != 0; }
    //: Test if we're at a valid element.
    // Returns true if we are.
    
    bool IsElm() const
    { return at != 0; }
    //: Test if we're at a valid element.
    // Returns true if we are.
    
    HEMeshBaseEdgeC Data()
    { return HEMeshBaseEdgeC(*at); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshBaseEdgeC Data() const
    { return HEMeshBaseEdgeC(*at); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshBaseEdgeC operator *() const
    { return HEMeshBaseEdgeC(*at); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshBaseEdgeC operator *()
    { return HEMeshBaseEdgeC(*at); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshBaseEdgeBodyC *operator ->() const
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshBaseEdgeBodyC *operator->()
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
  protected:
    HEMeshBaseEdgeBodyC *first;
    HEMeshBaseEdgeBodyC *at;
  };
  
  //////////////////////////////////////////////////////
  
  inline void HEMeshBaseEdgeC::SetFace(HEMeshBaseFaceC face)
  { Body().SetFace(face.Body()); }
  
  inline HEMeshBaseFaceC HEMeshBaseEdgeC::Face()
  { return HEMeshBaseFaceC(Body().Face()); }
  
}

#endif
