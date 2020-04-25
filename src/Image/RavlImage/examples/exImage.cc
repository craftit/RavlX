// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/exImage.cc"
//! docentry="Ravl.API.Images"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/Option.hh"

#include "Ravl/IO.hh"

using namespace RavlImageN;

// Note: If you wish image IO to work the 'ImageIO' library must be
// specified in USESLIBS or PROGLIBS is the defs.mk file. To use
// many common image file formats you must include 'RavlExtImgIO'.

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC inf = opt.String("","in.pgm","Input image 1. ");
  StringC inf2 = opt.String("","in2.pgm","Input image 2. ");
  StringC outf = opt.String("","out.pgm","Input image. ");
  opt.Check();
  
  ImageC<ByteT> image;
  
  // Load an image from a file.
  
  if(!RavlN::Load(inf,image)) {
    std::cerr << "Failed to load file '" << inf << "' \n";
    return 1;
  }
  
  // Sum all the pixel in an image.
  
  UIntT total = 0;
  for(Array2dIterC<ByteT> it(image);it;it++)
    total += *it;
  
  std::cerr << "Total of pixels in the image = " << total << "\n";
  
  // Load a second image.
  ImageC<ByteT> image2;
  if(!RavlN::Load(inf2,image2)) {
    std::cerr << "Failed to load file '" << inf2 << "' \n";
    return 1;
  }
  
  // Sum the diffrences between the two images.
  
  total = 0;
  for(Array2dIter2C<ByteT,ByteT> it2(image,image2);it2;it2++)
    total += Abs(it2.Data1() - it2.Data2());
  
  std::cerr << "The sum of the differences between image and image2 is " << total << "\n";
  
  // Set a pixel to a value.
  
  image[2][3] = 2; 
  
  // Save image to a file.
  
  if(!RavlN::Save(outf,image)) {
    std::cerr << "Failed to save to file '" << inf << "' \n";
    return 1;
  }
  
  return 0;
}
