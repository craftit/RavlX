// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOMeteor1
//! file="Ravl/Contrib/Meteor1/exMeteor.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.Meteor1"
//! userlevel=Normal

#include "Ravl/Image/ImgIOMeteor1.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/ByteRGBValue.hh"

extern void InitDPImageIO(void);

using namespace RavlImageN;

int main()
{
  InitDPImageIO();
  cerr << "Opening stream. \n";
  DPIImageMeteor1C<ByteRGBValueC> imgIn("/dev/meteor0");
  if(!imgIn.IsGetReady()) {
    cerr << "Failed to open grabber. \n";
    return 1;
  }
  ImageC<ByteRGBValueC> testImg;
  
  cerr << "Get image 1 \n";
  if(!imgIn.Get(testImg)) {
    cerr << "Failed to get image 1. \n";
    return 1;
  }
  cerr << "Save image 1\n";
  if(!RavlN::Save("test1.ppm",testImg)) {
    cerr << "Failed to save image. \n";
    return 1;
  }
  cerr << "Get image 2 \n";
  if(!imgIn.Get(testImg)) {
    cerr << "Failed to get image 2. \n";
    return 1;
  }
  cerr << "Save image 2\n";
  if(!RavlN::Save("test2.ppm",testImg)) {
    cerr << "Failed to save image. \n";
    return 1;
  }
  cerr << "Done.\n";
  return 0;
}
