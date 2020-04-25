// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/TriMeshIO.cc"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"

namespace Ravl3DN {

  void InitTriMeshIO() 
  {}
  
  static TypeNameC type1(typeid(TriMeshC),"TriMeshC");  
  
  FileFormatStreamC<TriMeshC > FileFormatStream_TriMeshC;
  FileFormatBinStreamC<TriMeshC > FileFormatBinStream_TriMeshC;
}
