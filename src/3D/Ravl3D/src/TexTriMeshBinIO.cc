// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Exception.hh"
#include "Ravl/EntryPnt.hh"
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/3D/Mesh/TexTriMeshBinIO.cc"
//! lib=Ravl3D
//! docentry="Ravl.3D.Mesh"
//! author="Jonathan Starck"

namespace Ravl3DN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using namespace RavlImageN;
#endif
  
  BinOStreamC &operator<<(BinOStreamC &s,const TexTriMeshC &ts) {
    UByteT version = 0;
    s << version;
    RavlAssert(ts.IsValid());
    // Write the mesh info
    s << ts.Vertices(); 
    s << (IntT)ts.HaveTextureCoord();
    s << ts.Faces().Size(); 
    const VertexC *x = &(ts.Vertices()[0]);
    SArray1dIterC<TriC> it(ts.Faces());
    for(; it; it++) {
      s << (int) (it->VertexPtr(0) - x) 
	<< (int) (it->VertexPtr(1) - x) 
	<< (int) (it->VertexPtr(2) - x);
      s << it->TextureID();
      s << it->TextureCoords();
      s << it->Colour();
    }
    // Write the texture info
    s << ts.TexFilenames();
    s << ts.Textures();
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,TexTriMeshC &ts) {
    UByteT version;
    s >> version;
    if(version != 0) {
      if(!UsingRavlMain()) 
	// We're probably not going to catch an exception so write an error on stdout as well. 
	cerr << "ERROR: Unknown version number in TriMeshC binary stream. ";
      throw ExceptionOutOfRangeC("Unknown version number in TriMeshC binary stream. ");
    }
    // Read the mesh info
    SArray1dC<VertexC> vecs;
    s >> vecs;
    IntT iHaveTexture;
    s >> iHaveTexture;
    if (!iHaveTexture) {
      if(!UsingRavlMain()) 
	// We're probably not going to catch an exception so write an error on stdout as well. 
	cerr << "ERROR: No texture in TexTriMeshC binary stream. ";
      throw ExceptionOutOfRangeC("No texture in TexTriMeshC binary stream. ");
    }
    UIntT nfaces,i1,i2,i3;
    s >> nfaces;
    SArray1dC<TriC> faces(nfaces);
    SArray1dIterC<TriC> it(faces);
    for(; it; it++) {
      s >> i1 >> i2 >> i3;
      s >> it->TextureID();
      s >> it->TextureCoords();
      s >> it->Colour();
      it->VertexPtr(0) = &(vecs[i1]);
      it->VertexPtr(1) = &(vecs[i2]);
      it->VertexPtr(2) = &(vecs[i3]);
      it->UpdateFaceNormal();
    }
    // Read the texture info
    SArray1dC< StringC > strFilenames;
    s >> strFilenames;
    SArray1dC< ImageC<ByteRGBValueC> > textures;
    s >> textures;
    // Create the textured tri mesh
    ts = TexTriMeshC(vecs,faces,textures,strFilenames);
    return s;
  }
  
}
