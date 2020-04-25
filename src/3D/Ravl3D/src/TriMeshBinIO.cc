// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/TriMeshBinIO.cc"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Exception.hh"
#include "Ravl/EntryPnt.hh"

namespace Ravl3DN {
  
  BinOStreamC &operator<<(BinOStreamC &s,const VertexC &v) {
    s << v.Position() << v.Normal();
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,VertexC &v) {
    s >> v.Position() >> v.Normal();
    return s;    
  }
  
  BinOStreamC &operator<<(BinOStreamC &s,const TriMeshC &ts) {
    UByteT version = 0;
    s << version;
    RavlAssert(ts.IsValid());
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
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,TriMeshC &ts) {
    UByteT version;
    s >> version;
    if(version != 0) {
      if(!UsingRavlMain()) 
	// We're probably not going to catch an exception so write an error on stdout as well. 
	cerr << "ERROR: Unknown version number in TriMeshC binary stream. ";
      throw ExceptionOutOfRangeC("Unknown version number in TriMeshC binary stream. ");
    }
    SArray1dC<VertexC> vecs;
    s >> vecs;
    IntT iHaveTexture;
    s >> iHaveTexture;
    bool bHaveTexture = (iHaveTexture) ? true : false;
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
    ts = TriMeshC(vecs,faces,bHaveTexture);
    return s;
  }
  
}
