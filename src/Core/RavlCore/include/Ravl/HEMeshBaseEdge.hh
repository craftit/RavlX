// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESHBASEEDGE_HEADER
#define RAVL_HEMESHBASEEDGE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Graph/HEMeshBaseEdge.hh"

#include "Ravl/DLink.hh"
#include "Ravl/HEMeshBaseVertex.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  class HEMeshBaseFaceC;
  class HEMeshBaseFaceBodyC;
  class HEMeshBaseC;
  class HEMeshBaseFaceEdgeIterC;
  class HEMeshBaseFaceBodyC;  
  class HEMeshBaseEdgeBodyC;
  class HEMeshBaseBodyC;
  class HEMeshBaseFaceBodyC;
  
  //! userlevel=Develop
  //: Half edge in mesh.
  
  class HEMeshBaseEdgeBodyC 
    : public DLinkC // Link in list of  edge's around a face.
  {
  public:
    HEMeshBaseEdgeBodyC &Next()
    { return static_cast<HEMeshBaseEdgeBodyC &>(DLinkC::Next()); }
    //: Get next edge on face.
    
    HEMeshBaseEdgeBodyC &Prev()
    { return static_cast<HEMeshBaseEdgeBodyC &>(DLinkC::Prev()); }
    //: Get previous edge on face.

    const HEMeshBaseEdgeBodyC &Next() const
    { return static_cast<const HEMeshBaseEdgeBodyC &>(DLinkC::Next()); }
    //: Get next edge on face.
    
    const HEMeshBaseEdgeBodyC &Prev() const
    { return static_cast<const HEMeshBaseEdgeBodyC &>(DLinkC::Prev()); }
    //: Get previous edge on face.
    
    HEMeshBaseEdgeBodyC &Pair()
    { return *pair; }
    //: Edge pair.
    // HasPair() must have return
    // true before using this method.
    
    const HEMeshBaseEdgeBodyC &Pair() const
    { return *pair; }
    //: Edge pair.
    // HasPair() must have return
    // true before using this method.
    
    bool HasPair() const
    { return pair != 0; }
    //: Does this edge have a pair.
    
    void SetPair(HEMeshBaseEdgeBodyC &edge)
    { pair = &edge; }
    //: Set edge pair.
    
    void SetFace(HEMeshBaseFaceBodyC &aface)
    { face = &aface; }
    //: Set the face associated with the edge.
    
    void SetVertex(HEMeshBaseVertexBodyC &vert)
    { vertex = &vert; }
    //: Set vertex.
    
    HEMeshBaseFaceBodyC &Face()
    { return *face; }
    //: Access the face the edge lies on.
    
    bool HasFace() const 
    { return face != 0; }
    //: Is there a face associated with this edge ?
    
    void LinkAfter(HEMeshBaseEdgeBodyC &edge) 
    { DLinkC::LinkAft(edge); }
    //: Link 'edge' after this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().

    void LinkAfter(const HEMeshBaseEdgeC &edge);
    //: Link 'edge' after this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().
    
    void LinkBefore(HEMeshBaseEdgeBodyC &edge) 
    { DLinkC::LinkBef(edge); }
    //: Link 'edge' before this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().

    void CutPaste(HEMeshBaseEdgeBodyC &firstCut, HEMeshBaseEdgeBodyC &firstNotCut) 
    { DLinkC::CutPaste(firstCut,firstNotCut); }
    //: Splice edges between firstCut, and firstNotCut into this list.
    // Cuts the chain of edges starting at 'firstCut' and
    // ending at the edge before 'firstNotCut' from the chain.
    // The rest of the chain is linked together again. The cut part
    // is linked in after this edge.
    
    HEMeshBaseVertexC Vertex() const
    { return HEMeshBaseVertexC(const_cast<HEMeshBaseVertexBodyC &>(*vertex)); }
    //: Access vertex this edge goes to
    
    const HEMeshBaseVertexBodyC *VertexPtr() const
    { return const_cast<HEMeshBaseVertexBodyC *>(vertex); }
    //: Access vertex this edge goes to
    
    HEMeshBaseVertexBodyC *VertexPtr()
    { return const_cast<HEMeshBaseVertexBodyC *>(vertex); }
    //: Access vertex this edge goes to
    
    HEMeshBaseVertexC SourceVertex() const 
    { return Prev().Vertex(); }
    //: Access vertex this edge comes from.
    
    HEMeshBaseVertexC CollapseEdge();
    //: Collapse edge to zero length.
    // This deletes the edge from the mesh and merges the vertex's
    // at either end.  The remaining vertex is returned.
    
    bool operator==(const HEMeshBaseEdgeBodyC &bod) const
    { return this == &bod; }
    //: Is this the same body ?
    
    bool operator!=(const HEMeshBaseEdgeBodyC &bod) const
    { return this != &bod; }
    //: Is this the same body ?
    
  protected:
    
    HEMeshBaseEdgeBodyC()
      : vertex(0),
	face(0),
	pair(0)
    {}
    //: Default cosntructor.
    
    HEMeshBaseEdgeBodyC(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &nface)
      : vertex(&vert),
	face(&nface),
	pair(0)
    {}
    //: Cosntructor.
    // the contents are left undefined.
    
    virtual ~HEMeshBaseEdgeBodyC();
    //: Destructor.
    
    void RemoveHalfEdge();
    //: Remove this half edge.
    // The use must ensure that the matching half is also removed.
    // The memory of for this edge is not free'd, so if it is no
    // longer needed it must be done by the caller.
    
    bool CorrectVertexEdgePtr();
    //: Correct edge's vertex pointer.
    // This ensures the vertex's edge pointer doesn't point to this edge.
    // Usefull if your about to delete the edge or restructure the mesh.
    
    HEMeshBaseVertexBodyC *vertex;
    HEMeshBaseFaceBodyC *face;    // Adjacent face.
    HEMeshBaseEdgeBodyC *pair;     // Other edge in pair, is its zero this in an edge
    
    friend class HEMeshBaseEdgeC;
    friend class HEMeshBaseFaceBodyC;
    friend class HEMeshBaseBodyC;
  };
  
  //! userlevel=Normal
  //: Half edge in mesh.
  
  class HEMeshBaseEdgeC {
  public:
    HEMeshBaseEdgeC()
      : body(0)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    bool IsValid() const
    { return body != 0; }
    //: Is this a valid handle ?

    void Invalidate()
    { body = 0; }
    //: Invalidate this handle.
    
  protected:
    explicit HEMeshBaseEdgeC(bool)
      : body(new HEMeshBaseEdgeBodyC())
    {}
    //: Constructor.
    
    HEMeshBaseEdgeC(HEMeshBaseVertexBodyC &vert,HEMeshBaseFaceBodyC &face)
      : body(new HEMeshBaseEdgeBodyC(vert,face))
    {}
    //: Constructor.
    
    HEMeshBaseEdgeC(HEMeshBaseVertexC vert,HEMeshBaseFaceC face);
    //: Constructor.
    
    HEMeshBaseEdgeC(HEMeshBaseEdgeBodyC &bod)
      : body(&bod)
    {}
    //: Body constructor.
    
    HEMeshBaseEdgeBodyC &Body()
    { return *body; }
    //: Access body.
    
    const HEMeshBaseEdgeBodyC &Body() const
    { return *body; }
    //: Access body.

    void SetSelfPointing()
    { body->SetSelfPointing(); }
    //: Set edge pointers to point to itself.
    
  public:
    HEMeshBaseEdgeC Next()
    { return HEMeshBaseEdgeC(Body().Next()); }
    //: Get next edge on face.
    
    HEMeshBaseEdgeC Prev()
    { return HEMeshBaseEdgeC(Body().Prev()); }
    //: Get previous edge on face.
    
    HEMeshBaseEdgeC Next() const
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(Body().Next())); }
    //: Get next edge on face.
    
    HEMeshBaseEdgeC Prev() const
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(Body().Prev())); }
    //: Get previous edge on face.
    
    void LinkAfter(const HEMeshBaseEdgeC &edge) 
    { Body().LinkAfter(const_cast<HEMeshBaseEdgeC &>(edge).Body()); }
    //: Link 'edge' after this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().
    
    void LinkBefore(HEMeshBaseEdgeC &edge) 
    { Body().LinkBefore(edge.Body()); }
    //: Link 'edge' before this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().

    void CutPaste(HEMeshBaseEdgeC firstCut, HEMeshBaseEdgeC firstNotCut) 
    { Body().CutPaste(firstCut.Body(),firstNotCut.Body()); }
    //: Splice edges between firstCut, and firstNotCut into this list.
    // Cuts the chain of edges starting at 'firstCut' and
    // ending at the edge before 'firstNotCut' from the chain.
    // The rest of the chain is linked together again. The cut part
    // is linked in after this edge.
    
    void Unlink()
    { Body().Unlink(); Body().SetSelfPointing(); }
    //: Unlink the edge from a faces edge list.
    
    bool HasPair() const
    { return Body().HasPair(); }
    //: Does this edge have a pair.
    
    void SetPair(HEMeshBaseEdgeC edge)
    { Body().SetPair(edge.Body()); }
    //: Set edge pair.
    
    void SetFace(HEMeshBaseFaceC face);
    //: Set the face associated with the edge.
    
    HEMeshBaseEdgeC Pair() const
    { return HEMeshBaseEdgeC(*Body().pair); }
    //: Edge pair.
    // HasPair() must have return
    // true before using this method.
    
    void SetVertex(HEMeshBaseVertexBodyC &vert)
    { Body().SetVertex(vert); }
    //: Set vertex.
    
    inline HEMeshBaseFaceC Face();
    //: Access the face the edge lies on.
    
    bool HasFace() const 
    { return Body().HasFace(); }
    //: Is there a face associated with this edge ?

    HEMeshBaseEdgeC Pair()
    { return HEMeshBaseEdgeC(Body().Pair()); }
    //: Access edge's apair.
    
    HEMeshBaseVertexC Vertex() const
    { return Body().Vertex(); }
    //: Access vertex this edge goes to
    
    HEMeshBaseVertexC SourceVertex() const 
    { return Prev().Vertex(); }
    //: Access vertex this edge comes from.
    
    SizeT Hash() const
    { return StdHash(static_cast<const void *>(body)); }
    //: Hash value for handle.
    
    bool operator==(const HEMeshBaseEdgeC &oth) const
    { return body == oth.body; }
    //: Is this a handle to the same object ?

    bool operator!=(const HEMeshBaseEdgeC &oth) const
    { return body != oth.body; }
    //: Is this not a handle to the same object ?
    
    bool operator==(const HEMeshBaseEdgeBodyC *oth) const
    { return body == oth; }
    //: Is this a handle to oth ?

    bool operator!=(const HEMeshBaseEdgeBodyC *oth) const
    { return body != oth; }
    //: Is this not a handle to oth ?

    HEMeshBaseVertexC CollapseEdge() { 
      HEMeshBaseVertexC ret = Body().CollapseEdge(); 
      delete body;
      body = 0;
      return ret;
    }
    //: Collapse edge to zero length.
    // This deletes the edge from the mesh and merges the vertex's
    // at either end.  The new vertex is returned. The edge is deleted,
    // and the handle becomes invalid after the call completes.
    
  protected:
    HEMeshBaseEdgeBodyC *body;
    
    friend class HEMeshBaseBodyC;
    friend class HEMeshBaseVertexBodyC;
    friend class HEMeshBaseVertexC;
    friend class HEMeshBaseVertexEdgeIterC;
    friend class HEMeshBaseToVertexEdgeIterC;
    friend class HEMeshBaseFaceC;
    friend class HEMeshBaseFaceEdgeIterC;
    friend class HEMeshBaseFaceBodyC;
    friend class HEMeshBaseEdgeBodyC;
  };
  
  std::ostream &operator<<(std::ostream &strm,const HEMeshBaseEdgeC &vertex);
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,HEMeshBaseEdgeC &vertex);
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const HEMeshBaseEdgeC &vertex);
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,HEMeshBaseEdgeC &vertex);
  //: Binary stream input.
  // Not implemented
  
  //////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Iterate through edges around a vertex. 
  // This goes through all the half edges pointing away from the vertex.
  
  class HEMeshBaseVertexEdgeIterC {
  public:
    HEMeshBaseVertexEdgeIterC()
      : first(0),
	at(0)
    {}
    //: Default constructor.
    
    HEMeshBaseVertexEdgeIterC(HEMeshBaseVertexBodyC &vert) { 
      if(!vert.HasEdge()) {
	at = 0;
	first = 0;
	return ;
      }
      first = &vert.FirstEdge().Pair();
      at = first; 
    }
    //: Construct from a face
    
    HEMeshBaseVertexEdgeIterC(HEMeshBaseVertexC &vert) { 
      RavlAssert(vert.IsValid());
      if(!vert.HasEdge()) {
	first = 0;
	at = 0;
	return ;
      }
      first = &vert.FirstEdge().Pair().Body();
      at = first; 
    }
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
      at = &at->Pair().Next();
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    bool Prev() {
      at = &at->Prev().Pair();
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto previous edge around face.
    // Returns true if the new element is valid.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next edge around face.
    // Returns true if the new element is valid.

    bool operator--(int)
    { return Prev(); }
    //: Goto previous edge around face.
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
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(*at)); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshBaseEdgeC operator *() const
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(*at)); }
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
    
    HEMeshBaseEdgeBodyC *operator ->()
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
  protected:
    HEMeshBaseEdgeBodyC *first;
    HEMeshBaseEdgeBodyC *at;
  };

  /////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Iterate through edges around a vertex. 
  // This goes through all the half edges pointing to a vertex.
  
  class HEMeshBaseToVertexEdgeIterC {
  public:
    HEMeshBaseToVertexEdgeIterC()
      : first(0),
	at(0)
    {}
    //: Default constructor.

    HEMeshBaseToVertexEdgeIterC(HEMeshBaseVertexBodyC &vert)
      : first(&vert.FirstEdge())
    { at = first; }
    //: Construct from a face
    
    HEMeshBaseToVertexEdgeIterC(HEMeshBaseVertexC &vert) { 
      RavlAssert(vert.IsValid());
      if(!vert.HasEdge()) {
	first = 0;
	at = 0;
	return ;
      }
      first = &vert.FirstEdge().Body();
      at = first; 
    }
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
      at = &at->Next().Pair();
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto next edge around face.
    // Returns true if the new element is valid.

    bool Prev() {
      if(!at->HasPair()) {
	at = 0;
	return false;
      }
      at = &at->Pair().Prev();
      if(at == first) {
	at = 0;
	return false;
      }
      return true;
    }
    //: Goto previous edge around face.
    // Returns true if the new element is valid.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next edge around face.
    // Returns true if the new element is valid.
    
    bool operator--(int)
    { return Prev(); }
    //: Goto previous edge around face.
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
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(*at)); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshBaseEdgeC operator *() const
    { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(*at)); }
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
    
    HEMeshBaseEdgeBodyC *operator ->()
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
  protected:
    HEMeshBaseEdgeBodyC *first;
    HEMeshBaseEdgeBodyC *at;
  };

  /////////////////////////////////////////////////////////////////////////
  
  inline
  HEMeshBaseEdgeC HEMeshBaseVertexC::FindEdge(const HEMeshBaseVertexC &oth) const
  { return Body().FindEdge(oth); }

  inline
  HEMeshBaseEdgeC HEMeshBaseVertexC::FirstEdge() const
  { return HEMeshBaseEdgeC(const_cast<HEMeshBaseEdgeBodyC &>(Body().FirstEdge())); }
  
  inline
  void HEMeshBaseEdgeBodyC::LinkAfter(const HEMeshBaseEdgeC &edge) 
  { DLinkC::LinkAft(const_cast<HEMeshBaseEdgeC &>(edge).Body()); }

}



#endif
