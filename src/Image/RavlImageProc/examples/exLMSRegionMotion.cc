// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Bill Christmas"
//! date="10/1/1999"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Motion"
//! file="Ravl/Image/Processing/Motion/LMSGradient/exLMSRegionMotion.cc"

//: Calculates motion vectors for a region from a pair of images using LMS fit
//
// Synopsis:
//   exLMSRegion <image1> <image2> <region_image> [-r region_label] <p>
//
// In the region image, all pixels of each region are set to the label for that
// region, starting from 1.  Label 0 is reserved for "not a region" areas.


#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Image/LMSRegionMotion.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"

using namespace RavlN;
using namespace RavlImageN;

int main (int argc, char **argv) 
{
  // process command-line parameters
  OptionC opt(argc, argv);
  IntT region_id = opt.Int("r", 0, "Region I.D. (default: all regions)"); 
  StringC ImageFileName1 = opt.String("", "", "Input image 1");
  StringC ImageFileName2 = opt.String("", "", "Input image 2");
  StringC regionName  = opt.String("", "", "Image of regions");
  opt.CompulsoryArgs(3);
  opt.Check();
  
  // load pair of images
  PairC<ImageC<RealT> > image, filtered;
  if(!RavlN::Load(ImageFileName1, image[0])) {
    cerr << "Failed to load " << ImageFileName1 << "\n";
    return 1;
  }
  if(!RavlN::Load(ImageFileName2, image[1])) {
    cerr << "Failed to load " << ImageFileName2 << "\n";
    return 1;
  }
  // filter images with antialias filter
  Array1dC<RealT> coeffs(-3,3); 
  coeffs[-2]=0.25;
  coeffs[-1]=0.5; 
  coeffs[0]=1;
  coeffs[1]=0.5; 
  coeffs[2]=0.25;
  ConvolveSeparable2dC<RealT> filter(coeffs, coeffs);
  
  for (UIntT i=0; i<=1; ++i) {
    filtered[i] = filter.Apply(image[i]);
  }
  // load image of regions & centre it
  ImageC<IntT> regions;
  if(!RavlN::Load(regionName, regions)) {
    cerr << "Failed to load region " << regionName << "\n";
    return 1;
  }
  
  // compute motion
  LMSRegionMotionC motion(filtered);
  if (opt.IsOnCommandLine("r"))
    cout << "Motion vector: " << motion.Estimate(regions, region_id) << '\n';
  else {
    IntT region_id = 1;
    do {
      Vector2dC vec (motion.Estimate(regions, region_id));
      if (vec.SqrNorm() > -1)  // (i.e. not NaN)
	cout << "Region #" << region_id << "; motion vector: " << vec << '\n';
      else break;
      ++region_id;
    } while (true);
  }
  return 1;
}
