// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMPEG2
//! author="Charles Galambos"
//! file="Ravl/Contrib/MPEG/mpeg2dec/ImgIOmpeg2dec.cc"

#include "Ravl/Image/ImgIOmpeg2dec.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  //: Constructor.
  
  ImgIOmpeg2decBaseC::ImgIOmpeg2decBaseC(const StringC &filename,bool transportStream)
    : ok(false)
  {
    ONDEBUG(cerr << "ImgIOmpeg2decBaseC::ImgIOmpeg2decBaseC(), Called for '" << filename << "'. \n");
    StringC cmd;
    if(transportStream)
      cmd = StringC("mpeg2dec -s -o pgmpipe ") + filename;
    else
      cmd = StringC("mpeg2dec -o pgmpipe ") + filename;
    ONDEBUG(cerr << "ImgIOmpeg2decBaseC::ImgIOmpeg2decBaseC(), Command='" << cmd << "'. \n");
    decodeProg = ChildOSProcessC(cmd,true,false,false);
    IStreamC &ins = decodeProg.StdOut();
    // Wait for a charactor to appear on the output...
#if 1
    if(ins) {
      char let;
      ins.read(&let,1);
      ins.Unget(&let,1);
    }
#endif
    // Check everything's looking ok.
    if(decodeProg.IsRunning()) {
      ok = true;
    } else 
      cerr << "Failed to run mpeg2dec on file '" << filename << "'.  Command:'" << cmd << "'\n";
    ONDEBUG(cerr << "ImgIOmpeg2decBaseC::ImgIOmpeg2decBaseC(), Done. Ok=" << (IntT) ok << " \n");
  }
  
  //: Destructor.

  ImgIOmpeg2decBaseC::~ImgIOmpeg2decBaseC() {
    decodeProg.Terminate(0.2);
  }
  
  
  //: Get next frame of video.
  
  bool ImgIOmpeg2decBaseC::NextFrame(ImageC<ByteYUVValueC> &yuvImg) {
    if(!ok)
      return false;
    IStreamC &ins = decodeProg.StdOut();
    if(!ins) {
      ok = false;
      return false;
    }
    ByteT header;
    IntT x,y;
    
    if(!LoadHeader(ins,header,x,y)) {
      cerr << "ImgIOmpeg2decBaseC::NextFrame(), Failed to find PNM header in stream. ";
      ok = false;
      return false;
    }
    if(header != '5') {
      cerr << "ImgIOmpeg2decBaseC::NextFrame(), Output stream in unexpected format. ";
      ok = false;
      return false;
    }
    IntT realY = (y / 3) * 2;
    
    // Make sure buffer is appropriate size.
    
    if(rawBuffer.Frame().Rows() != (UIntT) y || rawBuffer.Frame().Cols() != (UIntT) x) 
      rawBuffer = Array2dC<ByteT>(y,x);
    
    // Get binary data from the stream.
    
    IntT readSize = x * y;
    ins.read((char *) &(rawBuffer[0][0]),readSize);
    if(ins.gcount() != readSize) {
      cerr << "ImgIOmpeg2decBaseC::NextFrame(), Failed to read complete frame. ";
      ok = false;
      return false;
    }
    
    // Unpack data into a normal YUV image.
    
    if(yuvImg.Rows() != (UIntT) realY || yuvImg.Cols() != (UIntT) x)
       yuvImg = ImageC<ByteYUVValueC>(realY,x);
    
    UIntT row = 0;
    for(Array2dIterC<ByteYUVValueC> it(yuvImg);it;row++) {
      ByteT *yd = &(rawBuffer[row][0]);
      IntT crow = ((row >> 1) & ~((UIntT)1)) + realY + row % 2;
      
      ByteT *ud = &(rawBuffer[crow][0]);
      ByteT *vd = &(rawBuffer[crow][x/2]);
      do {
	*it = ByteYUVValueC(*yd,*ud + 128,*vd + 128);
	yd++;
	it++;
	
	*it = ByteYUVValueC(*yd,*ud + 128,*vd + 128);
	yd++;
	ud++;
	vd++;
      } while(it.Next()) ;
    }
    return true;
  }

}
