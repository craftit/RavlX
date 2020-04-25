// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/2D/TriMesh2d.cc"

#include "Ravl/TriMesh2d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Find face containing point 'pnt'.
  // This assumes the edges around a face are ordered clockwise.
  // Note: This does a linear searh through the faces, it doesn't
  // use any clever indexing/search scheme at the moment
  
  IntT TriMesh2dC::FindFace(const Point2dC &pnt) {
    for(SArray1dIterC<Index3dC> it(faces);it;it++) {
      const  Point2dC &p1 = vertices[(*it)[0]];
      const  Point2dC &p2 = vertices[(*it)[1]];
      const  Point2dC &p3 = vertices[(*it)[2]];
      if(!LinePP2dC(p1,p2).IsPointToRight(pnt))
	continue;
      if(!LinePP2dC(p2,p3).IsPointToRight(pnt))
	continue;
      if(!LinePP2dC(p3,p1).IsPointToRight(pnt))
	continue;
      return it.Index().V(); // Found !
    }
    return -1;
  }
  
  //: Compute the bounding rectangle for the points in the mesh.
  
  RealRange2dC TriMesh2dC::BoundingRectangle() const {
    RealRange2dC ret(0,0);
    SArray1dIterC<Point2dC> it(Vertices());
    if(!it)
      return ret;
    ret = RealRange2dC(*it,0);
    for(;it;it++)
      ret.Involve(*it);
    return ret;
  }
  
  //: Check that all faces in the mesh are oriented in the same direction.
  
  bool TriMesh2dC::IsFlat() const {
    bool clock;
    SArray1dIterC<Index3dC> it(faces);
    if(!it) 
      return true;    
    clock = LinePP2dC(vertices[(*it)[0]],vertices[(*it)[1]]).IsPointToRight(vertices[(*it)[2]]);
    for(it++;it;it++)
      if(LinePP2dC(vertices[(*it)[0]],vertices[(*it)[1]]).IsPointToRight(vertices[(*it)[2]]) != clock)
	return false;
    return true;
  }


  ostream & operator<<(ostream & outS, const TriMesh2dC & mesh) {
    outS << mesh.Vertices() << ' ' << mesh.Faces();
    return outS;
  }
  
  istream & operator>>(istream & inS, TriMesh2dC & mesh) {
    SArray1dC<Point2dC> vertices;
    SArray1dC<Index3dC> faces;
    inS >> vertices >> faces;
    mesh = TriMesh2dC(vertices,faces);
    return inS;
  }
  
  BinOStreamC & operator<<(BinOStreamC & outS, const TriMesh2dC & mesh) {
    outS << mesh.Vertices() << mesh.Faces();
    return outS;
  }
  
  BinIStreamC & operator>>(BinIStreamC & inS, TriMesh2dC & mesh) {
    SArray1dC<Point2dC> vertices;
    SArray1dC<Index3dC> faces;
    inS >> vertices >> faces;
    mesh = TriMesh2dC(vertices,faces);
    return inS;
  }

}
