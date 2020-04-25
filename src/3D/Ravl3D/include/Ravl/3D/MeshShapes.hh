// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MESHSHAPES_HEADER
#define RAVL_MESHSHAPES_HEADER 1
//! lib=Ravl3D
//! docentry="Ravl.API.3D.Mesh"
//! author="Charles Galambos"
//! file="Ravl/3D/Mesh/MeshShapes.hh"

#include "Ravl/3D/TriMesh.hh"

namespace Ravl3DN {
  
  //: Create a flat plane
  // Where the length of each side is 'size'
  
  TriMeshC CreateTriMeshPlane(RealT size = 1.0);
  
  //: Create a cube.
  // Where the length of each side is 'size'
  
  TriMeshC CreateTriMeshCube(RealT size = 1.0, const Point3dC &origin = Point3dC(0, 0, 0));
  
  //: Create a sphere.
  // There must be at least 2 layers, and 2 slices.
  TriMeshC CreateTriMeshSphere(UIntT layers,UIntT slices,RealT radius = 1.0);
  
}



#endif
