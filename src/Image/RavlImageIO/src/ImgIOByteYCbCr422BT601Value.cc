// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id: ImgIOByteYCbCrBT601422.cc 6170 2007-05-28 16:05:12Z craftit $"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOByteYCbCrBT601422.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ByteYCbCr422BT601Value.hh"
#include "Ravl/Image/ByteYCbCrBT601Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ImageConv.hh"

namespace RavlImageN {
  
  void InitImgIOByteYCbCr422BT601()
  {}
  
  static TypeNameC type1(typeid(ImageC<ByteYCbCr422BT601ValueC>),"RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC>");  
  
  ImageC<ByteT> ByteYCbCr422BT601ImageCT2ByteImageCT(const ImageC<ByteYCbCr422BT601ValueC> &dat) { 
    ImageC<ByteT> ret(dat.Rectangle());
    for(Array2dIter2C<ByteT,ByteYCbCr422BT601ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }
  
  ImageC<ByteYCbCr422BT601ValueC> ByteImageCT2ByteYCbCr422BT601ImageCT(const ImageC<ByteT> &dat) { 
    ImageC<ByteYCbCr422BT601ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYCbCr422BT601ValueC,ByteT> it(ret,dat);it;it++) 
      it.Data1() = ByteYCbCr422BT601ValueC(128,it.Data2());
    return ret;
  }
  
  // Convert a YCbCr422BT601 image into a YCbCrBT601444 image.
  // What to do about interlace ?
  
  ImageC<ByteYCbCrBT601ValueC> ByteYCbCr422BT601ImageCT2ByteYCbCrBT601ImageCT(const ImageC<ByteYCbCr422BT601ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<ByteYCbCrBT601ValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<ByteYCbCrBT601ValueC> ret(outRect);
    for(Array2dIter2C<ByteYCbCrBT601ValueC,ByteYCbCr422BT601ValueC> it(ret,dat,outRect);it;it++) {
      ByteT cb = it.Data2().Cbr();
      ByteT y1 = it.Data2().Y();
      ByteYCbCrBT601ValueC &p1 = it.Data1();
      it++;
      ByteT cr = it.Data2().Cbr();
      p1 = ByteYCbCrBT601ValueC(y1,cb,cr);
      it.Data1() = ByteYCbCrBT601ValueC(it.Data2().Y(),cb,cr);
    }
    return ret;
  }

  // Convert a YCbCr422BT601 image into a RGB image.
  // The rgb image may be a
  
  ImageC<ByteRGBValueC> ByteYCbCr422BT601ImageCT2ByteRGBImageCT(const ImageC<ByteYCbCr422BT601ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<ByteRGBValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
   
    ImageC<ByteRGBValueC> ret(outRect);
    for(Array2dIter2C<ByteRGBValueC,ByteYCbCr422BT601ValueC> it(ret,outRect,dat,outRect);it;it++) {
      // Read the first pixel.
      ByteT cb= it.Data2().Cbr();
      ByteT y1 = it.Data2().Y();
      ByteRGBValueC &p1 = it.Data1();
      
      it++;
      // Read the second pixel.
      ByteT cr = it.Data2().Cbr();
      
      p1 = ByteYCbCrBT601ValueC(y1,cb,cr).ByteRGB();
      it.Data1() = ByteYCbCrBT601ValueC(it.Data2().Y(),cb,cr).ByteRGB();
    }
    return ret;
  }
  
  ImageC<ByteYCbCr422BT601ValueC> ByteRGBImageCT2ByteYCbCr422BT601ImageCT(const ImageC<ByteRGBValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<ByteYCbCr422BT601ValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<ByteYCbCr422BT601ValueC> ret(outRect);
    UIntT i(0);
    for(Array2dIter2C<ByteYCbCr422BT601ValueC,ByteRGBValueC> it(ret,dat,outRect);it;it++,i++) {
      ByteYCbCrBT601ValueC pix(it.Data2());      
      it.Data1().Y() = pix.Y();
      it.Data1().Cbr() = ((i & 1) ? pix.Cr() : pix.Cb());
    }
    return ret;
  }
  

  DP_REGISTER_CONVERSION_NAMED(ByteYCbCr422BT601ImageCT2ByteYCbCrBT601ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::ByteYCbCrBT601ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC> &)");
  
  // There is some data loss as the colour cubes don't entirely overlap.
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCr422BT601ImageCT2ByteRGBImageCT,1.1,
			       "RavlImageN::ImageC<RavlImageN::ByteRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCr422BT601ImageCT2ByteImageCT,2,
			       "RavlImageN::ImageC<RavlImageN::ByteT> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2ByteYCbCr422BT601ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteT> &)");

  DP_REGISTER_CONVERSION_NAMED(ByteRGBImageCT2ByteYCbCr422BT601ImageCT,1.5,
			       "RavlImageN::ImageC<RavlImageN::ByteYCbCr422BT601ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteRGBValueC> &)");
  
  static FileFormatStreamC<ImageC<ByteYCbCr422BT601ValueC> > FileFormatStream_ImageC_ByteYCbCr422BT601ValueC;
  static FileFormatBinStreamC<ImageC<ByteYCbCr422BT601ValueC> > FileFormatBinStream_ImageC_ByteYCbCr422BT601ValueC;
  

}
