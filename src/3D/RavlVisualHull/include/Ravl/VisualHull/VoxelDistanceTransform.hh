// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_VOXELDISTANCETRANSFORM_HEADER
#define RAVL_VISUALHULL_VOXELDISTANCETRANSFORM_HEADER 1
//! lib=RavlVisualHull

//
// VoxelDistanceTransformC provides methods to derive the distance function in a voxel grid.
//

#include "Ravl/Array3d.hh"

#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VisualHullN {

  using VoxelsN::ByteVoxelGridC;

   class VoxelDistanceTransformC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      VoxelDistanceTransformC()
      { }
      //: Default constructor

   public:
      void InitSurfaceVoxels(Array3dC<ByteT>& arr, const ByteVoxelGridC& voxels);
      // Extract the surface voxels from a voxel set as the zero level to compute the distance transform
      // Initialise arr as Array3dC<ByteT>( m_voxels.Array().Frame() );

      void ChamferDistance(Array3dC<ByteT>& arr, ByteT label = 0);
      // Determine the chamfer distance in the array to voxels with the given label
      // This uses the complete euclidean 3x3x3 mask, For further details see:
      //   "3D Distance Fields: A Survey of Techniques and Applications"
      //   IEEE TRANSACTIONS ON VISUALIZATION AND COMPUTER GRAPHICS, VOL. 12, NO. 4, pp 581 - 599 
      // The array is replaced with the distance field, the maximum distance being 255

   protected:
      static const IntT LDM[13], fDI[13], bDI[13], fDJ[13], bDJ[13], fDK[13], bDK[13];
   };

}}

#endif 
