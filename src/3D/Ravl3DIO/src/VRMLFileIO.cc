// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/VRMLFileIO.cc"

#include "Ravl/3D/VRMLFileIO.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/HSet.hh"
#include "Ravl/SubStringList.hh"
#include "Ravl/DListExtra.hh"
#include "Ravl/Index3d.hh"
#include "Ravl/Array2dIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  void GetValue(const StringC &str, RealT &val) {
    val = str.RealValue();
  }

  void GetValue(const StringC &str, IndexC &val) {
    val = str.IntValue();
  }

  //: Get list of 3d vectors.
  template<class VectorT>
  static bool GetVectors(IStreamC &inf, DListC<VectorT> &ret,
                         UIntT &count, bool negSep = false)
  {
    //cerr << "GetVectors called\n";
    inf.SkipTo('[');
    count = 0;
    VectorT tmp;
    UIntT elements = tmp.Size();
    if(negSep)
      elements++;
    UIntT i = 0;
    UIntT discard = elements - 1;
    while(inf)
    {
      if(!inf.Skip(" \t\n,")) // skip delimeters and check state of the stream
      {
        i = 1; //set error flag
        cerr << "error\n";
        break;
      }
      StringC word = inf.ClipWord(" \t\n,]",false).TopAndTail();
      //cerr << "word:" << word << endl;
      char let = inf.GetChar();
      //cerr << "let:" << int(let) << "   " << let << endl;
      if(!word.IsEmpty()) {
        if(!(negSep && discard == i))
          GetValue(word, tmp[i]);
        i++;
      }
      if(i == elements) {
        ret.InsLast(tmp);
        //cerr << "[" << tmp << "] ";
        count++;
        i = 0;
        //cerr << "count:" << count << "   vert:" << tmp << endl;
      }
      if(let == ']') { // Done ?
        //cerr << "\n";
        //cerr << "Done:" << count << endl;
        break;
      }
    }
    //cerr << "Error!!!\n";
    if(i != 0)
    {
      cerr << "i:" << i << endl;
      throw ExceptionEndOfStreamC("Unexpected end of stream.");
    }
    return true;
  }

  static bool ReadVRML(IStreamC &inf, const StringC &path, TriMeshC &mesh)
  {
    ONDEBUG(cerr << "----------   ReadVRML called\n");
    DListC<Vector3dC> verts; //3d vertices
    DListC<Vector3dC> norms;
    DListC<Vector2dC> tex;   // vertices in texture
    DListC<Index3dC> triVert;// 3d faces
    DListC<Index3dC> triTex; // texture faces
    ImageC<ByteRGBValueC> texImage;

    UIntT numVerts = 0;
    UIntT numNorms = 0;
    UIntT numTexCoord = 0;
    UIntT numFaces = 0;
    UIntT numFacesTex = 0;
    StringC texFile;
    bool eosOk = false; // it is ok to get EndOfStream exception
    try
    {
      // read stream
      while(inf)
      {
        eosOk = true;
        if(!inf.Skip())
        {
          //cerr << "end of stream\n";
          break;
        }
        eosOk = false;
        StringC word = inf.ClipWord(" \t\n",true);
        if(word == "ImageTexture")
        {
          if(!inf.SkipTo('{'))
            continue;
          StringC texType = inf.ClipWord(" \t\n",true);
          if(!inf.SkipTo('\"')) {
            cerr << "ERROR: Failed to find quoted texture name. \n";
            continue;
          }
          texFile = path + inf.ClipTo('\"');

          ONDEBUG(cerr << "Loading texture map '" << texFile << "'\n");
           if(!RavlN::Load(texFile,texImage)) {
            cerr << "WARNING: Failed to load texture map '" << texFile << "'\n";
          }

          inf.SkipTo('}'); // Skip to end of texture block.
        }
        else if(word == "Coordinate" || word == "Coordinate3")
        {
          ONDEBUG(cerr << "Found vertexs. \n");
          GetVectors(inf, verts, numVerts);
        }
        else if(word == "Normal")
        {
          ONDEBUG(cerr << "Found normals \n");
          GetVectors(inf, norms, numNorms);
        }
        else if(word == "TextureCoordinate" || word == "TextureCoordinate2")
        {
          ONDEBUG(cerr << "Found texture coordinates \n");
          GetVectors(inf, tex, numTexCoord);
        }
        else if(word == "coordIndex")
        {
          ONDEBUG(cerr << "Found face index \n");
          GetVectors(inf, triVert, numFaces, true);
        }
        else if(word == "texCoordIndex" || word == "textureCoordIndex")
        {
          ONDEBUG(cerr << "Found face texture index \n");
          GetVectors(inf, triTex, numFacesTex, true);
        }
        else
        {
          //cerr << "Skipping:" << word << endl;
        }
      }
    } catch (ExceptionEndOfStreamC &exc) {
      if(!eosOk)
      {
        cerr << "ERROR: Unexpected end of stream. \n" << exc.Text() << endl;
        throw;
      }
    } catch( ExceptionOutOfRangeC &exc) {
      cerr << "ERROR: Unexpected vector size. \n" << exc.Text() << endl;
      return false;
    }

    // sort out what has been read
    ONDEBUG(cerr << "Vertices=" << numVerts << " Faces=" << numFaces << "\n");

    // Setup vertices.
    SArray1dC<VertexC> vertArray(numVerts);
    SArray1dIterC<VertexC> vait(vertArray);

    if(norms.IsEmpty()) {
      // just setup positions.
      ONDEBUG(cerr << "Creating vertex table. \n");
      for(DLIterC<Vector3dC> vit(verts); vit; vit++, vait++)
        vait->Position() = *vit;
    } else {
      ONDEBUG(cerr << "Creating vertex table using given normals. \n");
      // Setup positions and normals
      if(numVerts != numNorms) {
        cerr << "ERROR: Mismatch in number of vertices and number of normals. \n";
        return false;
      }
      vait.First();
      DLIterC<Vector3dC> vit(verts);
      DLIterC<Vector3dC> nit(norms);
      for(; vait; nit++, vit++, vait++) {
        vait->Position() = *vit;
        vait->Normal() = *nit;
      }
    }

    // Setup tri's
    SArray1dC<Vector2dC> texPos = SArrayOf(tex);
    tex.Empty(); // Free up some memory.

    SArray1dC<TriC> triArray(numFaces);
    SArray1dIterC<TriC> tait(triArray);
    ONDEBUG(cerr << "numFaceTex=" << numFacesTex << " numTexCoord=" << numTexCoord << "\n");

    if(!triTex.IsEmpty()) {
      ONDEBUG(cerr << "Creating tri table using texture coords. \n");
      if(numFaces > numFacesTex) {
        cerr << "ERROR: Mismatch in number of faces and number of texture coords. \n";
        return false;
      }
      if(numFaces != numFacesTex) {
        cerr << "WARNING: Mismatch in number of faces (" << numFaces << ") and number of texture coords.(" << numFacesTex << ") \n";
        return false;
      }

      DLIterC<Index3dC> tit(triVert);
      DLIterC<Index3dC> ttit(triTex);
      for(; tait; tait++, tit++, ttit++) {
        for(int i = 0; i < 3; i++) {
          tait->VertexPtr(i) = &(vertArray[(*tit)[i]]);
          tait->TextureCoord(i) = texPos[(*ttit)[i]];
        }
      }
    } else {
      ONDEBUG(cerr << "Creating tri table. \n");
      DLIterC<Index3dC> tit(triVert);
      for(; tait; tait++, tit++) {
        for(int i = 0; i < 3; i++)
          tait->VertexPtr(i) = &(vertArray[(*tit)[i]]);
      }
    }

    // Create a textured mesh.
    if(numTexCoord > 0) {
      ONDEBUG(cerr << "Creating textured mesh.\n");
      SArray1dC<ImageC<ByteRGBValueC> > textures(1);
      textures[0] = texImage;
      SArray1dC<StringC> texFiles(1);
      texFiles[0] = texFile;
      mesh = TexTriMeshC(vertArray, triArray, textures, texFiles);
    } else {
      ONDEBUG(cerr << "Creating mesh.\n");
      mesh = TriMeshC(vertArray, triArray);
    }

    // If there were no normals provided, generate some.
    if(norms.IsEmpty())
      mesh.UpdateVertexNormals();

    return true;
  }


  // TriMesh VRML File IO /////////////////////////////////////////////
  //: Open file.
  DPIVRMLFileBodyC::DPIVRMLFileBodyC(const StringC &fn)
    : inf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if(!path.IsEmpty())
      path += filenameSeperator ;
  }

  //: Open file.
  DPIVRMLFileBodyC::DPIVRMLFileBodyC(IStreamC &is)
    : inf(is),
      done(false)
  {}

  //: Is valid data ?
  bool DPIVRMLFileBodyC::IsGetEOS() const
  {
    return !done && inf;
  }

  //: Get next piece of data.
  TriMeshC DPIVRMLFileBodyC::Get()
  {
    TriMeshC ret;
    if(done) return ret;
    done = true;
    ReadVRML(inf, path, ret);
    return ret;
  }

  /////////////////////////////////////////////////////////////////////////////
  //: Open file.
  DPOVRMLFileBodyC::DPOVRMLFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {}

  //: Open file.
  DPOVRMLFileBodyC::DPOVRMLFileBodyC(OStreamC &is)
    : outf(is),
      done(false)
  {}

  //: Put data.
  bool DPOVRMLFileBodyC::Put(const TriMeshC &dat)
  {
    if(done || !outf)
      return false;

    // Output header
    outf << "#VRML V2.0 utf8\n";
    outf << "DEF Sun DirectionalLight {\n";
    outf << "\tintensity 0.5\n";
    outf << "\tcolor 0.8 0.8 0.8\n";
    outf << "\tdirection 0.8824 -0.4578 -0.1085\n";
    outf << "\ton TRUE\n";
    outf << "}\n";

    // Output an IFS
    outf << "Shape {\n";
    outf << "\tappearance Appearance {\n";
    outf << "\t\tmaterial Material {\n";
    outf << "\t\t\tdiffuseColor 0.7 0.7 0.7\n";
    outf << "\t\t}\n";
    outf << "\t}\n";
    outf << "\tgeometry IndexedFaceSet {\n";
    // Output all vertex coordinates
    outf << "\t\tcoord Coordinate {\n";
    outf << "\t\t\tpoint [\n";
    if(dat.Vertices().Size() != 0) {
      SArray1dIterC<VertexC> vit(dat.Vertices());
      for(; vit; vit++) {
        outf << "\t\t\t" << vit->Position() << ",\n";
      }
    }
    outf << "\t\t\t]\n";
    outf << "\t\t}\n";
    // Output vertex indices for tri's
    outf << "\t\tcoordIndex [\n";
    if(dat.Vertices().Size() != 0) {
      const VertexC *x = &(dat.Vertices()[0]);
      SArray1dIterC<TriC> fit(dat.Faces());
      for(; fit; fit++) {
        outf << "\t\t" << (fit->VertexPtr(0) - x) << ','
             << (fit->VertexPtr(1) - x) << ','
             << (fit->VertexPtr(2) - x) << ",-1,\n";
      }
    }
    outf << "\t\t]\n";
    // Output vertex normals
    // NOTE: This should be optional as it increases the file size
    outf << "\t\tnormal Normal {\n";
    outf << "\t\t\tvector [\n";
    if(dat.Vertices().Size() != 0) {
      SArray1dIterC<VertexC> vit(dat.Vertices());
      for(vit.First(); vit; vit++) {
        outf << "\t\t\t" << vit->Normal() << ",\n";
      }
    }
    outf << "\t\t\t]\n";
    outf << "\t\t}\n";
    // Output the normal indices
    outf << "\t\tnormalIndex [\n";
    if(dat.Vertices().Size() != 0) {
      const VertexC *x = &(dat.Vertices()[0]);
      SArray1dIterC<TriC> fit(dat.Faces());
      for(; fit; fit++) {
        outf << "\t\t" << (fit->VertexPtr(0) - x) << ','
             << (fit->VertexPtr(1) - x) << ','
             << (fit->VertexPtr(2) - x) << ",-1,\n";
      }
    }
    outf << "\t\t]\n";
    // Finish the IFS
    outf << "\t}\n";
    outf << "}\n";

    // Output the footer

    // Done
    return true;
  }

  //: Is port ready for data ?
  bool DPOVRMLFileBodyC::IsPutReady() const
  {
    return !done && outf;
  }

  // TexTriMesh VRML File IO /////////////////////////////////////////////
  //: Open file.
  DPITexVRMLFileBodyC::DPITexVRMLFileBodyC(const StringC &fn)
    : inf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator ;
  }

  //: Is valid data ?
  bool DPITexVRMLFileBodyC::IsGetEOS() const
  {
    return !done && inf;
  }

  //: Get next piece of data.
  TexTriMeshC DPITexVRMLFileBodyC::Get() {
    TexTriMeshC ret;
    if(done)
      return ret;
    TriMeshC tMesh;
    if(!ReadVRML(inf, path, tMesh))
      return ret;
    ret = tMesh;
    if(!ret.IsValid())
    {
      cerr << "Something wrong, trying to greate simple mesh\n";
      ret = TexTriMeshC(tMesh.Vertices(),tMesh.Faces());
    }
    return ret;
  }

  /////////////////////////////////////////////////////////////////////////////
  //: Open file.
  DPOTexVRMLFileBodyC::DPOTexVRMLFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator ;
    fullname = fn;
  }

  //: Put data.
  bool DPOTexVRMLFileBodyC::Put(const TexTriMeshC &dat)
  {
    if(done || !outf)
      return false;

    // Output header
    outf << "#VRML V2.0 utf8\n";
    outf << "DEF Sun DirectionalLight {\n";
    outf << "\tintensity 0.5\n";
    outf << "\tcolor 0.8 0.8 0.8\n";
    outf << "\tdirection 0.8824 -0.4578 -0.1085\n";
    outf << "\ton TRUE\n";
    outf << "}\n";

    // Output each texture as a separate IFS
    outf << "Group {\n";
    outf << "\tchildren [\n";
    const SArray1dC<VertexC>& verts = dat.Vertices();
    UByteT texid;
    StringC tmpName = FilenameC(fullname).NameComponent();
    IntT pnt = tmpName.index('.',-1);
    if(pnt > 0)
      tmpName = tmpName.before('.');
    tmpName += '-';

    SArray1dIter2C<ImageC<ByteRGBValueC>,StringC> itTex(dat.Textures(),dat.TexFilenames());
    int tn = 0;
    for (texid=0; itTex; texid++, itTex++) {
      FilenameC texName = itTex.Data2();
      if(texName.IsEmpty()) {
        texName = tmpName + StringC(tn) + StringC(".jpg");
        tn++;
      }
      else
      {
        texName = path + texName.BaseNameComponent() + StringC(".jpg");
      }
      ONDEBUG(cerr << "Saving texture '" << texName << "'\n");
      // Output IFS

      outf << "\tShape {\n";
      outf << "\t\tappearance Appearance {\n";
      // Output the texture name
      outf << "\t\t\ttexture ImageTexture {\n";
      outf << "\t\t\t\turl \"" << texName << "\"\n";
      outf << "\t\t\t}\n";
      outf << "\t\t}\n";
      outf << "\t\tgeometry IndexedFaceSet {\n";
      // Output the vertices used by this texture
      // and create lookups for the vertex indices
      outf << "\t\t\tcoord Coordinate {\n";
      outf << "\t\t\t\tpoint [\n";
      HashC<UIntT,UIntT> lookup, lookupnew;
      UIntT vid=0;
      SArray1dIterC<TriC> fit(dat.Faces());
      for(; fit; fit++) {
        if (fit.Data().TextureID()==texid) {
          IntT v;
          for (v=0; v<3; v++) {
            UIntT index = dat.Index(fit.Data(),v);
            if (!lookup[index]) {
              // Create lookup
              lookup[index] = vid;
              lookupnew[vid] = index;
              vid++;
              // Output vertex
              outf << "\t\t\t\t" << verts[index].Position() << ",\n";
            }
          }
        }
      }
      outf << "\t\t\t\t]\n";
      outf << "\t\t\t}\n";
      // Output new vertex indices for tri's
      outf << "\t\t\tcoordIndex [\n";
      if(vid!=0) {
        for(fit.First(); fit; fit++) {
          if (fit.Data().TextureID()==texid) {
            // Output new vertex indices for tri
            outf << "\t\t\t";
            IntT v;
            for (v=0; v<3; v++) {
              UIntT index = dat.Index(fit.Data(),v);
              outf << lookup[index] << ",";
            }
            outf << "-1,\n";
          }
        }
      }
      outf << "\t\t\t]\n";
      // Output vertex normals
      // NOTE: This should be optional as it increases the file size
      outf << "\t\t\tnormal Normal {\n";
      outf << "\t\t\t\tvector [\n";
      if(vid!=0) {
        UIntT index;
        for (index=0; index<vid; index++) {
          outf << "\t\t\t\t" << verts[lookupnew[index]].Normal() << ",\n";
        }
      }
      outf << "\t\t\t\t]\n";
      outf << "\t\t\t}\n";
      // Output new vertex indices for tri's
      outf << "\t\t\tnormalIndex [\n";
      if(vid!=0) {
        for(fit.First(); fit; fit++) {
          if (fit.Data().TextureID()==texid) {
            // Output new vertex indices for tri
            outf << "\t\t\t";
            IntT v;
            for (v=0; v<3; v++) {
              UIntT index = dat.Index(fit.Data(),v);
              outf << lookup[index] << ",";
            }
            outf << "-1,\n";
          }
        }
      }
      outf << "\t\t\t]\n";
      // Output texture coordinates
      outf << "\t\t\ttexCoord TextureCoordinate {\n";
      outf << "\t\t\t\tpoint [\n";
      if(vid!=0) {
        for(fit.First(); fit; fit++) {
          if (fit.Data().TextureID()==texid) {
            outf << "\t\t\t\t";
            IntT v;
            for (v=0; v<3; v++) {
              const Vector2dC coord = fit.Data().TextureCoord(v);
              outf << coord[0] << ' ' << coord[1] << ", ";
            }
            outf << '\n';
          }
        }
      }
      outf << "\t\t\t\t]\n";
      outf << "\t\t\t}\n";
      // Output texture indices
      outf << "\t\t\ttexCoordIndex [\n";
      if(vid!=0) {
        vid=0;
        for(fit.First(); fit; fit++) {
          if (fit.Data().TextureID()==texid) {
            outf << "\t\t\t" << vid << ',' << vid+1 << ',' << vid+2 << ",-1,\n";
            vid+=3;
          }
        }
      }
      outf << "\t\t\t]\n";
      // Finish the IFS
      outf << "\t\t}\n";
      outf << "\t}\n";
      // Save the texture image
      if(itTex.Data1().Frame().Area() > 0) {
        if (!RavlN::Save(path + texName,itTex.Data1())) {
          cerr << "Error: Could not save texture\n";
          done = true;
          return false;
        }
      }
    }

    // Finish the group of IFS
    outf << "\t]\n";
    outf << "}\n";

    // Output the footer

    // Done
    return true;
  }

  //: Is port ready for data ?
  bool DPOTexVRMLFileBodyC::IsPutReady() const
  { return !done && outf; }


#if RAVL_COMPILER_MIPSPRO
  // force template instantiation
  DPIPortBodyC<ImageC<ByteRGBValueC> > silly_dummy_variable_for_irix_template1 ;
  DPOPortBodyC<ImageC<ByteRGBValueC> > silly_dummy_variable_for_irix_template2 ;
#endif

}

