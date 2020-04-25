// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Ravl3D

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/MeshShapes.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/DrawPolygon.hh"
#include "Ravl/IO.hh"

using namespace Ravl3DN;

int testMeshPlane();
int testMeshCube();
int testMeshSphere();

int main() {
  int ln;
#if 1
  if((ln = testMeshPlane()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  if((ln = testMeshCube()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
#endif
  if((ln = testMeshSphere()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed. \n";
  return 0;
}

// Display the texture coorinates of a mesh.

bool TestTexCoords(const TriMeshC &mesh) {
  RavlImageN::ImageC<ByteT> img(512,512);
  img.Fill(0);

  RealRange2dC texRect(RealRangeC(0.0,1.0),RealRangeC(0.0,1.0));
  
  for(SArray1dIterC<TriC> it(mesh.Faces());it;it++) {
    Polygon2dC poly;
#if 1
    // Check texture coordinates are legal.
    if(!texRect.Contains(it->TextureCoord(0)))
      return false;
    if(!texRect.Contains(it->TextureCoord(1)))
      return false;
    if(!texRect.Contains(it->TextureCoord(2)))
      return false;
#endif
    
    // Check area.
    poly.InsLast(it->TextureCoord(0)*512);
    poly.InsLast(it->TextureCoord(1)*512);
    poly.InsLast(it->TextureCoord(2)*512);
    
    if(poly.Area() == 0) {
      cerr << "Warning: Texture map for tri " << it.Index() << " has zero area. \n";
      return false;
    }
    
    // For visual check.
    RavlImageN::DrawPolygon(img,(ByteT) 255,poly);
  }
#if 0
  // Enable to display mapping for faces onto the texture.
  static int testCount =0;
  Save(StringC("@X:Tex") + StringC(testCount++),img);
#endif
  return true;
}


int testMeshPlane() {
  TriMeshC mesh = CreateTriMeshPlane(1.0);
  
  RealT ns = mesh.Vertices()[0].Normal()[2];
  mesh.UpdateVertexNormals();
  // Check the normals are consistant
  if(Sign(ns) != Sign(mesh.Vertices()[0].Normal()[2])) return __LINE__;
  
  if(!TestTexCoords(mesh))
    return __LINE__;
  
  return 0;
}

int testMeshCube() {
  TriMeshC mesh = CreateTriMeshCube(1.0);
  
  if(!TestTexCoords(mesh))
    return __LINE__;
  
  return 0;
}

int testMeshSphere() {
  TriMeshC mesh = CreateTriMeshSphere(3,8,1.0);
  
  if(!TestTexCoords(mesh))
    return __LINE__;
  
  return 0;
}
