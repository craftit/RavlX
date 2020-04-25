// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_HEMESHEDGE_HEADER
#define RAVL3D_HEMESHEDGE_HEADER 1
//! rcsid="$Id$"
//! lib=Ravl3D
//! docentry="Ravl.API.3D.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/HEMeshEdge.hh"

#include "Ravl/DLink.hh"
#include "Ravl/3D/HEMeshVertex.hh"

namespace Ravl3DN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using RavlN::UIntT;
  using RavlN::DLinkC;
#endif
  
  class HEMeshFaceC;
  class HEMeshFaceBodyC;
  class HEMeshC;
  class HEMeshFaceEdgeIterC;
  class HEMeshFaceBodyC;  
  class HEMeshEdgeBodyC;
  class HEMeshBodyC;
  class HEMeshFaceBodyC;
  
  //! userlevel=Develop
  //: Half edge in mesh.
  
  class HEMeshEdgeBodyC 
    : public DLinkC // Link in list of  vertex's around a face.
  {
  public:
    HEMeshEdgeBodyC &Next()
    { return static_cast<HEMeshEdgeBodyC &>(DLinkC::Next()); }
    //: Get next edge on face.
    
    HEMeshEdgeBodyC &Prev()
    { return static_cast<HEMeshEdgeBodyC &>(DLinkC::Prev()); }
    //: Get previous edge on face.

    const HEMeshEdgeBodyC &Next() const
    { return static_cast<const HEMeshEdgeBodyC &>(DLinkC::Next()); }
    //: Get next edge on face.
    
    const HEMeshEdgeBodyC &Prev() const
    { return static_cast<const HEMeshEdgeBodyC &>(DLinkC::Prev()); }
    //: Get previous edge on face.
    
    HEMeshEdgeBodyC &Pair()
    { return *pair; }
    //: Edge pair.
    // HasPair() must have return
    // true before using this method.
    
    const HEMeshEdgeBodyC &Pair() const
    { return *pair; }
    //: Edge pair.
    // HasPair() must have return
    // true before using this method.
    
    bool HasPair() const
    { return pair != 0; }
    //: Does this edge have a pair.
    
    void SetPair(HEMeshEdgeBodyC &edge)
    { pair = &edge; }
    //: Set edge pair.
    
    void SetFace(HEMeshFaceBodyC &aface)
    { face = &aface; }
    //: Set the face associated with the edge.
    
    HEMeshFaceBodyC &Face()
    { return *face; }
    //: Access the face the edge lies on.
    
    void LinkAfter(HEMeshEdgeBodyC &edge) 
    { DLinkC::LinkAft(edge); }
    //: Link 'edge' after this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().
    
    void LinkBefore(HEMeshEdgeBodyC &edge) 
    { DLinkC::LinkBef(edge); }
    //: Link 'edge' before this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().

    void CutPaste(HEMeshEdgeBodyC &firstCut, HEMeshEdgeBodyC &firstNotCut) 
    { DLinkC::CutPaste(firstCut,firstNotCut); }
    //: Splice edges between firstCut, and firstNotCut into this list.
    // Cuts the chain of edges starting at 'firstCut' and
    // ending at the edge before 'firstNotCut' from the chain.
    // The rest of the chain is linked together again. The cut part
    // is linked in after this edge.
    
    HEMeshVertexC Vertex() const
    { return HEMeshVertexC(const_cast<HEMeshVertexBodyC &>(*vertex)); }
    //: Access vertex this edge goes to
    
    HEMeshVertexC SourceVertex() const 
    { return Prev().Vertex(); }
    //: Access vertex this edge comes from.
    
    HEMeshVertexC CollapseEdge();
    //: Collapse edge to zero length.
    // This deletes the edge from the mesh and merges the vertex's
    // at either end.  The remaining vertex is returned.
    
    HEMeshFaceC OpenEdge();
    //: Open an edge, merging the faces on either side.
    // This deletes the edge from the mesh and returns
    // a handle to the new face. <p>
    // NOT IMPLEMENTED
    
    bool operator==(const HEMeshEdgeBodyC &bod) const
    { return this == &bod; }
    //: Is this the same body ?
    
    bool operator!=(const HEMeshEdgeBodyC &bod) const
    { return this != &bod; }
    //: Is this the same body ?
    
  protected:
    
    HEMeshEdgeBodyC()
      : pair(0)
    {}
    //: Default cosntructor.
    // the contents are left undefined.

    HEMeshEdgeBodyC(HEMeshVertexBodyC &vert,HEMeshFaceBodyC &nface)
      : vertex(&vert),
	face(&nface),
	pair(0)
    {}
    //: Cosntructor.
    // the contents are left undefined.
    
    void RemoveHalfEdge();
    //: Remove this half edge.
    // The use must ensure that the matching half is also removed.
    // The memory of for this edge is not free'd, so if it is no
    // longer needed it must be done by the caller.
    
    void CorrectVertexEdgePtr();
    //: Correct edge's vertex pointer.
    // This ensures the vertex's edge pointer doesn't point to this edge.
    // Usefull if your about to delete the edge or restructure the mesh.
    
    HEMeshVertexBodyC *vertex;
    HEMeshFaceBodyC *face;    // Adjacent face.
    HEMeshEdgeBodyC *pair;     // Other edge in pair, is its zero this in an edge
    
    friend class HEMeshEdgeC;
    friend class HEMeshFaceBodyC;
  };
  
  //! userlevel=Normal
  //: Half edge in mesh.
  
  class HEMeshEdgeC {
  public:
    HEMeshEdgeC()
      : body(0)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    bool IsValid() const
    { return body != 0; }
    //: Is this a valid handle ?
    
  protected:
    HEMeshEdgeC(HEMeshVertexBodyC &vert,HEMeshFaceBodyC &face)
      : body(new HEMeshEdgeBodyC(vert,face))
    {}
    //: Constructor.
    
    HEMeshEdgeC(HEMeshVertexC vert,HEMeshFaceC face);
    //: Constructor.
    
    HEMeshEdgeC(HEMeshEdgeBodyC &bod)
      : body(&bod)
    {}
    //: Body constructor.
    
    HEMeshEdgeBodyC &Body()
    { return *body; }
    //: Access body.
    
    const HEMeshEdgeBodyC &Body() const
    { return *body; }
    //: Access body.

  public:
    HEMeshEdgeC Next()
    { return HEMeshEdgeC(Body().Next()); }
    //: Get next edge on face.
    
    HEMeshEdgeC Prev()
    { return HEMeshEdgeC(Body().Prev()); }
    //: Get previous edge on face.
    
    HEMeshEdgeC Next() const
    { return HEMeshEdgeC(const_cast<HEMeshEdgeBodyC &>(Body().Next())); }
    //: Get next edge on face.
    
    HEMeshEdgeC Prev() const
    { return HEMeshEdgeC(const_cast<HEMeshEdgeBodyC &>(Body().Prev())); }
    //: Get previous edge on face.
    
    void LinkAfter(HEMeshEdgeC &edge) 
    { Body().LinkAfter(edge.Body()); }
    //: Link 'edge' after this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().
    
    void LinkBefore(HEMeshEdgeC &edge) 
    { Body().LinkBefore(edge.Body()); }
    //: Link 'edge' before this one.
    // If the edge is already in a chain it MUST
    // be unlinked first with Unlink().

    void CutPaste(HEMeshEdgeC firstCut, HEMeshEdgeC firstNotCut) 
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
    
    void SetPair(HEMeshEdgeC edge)
    { Body().SetPair(edge.Body()); }
    //: Set edge pair.
    
    void SetFace(HEMeshFaceC face);
    //: Set the face associated with the edge.
    
    inline HEMeshFaceC Face();
    //: Access the face the edge lies on.
    
    HEMeshEdgeC Pair()
    { return HEMeshEdgeC(Body().Pair()); }
    //: Access edge's apair.
    
    HEMeshVertexC Vertex() const
    { return Body().Vertex(); }
    //: Access vertex this edge goes to
    
    HEMeshVertexC SourceVertex() const 
    { return Prev().Vertex(); }
    //: Access vertex this edge comes from.
    
    UIntT Hash() const{ 
#if RAVL_CPUTYPE_64
      return ((UInt64T) body) >> 3;
#else
      return ((UIntT) body) >> 3;
#endif
}
    //: Hash value for handle.
    
    bool operator==(const HEMeshEdgeC &oth) const
    { return body == oth.body; }
    //: Is this a handle to the same object ?

    bool operator!=(const HEMeshEdgeC &oth) const
    { return body != oth.body; }
    //: Is this not a handle to the same object ?
    
    bool operator==(const HEMeshEdgeBodyC *oth) const
    { return body == oth; }
    //: Is this a handle to oth ?

    bool operator!=(const HEMeshEdgeBodyC *oth) const
    { return body != oth; }
    //: Is this not a handle to oth ?

    HEMeshVertexC CollapseEdge() { 
      HEMeshVertexC ret = Body().CollapseEdge(); 
      delete body;
      body = 0;
      return ret;
    }
    //: Collapse edge to zero length.
    // This deletes the edge from the mesh and merges the vertex's
    // at either end.  The new vertex is returned. The edge is deleted,
    // and the handle becomes invalid after the call completes.
    
    HEMeshFaceC OpenEdge();
    //: Open an edge, merging the faces on either side.
    // This deletes the edge from the mesh and returns
    // a handle to the new face. <p>
    // NOT IMPLEMENTED
    
  private:
    HEMeshEdgeBodyC *body;
    
    friend class HEMeshBodyC;
    friend class HEMeshVertexBodyC;
    friend class HEMeshVertexC;
    friend class HEMeshVertexEdgeIterC;
    friend class HEMeshToVertexEdgeIterC;
    friend class HEMeshFaceC;
    friend class HEMeshFaceEdgeIterC;
    friend class HEMeshFaceBodyC;
  };
  
  //////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Iterate through edges around a vertex. 
  // This goes through all the half edges pointing away from the vertex.
  
  class HEMeshVertexEdgeIterC {
  public:
    HEMeshVertexEdgeIterC()
      : first(0),
	at(0)
    {}
    //: Default constructor.

    HEMeshVertexEdgeIterC(HEMeshVertexBodyC &vert) { 
      if(!vert.HasEdge()) {
	at = 0;
	first = 0;
	return ;
      }
      first = &vert.FirstEdge().Pair();
      at = first; 
    }
    //: Construct from a face
    
    HEMeshVertexEdgeIterC(HEMeshVertexC &vert) { 
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
    { return at != first && at != 0; }
    //: Is this the first element in list.
    
    bool Next() {
      if (!at->HasPair()) {
	at = 0;
	return false;
      }
      at = &at->Pair().Next();
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
    
    operator bool() const
    { return at != 0; }
    //: Test if we're at a valid element.
    // Returns true if we are.
    
    bool IsElm() const
    { return at != 0; }
    //: Test if we're at a valid element.
    // Returns true if we are.
    
    HEMeshEdgeBodyC &Data()
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshEdgeBodyC &Data() const
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshEdgeBodyC &operator *() const
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshEdgeBodyC &operator *()
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.

    const HEMeshEdgeBodyC *operator ->() const
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshEdgeBodyC *operator ->()
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
  protected:
    HEMeshEdgeBodyC *first;
    HEMeshEdgeBodyC *at;
  };

  /////////////////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Iterate through edges around a vertex. 
  // This goes through all the half edges pointing to a vertex.
  
  class HEMeshToVertexEdgeIterC {
  public:
    HEMeshToVertexEdgeIterC()
      : first(0),
	at(0)
    {}
    //: Default constructor.

    HEMeshToVertexEdgeIterC(HEMeshVertexBodyC &vert)
      : first(&vert.FirstEdge())
    { at = first; }
    //: Construct from a face
    
    HEMeshToVertexEdgeIterC(HEMeshVertexC &vert) { 
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
    { return at != first && at != 0; }
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
    
    bool operator++(int)
    { return Next(); }
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
    
    HEMeshEdgeBodyC &Data()
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshEdgeBodyC &Data() const
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const HEMeshEdgeBodyC &operator *() const
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshEdgeBodyC &operator *()
    { return *at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.

    const HEMeshEdgeBodyC *operator ->() const
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    HEMeshEdgeBodyC *operator ->()
    { return at; }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
  protected:
    HEMeshEdgeBodyC *first;
    HEMeshEdgeBodyC *at;
  };

  /////////////////////////////////////////////////////////////////////////
  
  inline
  HEMeshEdgeC HEMeshVertexC::FindEdge(const HEMeshVertexC &oth) const
  { return Body().FindEdge(oth); }

  inline
  HEMeshEdgeC HEMeshVertexC::FirstEdge() const
  { return HEMeshEdgeC(const_cast<HEMeshEdgeBodyC &>(Body().FirstEdge())); }

}



#endif
