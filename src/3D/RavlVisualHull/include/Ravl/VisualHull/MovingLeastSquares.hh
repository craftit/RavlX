// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_MOVINGLEASTSQUARES_HEADER
#define RAVL_VISUALHULL_MOVINGLEASTSQUARES_HEADER 1
//! lib=RavlVisualHull

//
// MovingLeastSquaresC
//   Derive the surface for a ByteVoxelGridC
//   Computes the depth to the surface using a moving least squares fit
//   Constructs a FloatVoxelGridC for the surface and extracts the surface using VoxelSurfaceC
//
// MovingLeastSquaresC holds a reference to the voxel set for surface extraction.
//

#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"
#include "Ravl/3D/TriMesh.hh"

#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VisualHullN {

  using Ravl3DN::TriMeshC;
  using Ravl3DN::VertexC;
  using VoxelsN::ByteVoxelGridC;
  using VoxelsN::FloatVoxelGridC;

   TriMeshC
   ExtractVoxelSurfaceMLS(
      const ByteVoxelGridC& voxels, 
      IntT supportSize = 3,
      bool filter = false,
      bool verbose = false);
   // Extract the depth using MLS and reconstruct the surface

   class MovingLeastSquaresC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      MovingLeastSquaresC(const ByteVoxelGridC& voxels)
         : m_voxels(voxels)
      { }
      //: Default constructor

   public:
      //:-
      // SURFACE EXTRACTION ///////////////////////////////////////////////////

      FloatVoxelGridC ExtractDepth(
         IntT supportSize = 3,
         bool filter = false,
         bool verbose = false) const;
      // Extract the depth to the surface in the volume
      // Performs a moving least squares fit to the surface with a quadratic surface
      // supportSize defines the 3D window in the volume to sample points
      //   set the support size to smooth out the discretization on the voxel grid
      // set filter to remove spurious surface points, a median filter is run on the 
      //   depth and where the sign disagrees the filtered value is taken

   protected:

      IntT ExtractSurface(SArray1dC<VertexC>& surfacepts, HashC<Index3dC, IntT>& surface) const;
      // Extract the surface points from a voxel set and returns the number of surface points
      // Returns the surface points as an array of vertices
      // Returns a lookup table to match voxels to the array

   protected:
      ByteVoxelGridC m_voxels; 
   };
}}

#endif 
