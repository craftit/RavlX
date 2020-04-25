// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/3D/HEMesh3d.cc"

#include "Ravl/HEMesh3d.hh"
#include "Ravl/THEMeshVertexIter.hh"
#include "Ravl/THEMeshFaceIter.hh"
#include "Ravl/THEMeshFace.hh"
#include "Ravl/Hash.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  HEMesh3dC::HEMesh3dC()
  {}
  
  //: Constructor.
  // Creates an empty mesh.
  
  HEMesh3dC::HEMesh3dC(bool)
    : THEMeshC<Point3dC>(true)
  {}
  
  //: Compute the mean position of points in the mesh.
  
  Point3dC HEMesh3dC::Mean() const {
    Point3dC ret(.0,.0,.0);
    UIntT count = 0;
    for(THEMeshVertexIterC<Point3dC> it(Vertices());it;it++) {
      ret += (*it).Data();
      count++;
    }
    return ret / (RealT) count;
  }
  
  //: Compute the bounding rectangle for the points in the mesh.
  
  RealRange3dC HEMesh3dC::BoundingRectangle() const {
    RealRange3dC ret(.0,.0,.0);
    THEMeshVertexIterC<Point3dC> it(Vertices());
    if(!it)
      return ret;
    ret = RealRange3dC((*it).Data(),0);
    for(;it;it++)
      ret.Involve((*it).Data());
    return ret;
  }

  
}
