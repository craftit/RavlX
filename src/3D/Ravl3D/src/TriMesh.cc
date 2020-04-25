// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3D
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/TriMesh.cc"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Index3d.hh"

namespace Ravl3DN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  //: Construct from an array of vertexes and an array of indices.
  // The length of faceInd should be a power of 3, success triples are taken
  // from it to form the faces in the mesh.
  
  TriMeshBodyC::TriMeshBodyC(const SArray1dC<Vector3dC> &v,const SArray1dC<UIntT> &faceInd) 
    : vertices(v.Size()),
      faces(faceInd.Size()/3),
      haveTexture(false)
  {
    Vector3dC zero(0,0,0);
    for(SArray1dIter2C<VertexC,Vector3dC> it(vertices,v);it;it++) {
      it.Data1().Position() = it.Data2();
      it.Data1().Normal() = zero;
    }
    RavlAssert(faceInd.Size() == (faces.Size() * 3));
    SArray1dIterC<UIntT> iit(faceInd);
    /* Create vertex pointers, and some inital vertex normals. */
    for(SArray1dIterC<TriC> fit(faces);fit;fit++) {
      int i;
      for(i = 0;i < 3;i++,iit++) {
	fit.Data1().VertexPtr(i) = &(vertices[*iit]);
      }
      fit->UpdateFaceNormal();
      Vector3dC norm = fit->FaceNormal();
      for( i = 0;i < 3;i++)
	fit->Normal(i) += norm;
    }
    /* Make unit normals. */
    for(SArray1dIterC<VertexC> itv(vertices);itv;itv++)
      itv->Normal().MakeUnit();
  }

  //: Construct from a list of vertexes and a list of indices.
  // The length of faceInd should be a power of 3, success triples are taken
  // from it to form the faces in the mesh.
  
  TriMeshBodyC::TriMeshBodyC(const DListC<Vector3dC> &v,const DListC<UIntT> &faceInd) 
    : vertices(v.Size()),
      faces(faceInd.Size()/3),
      haveTexture(false)
  {
    // Create the vertices from the vertex positions v
    Vector3dC zero(0,0,0);
    DLIterC<Vector3dC> itPos(v);
    SArray1dIterC<VertexC> itVerts(vertices);
    for (; itPos; itPos++, itVerts++) {
      itVerts.Data().Position() = itPos.Data();
      itVerts.Data().Normal() = zero;
    }
    // Create the tri faces from the face vertex indices
    RavlAssert(faceInd.Size() == (faces.Size() * 3));
    DLIterC<UIntT> iit(faceInd);
    SArray1dIterC<TriC> fit(faces);
    /* Create vertex pointers, and some inital vertex normals. */
    for(;fit;fit++) {
      int i;
      for(i = 0;i < 3;i++,iit++) {
        fit.Data().VertexPtr(i) = &(vertices[*iit]);
      }
      fit->UpdateFaceNormal();
      Vector3dC norm = fit->FaceNormal();
      for( i = 0;i < 3;i++)
        fit->Normal(i) += norm;
    }
    /* Make unit normals. */
    SArray1dIterC<VertexC> itv(vertices);
    for(; itv; itv++)
      itv->Normal().MakeUnit();
  }

  //: Construct from a list of vertices and a list of indices.
  
  TriMeshBodyC::TriMeshBodyC(const DListC<Vector3dC> &v,const DListC<Index3dC> &faceInd) 
    : vertices(v.Size()),
      faces(faceInd.Size()),
      haveTexture(false)
  {
    // Create the vertices from the vertex positions v
    Vector3dC zero(0,0,0);
    DLIterC<Vector3dC> itPos(v);
    SArray1dIterC<VertexC> itVerts(vertices);
    for (; itPos; itPos++, itVerts++) {
      itVerts.Data().Position() = itPos.Data();
      itVerts.Data().Normal() = zero;
    }
    // Create the tri faces from the face vertex indices
    RavlAssert(faceInd.Size() == (faces.Size()));
    DLIterC<Index3dC> iit(faceInd);
    SArray1dIterC<TriC> fit(faces);
    /* Create vertex pointers, and some inital vertex normals. */
    for(;fit;fit++,iit++) {
      UIntT i;
      for(i = 0;i < 3;i++)
        fit.Data().VertexPtr(i) = &(vertices[(*iit)[i]]);
      fit->UpdateFaceNormal();
      Vector3dC norm = fit->FaceNormal();
      for( i = 0;i < 3;i++)
        fit->Normal(i) += norm;
    }
    /* Make unit normals. */
    SArray1dIterC<VertexC> itv(vertices);
    for(; itv; itv++)
      itv->Normal().MakeUnit();    
  }


  //: Copy constructor

  TriMeshBodyC::TriMeshBodyC(const TriMeshBodyC& oth)
  {
    haveTexture = oth.haveTexture;
    vertices = oth.vertices.Copy();
    faces = SArray1dC<TriC>(oth.faces.Size());
    SArray1dIter2C<TriC,TriC> it(faces,oth.faces);
    for(; it; it++) {
      int i;
      for(i=0 ; i<3; i++)
	it.Data1().VertexPtr(i) = &(vertices[oth.Index(it.Data2(),i)]);
      it.Data1().TextureID() = it.Data2().TextureID();
      it.Data1().TextureCoords() = it.Data2().TextureCoords();
      it.Data1().Colour() = it.Data2().Colour();
      it.Data1().FaceNormal() = it.Data2().FaceNormal();
    }
  }

  
  //: Flips the mesh surface
  
  void TriMeshBodyC::FlipTriangles(void) {
    for(SArray1dIterC<TriC> it(faces);it;it++)
      it->Flip();
  }

  //: Centre of triset.
  // - average vertex position
  
  Vector3dC TriMeshBodyC::Centroid() const {
    Vector3dC ret(0,0,0);
    if(vertices.Size() == 0)
      return ret; // Can't take a centroid of an empty mesh.
    for(SArray1dIterC<VertexC> it(vertices);it;it++)
      ret += it->Position();
    return ret / vertices.Size();
  }
  
  //: Transform mesh with RT
  
  void TriMeshBodyC::Transform(const RigidTransform3dC & rt){
    
    for(SArray1dIterC<VertexC> it(vertices);it;it++){
      Vector3dC v(it->Position());
      it->Position() = rt.Transform(v);
      it->Normal()=rt.ExportRotationMatrix()*(it->Normal());
    }
    
    for(SArray1dIterC<TriC> iv(faces);iv;iv++)
      iv->FaceNormal()=rt.ExportRotationMatrix()*(iv->FaceNormal());
    
  }

  //: Create an array of faces indices.
  // each successive triple of indices represents a face in the mesh.
  
  SArray1dC<UIntT> TriMeshBodyC::FaceIndices() const {
    SArray1dC<UIntT> ret(3 * faces.Size());
    if(faces.Size() == 0)
      return ret;
    SArray1dIterC<UIntT> rit(ret);
    const VertexC *x = &(vertices[0]);
    for(SArray1dIterC<TriC> it(faces);it;it++) {
      *rit = it->VertexPtr(0) - x; rit++;
      *rit = it->VertexPtr(1) - x; rit++;
      *rit = it->VertexPtr(2) - x; rit++;
    }
    return ret;
  }

  //: Find largest and smallest for each compoent of all vertices.
  
  void TriMeshBodyC::Limits(Vector3dC &min,Vector3dC &max) const{
    SArray1dIterC<VertexC> it(vertices);
    if(!it)
      return ; // Empty mesh!
    min = it->Position();
    max = it->Position();
    for(it++;it;it++) {
      for(int i = 0;i < 3;i++) {
	if(min[i] > it->Position()[i])
	  min[i] = it->Position()[i];
	if(max[i] < it->Position()[i])
	  max[i] = it->Position()[i];
      }
    }
  }
  
  //: Offset and Scale mesh by given values.
  
  void TriMeshBodyC::OffsetScale(const Vector3dC &off,RealT scale) {
    for(SArray1dIterC<VertexC> it(vertices);it;it++)
      it->Position() = (it->Position() + off) * scale;
  }
  
  //: Recalculate vertex normals.
  
  void TriMeshBodyC::UpdateVertexNormals() {
    Vector3dC zero(0,0,0);
    for(SArray1dIterC<VertexC> it(vertices);it;it++)
      it->Normal() = zero;
    for(SArray1dIterC<TriC> itf(faces);itf;itf++) {
      itf->UpdateFaceNormal();
      Vector3dC norm = itf->FaceNormal();
      for(int i = 0;i < 3;i++)
	itf->Normal(i) += norm;
    }
    for(SArray1dIterC<VertexC> itv(vertices);itv;itv++)
      itv->Normal() = itv->Normal().Unit();
  }
  
  
  TriMeshC TriMeshBodyC::operator+ (const TriMeshC &t2) const {
    SArray1dC<VertexC> verts(Vertices().Size()+t2.Vertices().Size());

    // put this in first 
    SArray1dIterC<VertexC> vit(verts);
    for(UIntT ivL=0;ivL<Vertices().Size();ivL++){
      vit->Position()=Vertices()[ivL].Position();
      vit->Normal() = Vertices()[ivL].Normal();
      vit++;
    }
    for(UIntT ivM=0;ivM<t2.Vertices().Size();ivM++){
      vit->Position()=t2.Vertices()[ivM].Position();
      vit->Normal() = t2.Vertices()[ivM].Normal();
      vit++;
    }
    
    SArray1dC<TriC> faces(Faces().Size()+t2.Faces().Size());    
    SArray1dIterC<TriC> it(faces);
    UIntT ii = 0;
    const VertexC *Lv0 = &(Vertices()[0]);
    for(;ii < Faces().Size();ii++) {
      UIntT i0=Faces()[ii].VertexPtr(0)-Lv0;
      UIntT i1=Faces()[ii].VertexPtr(1)-Lv0;
      UIntT i2=Faces()[ii].VertexPtr(2)-Lv0;
      
      it->VertexPtr(0) = &(verts[i0]);
      it->VertexPtr(1) = &(verts[i1]);
      it->VertexPtr(2) = &(verts[i2]);
      it->FaceNormal()=Faces()[ii].FaceNormal();
      it++;
    }
    
    
    const VertexC *Mv0 = &(t2.Vertices()[0]);
    for(ii=0;ii < t2.Faces().Size();ii++) {
      UIntT i0=t2.Faces()[ii].VertexPtr(0)-Mv0+Vertices().Size();
      UIntT i1=t2.Faces()[ii].VertexPtr(1)-Mv0+Vertices().Size();
      UIntT i2=t2.Faces()[ii].VertexPtr(2)-Mv0+Vertices().Size();
      
      it->VertexPtr(0) = &(verts[i0]);
      it->VertexPtr(1) = &(verts[i1]);
      it->VertexPtr(2) = &(verts[i2]);
      it->FaceNormal()=t2.Faces()[ii].FaceNormal();
      it++;
    }
    
    return TriMeshC(verts,faces);
  }
  
  //: Automatically generate texture coordinates.
  
  bool TriMeshBodyC::GenerateTextureCoords(void) {
    
    // Check that we have a valid mesh
    IntT iNumFaces = faces.Size();
    if (iNumFaces==0) return false;

    // Work out how many squares we need to take all our triangles
    IntT iNumSquares = iNumFaces / 2;
    if ( (iNumFaces%2) ) iNumSquares++;
    // Work out how many squares to a side of the texture (rounded up square root)
    IntT iDim = IntT(ceil(Sqrt(iNumSquares)));
    RealT dSize = 1.0/(RealT)iDim;
    // Generate texture coordinates for each triangle.
    SArray1dIterC<TriC> itFaces(faces);
    IntT x,y;
    for (x=0; (x<iDim && itFaces); x++) {
      RealT dXBase = x*dSize;
      for (y=0; (y<iDim && itFaces); y++) {
	RealT dYBase = y*dSize;
	// Generate texture coordinates for the triangle in the top left corner of the square
	TriC& faceTL = itFaces.Data();
	faceTL.TextureID() = 0;
	faceTL.TextureCoord(0) = Vector2dC(dXBase + dSize*0.05, dYBase + dSize*0.90);
	faceTL.TextureCoord(1) = Vector2dC(dXBase + dSize*0.05, dYBase + dSize*0.05);
	faceTL.TextureCoord(2) = Vector2dC(dXBase + dSize*0.90, dYBase + dSize*0.05);
	itFaces++;
	if (itFaces) {
	  // Generate texture coordinates for the triangle in the bottom right corner of the square
	  TriC& faceBR = itFaces.Data();
	  faceBR.TextureID() = 0;
	  faceBR.TextureCoord(0) = Vector2dC(dXBase + dSize*0.95, dYBase + dSize*0.10);
	  faceBR.TextureCoord(1) = Vector2dC(dXBase + dSize*0.95, dYBase + dSize*0.95);
	  faceBR.TextureCoord(2) = Vector2dC(dXBase + dSize*0.10, dYBase + dSize*0.95);
	  itFaces++;
	}
      }
    }
    
    haveTexture = true;
    
    return true;
  }


  ostream &operator<<(ostream &s,const TriMeshC &ts) {
    RavlAssert(ts.IsValid());
    s << ts.Vertices(); 
    s << (IntT)ts.HaveTextureCoord() << '\n';
    s << ts.Faces().Size() << '\n'; 
    const VertexC *x = &(ts.Vertices()[0]);
    SArray1dIterC<TriC> it(ts.Faces());
    for(; it; it++) {
      s << (it->VertexPtr(0) - x) << ' ' 
	<< (it->VertexPtr(1) - x) << ' ' 
	<< (it->VertexPtr(2) - x) << ' ';
      s << it->TextureID() << ' ';
      s << it->TextureCoords() << ' ';
      s << it->Colour() << '\n';
    }
    return s;
  }
  
  istream &operator>>(istream &s,TriMeshC &ts) {
    
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
    ts = TriMeshC(verts,faces,bHaveTexture);
    return s;
  }

  
}
