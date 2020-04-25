// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_VOXELCLEAN_HEADER
#define RAVL_VISUALHULL_VOXELCLEAN_HEADER 1
//! lib=RavlVisualHull

//
// VoxelCleanC provides methods to clean a carved voxel set.
//
// VoxelCleanC holds a reference to the voxel set to be cleaned. Construct once for the voxel
// set you are carving, then use whenever the voxel set is updated.
//

#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VisualHullN {
  using VoxelsN::ByteVoxelGridC;


   //: Class for cleaning a voxel set after carving
   class VoxelCleanC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      VoxelCleanC()
      { }
      //: Default constructor

      VoxelCleanC(const ByteVoxelGridC& voxels);
      //: Construct from a voxel set

   public:
      //:-
      // VOXEL SET CLEANING ///////////////////////////////////////////////////

      void ExtractSingleRegion();
      // Use connected components to extract the largest 26-connected region in the voxel set
      // This function removes the disconnected regions in the voxel set
      // NOTE: you can alternatively extract a single connected mesh from a voxel set using VoxelSurfaceC

      void SmoothSurface()
      {
         CleanEndPoints();
         FillHoles();
      }
      // Erode end points and fill holes to smooth the surface of a voxel set

      void RemoveNoise()
      {
         MorphologicalOpen();
         MorphologicalClose();
      }
      // Remove 'salt & pepper' noise using an open-close filter

      void Dilate();
      // Dilate the voxel set while not connecting disconnected regions

      void Erode();
      // Erode the voxel set

      void MorphologicalOpen();
      // Remove surface noise by eroding then dilating

      void MorphologicalClose();
      // Fill holes in the voxel set by dilating then eroding
      // NOTE: this may end up connecting voxel regions

      void CleanEndPoints();
      // Erode end points from a voxel set to remove noise on the surface

      void FillHoles();
      // Dilate a voxel set to fill holes

   protected:
      ByteVoxelGridC m_voxels; // Voxel set for cleaning
      IndexRange3dC voxframe;
      IntT n, m, l, m_l;
      IntT n_minus_1, m_minus_1, l_minus_1;
   };

}}

#endif 
