// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Warp/WarpMesh2d.cc"

#include "Ravl/Image/WarpMesh2d.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/SArray2dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/IO.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  //: Constructor.
  
  WarpMesh2dBaseC::WarpMesh2dBaseC(const TriMesh2dC &mesh,bool nFillBackground) 
    : homeMesh(mesh),
      fillBackground(nFillBackground)
  { 
    IndexRange2dC rng =  mesh.BoundingRectangle().IndexRange();
    CreateWarpMap(mesh,rng); 
  }
  
  //: Constructor.
  
  WarpMesh2dBaseC::WarpMesh2dBaseC(const TriMesh2dC &mesh,const IndexRange2dC &rng,bool nFillBackground)
    : homeMesh(mesh),
      fillBackground(nFillBackground)
  {
    CreateWarpMap(mesh,rng);
  }

  void WarpMesh2dBaseC::CreateWarpMap(const TriMesh2dC &mesh,const IndexRange2dC &rng) {
    homeMesh = mesh;
    rng.TRow() = 0;
    rng.LCol() = 0;
    ONDEBUG(cerr << "Warp map size=" << rng << "\n");
    warpMap = ImageC<Affine2dC *>(rng);
    warpMap.Fill(0);
    
    transforms = SArray1dC<Affine2dC>(mesh.NoFaces());
    //ImageC<ByteT> test(rng);
    //test.Fill(0);
    // Generate warp map.
    for(Array2dIterC<Affine2dC *> it(warpMap);it;it++) {
      IntT x = homeMesh.FindFace(Point2dC(it.Index()));
      //cout << " " << x;
      if(x >= 0) {
	*it = &(transforms[x]);
	//test[it.Index()] = x;
      } else {
	*it = 0;
	//test[it.Index()] = 0;
      }
    }
    //Save("@X:WarpMap",test);
    //cout << "\n";
  }
  
  //: Compute transforms.
  // Returns true if all transforms are defined.
  
  bool WarpMesh2dBaseC::ComputeTransforms(const SArray1dC<Point2dC> &homeVerts,const SArray1dC<Point2dC> &newMeshPositions) {
    //ONDEBUG(cerr << "HomeVerts=" << homeVerts << "\n");
    //ONDEBUG(cerr << "NewsVerts=" << newMeshPositions << "\n");
#if 0
    SArray1dC<Point2dC> opnts(3);
    SArray1dC<Point2dC> npnts(3);
#endif
    RavlAssert(newMeshPositions.Size() == homeVerts.Size());
    for(SArray1dIter2C<Index3dC,Affine2dC> it(homeMesh.Faces(),transforms);it;it++) {
      const Index3dC &vs = it.Data1();
#if 0
      for(int i = 0;i < 3;i++) {
	IntT vert = vs[i].V();
	opnts[i] = homeVerts[vert];
	npnts[i] = newMeshPositions[vert];
      }
      it.Data2() = FitAffine(opnts,npnts);
#else
      FitAffine(homeVerts[vs[0]],newMeshPositions[vs[0]],
		homeVerts[vs[1]],newMeshPositions[vs[1]],
		homeVerts[vs[2]],newMeshPositions[vs[2]],
		it.Data2());
#endif
      //cout << "Mapping=" << it.Data2() << "\n";
    }
    return true;
  }

  //: Write to stream.
  
  ostream & operator<<(ostream & outS, const WarpMesh2dBaseC & mesh) {
    outS << mesh.Mesh() << ' ' << mesh.FillBackground() << ' ' << mesh.WarpMapFrame();
    return outS;
  }
  
  //: Read from stream.
  
  istream & operator>>(istream & inS,WarpMesh2dBaseC & mesh) {
    IndexRange2dC rng;
    inS >> mesh.Mesh() >> mesh.FillBackground() >> rng;
    mesh.CreateWarpMap(mesh.Mesh(),rng);
    return inS;    
  }
  
  //: Write to binary stream.
  
  BinOStreamC & operator<<(BinOStreamC & outS, const WarpMesh2dBaseC & mesh) {
    outS << mesh.Mesh() << mesh.FillBackground() << mesh.WarpMapFrame();
    return outS;
  }
  
  //: Read from binary stream.
  
  BinIStreamC & operator>>(BinIStreamC & inS,WarpMesh2dBaseC & mesh) {
    IndexRange2dC rng;
    bool flag;
    inS >> mesh.Mesh() >> flag >> rng;
    mesh.FillBackground() = flag;
    mesh.CreateWarpMap(mesh.Mesh(),rng);
    return inS;    
  }
  
}
