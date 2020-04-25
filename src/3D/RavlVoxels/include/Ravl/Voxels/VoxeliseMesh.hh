// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VOXELN_VOXELISEMESH_HEADER
#define RAVL_VOXELN_VOXELISEMESH_HEADER 1
//! lib=RavlVoxels

//
// Convert a mesh to a voxel grid
//
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VoxelsN {

   using Ravl3DN::TriMeshC;

   //: Class for cleaning a voxel set after carving
   class VoxeliseMeshC
   {
   public:
      //:-
      // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

      VoxeliseMeshC()
      { }
      //: Default constructor

      VoxeliseMeshC(const ByteVoxelGridC& voxels)
         : m_voxels(voxels)
      { }
      //: Construct from a voxel set

   public:

      void Update(const TriMeshC& mesh, bool verbose = false);
      // Derive the occupancy for the voxel set from the TriMesh.
      // Internal regions set to 255, external to 0 with a surface threshold of 127
      //
      // This provides an implementation of the voxelisation method in:
      //   "Simplification and Repair of Polygonal Models Using Volumetric Techniques"
      //   IEEE TRANSACTIONS ON VISUALIZATION AND COMPUTER GRAPHICS, VOL. 9, NO. 2, 2003, pp. 191-205
      // The mesh is rendered to a layered depth buffer in the IJ, JK, KI planes of the voxel set
      // Voxel occupancy is derived by counting the number of intersections along a ray for a pixel on these plane
      // The majority vote from the three planes is taken to define the occupancy.
      //

   protected:
      ByteVoxelGridC m_voxels;
   };

   // Wrapper function to automatically setup the voxel grid required for a mesh
   ByteVoxelGridC VoxeliseMesh(const TriMeshC& mesh, RealT voxelSize, bool verbose = false);
}}

#endif 
