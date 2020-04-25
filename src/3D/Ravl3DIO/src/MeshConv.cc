// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/MeshConv.cc"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/HEMesh.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"

namespace Ravl3DN {
  
  void InitMeshConv()
  {}
  
  static TypeNameC type1(typeid(HEMeshC),"Ravl3DN::HEMeshC");
  
  TriMeshC TexTriMesh2TriMesh(const TexTriMeshC &obj) 
  { return obj; }
  
  TexTriMeshC TriMesh2TexTriMesh(const TriMeshC &obj)
  { return TexTriMeshC(obj); }
  
  TriMeshC HEMesh2TriMesh(const HEMeshC &obj) 
  { return obj.TriMesh(); }

  HEMeshC TriMesh2HEMesh(const TriMeshC &obj) 
  { return HEMeshC(obj); }
  

  DP_REGISTER_CONVERSION_NAMED(TriMesh2TexTriMesh ,1,
			       "Ravl3DN::TexTriMeshC Ravl3DN::Convert(const Ravl3DN::TriMeshC &)");
  
  DP_REGISTER_CONVERSION_NAMED(TexTriMesh2TriMesh ,2,
			       "Ravl3DN::TriMeshC Ravl3DN::Convert(const Ravl3DN::TexTriMeshC &)");
  
  DP_REGISTER_CONVERSION_NAMED(HEMesh2TriMesh ,1,
			       "Ravl3DN::TriMeshC Ravl3DN::Convert(const Ravl3DN::HEMeshC &)");

  DP_REGISTER_CONVERSION_NAMED(TriMesh2HEMesh ,1,
			       "Ravl3DN::HEMeshC Ravl3DN::Convert(const Ravl3DN::TriMeshC &)");
  
  
}
