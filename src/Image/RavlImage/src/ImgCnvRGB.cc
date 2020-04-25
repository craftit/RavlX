// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/ImageIO/ImgCnvRGB.cc"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/RealRGBAValue.hh"


namespace RavlImageN
{

  // Conversions between RGBA and RGB
  
  // Byte RGBA to Byte RGB 
  
  ImageC<ByteRGBValueC> ByteRGBAImageCT2ByteRGBImageCT(const ImageC<ByteRGBAValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,ByteRGBAValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = ByteRGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }
  
  // Byte RGB to Byte RGBA 
  
  ImageC<ByteRGBAValueC> ByteRGBImageCT2ByteRGBAImageCT(const ImageC<ByteRGBValueC> &dat) {
    ImageC<ByteRGBAValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBAValueC,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = ByteRGBAValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue(),(char) 0xff);
    }
    return ret;
  }

  // Real RGBA to Real RGB 
  
  ImageC<RealRGBValueC> RealRGBAImageCT2RealRGBImageCT(const ImageC<RealRGBAValueC> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,RealRGBAValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = RealRGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }

  // Real RGB to Real RGBA 
  
  ImageC<RealRGBAValueC> RealRGBImageCT2RealRGBAImageCT(const ImageC<RealRGBValueC> &dat) {
    ImageC<RealRGBAValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBAValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = RealRGBAValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue(),(char) 0xff);
    }
    return ret;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  
  // Byte colour to real colour image.
  
  ImageC<RealRGBValueC> ByteRGBImageCT2RealRGBImageCT(const ImageC<ByteRGBValueC> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = RealRGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    }
    return ret;
  }
  
  // Real colour to byte colour image.

  ImageC<ByteRGBValueC> RealRGBImageCT2ByteRGBImageCT(const ImageC<RealRGBValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      RealRGBValueC v(it.Data2());
      v.Limit(0,255);
      it.Data1() = ByteRGBValueC((ByteT)v.Red(),(ByteT)v.Green(),(ByteT)v.Blue());
    }
    return ret;
  }
  
  // Real RGB to real grey image

  ImageC<RealT>  RealRGBImageCT2DoubleImageCT(const ImageC<RealRGBValueC> &dat) {
    ImageC<RealT> ret(dat.Rectangle());
    for(Array2dIter2C<RealT,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = it.Data2().Y();
    }
    return ret;
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  
  // Byte colour to UInt16 colour image.
  
  ImageC<UInt16RGBValueC> ByteRGBImageCT2UInt16RGBImageCT(const ImageC<ByteRGBValueC> &dat) {
    ImageC<UInt16RGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<UInt16RGBValueC,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = UInt16RGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }
  
  //  UInt16 colour image to Byte colour.
  
  ImageC<ByteRGBValueC> UInt16RGBImageCT2ByteRGBImageCT(const ImageC<UInt16RGBValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,UInt16RGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = ByteRGBValueC(static_cast<ByteT>(it.Data2().Red()),
				 static_cast<ByteT>(it.Data2().Green()),
				 static_cast<ByteT>(it.Data2().Blue()));
    return ret;
  }
  
  //  UInt16 colour image to RealT colour.
  
  ImageC<RealRGBValueC> UInt16RGBImageCT2RealRGBImageCT(const ImageC<UInt16RGBValueC> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,UInt16RGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = RealRGBValueC(static_cast<RealT>(it.Data2().Red()),
				 static_cast<RealT>(it.Data2().Green()),
				 static_cast<RealT>(it.Data2().Blue()));
    return ret;
  }
  
  // RealT colour image to UInt16 colour.
  
  ImageC<UInt16RGBValueC> RealRGBImageCT2UInt16RGBImageCT(const ImageC<RealRGBValueC> &dat) {
    ImageC<UInt16RGBValueC> ret(dat.Rectangle());
    RealT r,g,b;
    for(Array2dIter2C<UInt16RGBValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      r = it.Data2().Red();
      g = it.Data2().Green();
      b = it.Data2().Blue();
      it.Data1().Red() = static_cast<UInt16T>(r > 0 ? r : 0);
      it.Data1().Green() = static_cast<UInt16T>(g > 0 ? g : 0);
      it.Data1().Blue() = static_cast<UInt16T>(b > 0 ? b : 0);
    }
    return ret;
  }
  
  
}
