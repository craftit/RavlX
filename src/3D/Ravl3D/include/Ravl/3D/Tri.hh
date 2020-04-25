// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL3D_TRI_HEADER
#define RAVL3D_TRI_HEADER 1
/////////////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.3D.Mesh"
//! lib=Ravl3D
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/Tri.hh"

#include "Ravl/3D/Vertex.hh"
#include "Ravl/Point2d.hh"

namespace Ravl3DN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using RavlN::TFVectorC;
  using RavlN::Vector2dC;
  using RavlN::ByteT;
  using RavlN::UByteT;
#endif
  //! userlevel=Normal
  //: Triangle in TriMesh.
  // Tri's must be associated with a tri set, in the parent
  // triset goes out of scope the behavour of a tri from
  // it is undefined.
  
  class TriC 
  {
  public:
    TriC() 
      : normal(0,0,0)
    {
      colour[0] = 196;
      colour[1] = 196;
      colour[2] = 196;
    }
    //: Default constructor.
    
    TriC(const TFVectorC<VertexC *,3> &v)
      : vertices(v),
	normal(0,0,0)
    {
      colour[0] = 196;
      colour[1] = 196;
      colour[2] = 196;
    }
    //: Construct from another vector.
    
    TriC(VertexC &v0,VertexC &v1,VertexC &v2) 
      : normal(0,0,0)
    {
      vertices[0] = &v0;
      vertices[1] = &v1;
      vertices[2] = &v2;
      colour[0] = 196;
      colour[1] = 196;
      colour[2] = 196;
    }
    //: Construct from vertices.

    TriC(VertexC &v0,VertexC &v1,VertexC &v2,
	 const Point2dC &tex0,const Point2dC &tex1, const Point2dC &tex2,
	 UByteT texId = 0
	 )
      : normal(0,0,0)
    {
      vertices[0] = &v0;
      vertices[1] = &v1;
      vertices[2] = &v2;
      texture[0] = tex0;
      texture[1] = tex1;
      texture[2] = tex2;
      textureID = texId;
      colour[0] = 196;
      colour[1] = 196;
      colour[2] = 196;
    }
    //: Construct from vertices and texture coordinates.
    
    TriC(const TFVectorC<VertexC *,3> &v, 
	 const TFVectorC<Vector2dC,3> &t, 
	 UByteT texID)
      : vertices(v), 
	textureID(texID), 
	texture(t),
	normal(0,0,0)
    {
      colour[0] = 196;
      colour[1] = 196;
      colour[2] = 196;
    }
    //: Construct from vertices, texture co-ords, and texture ID.

    void Flip(void);
    //: Flips the triangle.
    // Reverse the order of the vertices in the triangle.
    
    VertexC &Vertex(UIntT ind) {
      RavlAssert(ind < 3);
      return *(vertices[ind]);
    }
    //: Access vertex.
    
    const VertexC &Vertex(UIntT ind) const 
    { return *(vertices[ind]); }
    //: Access vertex.
    
    const Vector3dC &operator[](UIntT ind) const
    { return vertices[ind]->Position(); }
    //: Access position of vertex.

    Vector3dC &operator[](UIntT ind)
    { return vertices[ind]->Position(); }
    //: Access position of vertex.
    
    const Vector3dC &FaceNormal() const 
    { return normal; }
    //: Unit normal orthogonal to triangle plane
    
    Vector3dC &FaceNormal()
    { return normal; }
    //: Unit normal orthogonal to triangle plane
    
    void SetFaceNormal(const Vector3dC &val)
    { normal = val; }
    //: Update the face normal.
    
    Vector3dC &Normal(UIntT n) 
    { return vertices[n]->Normal(); }
    //: Access normal for a vertex.
    
    const Vector3dC Normal(UIntT n) const
    { return vertices[n]->Normal(); }
    //: Access normal for a vertex.

    Vector3dC &Position(UIntT n) 
    { return vertices[n]->Position(); }
    //: Access normal for a vertex.
    
    const Vector3dC &Position(UIntT n) const
    { return vertices[n]->Position(); }
    //: Access normal for a vertex.
    
    void UpdateFaceNormal();
    //: Update the face normal.
    
    VertexC *&VertexPtr(UIntT n)
    { return vertices[n]; }
    //: Access vertex pointer.
    // Advanced users only.
    
    void SetVertexPtr(UIntT n,VertexC *vp)
    { vertices[n] = vp; }
    //: Access vertex pointer.
    // Advanced users only.
    
    VertexC *VertexPtr(UIntT n) const
    { return vertices[n]; }
    //: Access vertex pointer.
    // Advanced users only.
    
    UByteT& TextureID()
    { return textureID; }
    //: Access the texture ID.
    
    UByteT TextureID() const
    { return textureID; }
    //: Access the texture ID.
    
    void SetTextureID(UByteT id)
    { textureID = id; }
    //: Set the texture id.
    
    Vector2dC &TextureCoord(UIntT n) 
    { return texture[n]; }
    //: Access texture co-ordinates.
    
    void SetTextureCoord(UIntT n,const Vector2dC &tc) 
    { texture[n] = tc; }
    //: Access texture co-ordinates.
    
    const Vector2dC &TextureCoord(UIntT n) const
    { return texture[n]; }
    //: Access texture co-ordinates.
    
    TFVectorC<Vector2dC,3> &TextureCoords() 
    { return texture; }
    //: Access texture co-ordinates.
    
    const TFVectorC<Vector2dC,3> &TextureCoords() const
    { return texture; }
    //: Access texture co-ordinates.
    
    TFVectorC<ByteT,3> &Colour()
    { return colour; }
    //: Colour of face.
    
    void SetColour(const TFVectorC<ByteT,3> &col)
    { colour = col; }
    //: Set colour of face.
    
    const TFVectorC<ByteT,3> &Colour() const
    { return colour; }
    //: Colour of face.
    
  protected:
    TFVectorC<VertexC *,3> vertices;
    UByteT textureID;
    TFVectorC<Vector2dC,3> texture;
    Vector3dC normal;
    TFVectorC<ByteT,3> colour;
  };

}

#endif
