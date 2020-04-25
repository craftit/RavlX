// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Ravl3DIO

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/3D/TriMesh.hh"

using namespace Ravl3DN;

int main() {
  
  TriMeshC ts;
  if(!Load(PROJECT_OUT "/share/RAVL/data/cube.tri",ts,"",true)) {
    cerr << "Failed to load file. \n";
    return 1;
  }
  
  if(!Save("testoutput.tri",ts,"",true)) {
    cerr<< "Failed to save. \n";
    return 1;
  }
  
  return 0;
}
