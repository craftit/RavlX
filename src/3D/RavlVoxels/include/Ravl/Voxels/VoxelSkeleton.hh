// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXEL_VOXELSKELETON_HEADER
#define RAVL_VOXEL_VOXELSKELETON_HEADER 1
//! lib=RavlVoxels

//
// VoxelSkeletonC implements a variant of 
//   Bertrand, "A parallel thinning algorithm for medial surfaces"
//   Pattern Recognition Letters, 16:979-986, 1995
// Border erosion is randomised and the medial surface can optionally be thinned to a single voxel thick.
// The class also provides useful voxel connectivity information (OutVoxel / EndPoint / SingleForeground)
//
// VoxelSkeletonC is designed for use with a ByteVoxelGridC
// VoxelSkeletonC holds a reference to the array used by a voxel set. 
// You can then operate on a working copy of the array without having to create a new voxel set.
//
#include "Ravl/Index3d.hh"
#include "Ravl/Types.hh"
#include "Ravl/Array3d.hh"
namespace RavlN { namespace VoxelsN {


   //: Class for eroding a voxel set to a skeleton
   class VoxelSkeletonC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      VoxelSkeletonC()
      { }
      //: Default constructor

      VoxelSkeletonC(const Array3dC<ByteT>& voxelArray, ByteT thresh);
      //: Construct from a voxel set
      // NOTE: this class is constructed from the array for a voxel set so that you can use a working copy.

   public:
      //:-
      // SURFACE EXTRACTION ///////////////////////////////////////////////////

      void ExtractSkeleton(bool thin = false);
      //: erode a voxel set down to a skeleton 

      bool VoxelEndPoint(const Index3dC& voxel) const;
      // Determine if a voxel is an end-point with only 1 26-connected neighbour

      bool SingleForeground(const Index3dC& voxel) const;
      // Return if a given voxel has a single surrounding foreground region
      // Checks that all foreground voxels in the 26 neighbourhood are 26-connected

      bool OuterVoxel(const Index3dC& voxel) const;
      // Return if a voxel is an outer voxel
      // Checks for a single connected background component under 6-connectivity in the 18-neighbourhood

   protected:
      enum Border
      {
         East, West, North, South, Up, Down
      };

      bool ErodeBorder(Array3dC<ByteT>& array, Array3dC<ByteT>& workArray, Border border, bool thin = false);
      // Erode a border using a working array

   protected:

      Array3dC<ByteT> m_array; // Voxel set for skeleton extraction
      IndexRange3dC frame;
      IntT n, m, l, m_l;
      ByteT threshold;

   };

}}

#endif 
