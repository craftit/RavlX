// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/3D/CTriFileIO.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/CTriFileIO.cc"

namespace Ravl3DN {

  //: Open file. 
  DPICTriFileBodyC::DPICTriFileBodyC(const StringC &fn)
    : inf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator ;
  }
    
  //: Is valid data ?
  bool DPICTriFileBodyC::IsGetEOS() const {
    return !done && inf;
  }
  
  //: Get next piece of data.
  TexTriMeshC DPICTriFileBodyC::Get() 
  {
    if (done) return TexTriMeshC();

    // Read the number of vertices and faces
    UIntT nvertex,nelement;
    inf >> nvertex >>  nelement;
    // Read in all the vertex positions
    SArray1dC<VertexC> verts(nvertex);
    SArray1dIterC<VertexC> vit(verts);
    for(;vit;vit++) {
      inf >> vit->Position();
      vit->Normal() = Vector3dC(0,0,0);
    }
    // Read in all the faces
    SArray1dC<TriC> faces(nelement);
    UIntT n,i1,i2,i3;
    SArray1dIterC<TriC> fit(faces);
    for (; fit; fit++) {
      // Read in the number of vertices for this faces
      inf >> n;
      if (n !=3) { // This isn't a tri-mesh
	cerr << "Error: non-triangular element in file\n";
	done = true;
	return TexTriMeshC();
      }
      // Read in the vertex indices
      inf >> i1 >> i2 >> i3;
      fit->VertexPtr(0) = &(verts[i1]);
      fit->VertexPtr(1) = &(verts[i2]);
      fit->VertexPtr(2) = &(verts[i3]);
      fit->UpdateFaceNormal();
    }
    // Read in the texture mode
    IntT iPerVertex;
    inf >> iPerVertex;
    if (!inf) {
      // There is no texture info
      TexTriMeshC ret(verts,faces);
      ret.UpdateVertexNormals();
      done = true;
      return ret;
    }
    // Read the texture filename and load the texture image
    SArray1dC< StringC > strTexnames(1);
    SArray1dC< ImageC<ByteRGBValueC> > textures(1);
    inf.Skip();
    inf >> strTexnames[0];
    if (!Load(path + strTexnames[0],textures[0])) {
      cerr << "Error: could not load image texture\n";
      done = true;
      return TexTriMeshC();
    }
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
    for (findex=0, fit.First(); fit; findex++, fit++) {
      TriC& tri = fit.Data();
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
    // Create a textured tri mesh from the vertices, faces and textures
    TexTriMeshC ret(verts,faces,textures,strTexnames);
    ret.UpdateVertexNormals();
    return ret;
  }

  ////////////////////////////////////////////////////////////////////////////////////////

  //: Open file.
  DPOCTriFileBodyC::DPOCTriFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator ;
  }
    
  //: Put data.
  bool DPOCTriFileBodyC::Put(const TexTriMeshC &dat) 
  {
    if (done || !outf) return false;
    
    // Write the number of vertices and tris
    outf << dat.Vertices().Size() << ' ' << dat.Faces().Size() << '\n';
    if (dat.Vertices().Size()==0 || dat.Faces().Size()==0) {
      done=true;
      return true;
    }
    // Write the vertex positions
    SArray1dIterC<VertexC> vit(dat.Vertices());
    for(; vit; vit++) {
      outf << vit->Position() << '\n';
    }
    // Write the vertex indices for each face
    const VertexC *x = &(dat.Vertices()[0]);
    SArray1dIterC<TriC> fit(dat.Faces());
    for(; fit; fit++) {
      outf << "3 " << (fit->VertexPtr(0) - x) << ' ' 
	   << (fit->VertexPtr(1) - x) << ' ' 
	   << (fit->VertexPtr(2) - x) << '\n';
    }
    // Write the texture info
    if (dat.NumTextures()==1) {
      // Write the texture name and save the texture image
      StringC strFilename = dat.TexFilenames()[0];
      outf << "0 " << strFilename << '\n';
      if (!RavlN::Save(path + strFilename,dat.Textures()[0])) {
	cerr << "Error: Could not save texture\n";
	done = true;
	return false;
      }
      // Write the texture coords per face
      for(fit.First(); fit; fit++) {
	outf << fit->TextureCoord(0) << '\n' 
	     << fit->TextureCoord(1) << '\n' 
	     << fit->TextureCoord(2) << '\n';
      }
    }

    // Done
    return true;
  }
  
  //: Is port ready for data ?
  bool DPOCTriFileBodyC::IsPutReady() const 
  { return !done && outf; }

}
