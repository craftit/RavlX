// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THEMESHVERTEX_HEADER 
#define RAVL_THEMESHVERTEX_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Graphs.Half Edge Mesh"
//! date="24/4/2002"
//! author="Charles Galambos"
//! file="Ravl/Core/Container/Graph/THEMeshVertex.hh"

#include "Ravl/HEMeshBaseVertex.hh"
#include "Ravl/Empty.hh"

namespace RavlN {
  
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshVertexC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshEdgeC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshFaceEdgeIterC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshEdgeBodyC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshBodyC;
  template<class VertexDataT,class FaceDataT,class EdgeDataT> class THEMeshVertexIterC;
  
  //! userlevel=Develop
  //: Vertex body.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>  
  class THEMeshVertexBodyC
    : public HEMeshBaseVertexBodyC // Link in list of all vertexes
  {
  public:    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> FindEdge(const THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> &oth) const;
    //: Look for a connection from this vertex to oth.
    // Returns an invalid handle if ones is not found.
    
    VertexDataT &Data()
    { return data; }
    //: Access data.
    
    const VertexDataT &Data() const
    { return data; }
    //: Access data.
    
  protected:
    THEMeshVertexBodyC(UIntT nid = 0)
      : HEMeshBaseVertexBodyC(nid)
    {}
    //: Default constructor.

    THEMeshVertexBodyC(const VertexDataT &ndata,UIntT nid = 0)
      : HEMeshBaseVertexBodyC(nid),
	data(ndata)
    {}
    //: Construct from a vertex data.
    
    VertexDataT data;
    
    friend class THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT>;
  };
  
  //! userlevel=Normal
  //: Vertex.
  
  template<class VertexDataT,class FaceDataT = EmptyC,class EdgeDataT = EmptyC>
  class THEMeshVertexC 
    : public HEMeshBaseVertexC // Link in list of all vertexes
  {
  public:
    THEMeshVertexC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT> FindEdge(const THEMeshVertexC<VertexDataT,FaceDataT,EdgeDataT> &oth) const;
    //: Look for a connection from this vertex to oth.
    // Returns an invalid handle if ones is not found.
    
    THEMeshVertexC(const HEMeshBaseVertexC &base)
      : HEMeshBaseVertexC(dynamic_cast<const THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> *>(BodyPtr(base)))
    {}
    //: Base constructor.
    
  protected:
    
    THEMeshVertexC(THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> &nbody)
      : HEMeshBaseVertexC(nbody)
    {}
    //: Body constructor.
    
    THEMeshVertexC(const VertexDataT &data,UIntT nid = 0)
      : HEMeshBaseVertexC(*new THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT>(data,nid))
    {}
    //: Constructor.
    
    THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body()
    { return static_cast<THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseVertexC::Body()); }
    //: Access body.
    
    const THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> &Body() const
    { return static_cast<const THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT> &>(HEMeshBaseVertexC::Body()); }
    //: Access body.
    
  public:
    
    VertexDataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const VertexDataT &Data() const
    { return Body().Data(); }
    //: Access data.
    
  private:
    friend class THEMeshBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshVertexBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshEdgeC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshEdgeBodyC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshVertexIterC<VertexDataT,FaceDataT,EdgeDataT>;
    friend class THEMeshC<VertexDataT,FaceDataT,EdgeDataT>;
  };
  
  
}
#endif
