// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! file = "Ravl/Contrib/LibGd/testGd.cc"
//! lib=RavlImgGd
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Converters.LibGD"

#include "Ravl/Option.hh"
#include "Ravl/Image/Gd.hh"
#include "Ravl/DP/FileFormatIO.hh"

using namespace RavlN;
using namespace RavlImageN;
 
int main(int nargs,char **argv)
{
  OptionC opts(nargs,argv);
  bool doDisplay = opts.Boolean("d",false,"Display output ");
  opts.Check();
  
  // Create an image
  ImageC<ByteRGBValueC> image(256, 256);
  for (int i = 0; i <256; i++)
    for (int j = 0; j < 256; j++)
      image[i][j] = ByteRGBValueC(i, j, 0);

  // Copy to a Gd image
  GdImageC imageGd(256, 256);
  imageGd.Copy(image);

  // Perform some Gd operations  
  int col = gdImageColorAllocate(imageGd.Ptr(), 255, 0, 0);
  gdImageRectangle(imageGd.Ptr(), 0, 0, 127, 127, col);

  // Copy back to a Ravl image
  ImageC<ByteRGBValueC> imageOut(imageGd.GetImage());
  
  // Display the image
  if(doDisplay)
    Save("@X", imageOut);
  
  return 0;
}
