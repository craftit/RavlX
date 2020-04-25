// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/RavlMeshIO.cc"

#include "Ravl/3D/RavlMeshIO.hh"

namespace Ravl3DN {
  
  void InitRavlMeshIO() {
    InitVRMLFormat();
    InitCTriFormat();
    InitTriFormat();
    InitObjFormat();
    InitTexTriMeshIO();
    InitTriMeshIO();
    InitPOVRayFormat();
    InitMeshConv();
  }
}
