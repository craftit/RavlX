// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
#ifndef RAVL3D_MESHIO_HEADER
#define RAVL3D_MESHIO_HEADER 1
//! file="Ravl/3D/MeshIO/RavlMeshIO.hh"

namespace Ravl3DN {

  extern void InitVRMLFormat();
  extern void InitCTriFormat();
  extern void InitObjFormat();
  extern void InitTriFormat();
  extern void InitTexTriMeshIO();
  extern void InitTriMeshIO();
  extern void InitPOVRayFormat();
  extern void InitMeshConv();

}

#endif
