// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/testV4LIO.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.Video4Linux"
//! userlevel=Develop

#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/ImgIOV4L.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Option.hh"

using namespace RavlImageN;
using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool seq = opt.Boolean("s",false,"Display a sequence.");
  bool rgb = opt.Boolean("rgb",false,"Convert via RGB. ");
  IntT frames = opt.Int("f",-1,"Number of frames to capture, -1 = infinite");
  StringC inName = opt.String("","@V4LH:/dev/video0","Input name. ");
  StringC outName = opt.String("","@X","Output name. ");
  opt.Check();
  
  if(frames > 0)
    seq = true;
  
  if(!rgb) {
    DPIPortC<ImageC<ByteYUVValueC> > imgsIn;
    DPOPortC<ImageC<ByteYUVValueC> > imgsOut;
    if(!OpenISequence(imgsIn,inName,"",true)) {
      cerr << "Failed to open input sequence '" << inName << "'\n";
      return 1;
    }
    if(!OpenOSequence(imgsOut,outName,"",true)) {
      cerr << "Failed to open output sequence '" << outName << "'\n";
      return 1;
    }
    do {
      imgsOut.Put(imgsIn.Get());
    } while(seq && ((--frames) != -1));
  } else {
    DPIPortC<ImageC<ByteRGBValueC> > imgsIn;
    DPOPortC<ImageC<ByteRGBValueC> > imgsOut;
    if(!OpenISequence(imgsIn,inName,"",true)) {
      cerr << "Failed to open input sequence '" << inName << "'\n";
      return 1;
    }
    if(!OpenOSequence(imgsOut,outName,"",true)) {
      cerr << "Failed to open output sequence '" << outName << "'\n";
      return 1;
    }
    do {
      imgsOut.Put(imgsIn.Get());      
    } while(seq && ((--frames) != -1));
  }
  return 0;
}

