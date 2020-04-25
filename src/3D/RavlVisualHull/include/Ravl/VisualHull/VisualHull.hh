// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_HEADER
#define RAVL_VISUALHULL_HEADER 1
//! lib=RavlVisualHull

//
// Visual hull functions
//

#include "Ravl/HSet.hh"
#include "Ravl/DList.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/3D/PinholeCamera.hh"

#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VisualHullN {

  using VoxelsN::ByteVoxelGridC;
  using Ravl3DN::PinholeCameraC;

   HashC<Index2dC, DListC<Index3dC> > VoxelProjection(
      const HSetC<Index2dC>& pixels,
      const ByteVoxelGridC& voxels,
      const PinholeCameraC& camera,
      IntT expand = 1);
   // Determine the set of voxels corresponding to each pixel in a camera image
   // Returns an unordered list of voxels for each pixel
   // expand defines the size to expand each pixel to catch the voxel projection

   HashC<Index2dC, DListC<Index3dC> > OrderedVoxelProjection(
      const HSetC<Index2dC>& pixels,
      const ByteVoxelGridC& voxels,
      const PinholeCameraC& camera,
      bool singleIntersection = false,
      IntT expand = 1);
   // Determine the ordered set of voxels corresponding to each pixel in a camera image
   // This determines the complete set of voxels whose projection overlaps each pixel
   // set singleIntersection to only return pixels where there is a single intersection with the voxel set
   // expand defines the size to expand each pixel to catch the voxel projection

   HSetC<Index3dC> ExtractSurfaceVoxels(const ByteVoxelGridC& voxels);
   // Extract the surface voxels from a voxel set as all foreground voxels 6-connected to background voxels

}}
#endif
