// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id: ImgIOUInt16YCbCrBT601422.cc 6170 2007-05-28 16:05:12Z craftit $"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOUInt16YCbCrBT709422.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/UInt16YCbCr422BT709Value.hh"
#include "Ravl/Image/UInt16YCbCrBT709Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ImageConv.hh"

namespace RavlImageN {
  
  void InitImgIOUInt16YCbCr422BT709()
  {}
  
  static TypeNameC type1(typeid(ImageC<UInt16YCbCr422BT709ValueC>),"RavlImageN::ImageC<RavlImageN::UInt16YCbCr422BT709ValueC>");  
  
  ImageC<UInt16T> UInt16YCbCr422BT709ImageCT2UInt16ImageCT(const ImageC<UInt16YCbCr422BT709ValueC> &dat) { 
    ImageC<UInt16T> ret(dat.Rectangle());
    for(Array2dIter2C<UInt16T,UInt16YCbCr422BT709ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }
  
  ImageC<UInt16YCbCr422BT709ValueC> UInt16ImageCT2UInt16YCbCr422BT709ImageCT(const ImageC<UInt16T> &dat) { 
    ImageC<UInt16YCbCr422BT709ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<UInt16YCbCr422BT709ValueC,UInt16T> it(ret,dat);it;it++) 
      it.Data1() = UInt16YCbCr422BT709ValueC(128*256,it.Data2());
    return ret;
  }
  
  // Convert a YCbCr422BT709 image into a YCbCrBT709444 image.
  // What to do about interlace ?
  
  ImageC<UInt16YCbCrBT709ValueC> UInt16YCbCr422BT709ImageCT2UInt16YCbCrBT709ImageCT(const ImageC<UInt16YCbCr422BT709ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<UInt16YCbCrBT709ValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<UInt16YCbCrBT709ValueC> ret(outRect);
    for(Array2dIter2C<UInt16YCbCrBT709ValueC,UInt16YCbCr422BT709ValueC> it(ret,dat,outRect);it;it++) {
      UInt16T cb = it.Data2().Cbr();
      UInt16T y1 = it.Data2().Y();
      UInt16YCbCrBT709ValueC &p1 = it.Data1();
      it++;
      UInt16T cr = it.Data2().Cbr();
      p1 = UInt16YCbCrBT709ValueC(y1,cb,cr);
      it.Data1() = UInt16YCbCrBT709ValueC(it.Data2().Y(),cb,cr);
    }
    return ret;
  }

  
  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCr422BT709ImageCT2UInt16YCbCrBT709ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::UInt16YCbCrBT709ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCr422BT709ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCr422BT709ImageCT2UInt16ImageCT,2,
			       "RavlImageN::ImageC<RavlN::UInt16T> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCr422BT709ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(UInt16ImageCT2UInt16YCbCr422BT709ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::UInt16YCbCr422BT709ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlN::UInt16T> &)");
  
  static FileFormatStreamC<ImageC<UInt16YCbCr422BT709ValueC> > FileFormatStream_ImageC_UInt16YCbCr422BT709ValueC;
  static FileFormatBinStreamC<ImageC<UInt16YCbCr422BT709ValueC> > FileFormatBinStream_ImageC_UInt16YCbCr422BT709ValueC;
  

}
