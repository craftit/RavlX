// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-14, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VISUALHULL_VOXELCOLOURCARVE_HEADER
#define RAVL_VISUALHULL_VOXELCOLOURCARVE_HEADER 1
//! lib=RavlVisualHull

//
// VoxelColourCarveC implements the Generalized Voxel Coloring algorithm (GVC-LDI)
// see
// @ARTICLE{
//  author =  "Culbertson,W. B. and Malzbender, T. and Slabaugh, G.",
//  title =   "Generalized voxel coloring",
//  journal = "Proceedings of the International Workshop on Vision Algorithms, Lecture Notes in Computer Science",
//  year =    "1999",
//  volume =  "1883",
//  pages =   "100-115",
// }
//
// Colour consistency is tested using the RGB colour variance. Pixels are collected from
// all images for which a voxel is visible. The colour variance is then tested against a threshold.
// Normalised colour is used to account for specular reflection 

#include "Ravl/SArray1d.hh"
#include "Ravl/3D/PinholeCameraArray.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

#include "Ravl/Voxels/VoxelGrid.hh"

namespace RavlN { namespace VisualHullN {

  using VoxelsN::ByteVoxelGridC;
  using Ravl3DN::PinholeCameraArrayC;
  using RavlImageN::ImageC;
  using RavlImageN::ByteRGBValueC;

  //! userlevel = Develop
  //: Multi-view voxel colour carving

  class VoxelColourCarveBodyC : public RCBodyVC
  {
  public:
    //:-
    // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

    VoxelColourCarveBodyC(ByteVoxelGridC voxels,
                          PinholeCameraArrayC& cameras,
                          RealT threshold);
    //: Constructor for colour carving a voxel set

  public:
    //:-
    // CARVING //////////////////////////////////////////////////////////////

    IntT Update(const SArray1dC< ImageC<ByteRGBValueC> >& rgbimages,
                const SArray1dC< ImageC<ByteT> >& silimages);
    //: Carve the voxel set based on a set of images
    // Returns the numbder of carved voxels

  protected:
    //:-
    // CARVING //////////////////////////////////////////////////////////////

    bool LookupVoxel(IndexRange2dC& pixrange, const Index3dC& vox, const IntT cam) const;
    //: Determine the pixels that correspond to a voxel in an image
    // Returns true if a valid pixel range is returned
  
    ImageC< Index3dC > GenerateLDI(const ImageC<ByteT>& sil, const IntT cam) const;
    // Generate a layered depth image for a camera view

    bool FindFirstVoxel(Index3dC& voxel, const IntT cam, const Index2dC& pix) const;
    // Find the first occupied voxel along the ray corresponding to a pixel in a camera image

  protected:
    ByteVoxelGridC m_voxels; // Voxel set to carve
    RealT m_threshold; // Threshold on colour consistency
    PinholeCameraArrayC m_cameras; // Voxel index to image transformation
    SArray1dC<Vector3dC> m_origins; // Camera origin in voxel index coordinates
  };

  //: Multi-view voxel colour carving
  // Carves a volume from colour images and segemented silhouette images

  class VoxelColourCarveC : public RCHandleC<VoxelColourCarveBodyC>
  {
  public:
    //:-
    // CONSTRUCTION/DESTRUCTION /////////////////////////////////////////////

    VoxelColourCarveC()
    { }
    //: default constructor, creates an invalid handle

    VoxelColourCarveC(ByteVoxelGridC voxels,
                      PinholeCameraArrayC& cameras,
                      RealT threshold = 50)
    : RCHandleC<VoxelColourCarveBodyC>(*new VoxelColourCarveBodyC(voxels, cameras, threshold))
    { }
    //: Constructor for carving of a voxel set

  public:

    IntT Update(const SArray1dC< ImageC<ByteRGBValueC> >& rgbimages,
                const SArray1dC< ImageC<ByteT> >& silimages)
    { return Body().Update(rgbimages,silimages); }
    //: Carve the voxel set based on a set of segmented images

  };

}}


#endif
