// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/testHEMesh.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.3D.Half Edge Mesh"
//! userlevel=Develop

#include "Ravl/3D/HEMesh.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/Stream.hh"

using namespace Ravl3DN;

int testTri2HEMesh();
int testAssign();

int main() {
  int ln;
  if((ln = testTri2HEMesh()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  if((ln = testAssign()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed ok. \n";
  return 0;
}

int testTri2HEMesh() {
  cout << "testTri2HEMesh()... \n";
  SArray1dC<Vector3dC> verts(4);
  verts[0] = Point3dC(0,0,0);
  verts[1] = Point3dC(0,0,1);
  verts[2] = Point3dC(0,1,0);
  verts[3] = Point3dC(1,0,0);

  SArray1dC<UIntT> ind(4 * 3);
  ind[0] = 0; ind[1]  = 1; ind[2]  = 2;
  ind[3] = 3; ind[4]  = 1; ind[5]  = 0;
  ind[6] = 3; ind[7]  = 0; ind[8]  = 2;
  ind[9] = 3; ind[10] = 2; ind[11] = 1;
  
  TriMeshC tm(verts,ind);
  if(tm.Faces().Size() != 4) return __LINE__;
  if(tm.Vertices().Size() != verts.Size()) return __LINE__;
  
  HEMeshC mesh(tm);
  if(!mesh.IsValid()) return __LINE__;
  if(!mesh.CheckMesh()) return __LINE__;
  if(mesh.NoFaces() != 4) return __LINE__;
  if(mesh.NoVertices() != verts.Size()) return __LINE__;
  
  TriMeshC recon = mesh.TriMesh();
  if(recon.Faces().Size() != 4) return __LINE__;
  if(recon.Vertices().Size() != verts.Size()) return __LINE__;
  
  cout << "testTri2HEMesh(), checking collapse edge. \n";
  
  HEMeshEdgeC anEdge = mesh.FirstFace().Edge();
  if(!anEdge.IsValid()) return __LINE__;
  anEdge.CollapseEdge();
  if(!mesh.CheckMesh()) return __LINE__;
  if(mesh.NoFaces() != 2) return __LINE__;
  if(mesh.NoVertices() != (verts.Size()-1)) return __LINE__;
  
  return 0;
}

int testAssign() {
  
  SArray1dC<Vector3dC> verts(4);
#if 0
  verts[0] = Point3dC(0,0,0);
  verts[1] = Point3dC(0,0,1);
  verts[2] = Point3dC(0,1,0);
  verts[3] = Point3dC(1,0,0);
  
  SArray1dC<UIntT> ind(4 * 3);
  ind[0] = 0; ind[1]  = 1; ind[2]  = 2;
  ind[3] = 3; ind[4]  = 1; ind[5]  = 0;
  ind[6] = 3; ind[7]  = 0; ind[8]  = 2;
  ind[9] = 3; ind[10] = 2; ind[11] = 1;
  
  TriMeshC oMesh(verts,ind);
  
  {
    //HEMeshC oHEMesh(oMesh);
    // IF YOU DO THIS
    {
      //TriMeshC oNewMesh = oHEMesh.TriMesh();
    }
    // OR IF YOU DO THIS
    // {
    // // Construct a hash table to retrieve the indices of the HEMesh vertices
    // HashC<HEMeshVertexC,UIntT> oVIndex;
    // UIntT iIndex=0;
    // HEMeshVertexIterC itHEVert(oHEMesh.Vertices());
    // for (; itHEVert; itHEVert++, iIndex++) {
    // oVIndex[itHEVert.Data()] = iIndex;
    // }
    // }
  } 
#endif
  return 0;
}
