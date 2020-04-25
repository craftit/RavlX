// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/TriFileIO.cc"

#include "Ravl/3D/TriFileIO.hh"
#include "Ravl/SArray1dIter.hh"

namespace Ravl3DN {
  //: Open file.
  
  DPITriFileBodyC::DPITriFileBodyC(const StringC &fn)
    : inf(fn),
      done(false)
  {}
  
  //: Open file.
  
  DPITriFileBodyC::DPITriFileBodyC(IStreamC &is)
    : inf(is),
      done(false)
  {}
  
  //: Is valid data ?
  
  bool DPITriFileBodyC::IsGetEOS() const {
    return !done && inf;
  }
  
  //: Get next piece of data.
  
  TriMeshC DPITriFileBodyC::Get() {
    if(done)
      return TriMeshC();
    UIntT nvertex,nelement;
    inf >> nvertex >>  nelement;
	
    SArray1dC<VertexC> verts(nvertex);
    for(SArray1dIterC<VertexC> vit(verts);vit;vit++) {
      inf >> vit->Position();
      vit->Normal() = Vector3dC(0,0,0);
    }
    
	SArray1dC<TriC> faces(nelement);
	
    if(nelement==0) return TriMeshC(verts,faces);
	
	UIntT n,i1,i2,i3;
    SArray1dIterC<TriC> it(faces);
    UIntT i = 0;
    for(;i < nelement;i++) {
      inf >> n;
      if(n != 3) { // Dump non-triangular faces.
	for(UIntT i = 0;i < n;i++)
	  inf >> n;
	continue;
      }
      inf >> i1 >> i2 >> i3;
      it->VertexPtr(0) = &(verts[i1]);
      it->VertexPtr(1) = &(verts[i2]);
      it->VertexPtr(2) = &(verts[i3]);
      it->UpdateFaceNormal();
#if 0
      // Give each face a different colour.
      it->Colour()[0] = i % 255;
      it->Colour()[1] = (64 - i) % 255;
      it->Colour()[2] = (128 - i) % 255;
#else
      // Make the model grey.
      it->Colour()[0] = 196;
      it->Colour()[1] = 196;
      it->Colour()[2] = 196;      
#endif
      it++;
    }
  
	// new texture stuff

	// Read in the texture mode
    IntT iPerVertex;
    inf >> iPerVertex;
    if (!inf) {
      // There is no texture info
      TriMeshC ret(verts,faces);
      ret.UpdateVertexNormals();
      done = true;
      return ret;
    }
// ignore texture file
	StringC strTexnames;
    inf.Skip();
    inf >> strTexnames;

    // Read in all the texture coordinates
    IntT iNumTexCoords = (iPerVertex) ? nvertex : nelement*3;
    SArray1dC<Vector2dC> texCoords(iNumTexCoords);
    SArray1dIterC<Vector2dC> tit(texCoords);
    for (; tit; tit++) {
      RealT dX, dY;
      inf >> dX >> dY;
      tit.Data() = Vector2dC(dX,dY);
    }
    // Set the texture coordinates in the faces
    IntT findex;
    const VertexC *x = &(verts[0]);
    for (findex=0, it.First(); it; findex++, it++) {
      TriC& tri = it.Data();
      tri.TextureID() = 0;
      if (iPerVertex) {
	i1 = (tri.VertexPtr(0) - x);
	tri.TextureCoord(0) = texCoords[i1];
	i2 = (tri.VertexPtr(1) - x);
	tri.TextureCoord(1) = texCoords[i2];
	i3 = (tri.VertexPtr(2) - x);
	tri.TextureCoord(2) = texCoords[i3];
      } else {
	tri.TextureCoord(0) = texCoords[findex*3];
	tri.TextureCoord(1) = texCoords[findex*3+1];
	tri.TextureCoord(2) = texCoords[findex*3+2];
      }
    }

	done = true;
    // The faces array is resized in case we had to drop any non trianglular faces.
	
    TriMeshC ret(verts,SArray1dC<TriC>(faces,it.Index().V()));
    ret.SetTextureCoord(1);
	ret.UpdateVertexNormals();
    return ret;
  }

  ////////////////////////////////////////////////////////////////////////////////////////

  //: Open file.
  
  DPOTriFileBodyC::DPOTriFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {}
  
  //: Open file.
  
  DPOTriFileBodyC::DPOTriFileBodyC(OStreamC &is)
    : outf(is),
      done(false)
  {}
  
  //: Put data.
  
  bool DPOTriFileBodyC::Put(const TriMeshC &dat) {
    if(done || !outf)
      return false;
    outf << dat.Vertices().Size() << ' ' << dat.Faces().Size() << '\n';
    for(SArray1dIterC<VertexC> vit(dat.Vertices());vit;vit++)
      outf << vit->Position() << '\n';
    if(dat.Vertices().Size() != 0) {
      const VertexC *x = &(dat.Vertices()[0]);
      for(SArray1dIterC<TriC> it(dat.Faces());it;it++)
	outf << "3 " << (it->VertexPtr(0) - x) << ' ' << (it->VertexPtr(1) - x) << ' ' << (it->VertexPtr(2) - x) << '\n';
    }
	if(dat.HaveTextureCoord()){
	    outf << "0 " << "NoFile.ppm" << '\n';

      // Write the texture coords per face
      for(SArray1dIterC<TriC> fit(dat.Faces()); fit; fit++) {
	outf << fit->TextureCoord(0) << '\n' 
	     << fit->TextureCoord(1) << '\n' 
	     << fit->TextureCoord(2) << '\n';
      }
    }
    return true;
  }
  
  //: Is port ready for data ?
  
  bool DPOTriFileBodyC::IsPutReady() const 
  { return !done && outf; }

}
