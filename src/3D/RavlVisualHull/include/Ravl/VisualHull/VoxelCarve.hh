// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_VOXELCARVE_HEADER
#define RAVL_VISUALHULL_VOXELCARVE_HEADER 1
//! lib=RavlVisualHull

//
// VoxelCarveC implements a volumetric visual hull construction algorithm on an octree.
//
// The visual-hull provides the maximal volume consistent with a given set of silhouettes.
// Voxel-silhouette overlap is determine using a rectangular footprint in the images.
// Overlap is determined upto the reprojection error in the camera system.
// The voxel value provides a quantized estimate of the depth to the surface at the voxel centre.
// Set the voxel size according to the smallest feature in the silhouettes.
//
// NOTE: The occupied volume is always subdivided down to the finest voxel size sop that the
// voxel iterator traverses the internal region. This ensures that surface extraction does not leave
// any holes. i.e it is possible to get a surface plane that falls on the boundary of a parent voxel
// surface extraction would not then traverse the voxels in this region and would not triangulate the
// surface.

#include "Ravl/SArray1d.hh"
#include "Ravl/RealRange3d.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/3D/PinholeCameraArray.hh"

#include "Ravl/Voxels/VoxelOctree.hh"

namespace RavlN { namespace VisualHullN {

  using Ravl3DN::PinholeCameraArrayC;
  using VoxelsN::ByteVoxelOctreeC;
  using RavlImageN::ImageC;
  using VoxelsN::VoxelOctreeNodeC;

    // Volumetric shape from silhouette
    class VoxelCarveC
    {
    public:
        //:-
        // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////
        VoxelCarveC(ByteVoxelOctreeC voxels,
                    PinholeCameraArrayC cameras,
                    SArray1dC< ImageC<ByteT> > silimages,
                    SArray1dC<IntT> reprojError,
                    RealRange3dC vol,
                    IntT minViews,
                    RealT threshold,
                    ByteT silthresh,
                    bool conservative,
                    bool subdivide);
        //: Constructor for carving of a voxel set

    public:
        //:-
        // CARVING //////////////////////////////////////////////////////////////
        void OctreeCarve();
        // Carve an octree inside a restricted reconstruction volume
        void SetProb(bool bProb){m_probablistic = bProb;};

    protected:
        enum ClipState {
            NotClipped = 0,
            Clipped = 1,
            FullyClipped = 2
        };

        void CarveNode(VoxelOctreeNodeC<ByteT> node, int childIndex = -1);
        // Carve an octree node

        ImageC<IntT> ConstructIntegralImage(const ImageC<ByteT> &silhouette, ByteT silthresh);
        // Construct an integral image

        void SetVoxels(const ByteVoxelOctreeC &voxels);
        // Set up a voxel set for carving

        SArray1dC< Vector3dC > CalculateNodeExtents(const VoxelOctreeNodeC<ByteT> &node, int childIndex);
        RealT DetermineOverlap(const SArray1dC< Vector3dC > &extents, IntT cam_index, const ImageC<IntT> &int_image, ClipState &clipped);

        RealT CalculateDepth(const SArray1dC< Vector3dC > &extents);
        RealT CalculateConservativeDepth(const SArray1dC< Vector3dC > &extents);
        RealT CalculateConsensusDepth(const SArray1dC< Vector3dC > &extents);
        RealT CalculateProbablisticDepth(const SArray1dC< Vector3dC > &extents);

        PinholeCameraArrayC           m_cameras;
        SArray1dC< ImageC<ByteT> >    m_silimages;
        SArray1dC<IntT>               m_reprojErrors;
        RealRange3dC                  m_vol;
        IntT                          m_minViews;
        RealT                         m_threshold;
        RealT                         m_silthresh;
        bool                          m_conservative;
        bool                          m_subdivide;
        bool                          m_consensus;
        bool                          m_probablistic;

        SArray1dC< ImageC<IntT> >     m_intimages;
        SArray1dC< ImageC<IntT> >     m_cons_intimages;

        ByteVoxelOctreeC m_voxels;
        Vector3dC m_iDir;
        Vector3dC m_jDir;
        Vector3dC m_kDir;
   };

   // Carve an Octree voxel set
   void CarveVoxels(ByteVoxelOctreeC& voxels,
                    const PinholeCameraArrayC& cameras,
                    const SArray1dC< ImageC<ByteT> >& silimages,
                    IntT reprojError = 0,
                    ByteT silthresh = 127,
                    IntT minViews = 2,
                    bool conservative = false,
                    bool subdivide = true,
                    RealT threshold = 1.0);

   // Carve an Octree voxel set
   // Restrict reconstruction to a given volume range
   void CarveVoxels(ByteVoxelOctreeC& voxels,
                    const PinholeCameraArrayC& cameras,
                    const SArray1dC< ImageC<ByteT> >& silimages,
                    const RealRange3dC& range,
                    IntT reprojError = 0,
                    ByteT silthresh = 127,
                    IntT minViews = 2,
                    bool conservative = false,
                    bool subdivide = true,
                    RealT threshold = 1.0);

   // Carve an Octree voxel set
   // Restrict reconstruction to a given volume range and takes an array of reprojection errors
   void CarveVoxels(ByteVoxelOctreeC& voxels,
                    const PinholeCameraArrayC& cameras,
                    const SArray1dC< ImageC<ByteT> >& silimages,
                    const RealRange3dC& range,
                    SArray1dC<IntT> reprojError,
                    ByteT silthresh = 127,
                    IntT minViews = 2,
                    bool conservative = false,
                    bool subdivide = true,
                    RealT threshold = 1.0);

   // Carve an Octree voxel set
   void CarveVoxelsProbablistic(ByteVoxelOctreeC& voxels,
                    const PinholeCameraArrayC& cameras,
                    const SArray1dC< ImageC<ByteT> >& silimages,
                    const RealRange3dC& range,
                    ByteT silthresh = 127,
                    IntT minViews = 2);
}}


#endif
