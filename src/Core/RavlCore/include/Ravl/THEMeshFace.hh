// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THEMESHFACE_HEADER
#define RAVL_THEMESHFACE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Graph/THEMeshFace.hh"

#include "Ravl/DLink.hh"
#include "Ravl/HEMeshBaseFace.hh"
#include "Ravl/THEMeshEdge.hh"

namespace RavlN {
  
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshVertexC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceBodyC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceEdgeIterC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshEdgeC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshEdgeBodyC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshBodyC;  
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceIterC;
  
  //! userlevel=Develop
  //: Face in mesh.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshFaceBodyC 
    : public HEMeshBaseFaceBodyC
  {
  public:
    THEMeshFaceBodyC(UIntT nid = 0)
      : HEMeshBaseFaceBodyC(nid)
    {}
    //: Default constructor.

    THEMeshFaceBodyC(const FaceDataT &faceData,UIntT nid = 0)
      : HEMeshBaseFaceBodyC(nid),
	data(faceData)
    {}
    //: Constructor.
    
    THEMeshFaceBodyC(THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> &nedge)
      : HEMeshBaseFaceBodyC(&nedge)
    {}
    //: Default constructor.
    
  protected:
    THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> *FirstEdge()
    { return static_cast<THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> *>(edge); }
    //: Access first edge.
    
  public:        
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> Edge() 
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(*edge); }
    //: Get an edge leading to the face.
    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> FindEdge(THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> a);
    //: Find the edge linking to vertex a.
    // If edge is not found an invalid handle is returned.
    
    FaceDataT &Data()
    { return data; }
    //: Access data.
    
    const FaceDataT &Data() const
    { return data; }
    //: Access data.
    
  private:
    FaceDataT data;
    
    friend class THEMeshFaceEdgeIterC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT>;
  };
  
  //! userlevel=Normal
  //: Face in mesh.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshFaceC 
    : public HEMeshBaseFaceC
  {
  public:
    THEMeshFaceC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    THEMeshFaceC(const HEMeshBaseFaceC &base)
      : HEMeshBaseFaceC(dynamic_cast<const THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> *>(BodyPtr(base)))
    {}
    //: Base constructor.
    
  protected:
    THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body()
    { return static_cast<THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseFaceC::Body()); }
    //: Body access.
    
    const THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body() const
    { return static_cast<const THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseFaceC::Body()); }
    //: Body access.
    
  public:
    FaceDataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const FaceDataT &Data() const
    { return Body().Data(); }
    //: Access data.

  protected:
    THEMeshFaceC(THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &bod)
      : HEMeshBaseFaceC(bod)
    {}
    //: Body constructor.
    
    THEMeshFaceC(const FaceDataT &faceData,UIntT nid = 0)
      : HEMeshBaseFaceC(new THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT>(faceData,nid))
    {}
    //: Construct a new face.
    
    THEMeshFaceC(THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> edge)
      : HEMeshBaseFaceC(new THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT>(edge.Body()))
    {}
    //: Construct a new face.
    
    
#if 0
    THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> *FirstEdge()
    { return Body().FirstEdge(); }
    //: Access first edge.
#endif
  public:    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> Edge() 
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(Body().Edge()); }
    //: Get an edge leading to the face.

#if 0    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> FindEdge(THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> a)
    { return Body().FindEdge(a); }
    //: Find the edge linking to vertex a.
    // If edge is not found an invalid handle is returned.
#endif    
    
  private:
    friend class THEMeshFaceEdgeIterC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshFaceIterC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshC<VertexDataT,FaceDataT,EdgeDataT>;
  };
  
  //! userlevel=Normal
  //: Iterate through edges around a face.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshFaceEdgeIterC 
    : public HEMeshBaseFaceEdgeIterC
  {
  public:
    THEMeshFaceEdgeIterC()
    {}
    //: Default constructor.
    
    THEMeshFaceEdgeIterC(const THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> &face)
      : HEMeshBaseFaceEdgeIterC(face)
    {}
    //: Construct from a face
    
    THEMeshFaceEdgeIterC(THEMeshFaceBodyC<VertexDataT,FaceDataT,EdgeDataT> &face)
      : HEMeshBaseFaceEdgeIterC(face)
    {}
    //: Construct from a face

  protected:
    THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> &At()
    { return static_cast<THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(*at); }
    //: Access destination.

    const THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> &At() const
    { return static_cast<THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(*at); }
    //: Access destination.
    
  public:
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> Data()
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(At()); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> Data() const
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(At()); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> operator *() const
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(At()); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> operator *()
    { return THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>(At()); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    const THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> *operator->() const
    { return &At(); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.
    
    THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT> *operator->()
    { return &At(); }
    //: Access edge.
    // Iterator must be at a valid element
    // before calling this method.

  };

  template<class VertexDataT,class FaceDataT,class EdgeDataT>
  inline THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT> THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>::Face() 
  { return THEMeshFaceC<VertexDataT,FaceDataT,EdgeDataT>(HEMeshBaseEdgeC::Face()); }
  
}

#endif
