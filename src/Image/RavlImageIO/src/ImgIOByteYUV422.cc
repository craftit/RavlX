// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOByteYUV422.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ImageConv.hh"

namespace RavlImageN {

  void InitImgIOByteYUV422()
  {}

  static TypeNameC type1(typeid(ImageC<ByteYUV422ValueC>),"ImageC<ByteYUV422ValueC>");  
  
  ImageC<ByteT> ByteYUV422ImageCT2ByteImageCT(const ImageC<ByteYUV422ValueC> &dat) { 
    ImageC<ByteT> ret(dat.Rectangle());
    for(Array2dIter2C<ByteT,ByteYUV422ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }
  
  ImageC<ByteYUV422ValueC> ByteImageCT2ByteYUV422ImageCT(const ImageC<ByteT> &dat) { 
    ImageC<ByteYUV422ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUV422ValueC,ByteT> it(ret,dat);it;it++) 
      it.Data1() = ByteYUV422ValueC(128,it.Data2());
    return ret;
  }
  
  // Convert a YUV422 image into a YUV444 image.
  // What to do about interlace ?
  
  ImageC<ByteYUVValueC> ByteYUV422ImageCT2ByteYUVImageCT(const ImageC<ByteYUV422ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<ByteYUVValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<ByteYUVValueC> ret(outRect);
    for(Array2dIter2C<ByteYUVValueC,ByteYUV422ValueC> it(ret,dat,outRect);it;it++) {
      SByteT u = it.Data2().UV() - 128;
      ByteT y1 = it.Data2().Y();
      ByteYUVValueC &p1 = it.Data1();
      it++;
      SByteT v = it.Data2().UV() - 128;
      p1 = ByteYUVValueC(y1,u,v);
      it.Data1() = ByteYUVValueC(it.Data2().Y(),u,v);
    }
    return ret;
  }

  // Convert a YUV422 image into a RGB image.
  // The rgb image may be a
  
  ImageC<ByteRGBValueC> ByteYUV422ImageCT2ByteRGBImageCT(const ImageC<ByteYUV422ValueC> &dat) { 
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
    for(Array2dIter2C<ByteRGBValueC,ByteYUV422ValueC> it(ret,outRect,dat,outRect);it;it++) {
      // Read the first pixel.
      SByteT u = it.Data2().UV() + 128;
      ByteT i1 = it.Data2().Y();
      ByteRGBValueC &p1 = it.Data1();
      
      it++;
      // Read the second pixel.
      SByteT v = it.Data2().UV() + 128;
      ByteYUV2RGB2(i1,it.Data2().Y(),u,v,p1,it.Data1());
    }
    return ret;
  }


  ImageC<ByteYUV422ValueC> ByteRGBImageCT2ByteYUV422ImageCT(const ImageC<ByteRGBValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<ByteYUV422ValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<ByteYUV422ValueC> ret(outRect);
    UIntT i(0);
    for(Array2dIter2C<ByteYUV422ValueC,ByteRGBValueC> it(ret,dat,outRect);it;it++,i++) {
      RealYUVValueC pix(RealRGBValueC(it.Data2()));
      it.Data1().Y() = (ByteT)pix[0];
      RealT uv = ((i & 1) ? pix[2] : pix[1]) + 128.0;
      if (uv < 0.0)
	it.Data1().UV() = 0;
      else if (uv > 255.0)
	it.Data1().UV() = 255;
      else
	it.Data1().UV() = (ByteT)(uv+0.5);
    }
    return ret;
  }
  

  DP_REGISTER_CONVERSION_NAMED(ByteYUV422ImageCT2ByteYUVImageCT,1,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<ByteYUV422ValueC> &)");
  
  // There is some data loss as the colour cubes don't entirely overlap.
  DP_REGISTER_CONVERSION_NAMED(ByteYUV422ImageCT2ByteRGBImageCT,1.1,
			       "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<ByteYUV422ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteYUV422ImageCT2ByteImageCT,2,
			       "ImageC<ByteT> RavlImageN::Convert(const ImageC<ByteYUV422ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2ByteYUV422ImageCT,1,
			       "ImageC<ByteYUV422ValueC> RavlImageN::Convert(const ImageC<ByteT> &)");

  DP_REGISTER_CONVERSION_NAMED(ByteRGBImageCT2ByteYUV422ImageCT,1.5,
			       "ImageC<ByteYUV422ValueC> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  
  static FileFormatStreamC<ImageC<ByteYUV422ValueC> > FileFormatStream_ImageC_ByteYUV422ValueC;
  static FileFormatBinStreamC<ImageC<ByteYUV422ValueC> > FileFormatBinStream_ImageC_ByteYUV422ValueC;
  

}
