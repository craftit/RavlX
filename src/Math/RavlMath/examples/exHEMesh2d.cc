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
//! file="Ravl/Math/Geometry/Euclidean/2D/exHEMesh2d.cc"

#include "Ravl/HEMesh2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/Image/DrawLine.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/IO.hh"
#include "Ravl/Random.hh"
#include "Ravl/HashIter.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  IntT n = opt.Int("n",0,"Number of points to put in mesh. ");
  StringC file = opt.String("f","@X","Output filename. ");
  opt.Check();
  
  HEMesh2dC mesh(true);
  
  //: Put 2 initial triangular faces in the mesh to make a square
  
  SArray1dC<HEMeshBaseVertexC> tempFace(3);
  tempFace[0] = mesh.InsertVertex(Point2dC(10,10));
  tempFace[1] = mesh.InsertVertex(Point2dC(10,90));
  tempFace[2] = mesh.InsertVertex(Point2dC(90,10));
  
  HashC<Tuple2C<HEMeshBaseVertexC,HEMeshBaseVertexC> , HEMeshBaseEdgeC> edgeTab;
  mesh.InsertFace(tempFace,edgeTab); // Insert initial face.
  
  SArray1dC<HEMeshBaseVertexC> tempFace2(3);
  tempFace2[0] = tempFace[2];
  tempFace2[1] = tempFace[1];
  tempFace2[2] = mesh.InsertVertex(Point2dC(90,90));
  mesh.InsertFace(tempFace2,edgeTab); // Insert initial face.
  
  //: Insert vertexes randomly into the mesh.
  
  for(int i = 0;i < n;i++) {
    Point2dC npnt(Random1() * 70 + 15,Random1() * 70 + 15);
    cerr << "New point=" << npnt << "\n";
    THEMeshFaceC<Point2dC> face = mesh.FindFace(npnt);
    RavlAssert(face.IsValid());
    HEMeshBaseVertexC vertex    = mesh.InsertVertex(npnt);
    mesh.InsertVertexInFace(vertex,face);
  }
  
  // Draw mesh into an image, highlight edges which are open..
  
  ImageC<ByteT> img(100,100);
  img.Fill(0);
  
  for(THEMeshFaceIterC<Point2dC> mit(mesh.Faces());mit;mit++) {
    for(THEMeshFaceEdgeIterC<Point2dC> eit(*mit);eit;eit++) {
      if(!eit->HasPair()) {
	RavlImageN::DrawLine(img,(ByteT) 90,
			     Index2dC((*eit).Vertex().Data()),
			     Index2dC((*eit).Prev().Vertex().Data()));
      } else {
	RavlImageN::DrawLine(img,(ByteT) 255,
			     Index2dC((*eit).Vertex().Data()),
			     Index2dC((*eit).Prev().Vertex().Data()));	  
      }
    }
  }
  if(!file.IsEmpty())
    RavlN::Save(file,img);
  
  return 0;
}

