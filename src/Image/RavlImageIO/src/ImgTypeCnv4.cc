// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv4.cc"
//! author="Charles Galambos"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteBGRValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RealBGRValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/ByteBGRAValue.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN
{
  void InitStdImageCnv4()
  {}
  
  static TypeNameC type1(typeid(ImageC<ByteBGRValueC>),"RavlImageN::ImageC<ByteBGRValueC>");  
  static TypeNameC type2(typeid(ImageC<RealBGRValueC>),"RavlImageN::ImageC<RealBGRValueC>");  
  static TypeNameC type3(typeid(ImageC<ByteBGRAValueC>),"RavlImageN::ImageC<ByteBGRAValueC>");  
  
  ImageC<ByteBGRValueC> Image_ByteRGBValue2Image_ByteBGRValue(const ImageC<ByteRGBValueC> &img) {
    ImageC<ByteBGRValueC> ret(img.Frame());
    for(Array2dIter2C<ByteBGRValueC,ByteRGBValueC> it(ret,img);it;it++) 
      it.Data1() = ByteBGRValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }
  
  ImageC<ByteRGBValueC> Image_ByteBGRValue2Image_ByteRGBValue(const ImageC<ByteBGRValueC> &img) {
    ImageC<ByteRGBValueC> ret(img.Frame());
    for(Array2dIter2C<ByteBGRValueC,ByteRGBValueC> it(img,ret);it;it++) 
      it.Data2() = it.Data1().RGB();
    return ret;
  }
  
  DP_REGISTER_CONVERSION_NAMED(Image_ByteRGBValue2Image_ByteBGRValue,1,
			       "ImageC<ByteBGRValueC>  RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(Image_ByteBGRValue2Image_ByteRGBValue,1,
			       "ImageC<ByteRGBValueC>  RavlImageN::Convert(const ImageC<ByteBGRValueC> &)");

  
  ImageC<RealBGRValueC> Image_RealRGBValue2Image_RealBGRValue(const ImageC<RealRGBValueC> &img) {
    ImageC<RealBGRValueC> ret(img.Frame());
    for(Array2dIter2C<RealBGRValueC,RealRGBValueC> it(ret,img);it;it++) 
      it.Data1() = RealBGRValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }
  
  ImageC<RealRGBValueC> Image_RealBGRValue2Image_RealRGBValue(const ImageC<RealBGRValueC> &img) {
    ImageC<RealRGBValueC> ret(img.Frame());
    for(Array2dIter2C<RealBGRValueC,RealRGBValueC> it(img,ret);it;it++) 
      it.Data2() = it.Data1().RGB();
    return ret;
  }
  
  DP_REGISTER_CONVERSION_NAMED(Image_RealRGBValue2Image_RealBGRValue,1,
			       "ImageC<RealBGRValueC>  RavlImageN::Convert(const ImageC<RealRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(Image_RealBGRValue2Image_RealRGBValue,1,
			       "ImageC<RealRGBValueC>  RavlImageN::Convert(const ImageC<RealBGRValueC> &)");

  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // BGRA
  
  
  ImageC<ByteBGRAValueC> Image_ByteRGBAValue2Image_ByteBGRAValue(const ImageC<ByteRGBAValueC> &img) {
    ImageC<ByteBGRAValueC> ret(img.Frame());
    for(Array2dIter2C<ByteBGRAValueC,ByteRGBAValueC> it(ret,img);it;it++) 
      it.Data1() = ByteBGRAValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue(),it.Data2().Alpha());
    return ret;
  }
  
  ImageC<ByteRGBAValueC> Image_ByteBGRAValue2Image_ByteRGBAValue(const ImageC<ByteBGRAValueC> &img) {
    ImageC<ByteRGBAValueC> ret(img.Frame());
    for(Array2dIter2C<ByteBGRAValueC,ByteRGBAValueC> it(img,ret);it;it++) 
      it.Data2() = ByteRGBAValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue(),it.Data2().Alpha());
    return ret;
  }
  
  DP_REGISTER_CONVERSION_NAMED(Image_ByteRGBAValue2Image_ByteBGRAValue,1,
			       "ImageC<ByteBGRAValueC>  RavlImageN::Convert(const ImageC<ByteRGBAValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(Image_ByteBGRAValue2Image_ByteRGBAValue,1,
			       "ImageC<ByteRGBAValueC>  RavlImageN::Convert(const ImageC<ByteBGRAValueC> &)");

  
  
}
