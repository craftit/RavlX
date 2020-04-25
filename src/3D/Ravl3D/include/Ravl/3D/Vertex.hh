// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_VERTEX_HEADER
#define RAVL3D_VERTEX_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! docentry="Ravl.API.3D.Mesh"
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/Vertex.hh"

#include "Ravl/Vector3d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {
  using namespace RavlN;  
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::Vector3dC;
  using RavlN::Point3dC;
  using RavlN::BinOStreamC;
  using RavlN::BinIStreamC;
#endif
  
  //! userlevel=Normal
  //: Vertex in a mesh.
  // This holds a position and a normal.
  
  class VertexC {
  public:
    VertexC()
    {}
    //: Default constructor.
    
    VertexC(const Point3dC &pos,const Vector3dC &norm)
      : at(pos),
	normal(norm)
    {}
    //: Constructor from position and normal
    //!param: pos - Position of vertex.
    //!param: norm - Direction of surface normal at vertex.
    
    VertexC(const Point3dC &pos)
      : at(pos)
    {}
    //: Constructor from position
    //!param: pos - Position of vertex.
    // The surface normal is left undefined.
    
    Vector3dC &Position()
    { return at; }
    //: Access position of vertex.
    
    const Vector3dC &Position() const
    { return at; }
    //: Access position of vertex.
    
    Vector3dC &Normal()
    { return normal; }
    //: Access normal at vertex.

    const Vector3dC &Normal() const
    { return normal; }
    //: Access normal at vertex.
    
  protected:
    Vector3dC at;        // Position of vertex.
    Vector3dC normal;    // Normal to vertex.
  };
  
  ostream &operator<<(ostream &s,const VertexC &v);
  istream &operator>>(istream &s,VertexC &v);
  
  BinOStreamC &operator<<(BinOStreamC &s,const VertexC &v);
  BinIStreamC &operator>>(BinIStreamC &s,VertexC &v);

}

#endif
