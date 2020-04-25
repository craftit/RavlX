// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay3d/exDPWindow3d.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.3D Data Display"
//! userlevel=Normal

#include "Ravl/IO.hh"
#include "Ravl/3D/MeshShapes.hh"


int main() {
  // Create an object.
  Ravl3DN::TriMeshC cube = Ravl3DN::CreateTriMeshCube();
  
  // Save it to the display
  RavlN::Save("@GL",cube);
  return 0;
}
