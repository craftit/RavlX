// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id: ImgTypeCnv5.cc 3700 2003-11-19 17:44:12Z craftit $"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv5.cc"
//! author="Charles Galambos"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/YCbCrBT601Value.hh"
#include "Ravl/Image/ByteYCbCrBT601Value.hh"
#include "Ravl/Image/UInt16YCbCrBT601Value.hh"
#include "Ravl/Image/YCbCrBT709Value.hh"
#include "Ravl/Image/ByteYCbCrBT709Value.hh"
#include "Ravl/Image/UInt16YCbCrBT709Value.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN
{
  void InitStdImageCnv6()
  {}
  
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCrBT601ValueImage2UInt16YCbCrBT601ValueImage,1,
                               "ImageC<UInt16YCbCrBT601ValueC> RavlImageN::Convert(const ImageC<ByteYCbCrBT601ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT601ValueImage2ByteYCbCrBT601ValueImage,2,
                               "ImageC<ByteYCbCrBT601ValueC> RavlImageN::Convert(const ImageC<UInt16YCbCrBT601ValueC> &dat)");
  
  DP_REGISTER_CONVERSION_NAMED(YCbCrBT601ValueFloatImage2ImageRGBValueFloat,1,
                               "RavlImageN::ImageC<RavlImageN::RGBValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::YCbCrBT601ValueC<float>> &)");

  DP_REGISTER_CONVERSION_NAMED(ImageRGBValueFloat2YCbCrBT601ValueFloatImage,1,
                               "RavlImageN::ImageC<YCbCrBT601ValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::RGBValueC<float>> &dat)");
  
  
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCrBT601ValueImage2ByteRGBImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteYCbCrBT601ValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT601ValueImage2ByteRGBImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCrBT601ValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT601ValueImage2ImageRGBValueFloat,1,
                               "RavlImageN::ImageC<RavlImageN::RGBValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCrBT601ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteRGBImage2ByteYCbCrBT601ValueImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteYCbCrBT601ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteRGBValueC> &)");

  //709
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCrBT709ValueImage2UInt16YCbCrBT709ValueImage,1,
                               "ImageC<UInt16YCbCrBT709ValueC> RavlImageN::Convert(const ImageC<ByteYCbCrBT709ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT709ValueImage2ByteYCbCrBT709ValueImage,2,
                               "ImageC<ByteYCbCrBT709ValueC> RavlImageN::Convert(const ImageC<UInt16YCbCrBT709ValueC> &dat)");
  
  DP_REGISTER_CONVERSION_NAMED(YCbCrBT709ValueFloatImage2ImageRGBValueFloat,1,
                               "RavlImageN::ImageC<RavlImageN::RGBValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::YCbCrBT709ValueC<float>> &)");

  DP_REGISTER_CONVERSION_NAMED(ImageRGBValueFloat2YCbCrBT709ValueFloatImage,1,
                               "RavlImageN::ImageC<YCbCrBT709ValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::RGBValueC<float>> &dat)");
  
  
  DP_REGISTER_CONVERSION_NAMED(ByteYCbCrBT709ValueImage2ByteRGBImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteYCbCrBT709ValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT709ValueImage2ByteRGBImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteRGBValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCrBT709ValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(UInt16YCbCrBT709ValueImage2ImageRGBValueFloat,1,
                               "RavlImageN::ImageC<RavlImageN::RGBValueC<float> > RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::UInt16YCbCrBT709ValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteRGBImage2ByteYCbCrBT709ValueImage,1.1,
                               "RavlImageN::ImageC<RavlImageN::ByteYCbCrBT709ValueC> RavlImageN::Convert(const RavlImageN::ImageC<RavlImageN::ByteRGBValueC> &)");

  
}
