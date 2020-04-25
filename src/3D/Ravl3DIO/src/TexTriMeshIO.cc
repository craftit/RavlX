// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/TexTriMeshIO.cc"

namespace Ravl3DN {

  void InitTexTriMeshIO() 
  {}
  
  static TypeNameC type1(typeid(TexTriMeshC),"TexTriMeshC");  
  
  // Register Stream and BinStream IO for TexTriMeshC
  FileFormatStreamC<TexTriMeshC > FileFormatStream_TexTriMeshC;
  FileFormatBinStreamC<TexTriMeshC > FileFormatBinStream_TexTriMeshC;
}
