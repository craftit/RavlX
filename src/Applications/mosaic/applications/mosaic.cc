// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! file="Ravl/Applications/Tracker/mosaic.cc"
//! author="Phil McLauchlan"
//! lib=RavlImageProc
//! docentry="Ravl.Applications.Video"
//! userlevel=Normal

//: Creates a mosaic from a video sequence as a batch process
//
// <p>Runs as a batch process, in contrast to <a href=xmosaic.html>xmosaic</a>.</p>
// <p>Main operations are:</p> <ul>
// <li> corrects for lens distortion (optional)</li>
// <li> applies Harris corner detector to each frame</li>
// <li> tracks corners through sequence</li>
// <li> uses RANSAC on corners to compute a homography</li>
// <li> uses Levenberg-Marquart to improve homography </li>
// <li> uses homography to warp each frame into a reference frame</li>
// <li> median filter applied to warped frames to create the mosaic</li>
// <li> warp mosaic into each frame and subtract from frame to generate foreground objects (optional)</li>
// </ul>

#include "Ravl/Option.hh"
#include "Ravl/Image/MosaicBuilder.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"

#if RAVL_COMPILER_MIPSPRO
#include <stdlib.h> 
#endif 

using namespace RavlN;
using namespace RavlImageN;

// These are the 3rd vector elements in the respective homogeneous
// coordinate systems (i.e. it doesn't always have to be 1)
#define IMAGE_ZHOMOG 100.0
#define MOSAIC_ZHOMOG 1.0


int Mosaic(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  opt.Comment("Creates mosaic image from video sequence, and generates sequence of foreground images based on difference between input images and mosaic.\n\n");
  opt.Comment("It is not necessary to specify the mosaic size with this version; on the other hand, lens distortion correction cannot be performed (use unDistort as preprocessing step instead).\n\n");
  opt.Comment("Preprocessing:");
  int cropT = opt.Int("crt", 0, "Width of cropping region at top of image");
  int cropB = opt.Int("crb", 0, "Width of cropping region at bottom of image");
  int cropL = opt.Int("crl", 0, "Width of cropping region at left of image");
  int cropR = opt.Int("crr", 0, "Width of cropping region at right of image");
  opt.Comment("Camera details:");
  RealT cx_ratio = opt.Real("cx",0.5,"Image centre x coordinate as ratio of image width. ");
  RealT cy_ratio = opt.Real("cy",0.5,"Image centre y coordinate as ratio of image height. ");
  RealT fx = opt.Real("fx",1.0,"Focal distance in vertical pixels. ");
  RealT fy = opt.Real("fy",1.0,"Focal distance in horizontal pixels. ");
  RealT K1 = opt.Real("K1",0.0,"Cubic radial distortion coefficient. ");
  RealT K2 = opt.Real("K2",0.0,"Quintic radial distortion coefficient. ");
  opt.Comment("Tracking:");
  int newFreq    = opt.Int("ff",1,"Frequency of introducing new features. ");
  int cthreshold = opt.Int("ct",30,"Corner threshold. ");
  int cwidth     = opt.Int("cw",7,"Corner filter width. ");
  int mthreshold = opt.Int("mt",20,"Match threshold. ");
  int mwidth     = opt.Int("mw",17,"Tracker feature width (i.e. minimum distance between features). ");
  int lifeTime   = opt.Int("ml",8,"Lifetime of a point without a match in the incoming images. ");
  int searchSize = opt.Int("ss",25,"Search size. How far to look from the predicted position of the feature.");
  StringC maskName = opt.String("mask", "", "Mask to exclude image regions from tracking (black = omit point; default - no mask)");
  opt.Comment("Homographies:");
  StringC homogFile = opt.String("hf", "", "Output file for interframe projections used to construct mosaic (default: no output)");
  opt.Comment("Mosaic:");
  IntT refineSearchSize = opt.Int("rss", 5, "Search size for image matcher of homography refinement (0 to disable)");
  StringC mosaicFile = opt.String("mo", "/tmp/mosaic.ppm", "Output file for mosaic");
  Index2dC border = opt.Index2d("bo", 200, 200, "Width of vertical & horizontal borders around initial mosaic image (default is (0, 0) if auto is on)");
  autoResizeT resize = (autoResizeT) opt.Int("auto", 1, "Automatically expands the mosaic rectangle to accommodate projected images as necessary. 0 = no expansion; 1 = one-pass (slower); 2 = two-pass (faster)");
  Point2dC pointTL = opt.Point2d("ptl", 0.0, 0.0, "Top-left coordinates of projection of first image (in units of picture size)");
  Point2dC pointTR = opt.Point2d("ptr", 0.0, 1.0, "Top-right coordinates of projection of first image (in units of picture size)");
  Point2dC pointBL = opt.Point2d("pbl", 1.0, 0.0, "Bottom-left coordinates of projection of first image (in units of picture size)");
  Point2dC pointBR = opt.Point2d("pbr", 1.0, 1.0, "Bottom-right coordinates of projection of first image (in units of picture size)");
  UIntT filterSubsample = opt.Int("sub",1," Temporal subsample factor for median filter");
  opt.Comment("Foreground image generation:");
  StringC ofn = opt.String("fg","","Output sequence. (Default: no foreground sequence generated)");
  int fgThreshold = opt.Int("ft",8,"Minimum distance between image and mosaic pixel values to be a foreground pixel");
  opt.Comment("Input and output video:");
  int startFrame = opt.Int("sf", 0, "Start frame");
  int maxFrames = opt.Int("nf",200,"Maximum number of frames to process");
  StringC ifn = opt.String("","@V4LH:/dev/video0","Input sequence. ");
  bool verbose = opt.Boolean("v", false, "Verbose output");
  opt.Check();
    
#if 0
  inp.SetAttr("FrameBufferSize","2");
#endif

  // Create a mosaic class instance
  MosaicBuilderC mosaicBuilder(resize, verbose);

  // Set tracker params
  mosaicBuilder.SetTracker(cthreshold, cwidth, mthreshold, mwidth,
			   lifeTime, searchSize, newFreq);
  // Crop image borders
  mosaicBuilder.SetCropRegion(cropT, cropB, cropL, cropR);

  // Load mask to avoid unwanted regions if required 
  if(!maskName.IsEmpty())  mosaicBuilder.SetMask(maskName);

  // Set mosaic coordinate system
  mosaicBuilder.SetCoordinates(pointTL, pointTR, pointBL, pointBR);

  // If we are not automaically growing the mosaic size as required, we need to expand the mosaic border to allow for growth
  if ((resize == none) || opt.IsOnCommandLine("bo")) 
    mosaicBuilder.SetBorderExpansion(border.Row(), border.Col());

  // Trigger (or not) refinement of homography
  mosaicBuilder.SetHomogRefine( refineSearchSize);

  // If there is lens distortion, correct it (& save the results)
  if (K1 > 0.0 || K2 > 0.0) mosaicBuilder.SetLensCorrection(K1, K2, cx_ratio, cy_ratio, fx, fy);

  // Skip frames if required
  mosaicBuilder.SetSubsample(filterSubsample);

  // Build the mosaic homograpy data
  if (!mosaicBuilder.GrowMosaic(ifn, startFrame, maxFrames)) exit (-1);

  // construct mosaic image from median image & save it
  Save (mosaicFile, mosaicBuilder.GetMosaic());

  // save interframe projections if required
  if (homogFile != "") {
    OStreamC homogStream(homogFile);
    homogStream << mosaicBuilder.GetMotion()
		<< mosaicBuilder.GetCropRect() << endl 
		<< mosaicBuilder.GetMosaic().Rectangle() << endl
		<< K1 << " " << K2 << endl;
    homogStream.Close();
  }

  if (!opt.IsOnCommandLine("fg")) return 0;

  // Open the output stream for the foreground segmentation images.
  DPOPortC<ImageC<ByteRGBValueC> > outp;
  if(!OpenOSequence(outp,ofn,"")) {
    cerr << "Failed to open output '" << ofn << "'\n";
    return 1;
  }
  mosaicBuilder.GenerateForeground(startFrame, maxFrames, outp, fgThreshold);
  cout << "Output finished" << endl;
  return 0;
}

RAVL_ENTRY_POINT(Mosaic)
