// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HEMESH3D_HEADER
#define RAVL_HEMESH3D_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Geometry.3D"
//! lib=RavlMath
//! example=exHEMesh3d.cc
//! file="Ravl/Math/Geometry/Euclidean/3D/HEMesh3d.hh"

#include "Ravl/THEMesh.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/RealRange3d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 3D Half edge mesh.
  
  class HEMesh3dC
    : public THEMeshC<Point3dC>
  {
  public:
    HEMesh3dC();
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit HEMesh3dC(bool);
    //: Constructor.
    // Creates an empty mesh.
    
    Point3dC Mean() const;
    //: Compute the mean position of points in the mesh.
    
    RealRange3dC BoundingRectangle() const;
    //: Compute the bounding rectangle for the points in the mesh.
    
  };
    
}

#endif
