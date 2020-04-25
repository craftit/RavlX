// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Geometry.2D"
//! userlevel=Normal
//! file="Ravl/Math/Geometry/Euclidean/2D/exDelaunayTriangulation2d.cc"

#include "Ravl/DelaunayTriangulation2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/Image/DrawLine.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/Random.hh"
#include "Ravl/IO.hh"
#include "Ravl/Array1d.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC file = opt.String("f","@X","Output file. ");
  IntT npnts = opt.Int("p",5,"Number of points to use. ");
  IntT seed = opt.Int("rs",1,"Random number seed. ");
  Index2dC size = opt.Index2d("s",300,300,"Size of image. ");
  opt.Check();
  
  HEMesh2dC mesh;
  ImageC<ByteT> img(size[0],size[1]);
  img.Fill(0);

  RandomSeedDefault(seed);
  
  Array1dC<Point2dC> pnts(npnts);
  
  // Generate a point set.
  for(UIntT i = 0;i < pnts.Size();i++)
    pnts[i] = Point2dC(Random1() * size[0],Random1() * size[1]);
  
  mesh = DelaunayTriangulation(pnts);
  //cerr << "Checking mesh:\n";
  mesh.CheckMesh(true);
  
  Point2dC x(size[0]/2,size[1]/2);
  
  // Draw Mesh
  
  THEMeshFaceC<Point2dC> fface = mesh.FindFace(x);
  
  for(THEMeshFaceIterC<Point2dC> mit(mesh.Faces());mit;mit++) {
    for(THEMeshFaceEdgeIterC<Point2dC> eit(*mit);eit;eit++) {
      if(!eit->HasPair()) {
	RavlImageN::DrawLine(img,(ByteT) 255,
			     Index2dC((*eit).Vertex().Data()),
			     Index2dC((*eit).Prev().Vertex().Data()));
      } else {
	if((*eit).Face() == fface || (*eit).Pair().Face() == fface) {
	  
	  RavlImageN::DrawLine(img,(ByteT) 250,
			       Index2dC((*eit).Vertex().Data()),
			       Index2dC((*eit).Prev().Vertex().Data()));	  
	} else {
	  RavlImageN::DrawLine(img,(ByteT) 128,
			       Index2dC((*eit).Vertex().Data()),
			       Index2dC((*eit).Prev().Vertex().Data()));
	}
      }
    }
  }
  if(!file.IsEmpty())
    RavlN::Save(file,img);
  return 0;
}
