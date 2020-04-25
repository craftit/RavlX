// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIO1394dc
//! docentry="Ravl.API.Images.Video.Video IO.IIDC"
//! userlevel=Develop

//: Tests IIDC camera interface

#include "ImgIO1394dc.hh"

#include "Ravl/Option.hh"
//#include "Ravl/Image/ImgIO1394dc.hh"
#include "Ravl/IO.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"

using namespace RavlN;
using namespace RavlImageN;

//##define PixelType ByteRGBValueC
#define PixelType ByteT
//#define PixelType ByteYUV422ValueC

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC dev = opt.String("d","/dev/raw1394","Firewire device. ");
  StringC out = opt.String("o","@X","Output sequence");
  IntT n = opt.Int("n",-1,"Number of frames (default: unlimited)");
  UIntT camera = opt.Int("c",0,"Select camera");
  bool direct = opt.Boolean("b",false,"Direct open. ");
  opt.Check();
  
  DPIPortC<ImageC<PixelType> > imgIn;
  if(direct) {
    imgIn = DPIImage1394dcC<PixelType > (dev,camera);
  } else {
    StringC fn = StringC("@IIDC:") + dev + "#" + StringC(camera);
    if(!OpenISequence(imgIn,fn)) {
      std::cerr << "Failed to setup camera. \n";
      return 1;
    }
  }
  
  if(!imgIn.IsGetReady()) {
    std::cerr << "Failed to setup camera. \n";
    return 1;
  }
  DPOPortC<ImageC<PixelType> > imgOut;
  if(!OpenOSequence(imgOut,out))
  {
    std::cerr << "Failed to open output. \n";
    return 0;
  }
  RealT fr;
  imgIn.GetAttr("framerate", fr);
  std::cerr << "Frame rate:" << fr << '\n';
  for(IntT i(0); i!=n; ++i)
    imgOut.Put(imgIn.Get());
  
  return 0;
}
