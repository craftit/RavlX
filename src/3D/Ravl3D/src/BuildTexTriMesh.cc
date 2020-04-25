// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Ravl3D

#include "Ravl/3D/BuildTexTriMesh.hh"
#include "Ravl/Affine3d.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Triangulate2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Image/ImageConv.hh"

namespace Ravl3DN {
  
  //! Constructor.
  
  BuildTexTriMeshC::BuildTexTriMeshC(UIntT noVerticesEstimate)
    : m_vertices(noVerticesEstimate),
      m_faces(noVerticesEstimate),
      m_textures(Min(noVerticesEstimate/4,(UIntT) 4)),
      m_texFilenames(Min(noVerticesEstimate/4,(UIntT) 4))
  {}
  
  //! Append a new texture, its id is returned.
  
  UIntT BuildTexTriMeshC::AddTexture(const ImageC<ByteRGBValueC> &texture,const StringC &name) {
    UIntT ret = m_textures.Append(texture);
    // Make up a name if one isn't supplied.
    StringC theName = name;
    if(theName == "")
      theName = StringC("Tex") + StringC(ret);
    UIntT texNameId = m_texFilenames.Append(theName);
    RavlAssert(texNameId == ret);
    return texNameId;
  }

  //! Append a new texture, its id is returned.
  //! Note: transparency is not currently supported, but that may change in the future.
  UIntT BuildTexTriMeshC::AddTexture(const ImageC<ByteRGBAValueC> &texture,const StringC &name) {
    return AddTexture(RavlImageN::ByteRGBAImageCT2ByteRGBImageCT(texture),name);
  }

  //! Add face to texture.
  // The texture coorinates are assumed to those of the actual positions in the texture. (NOT scaled 0.0-1.0)

  bool BuildTexTriMeshC::AddFace(UIntT textureId,const SArray1dC<Point3dC> &position3d,const SArray1dC<Point2dC> &texturePoints) {
    if(position3d.Size() < 3)
      return false;
    RavlAssert(texturePoints.Size() == position3d.Size());
    SArray1dC<VertexC *> vertex(position3d.Size());
    Vector3dC normal = Vector3dC(position3d[1] - position3d[2]).Cross(position3d[1] - position3d[0]);
    normal.MakeUnit();
    
    // Add vertices
    if(m_geomStack.IsEmpty()) {
      for(SArray1dIter3C<Point3dC,Point2dC,VertexC *> it(position3d,texturePoints,vertex);it;it++) {
        it.Data3() = &m_vertices[m_vertices.Append(VertexC(it.Data1(),normal))];
      }
    } else {
      for(SArray1dIter3C<Point3dC,Point2dC,VertexC *> it(position3d,texturePoints,vertex);it;it++) {
        it.Data3() = &m_vertices[m_vertices.Append(VertexC((m_geomStack.Top() * PPoint3dC(it.Data1())).Point3d(),normal))];
      }      
    }
    
    Matrix2dC textureCoordScale(0,1,1,0);
    Vector2dC textureCoordOffset(0,0);
    if(textureId < m_textures.Size()) {
      IndexRange2dC frame = m_textures[textureId].Frame();
      textureCoordScale = Matrix2dC(0,1.0/frame.Cols(),
				    1.0/frame.Rows(),0);
      textureCoordOffset = Point2dC(-frame.Origin().Row(),-frame.Origin().Col());
    }
    
    SArray1dC<Index3dC> triangulation;
    if(!PolygonTriangulation(texturePoints,triangulation)) {
      return false;
    }
    for(SArray1dIterC<Index3dC> it(triangulation);it;it++) {
      const int p0 = (*it)[1].V();
      const int p1 = (*it)[0].V();
      const int p2 = (*it)[2].V();
      m_faces.Append(TriC(*vertex[p0],*vertex[p1],*vertex[p2],
			  textureCoordScale * (texturePoints[p0] + textureCoordOffset),
			  textureCoordScale * (texturePoints[p1] + textureCoordOffset),
			  textureCoordScale * (texturePoints[p2] + textureCoordOffset),
			  (UByteT) textureId
			  ));
      m_faces.Last().UpdateFaceNormal();
    }
    
    return true;
  }

  
  //! Add an existing mesh to this one.
  
  void BuildTexTriMeshC::AddMesh(const TexTriMeshC &mesh) {
    SArray1dC<unsigned> old2newVertex(mesh.Vertices().Size());
    SArray1dC<unsigned> old2newTex(mesh.NumTextures());
    
    // Map textures.
    for(RavlN::SArray1dIter3C<unsigned,ImageC<ByteRGBValueC>,StringC > it(old2newTex,mesh.Textures(),mesh.TexFilenames());it;it++) 
      it.Data1() = AddTexture(it.Data2(),it.Data3());
    
    // Map vertices.
    
    if(m_geomStack.IsEmpty()) {
      // No geometry transform needed.
      for(RavlN::SArray1dIter2C<unsigned,VertexC> it(old2newVertex,mesh.Vertices());it;it++) {
        it.Data1() = m_vertices.Append(it.Data2());
      }
    } else {
      for(RavlN::SArray1dIter2C<unsigned,VertexC> it(old2newVertex,mesh.Vertices());it;it++) {
        it.Data1() = m_vertices.Append(it.Data2());
        VertexC &vertex = m_vertices.Last();
        Point3dC oldPos = vertex.Position();
        vertex.Position() = (m_geomStack.Top() * PPoint3dC(oldPos)).Point3d();
        vertex.Normal() = (m_geomStack.Top() * PPoint3dC(Point3dC(oldPos + vertex.Normal()))).Point3d() - oldPos;
      }
    }
    
    // Translate triangles
    for(RavlN::SArray1dIterC<TriC> it(mesh.Faces());it;it++) {
      IntT nt = m_faces.Append(*it);
      TriC &tri = m_faces[nt];
      for(int i = 0;i < 3;i++)
	tri.SetVertexPtr(i,&(m_vertices[old2newVertex[mesh.Index(*it,i)]]));
      tri.SetTextureID(old2newTex[tri.TextureID()]);
    }
  }

  //! Add a transformed mesh to this one.
  
  void BuildTexTriMeshC::AddMesh(const TexTriMeshC &mesh,const Affine3dC &transform) {
    TransformPush(transform);
    AddMesh(mesh);
    TransformPop();
  }

  //! Push a transform onto the stack.
  
  void BuildTexTriMeshC::TransformPush(const Affine3dC &transform) {
    TransformPush(PProjection3dC(transform));
  }
  
  //! Push a transform onto the stack.
  void BuildTexTriMeshC::TransformPush(const PProjection3dC &transform) {
    if(m_geomStack.IsEmpty())
      m_geomStack.Push(transform);
    else
      m_geomStack.Push(m_geomStack.Top() * transform);
  }
  
  
}
