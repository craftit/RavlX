// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/Tri.cc"

#include "Ravl/3D/Tri.hh"

namespace Ravl3DN {

  //: Flips the triangle.
  // Reverse the order of the vertices in the triangle.
  
  void TriC::Flip(void) {  
    Swap(vertices[0],vertices[2]); 
    Swap(texture[0],texture[2]); 
    normal = normal * -1;
  }
    
  //: Update the face normal.
  
  void TriC::UpdateFaceNormal() { 
    normal = (Vector3dC(Position(1) - Position(0)).Cross(Position(2) - Position(0))).Unit(); 
  }


}
