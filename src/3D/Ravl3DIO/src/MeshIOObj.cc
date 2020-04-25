// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/MeshIOObj.cc"

#include "Ravl/3D/MeshIOObj.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Collection.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/IO.hh"
#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {
  //: Open file.
  
  DPIMeshObjBodyC::DPIMeshObjBodyC(const StringC &fn)
    : inf(fn),
      done(false)
  {}
  
  //: Open file.
  
  DPIMeshObjBodyC::DPIMeshObjBodyC(IStreamC &is)
    : inf(is),
      done(false)
  {}
  
  //: Is valid data ?
  
  bool DPIMeshObjBodyC::IsGetEOS() const 
  { return !done && inf; }
  
  //: Get next piece of data.
  
  TriMeshC DPIMeshObjBodyC::Get() {
    ONDEBUG(cerr << "DPIMeshObjBodyC::Get(), Called \n");
    if(done)
      return TriMeshC();
    TriMeshC ret;
    CollectionC<Vector3dC> vertPos(10000);
    CollectionC<Vector3dC> vertNorm(10000);
    CollectionC<Vector2dC> vertTexture(10000);
    DListC<TFVectorC<TFVectorC<UIntT,2>,3 > > faces;
    
    StringC line;
    UIntT faceCount = 0;
    HashC<Tuple2C<UIntT,UIntT>,UIntT> vertIndex;
    UIntT vertexAlloc = 0;
    
    try {
      while(inf) {
	if(readline(inf,line) <= 0)
	  continue;
	if(line[0] == '#') 
	  continue; // Skip comments.
	ONDEBUG(cerr << "DPIMeshObjBodyC::Get(), Got entry '" << line << "' \n");
	StringListC strs(line);
	DLIterC<StringC> it(strs);
	if(!it) continue; // Empty line.
	if(*it == "v" || *it == "vn") { // Vertex.
	  bool isPos = (*it == "v");
	  Vector3dC v;
	  it++;
	  if(!it) throw ExceptionC("Unexpected end of 3d vertex data. ");
	  v[0] = it->RealValue();
	  it++;
	  if(!it) throw ExceptionC("Unexpected end of 3d vertex data. ");
	  v[1] = it->RealValue();
	  it++;
	  if(!it) throw ExceptionC("Unexpected end of 3d vertex data. ");
	  v[2] = it->RealValue();
	  if(isPos)
	    vertPos.Insert(v);
	  else
	    vertNorm.Insert(v);
	  continue;
	}
	if(*it == "vt") { // Vertex.
	  Vector2dC v;
	  it++;
	  if(!it) throw ExceptionC("Unexpected end of 2d vertex data. ");
	  v[0] = it->RealValue();
	  it++;
	  if(!it) throw ExceptionC("Unexpected end of 2d vertex data. ");
	  v[1] = it->RealValue();
	  vertTexture.Insert(v);
	  continue;
	}
	if(*it == "f") { // Face, save for later.
	  // Decode face indes and save for later.
	  TFVectorC<TFVectorC<UIntT,2>,3 > faceInfo;
	  UIntT vi = 0;
	  for(it++;it;it++,vi++) {
	    int vertNo = -1;
	    int textNo = -1;
	    int normNo = -1;
	    int at = it->index('/');
	    if(at < 0) {
	      vertNo = atoi(*it);
	    } else {
	      SubStringC v1 = it->before(at);
	      vertNo = atoi(v1.chars());
	      SubStringC v2 = it->after(at);
	      int at2 = it->index('/',at+1);
	      if(at2 < 0) {
		textNo = it->after(at).IntValue();
	      } else {
		if((at2-1) > at)
		  textNo = it->after(at).IntValue();
		normNo = it->after(at2).IntValue();
	      }
	    }
	    Tuple2C<UIntT,UIntT> key(vertNo,normNo);
	    UIntT vn;
	    if(!vertIndex.Lookup(key,vn)) {
	      vn = vertexAlloc++;
	      vertIndex[key] = vn;
	    }
	    ONDEBUG(cerr << " " << vertNo << ":" << textNo << ":" << normNo);
	    if(vi > 1) {
	      faceInfo[2][0] = vn;
	      faceInfo[2][1] = textNo;
	      faces.InsLast(faceInfo);
	      faceCount++;
	      faceInfo[1][0] = vn;
	      faceInfo[1][1] = textNo;
	    } else {
	      faceInfo[vi][0] = vn;
	      faceInfo[vi][1] = textNo;
	    }
	  }
	  ONDEBUG(cerr << "\n");
	}
	// Ignore everything else silently.
      }

      SArray1dC<Vector3dC> vertPosArr = vertPos.Array();
      SArray1dC<Vector3dC> vertNormArr = vertNorm.Array();
      SArray1dC<Vector2dC> vertTextArr = vertTexture.Array();
      
      // Build vertex array.
      SArray1dC<VertexC> verts(vertexAlloc);
      for(HashIterC<Tuple2C<UIntT,UIntT>,UIntT> hit(vertIndex);hit;hit++) {
	VertexC &v = verts[hit.Data()];
	UIntT vp = hit.Key().Data1();
	vp -= 1;
	ONDEBUG(cerr << "Vertex " << vp << " (Max=" << vertPosArr.Size() << ")\n");
	if(vp >= vertPosArr.Size())
	  throw ExceptionC("Vertex position index out of range. ");	  
	v.Position() = vertPosArr[vp];
	UIntT vn = hit.Key().Data2() ;
	if(vn != ((UIntT) -1)) {
	  vn -= 1;
	  if(vn >= vertNormArr.Size())
	    throw ExceptionC("Vertex normal index out of range. ");	  
	  v.Normal() = vertNormArr[vn];
	}
      }
      
      // Build triangle faces.
      SArray1dC<TriC> tris(faceCount);
      RavlAssert(faceCount == faces.Size());
      UIntT find = 0;
      for(DLIterC<TFVectorC<TFVectorC<UIntT,2>,3 > > fit(faces);fit;fit++,find++) {
	TriC &tri = tris[find];
	for(int fn = 0;fn < 3;fn++) {
	  //ONDEBUG(cerr << " " << (*fit)[fn][0]);
	  tri.VertexPtr(fn) = &verts[(*fit)[fn][0]];
	  UIntT tind = (*fit)[fn][1];
	  if(tind != ((UIntT) -1))
	    tri.TextureCoords()[fn] = vertTextArr[tind];
	}
	//ONDEBUG(cerr << "\n");
      }
      
      ret = TriMeshC(verts,tris,vertTexture.Size() > 0);
    } catch(ExceptionC &x) {
      cerr << "ERROR Parsing OBJ file : " << x.Text() << "\n";
      cerr << "Load aborted. \n";
      return TriMeshC();  // Return an invalid object.
    } catch(...) {
      cerr << "Exception caught while loading obj file.\n";
      cerr << "Load aborted. \n";
      return TriMeshC();  // Return an invalid object.
    }
    if(vertNorm.Size() == 0)
      ret.UpdateVertexNormals(); // Generate some normals.
    return ret;
  }

  ////////////////////////////////////////////////////////////////////////////////////////

  //: Open file.
  
  DPOMeshObjBodyC::DPOMeshObjBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {}
  
  //: Open file.
  
  DPOMeshObjBodyC::DPOMeshObjBodyC(OStreamC &is)
    : outf(is),
      done(false)
  {}
  
  //: Put data.
  
  bool DPOMeshObjBodyC::Put(const TriMeshC &dat) {
    ONDEBUG(cerr << "DPIMeshObjBodyC::Put(), Called \n");
    if(done || !outf)
      return false;
    outf << "# OBJ file, created by RAVL MeshIO.\n";
    for(SArray1dIterC<VertexC> vit(dat.Vertices());vit;vit++)
      outf << "v " <<  vit->Position() << '\n';
    for(SArray1dIterC<VertexC> vit2(dat.Vertices());vit2;vit2++)
      outf << "vn " <<  vit2->Normal() << '\n';
    if(dat.HaveTextureCoord()) { // With texture co-ordinates ?
      for(SArray1dIterC<TriC> it(dat.Faces());it;it++) {
	for(int i = 0;i < 3;i++)
           outf << "vt " << it->TextureCoord(i).Col() << " " << 1 - it->TextureCoord(i).Row() << "\n";
      }
      for(SArray1dIterC<TriC> itf(dat.Faces());itf;itf++) {
	outf << "f ";
        IntT at = (itf.Index().V() * 3) + 1;
	for(int i = 0;i < 3;i++) {
	  IntT ind = dat.Index(*itf,i) + 1;
	  // Vertex/Texture/Normal
	  outf << ind << '/' << (at+i) << '/' << ind << ' '; 
	}
 	outf << "\n";
      }
    } else {
      for(SArray1dIterC<TriC> it(dat.Faces());it;it++) {
	outf << "f ";
	for(int i = 0;i < 3;i++) {
	  IntT ind = dat.Index(*it,i) + 1;
	  // Vertex/Texture/Normal
	  outf << ind << "//" << ind << ' '; 
	}
        outf << "\n";
      }
    }
      
    return true;
  }
  
  //: Is port ready for data ?
  
  bool DPOMeshObjBodyC::IsPutReady() const 
  { return !done && outf; }

  ////////////////////////////////////////////////////////////////////////////////////////

  //: Open file.
  
  DPOTexMeshObjBodyC::DPOTexMeshObjBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {
    base = FilenameC(fn).BaseNameComponent();
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator;
  }
  
  //: Open file.
  
  DPOTexMeshObjBodyC::DPOTexMeshObjBodyC(OStreamC &is)
    : outf(is),
      done(false),
      base("ravl"),
      path("./")
  {}
  
  //: Put data.
  
  bool DPOTexMeshObjBodyC::Put(const TexTriMeshC &dat) {
    ONDEBUG(cerr << "DPoTexMeshObjBodyC::Put(), Called \n");
    if(done || !outf)
      return false;

    // Create mtl file
    OStreamC mtlfile(path + base + ".mtl");
    if (mtlfile)
       mtlfile << "# MTL file, created by RAVL MeshIO.\n";

    // Write materials and textures
    for (SArray1dIter2C<ImageC<ByteRGBValueC>,StringC> it(dat.Textures(),dat.TexFilenames()); it; it++) {
      // Write the texture name and save the texture image
      StringC strFilename = it.Data2();
      if (!RavlN::Save(path + strFilename,it.Data1())) {
	cerr << "Error: Could not save texture";
      }
      if (mtlfile) {
         mtlfile << "newmtl " << FilenameC(it.Data2()).BaseNameComponent() << "\n";
         mtlfile << "Kd 1 1 1\n";
         mtlfile << "Ns 0\n";
         mtlfile << "Ks 0 0 0\n";
         mtlfile << "illum 2\n";
         mtlfile << "map_Kd " << it.Data2() << "\n\n";
      }    
    }

    // start writing file
    outf << "# OBJ file, created by RAVL MeshIO.\n";
    // Write mtllib
    if(dat.HaveTextureCoord())
       outf << "mtllib " << base << ".mtl\n";
    // Write vertices
    for(SArray1dIterC<VertexC> vit(dat.Vertices());vit;vit++)
      outf << "v " <<  vit->Position() << '\n';
    // Write normals
    for(SArray1dIterC<VertexC> vit2(dat.Vertices());vit2;vit2++)
      outf << "vn " <<  vit2->Normal() << '\n';
    // Are there texcoords?
    if(dat.HaveTextureCoord()) {
       // Write texcoords
       for(SArray1dIterC<TriC> it(dat.Faces());it;it++) {
          for(int i = 0;i < 3;i++)
             outf << "vt " << it->TextureCoord(i).Col() << " " << 1 - it->TextureCoord(i).Row() << "\n";
       }
       // Write group
       outf << "g object\n";
       IntT iLastMaterial = -1;
       // Write faces
       for(SArray1dIterC<TriC> itf(dat.Faces());itf;itf++) {
          if (itf->TextureID() != iLastMaterial) {
             iLastMaterial = itf->TextureID();
             outf << "usemtl " << FilenameC(dat.TexFilenames()[iLastMaterial]).BaseNameComponent() << "\n";
          }
          outf << "f ";
          IntT at = (itf.Index().V() * 3) + 1;
          for(int i = 0;i < 3;i++) {
             IntT ind = dat.Index(*itf,i) + 1;
             // Vertex/Texture/Normal
             outf << ind << '/' << (at+i) << '/' << ind << ' '; 
          }
          outf << "\n";
       }
    } else {
       // Write faces
       for(SArray1dIterC<TriC> it(dat.Faces());it;it++) {
          outf << "f ";
          for(int i = 0;i < 3;i++) {
             IntT ind = dat.Index(*it,i) + 1;
             // Vertex/Texture/Normal
             outf << ind << "//" << ind << ' '; 
          }
          outf << "\n";
       }
    }
      
    return true;
  }
  
  //: Is port ready for data ?
  
  bool DPOTexMeshObjBodyC::IsPutReady() const 
  { return !done && outf; }

}
