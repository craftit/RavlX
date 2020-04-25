// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Filters/unDistort.cc"
//! lib=RavlImageProc
//! docentry="Ravl.Applications.Image;Ravl.API.Images.Scaling and Warping"
//! userlevel=Normal
//: Corrects radial distortion in images

#include "Ravl/Option.hh"
#include "Ravl/Image/RemoveDistortion.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/EntryPnt.hh"
//#include "Ravl/Collection.hh"

using namespace RavlN;
using namespace RavlImageN;

int Mosaic(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  opt.Comment("Corrects radial distortion in images");
  RealT cx_ratio = opt.Real("cx",0.5,"Image centre x coordinate as ratio of image width. ");
  RealT cy_ratio = opt.Real("cy",0.5,"Image centre y coordinate as ratio of image height. ");
  RealT fx = opt.Real("fx",1.0,"Focal distance in vertical pixels. ");
  RealT fy = opt.Real("fy",1.0,"Focal distance in horizontal pixels. ");
  RealT K1 = opt.Real("K1",0.0,"Cubic radial distortion coefficient. ");
  RealT K2 = opt.Real("K2",0.0,"Quintic radial distortion coefficient. ");
  int cropT = opt.Int("crt", 0, "Width of cropping region at top of image");
  int cropB = opt.Int("crb", 0, "Width of cropping region at bottom of image");
  int cropL = opt.Int("crl", 0, "Width of cropping region at left of image");
  int cropR = opt.Int("crr", 0, "Width of cropping region at right of image");
  StringC inputFileName = opt.String("","","Input image. ");
  StringC outputFileName = opt.String("","","Output image. ");
  opt.Check();

  ImageC<ByteRGBValueC> inputImage;

  // Open the input image.
  if(!Load(inputFileName,inputImage)) {
    std::cerr << "Failed to open input image '" << inputFileName << "'\n";
    return 1;
  }

  // create distortion correction object
  RemoveDistortionC<ByteRGBValueC,ByteRGBValueC>
    remover(cx_ratio*(RealT)inputImage.Rows(),
	    cy_ratio*(RealT)inputImage.Cols(),
	    fx, fy, K1, K2);

  // set frame from first image
  IndexRange2dC rect(inputImage.Frame());

  // adjust frame for cropping region
  rect.TRow() += cropT; rect.BRow() -= cropB;
  rect.LCol() += cropL; rect.RCol() -= cropR;

  // crop image
  ImageC<ByteRGBValueC> croppedInputImage(inputImage,rect);

  // create output image
  ImageC<ByteRGBValueC> outputImage = inputImage.Copy();

  // apply distortion correction
  remover.Apply(croppedInputImage,outputImage);
  
  // save distortion-corrected image
  if(!Save(outputFileName,outputImage)) {
    std::cerr << "Failed to write to output image '" << outputFileName << "'\n";
    return 1;
  }

  return 0;
}

RAVL_ENTRY_POINT(Mosaic)

  // force template instansiation for irix 

#include "Ravl/config.h"
#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/Collection.hh"
#include "Ravl/Image/ByteRGBMedian.hh"
RavlN::CollectionBodyC<RavlImageN::ByteRGBGreyValueC> dummyvar_undistort_1 ;
#include "Ravl/EvaluateNumInliers.hh"
RavlN::EvaluateNumInliersBodyC dummyvar_undistort_2 ; 
#include "Ravl/StateVectorHomog2d.hh"
RavlN::StateVectorHomog2dBodyC dummyvar_undistort_3 ;
#include "Ravl/ObservationManager.hh"
RavlN::ObservationListManagerBodyC dummyvar_undistort_4 ;
#include "Ravl/ObservationHomog2dPoint.hh"
RavlN::ObservationHomog2dPointBodyC dummyvar_undistort_5 (Vector2dC(), MatrixRSC(),
							  Vector2dC(), MatrixRSC() );
#include "Ravl/FitHomog2dPoints.hh"
RavlN::FitHomog2dPointsBodyC dummyvar_undistort_6 ;
#include "Ravl/LevenbergMarquardt.hh"
StateVectorC dummyvar_undistort_7 ; 
ObservationC dummyvar_undistort_8 ;
DListC<ObservationC> dummyvar_undistort_9 ; 
RavlN::LevenbergMarquardtC dummyvar_undistort_10 ( dummyvar_undistort_7, dummyvar_undistort_9 ) ; 
#endif 
