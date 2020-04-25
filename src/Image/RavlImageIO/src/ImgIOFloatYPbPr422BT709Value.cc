// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id: ImgIOFloatYPbPrBT709422.cc 6170 2007-05-28 16:05:12Z craftit $"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOFloatYPbPrBT709422.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/FloatYPbPr422BT709Value.hh"
#include "Ravl/Image/FloatYPbPrBT709Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Image/ImageConv.hh"

#include "Ravl/Image/FloatYPbPr422BT709Value.hh"
#include "Ravl/Image/FloatYPbPrBT709Value.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"

namespace RavlImageN {
  
  void InitImgIOFloatYPbPr422BT709()
  {}
  
  static TypeNameC type1(typeid(ImageC<FloatYPbPr422BT709ValueC>),"RavlImageN::ImageC<RavlImageN::FloatYPbPr422BT709ValueC>");  
  
  ImageC<FloatT> FloatYPbPr422BT709ImageCT2FloatImageCT(const ImageC<FloatYPbPr422BT709ValueC> &dat) { 
    ImageC<FloatT> ret(dat.Rectangle());
    for(Array2dIter2C<FloatT,FloatYPbPr422BT709ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }

  ImageC<RealRGBValueC> FloatYPbPr422BT709ImageCT2RealRGBValueImageCT(const ImageC<FloatYPbPr422BT709ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    //cout << "outRect created " << std::endl;
    if(outRect.Area() == 0)
      return ImageC<RealRGBValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
   
    ImageC<RealRGBValueC> ret(outRect);

    ImageC<FloatYPbPr422BT709ValueC> cpy = dat.Copy();
    RGBValueC<FloatT> rgbcfloat;
    for(Array2dIter2C<RealRGBValueC,FloatYPbPr422BT709ValueC> it(ret,outRect,cpy,outRect);it;it++) {
       FloatT cb= (FloatT)it.Data2().Pbr()-128;
       FloatT y1 = (FloatT)it.Data2().Y()-16;
       RealRGBValueC &p1 = it.Data1();
       it++;
       FloatT cr = (FloatT)it.Data2().Pbr()-128;
       YPbPrBT709Float2RGBFloat(YPbPrBT709ValueC<FloatT>(y1,cb,cr),rgbcfloat);  //FloatYPbPrBT709ValueC(y1,cb,cr),p1);
       p1 = RealRGBValueC(rgbcfloat.Red(),rgbcfloat.Green(),rgbcfloat.Blue());
         
       YPbPrBT709Float2RGBFloat(YPbPrBT709ValueC<FloatT>((FloatT)it.Data2().Y()-16,cb,cr),rgbcfloat);   //FloatYPbPrBT709ValueC(it.Data2().Y(),cb,cr),it.Data1());
       it.Data1() = RealRGBValueC(rgbcfloat.Red(),rgbcfloat.Green(),rgbcfloat.Blue());

     }

    return ret;
  }
  //Convert FloatYPbPr422BT709ValueC into RealRGBValueC

  /*
  ImageC<UInt16YCbCr422BT709ValueC> UInt16ImageCT2UInt16YCbCr422BT709ImageCT(const ImageC<UInt16T> &dat) { 
    ImageC<UInt16YCbCr422BT709ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<UInt16YCbCr422BT709ValueC,UInt16T> it(ret,dat);it;it++) 
      it.Data1() = UInt16YCbCr422BT709ValueC(128*256,it.Data2());
    return ret;
  }
  */
  // Convert a YPbPr422BT709 image into a YPbPrBT709444 image.
  // What to do about interlace ?
  
  ImageC<FloatYPbPrBT709ValueC> FloatYPbPr422BT709ImageCT2FloatYPbPrBT709ImageCT(const ImageC<FloatYPbPr422BT709ValueC> &dat) { 
    ImageRectangleC outRect = dat.Rectangle();
    if(outRect.Area() == 0)
      return ImageC<FloatYPbPrBT709ValueC>();
    // Make sure we're aligned correctly.
    if(outRect.LCol().V() & 1)
      outRect.LCol().V()++; // Start on even boundary in image.
    if(!(outRect.RCol().V() & 1))
      outRect.RCol().V()--; // End on odd boundary in image.
    RavlAssert(outRect.LCol() < outRect.RCol()); // Make sure there's something left!
    
    ImageC<FloatYPbPrBT709ValueC> ret(outRect);
    for(Array2dIter2C<FloatYPbPrBT709ValueC,FloatYPbPr422BT709ValueC> it(ret,dat,outRect);it;it++) {
      FloatT cb = it.Data2().Pbr();
      FloatT y1 = it.Data2().Y();
      FloatYPbPrBT709ValueC &p1 = it.Data1();
      it++;
      FloatT cr = it.Data2().Pbr();
      p1 = FloatYPbPrBT709ValueC(y1,cb,cr);
      it.Data1() = FloatYPbPrBT709ValueC(it.Data2().Y(),cb,cr);
    }
    return ret;
  }

  
  DP_REGISTER_CONVERSION_NAMED(FloatYPbPr422BT709ImageCT2FloatYPbPrBT709ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::FloatYPbPrBT709ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::FloatYPbPr422BT709ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(FloatYPbPr422BT709ImageCT2FloatImageCT,2,
			       "RavlImageN::ImageC<RavlN::FloatT> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::FloatYPbPr422BT709ValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(FloatYPbPr422BT709ImageCT2RealRGBValueImageCT,1,
			       "RavlImageN::ImageC<RavlN::RealRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::FloatYPbPr422BT709ValueC> &)");

  /*
  DP_REGISTER_CONVERSION_NAMED(UInt16ImageCT2UInt16YCbCr422BT709ImageCT,1,
			       "RavlImageN::ImageC<RavlImageN::UInt16YCbCr422BT709ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlN::UInt16T> &)");
  */
  FileFormatStreamC<ImageC<FloatYPbPr422BT709ValueC> > FileFormatStream_ImageC_FloatYPbPr422BT709ValueC;
  FileFormatBinStreamC<ImageC<FloatYPbPr422BT709ValueC> > FileFormatBinStream_ImageC_FloatYPbPr422BT709ValueC;
  

}
