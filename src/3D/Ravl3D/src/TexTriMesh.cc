// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/3D/Mesh/TexTriMesh.cc"
//! lib=Ravl3D
//! docentry="Ravl.3D.Mesh"
//! author="Jonathan Starck"

#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using namespace RavlImageN;
#endif

  //: Copy constructor
  TexTriMeshBodyC::TexTriMeshBodyC(const TexTriMeshBodyC& oth)
  {
    // Copy the data in TriMeshBody
    haveTexture = oth.haveTexture;
    vertices = oth.vertices.Copy();
    faces = SArray1dC<TriC>(oth.faces.Size());
    SArray1dIter2C<TriC,TriC> it(faces,oth.faces);
    for(; it; it++) {
      int i;
      for(i=0 ; i<3; i++) {
	it.Data1().VertexPtr(i) = &(vertices[oth.Index(it.Data2(),i)]);
      }
      it.Data1().TextureID() = it.Data2().TextureID();
      it.Data1().TextureCoords() = it.Data2().TextureCoords();
      it.Data1().Colour() = it.Data2().Colour();
      it.Data1().FaceNormal() = it.Data2().FaceNormal();
    }
    // Copy the textures
    m_strFilenames = oth.m_strFilenames.Copy();
    m_textures = SArray1dC< ImageC<ByteRGBValueC> >(oth.m_textures.Size());
    SArray1dIter2C<ImageC<ByteRGBValueC>,ImageC<ByteRGBValueC> > itTex(m_textures,oth.m_textures);
    for(; itTex; itTex++) {
      itTex.Data1() = itTex.Data2().Copy();
    }    
  }

  //: Automatically generate texture coordinates and a texture image
  bool TexTriMeshBodyC::GenerateTextureMap(void)
  { 
    ONDEBUG(cerr << "Generating a texture map...\n"); 
    
    // Create coordinates.
    GenerateTextureCoords();
    
    // Create the texture image
    m_textures = SArray1dC< ImageC<ByteRGBValueC> >(1);
    m_textures[0] = ImageC<ByteRGBValueC>(1024,1024);
    m_strFilenames = SArray1dC< StringC >(1);
    m_strFilenames[0] = StringC("texture.ppm");
    
    // Done
    return true;
  }

  ostream &operator<<(ostream &s,const TexTriMeshC &ts) 
  {
    RavlAssert(ts.IsValid());
    // Write the mesh info
    s << ts.Vertices(); 
    s << (IntT)ts.HaveTextureCoord() << '\n';
    s << ts.Faces().Size() << '\n'; 
    const VertexC *x = &(ts.Vertices()[0]);
    SArray1dIterC<TriC> it(ts.Faces());
    for(; it; it++) {
      s << (it->VertexPtr(0) - x) << ' ' 
	<< (it->VertexPtr(1) - x) << ' ' 
	<< (it->VertexPtr(2) - x) << ' ';
      s << (IntT)it->TextureID() << ' ';
      s << it->TextureCoords() << ' ';
      s << it->Colour() << '\n';
    }
    // Write the texture info
    s << ts.TexFilenames();
    s << ts.Textures();
    return s;
  }
  
  istream &operator>>(istream &s, TexTriMeshC &ts) 
  {
    // Read the mesh info
    SArray1dC<VertexC> verts;
    s >> verts;
    IntT iHaveTexture;
    s >> iHaveTexture;
    bool bHaveTexture = (iHaveTexture) ? true : false;
    UIntT nfaces,i1,i2,i3;
    s >> nfaces;
    SArray1dC<TriC> faces(nfaces);
    for(SArray1dIterC<TriC> it(faces);it;it++) {
      s >> i1 >> i2 >> i3;
      s >> it->TextureID();
      s >> it->TextureCoords();
      s >> it->Colour();
      it->VertexPtr(0) = &(verts[i1]);
      it->VertexPtr(1) = &(verts[i2]);
      it->VertexPtr(2) = &(verts[i3]);
      it->UpdateFaceNormal();
    }
    // If there is no texture info generate a texture
    if (!bHaveTexture) {
      ts = TexTriMeshC(verts,faces);
      return s;
    }
    // Read the texture info
    UIntT nTex;
    s >> nTex;
    s.ignore(8,'\n');
    SArray1dC< StringC > strFilenames(nTex);
    SArray1dIterC<StringC> strit(strFilenames);
    for(; strit; strit++) {
      s >> strit.Data();
    }
    SArray1dC< ImageC<ByteRGBValueC> > textures;
    s >> textures;
    // Create the textured tri mesh
    ts = TexTriMeshC(verts,faces,textures,strFilenames);
    return s;
  }

}
