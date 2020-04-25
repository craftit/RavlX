// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Ravl3D
//! author="Charles Galambos"

#include "Ravl/3D/MeshShapes.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1d.hh"

namespace Ravl3DN {
  
  static Point2dC TexPnt(RealT v1,RealT v2)
  { return Point2dC(v1,v2); }
  
  static Point3dC World3d(RealT a1,RealT a2,RealT a3)
  { return Point3dC(a1,a2,a3); }
  
  //: Create a flat plane
  
  TriMeshC CreateTriMeshPlane(RealT size) {
    RealT hSize = size/2.0;
    
    SArray1dC<VertexC> vertex(4);
    Vector3dC norm(0,0,1);
    vertex[0] = VertexC(World3d(-hSize,-hSize,0),norm);
    vertex[1] = VertexC(World3d(-hSize, hSize,0),norm);
    vertex[2] = VertexC(World3d( hSize, hSize,0),norm);
    vertex[3] = VertexC(World3d( hSize,-hSize,0),norm);
    
    SArray1dC<TriC> tri(2);
    tri[0] = TriC(vertex[0],vertex[2],vertex[1],TexPnt(0,0),TexPnt(1,1),TexPnt(0,1));
    tri[1] = TriC(vertex[0],vertex[3],vertex[2],TexPnt(0,0),TexPnt(1,0),TexPnt(1,1));
    
    tri[0].SetFaceNormal(norm);
    tri[1].SetFaceNormal(norm);
    
    return TriMeshC(vertex,tri,true);
  }

  // Create a cube.
  
  TriMeshC CreateTriMeshCube(RealT size, const Point3dC &origin) {
    RealT hSize = size/2.0;
    SArray1dC<VertexC> vertex(8);
    vertex[0] = VertexC(World3d( hSize, hSize,-hSize) + origin);
    vertex[1] = VertexC(World3d( hSize,-hSize,-hSize) + origin);
    vertex[2] = VertexC(World3d(-hSize,-hSize,-hSize) + origin);
    vertex[3] = VertexC(World3d(-hSize, hSize,-hSize) + origin);
    
    vertex[4] = VertexC(World3d( hSize, hSize,hSize) + origin);
    vertex[5] = VertexC(World3d( hSize,-hSize,hSize) + origin);
    vertex[6] = VertexC(World3d(-hSize,-hSize,hSize) + origin);
    vertex[7] = VertexC(World3d(-hSize, hSize,hSize) + origin);
    
    SArray1dC<TriC> tri(12);
    UIntT tn = 0;
    RealT sep = 1.0/60.0;
    
    RealT tr0 = sep;
    RealT tr1 = 0.5 - sep;
    RealT tr2 = 0.5 + sep;
    RealT tr3 = 1.0 - sep;
    
    RealT tc0 = sep;
    RealT tc1 = 1.0/3.0 - sep;
    RealT tc2 = 1.0/3.0 + sep; 
    
    RealT tc3 = 2.0/3.0 - sep;
    RealT tc4 = 2.0/3.0 + sep; 
    RealT tc5 = 1.0 - sep;
    
    // Front
    tri[tn++] = TriC(vertex[0],vertex[1],vertex[2],TexPnt(tr0,tc0),TexPnt(tr0,tc1),TexPnt(tr1,tc1));
    tri[tn++] = TriC(vertex[2],vertex[3],vertex[0],TexPnt(tr1,tc1),TexPnt(tr1,tc0),TexPnt(tr0,tc0));
    
    // Back
    tri[tn++] = TriC(vertex[6],vertex[5],vertex[4],TexPnt(tr2,tc0),TexPnt(tr2,tc1),TexPnt(tr3,tc1));
    tri[tn++] = TriC(vertex[4],vertex[7],vertex[6],TexPnt(tr3,tc1),TexPnt(tr3,tc0),TexPnt(tr2,tc0));
    
    // Top
    tri[tn++] = TriC(vertex[4],vertex[5],vertex[1],TexPnt(tr0,tc2),TexPnt(tr0,tc3),TexPnt(tr1,tc3));
    tri[tn++] = TriC(vertex[0],vertex[4],vertex[1],TexPnt(tr1,tc2),TexPnt(tr0,tc2),TexPnt(tr1,tc3));
    
    // Bottom
    tri[tn++] = TriC(vertex[7],vertex[3],vertex[2],TexPnt(tr2,tc2),TexPnt(tr3,tc2),TexPnt(tr3,tc3));
    tri[tn++] = TriC(vertex[2],vertex[6],vertex[7],TexPnt(tr3,tc3),TexPnt(tr2,tc3),TexPnt(tr2,tc2));
    
    // Left
    tri[tn++] = TriC(vertex[4],vertex[0],vertex[7],TexPnt(tr0,tc4),TexPnt(tr0,tc5),TexPnt(tr1,tc4));
    tri[tn++] = TriC(vertex[0],vertex[3],vertex[7],TexPnt(tr0,tc5),TexPnt(tr1,tc5),TexPnt(tr1,tc4));
    
    // Right
    tri[tn++] = TriC(vertex[1],vertex[5],vertex[2],TexPnt(tr2,tc5),TexPnt(tr2,tc4),TexPnt(tr3,tc5));
    tri[tn++] = TriC(vertex[5],vertex[6],vertex[2],TexPnt(tr2,tc4),TexPnt(tr3,tc4),TexPnt(tr3,tc5));
    
    TriMeshC ret(vertex,tri,true);
    ret.UpdateVertexNormals();
    return ret;
  }
  
  // Create a sphere.
  // TODO: Create a nicer default texture map.
  // TODO: Fill in normals as we go
  
  TriMeshC CreateTriMeshSphere(UIntT layers,UIntT slices,RealT radius) {
    RavlAssert(layers >= 1);
    RavlAssert(slices >= 2);
    
    RealT texRowSize = 1.0/layers;
    RealT texColSize = 1.0/slices;
    
    RealT sliceStep = 2*RavlConstN::pi / slices;
    RealT layerStep = RavlConstN::pi / layers;
    
    SArray1dC<VertexC> vertex(2 + (layers-1) * slices);
    UIntT numTri = slices * 2 + (layers-2) * slices * 2;
    //SArray1dC<UIntT> tri(numTri * 3);
    SArray1dC<TriC> tri(numTri);
    
    
    UIntT tn = 0,vn = 0;
    
    // Cache cos and sin values for slice angles.
    SArray1dC<RealT> sliceCos(slices);
    SArray1dC<RealT> sliceSin(slices);
    
    RealT sliceAngle = -RavlConstN::pi;
    for(SArray1dIter2C<RealT,RealT> it(sliceCos,sliceSin);it;it++) {
      it.Data1() = Cos(sliceAngle) * radius;
      it.Data2() = Sin(sliceAngle) * radius;
      sliceAngle += sliceStep;
    }
      
    // Vertex's on last layer
    SArray1dC<UIntT> vLastLayer(slices);
    
    // ----- Put in top fan. -----
    
    IntT topVert = vn;
    vertex[vn++] = VertexC(World3d(0,radius,0));
    
    RealT layerAngle = layerStep;
    
    RealT lc = Cos(layerAngle) * radius;
    RealT ls = Sin(layerAngle);
    
    vLastLayer[0] = vn;
    vertex[vn++] = VertexC(World3d(ls * sliceSin[0],lc,ls  * sliceCos[0]));
    
    RealT texRow0 = 0;
    RealT texRow1 = texRowSize;
    RealT texCol0; 
    RealT texCol1 = texColSize; 
    
    for(UIntT s = 1;s < slices;s++) {
      
      texCol0 = texColSize * s;
      texCol1 = texCol0 + texColSize;
      
      tri[tn++] = TriC(vertex[topVert],vertex[vn-1],vertex[vn],
                       TexPnt(texRow0,texCol0 + texColSize/2.0),TexPnt(texRow1,texCol0),TexPnt(texRow1,texCol1));
      
      vLastLayer[s] = vn;
      vertex[vn++] = VertexC(World3d(ls * sliceSin[s],lc,ls  * sliceCos[s]));
    }
    
    texCol0 = 0;
    texCol1 = texColSize;
    
    tri[tn++] = TriC(vertex[topVert],vertex[vn-1],vertex[vLastLayer[0]],
                     TexPnt(texRow0,texCol0 + texColSize/2.0),TexPnt(texRow1,texCol0),TexPnt(texRow1,texCol1)
                     );
    
    
    // ---- Put in mid layers ----
    
    for(UIntT l = 1;l < (layers-1);l++) {
      layerAngle += layerStep;
      lc = Cos(layerAngle) * radius;
      ls = Sin(layerAngle);
      
      UIntT lastTopVert = vLastLayer[0];
      UIntT lastBotVert = vn;
      UIntT firstVirt = vn;
      vertex[vn++] = VertexC(World3d(ls * sliceSin[0],lc,ls  * sliceCos[0]));
      
      RealT texRow0 = l * texRowSize;
      RealT texRow1 = texRow0 + texRowSize;
      
      for(UIntT s = 1;s < slices;s++) {
        texCol0 = texColSize * s;
        texCol1 = texCol0 + texColSize;
        
	// Put in face triangles.
        tri[tn++] = TriC(vertex[vn],vertex[lastTopVert],vertex[lastBotVert],
                         TexPnt(texRow1,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol0));
        
        tri[tn++] = TriC(vertex[vLastLayer[s]],vertex[lastTopVert],vertex[vn],
                         TexPnt(texRow0,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol1));
        
	// Put in new vertex.
	lastTopVert = vLastLayer[s];
	lastBotVert = vn;
	vLastLayer[s] = vn;
	vertex[vn++] = VertexC(World3d(ls * sliceSin[s],lc,ls  * sliceCos[s]));
      }
      
      texCol0 = 0;
      texCol1 = texColSize;
      
      // Put in final face triangles.
      tri[tn++] = TriC(vertex[firstVirt],vertex[lastTopVert],vertex[lastBotVert],
                       TexPnt(texRow1,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol0));
      
      tri[tn++] = TriC(vertex[vLastLayer[0]],vertex[lastTopVert],vertex[firstVirt],
                       TexPnt(texRow0,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol1));
      
      vLastLayer[0] = firstVirt;
    }
    
    // ---- Put in bottom fan ----
    
    IntT botVert = vn;
    vertex[vn++] = VertexC(World3d(0,-radius,0));
    
    // Put together face.
    texRow0 = 1.0-texRowSize;
    texRow1 = 1.0;
    
    UIntT nextVert = vLastLayer[0];
    texCol0 = 0;
    texCol1 = texColSize;

    //tri[tn++] = TriC(vertex[vLastLayer[slices-1]],vertex[botVert],vertex[nextVert],
    //                 TexPnt(texRow0,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol0 + texColSize/2.0));

    tri[tn++] = TriC(vertex[nextVert],vertex[vLastLayer[slices-1]],vertex[botVert],
                     TexPnt(texRow0,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol0 + texColSize/2.0));
    
    UIntT lastVert = nextVert;
    
    
    for(UIntT s = 1;s < slices;s++) {
      // Put together face.
      UIntT nextVert = vLastLayer[s];
      
      texCol0 = texColSize * s;
      texCol1 = texCol0 + texColSize;
      
      tri[tn++] = TriC(vertex[nextVert],vertex[lastVert],vertex[botVert],
                       TexPnt(texRow0,texCol1),TexPnt(texRow0,texCol0),TexPnt(texRow1,texCol0 + texColSize/2.0));
      
      lastVert = nextVert;
    }
    
    RavlAssert(tn == tri.Size());
    RavlAssert(vn == vertex.Size());
    
    TriMeshC ret(vertex,tri,true);
    //TriMeshC ret(vertex,tri);
    //ret.GenerateTextureCoords();
    ret.UpdateVertexNormals();
    return ret;
  }
  
}


