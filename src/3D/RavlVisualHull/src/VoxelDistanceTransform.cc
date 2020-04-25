// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// Ravl includes
#include "Ravl/Array3dIter2.hh"
//! lib=RavlVisualHull

#include "Ravl/VisualHull/VisualHull.hh"
#include "Ravl/VisualHull/VoxelDistanceTransform.hh"

namespace RavlN { namespace VisualHullN {

   // Extract the surface voxels from a voxel set as the zero level to compute the distance transform
   void VoxelDistanceTransformC::InitSurfaceVoxels(Array3dC<ByteT>& arr, const ByteVoxelGridC& voxels)
   {
      RavlAssert( arr.Frame() == voxels.Array().Frame() );

      arr.Fill(255);
      HSetC<Index3dC> surface = ExtractSurfaceVoxels(voxels);
      HSetIterC<Index3dC> its(surface);
      for (; its; its++)
      {
         arr[its.Data()] = 0;
      }
   }

   // 3D CHAMFER DISTANCE //////////////////////////////////////////////////

   const IntT VoxelDistanceTransformC::LDM[13] = {5, 4, 5, 4, 3, 4, 5, 4, 5, 4, 3, 3, 0};
   // Forward scan constants
   const IntT VoxelDistanceTransformC::fDI[13] = {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0, 0};
   const IntT VoxelDistanceTransformC::fDJ[13] = {-1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1,  0, 0};
   const IntT VoxelDistanceTransformC::fDK[13] = {-1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0, -1, 0};
   // Backward scan constants
   const IntT VoxelDistanceTransformC::bDI[13] = {1,  1,  1, 1,  1,  1,  1,  1,  1,  0,  0,  0, 0};
   const IntT VoxelDistanceTransformC::bDJ[13] = {1,  1,  1, 0,  0,  0, -1, -1, -1,  1,  1,  0, 0};
   const IntT VoxelDistanceTransformC::bDK[13] = {1,  0, -1, 1,  0, -1,  1,  0, -1,  1,  0,  1, 0};

   // Determine the chamfer distance in the voxel set
   // This uses the complete euclidean 3x3x3 mask
   // For further details see:
   //   "3D Distance Fields: A Survey of Techniques and Applications"
   //   IEEE TRANSACTIONS ON VISUALIZATION AND COMPUTER GRAPHICS, VOL. 12, NO. 4, pp 581 - 599 
   void 
      VoxelDistanceTransformC::ChamferDistance(Array3dC<ByteT>& arr, ByteT label)
   {
      IndexRange3dC voxframe = arr.Frame();
      IntT n = voxframe.Range1().Size();
      IntT m = voxframe.Range2().Size(); 
      IntT l = voxframe.Range3().Size();
      IntT n_minus_1 = n - 1;
      IntT m_minus_1 = m - 1;
      IntT l_minus_1 = l - 1;
      IntT m_l = m*l;

      // Initialise the distance map to the maximum possible distance
      // An integer approximation to the distance is used using the {5,4,3} mask
      Array3dC<UInt16T> dist(voxframe);
      UInt16T maxdist = 255*3;
      Array3dIter2C<ByteT,UInt16T> it(arr, dist);
      for (; it; it++)
      {
         it.Data2() = ( (it.Data1() == label) ? 0 : maxdist );
      }

      // Forward scan
      UInt16T* pvoxel0 = &dist[voxframe.Origin()];
      UInt16T* pvoxel = pvoxel0;
      // Iterate over the slices
      IntT i,j,k,v;
      for (i = 0; i < n; i++) 
      {
         // Iterate over the rows
         for (j = 0; j < m; j++)
         {
            // Iterate over the cells
            for (k = 0; k < l; k++, pvoxel++)
            {
               // Check neighbourhood
               UInt16T val = *pvoxel;
               for (v=0; v<13; v++)
               {
                  IntT ii = i + fDI[v];
                  IntT jj = j + fDJ[v];
                  IntT kk = k + fDK[v];
                  if (ii>=0 && ii<n && jj>=0 && jj<m && kk>=0 && kk<l)
                  {
                     UInt16T nval = pvoxel0[m_l*ii + l*jj + kk] + LDM[v];
                     if (nval < val)
                     {
                        val = nval;
                     }
                  }
               }
               *pvoxel = val;
            }
         }
      }
      // Backward scan
      pvoxel = &dist[voxframe.End()];
      // Iterate over the slices
      for (i = n_minus_1; i >= 0; i--) 
      {
         // Iterate over the rows
         for (j = m_minus_1; j >= 0; j--)
         {
            // Iterate over the cells
            for (k = l_minus_1; k >= 0; k--, pvoxel--)
            {
               // Check neighbourhood
               UInt16T val = *pvoxel;
               for (v=0; v<13; v++)
               {
                  IntT ii = i + bDI[v];
                  IntT jj = j + bDJ[v];
                  IntT kk = k + bDK[v];
                  if (ii>=0 && ii<n && jj>=0 && jj<m && kk>=0 && kk<l)
                  {
                     UInt16T nval = pvoxel0[m_l*ii + l*jj + kk] + LDM[v];
                     if (nval < val)
                     {
                        val = nval;
                     }
                  }
               }
               *pvoxel = val;
            }
         }
      }

      // Now set the returned distance
      for (it.First(); it; it++)
      {
         it.Data1() = (ByteT)Round((RealT)it.Data2() / 3.0);
      }
   }

}}
