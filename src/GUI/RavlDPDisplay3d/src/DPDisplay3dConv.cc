// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDPDisplay3d

#include "Ravl/GUI/DPointSet3d.hh"
#include "Ravl/GUI/DTriMesh3D.hh"
#include "Ravl/GUI/DTexTriMesh3D.hh"
#include "Ravl/GUI/DSurfacePointVertexByteRGBArray.hh"
#include "Ravl/GUI/DSurfacePointVertexArray.hh"

#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"

namespace RavlGUIN {
  
  static TypeNameC type1(typeid(DObject3DC),"RavlGUIN::DObject3DC");
  
  DObject3DC PointSet3d2DObject3d(const PointSet3dC &points) 
  { return DPointSet3dC(points);  }
  
  DObject3DC TriMeshC2DObject3d(const TriMeshC &mesh) 
  { return DTriMesh3DC(mesh);  }
  
  DObject3DC TexTriMeshC2DObject3d(const TexTriMeshC &mesh) 
  { return DTexTriMesh3DC(mesh); }

  DObject3DC SurfacePoint3dArrayC_VertexC2DObject3d(const SurfacePoint3dArrayC<VertexC> &mesh) 
  { return DSurfacePointVertexArrayC(mesh); }
  
  DObject3DC SurfacePoint3dArrayC_VertexColourByteRGBC2DObject3d(const SurfacePoint3dArrayC<VertexColourByteRGBC> &mesh) 
  { return DSurfacePointVertexByteRGBArrayC(mesh); }
  
  DP_REGISTER_CONVERSION_NAMED(PointSet3d2DObject3d,1,"RavlGUIN::DObject3DC PointSet3d2DObject3d(const RavlN::PointSet3dC &)");
  DP_REGISTER_CONVERSION_NAMED(TriMeshC2DObject3d,1,"RavlGUIN::DObject3DC TriMeshC2DObject3d(const Ravl3DN::TriMeshC &)");
  DP_REGISTER_CONVERSION_NAMED(TexTriMeshC2DObject3d,1,"RavlGUIN::DObject3DC TexTriMeshC2DObject3d(const Ravl3DN::TexTriMeshC &)");
  
  DP_REGISTER_CONVERSION_NAMED(SurfacePoint3dArrayC_VertexC2DObject3d,1,"RavlGUIN::DObject3DC SurfacePoint3dArrayC_VertexC2DObject3d(const Ravl3DN::SurfacePoint3dArrayC<VertexC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(SurfacePoint3dArrayC_VertexColourByteRGBC2DObject3d,1,"RavlGUIN::DObject3DC SurfacePoint3dArrayC_VertexColourByteRGBC2DObject3d(const Ravl3DN::SurfacePoint3dArrayC<VertexColourByteRGBC> &)");
  
  
  void InitDPDisplay3dConv() 
  {}
}
