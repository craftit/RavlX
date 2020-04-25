// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIO1394dc
//! docentry="Ravl.API.Images.Video.Video IO.IIDC"
//! userlevel=Normal
//: Grabs from pair of IIDC cameras
// Grabs images sequences from a pair of IIDC cameras and stores or displays them.
//! file="Ravl/Contrib/dc1394/twocam.cc"

#include "Ravl/Option.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Image/ImgIO1394dc.hh"
#include "Ravl/IO.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC out = opt.String("o","@X","Output sequence");
  IntT n = opt.Int("n",-1,"Number of frames (default: unlimited)");
  opt.Check();
  
  PairC<DPIPortC<ImageC<ByteT> > > cameras;
  PairC<DPOPortC<ImageC<ByteT> > > imgOut;
  StringC cameraName("@IIDC:#");
  for (UIntT i(0); i<=1; ++i) {
    if(!OpenISequence(cameras[i],cameraName+i)) {
      std::cerr << "Failed to setup camera " << i << ". \n";
      return 1;
    }
    StringC outname;
    if (i == 0)  outname = "left";
    else  outname = "right";
    if (out == "@X") outname = StringC("@X:") + outname;
    else outname = outname + "%4d.pgm";
    if(!OpenOSequence(imgOut[i],outname)) {
      std::cerr << "Failed to open output " << outname << ". \n";
      return 0;
    }
  }
  ImageC<ByteT> img;
  for(IntT i(0); i!=n; ++i) {
    for (UIntT i(0); i<=1; ++i) {
      cameras[i].Get(img);
      imgOut[i].Put(img);
    }
  }
  return 0;
}
