// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv3.cc"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/ByteRGBMedian.hh"
#include "Ravl/Image/RealRGBAverage.hh"
#include "Ravl/Image/RealHSVValue.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN
{
  void InitStdImageCnv3()
  {}
  
  static TypeNameC type1(typeid(ImageC<RealRGBAverageC>),"ImageC<RealRGBAverageC>");  
  static TypeNameC type2(typeid(ImageC<ByteRGBMedianC>),"ImageC<ByteRGBMedianC>");  
  static TypeNameC type3(typeid(ImageC<RealHSVValueC>),"ImageC<RealHSVValueC>");  
  
  DP_REGISTER_CONVERSION_NAMED(RealRGBAverageImageC2ByteRGBImageCT,1,
			       "ImageC<ByteRGBValueC>  RavlImageN::Convert(const ImageC<RealRGBAverageC> &)");

  DP_REGISTER_CONVERSION_NAMED(ByteRGBMedianImageC2ByteRGBImageCT,1,
			       "ImageC<ByteRGBValueC>  RavlImageN::Convert(const ImageC<ByteRGBMedianC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RealRGBImageCT2RealHSVImageCT,1,
			       "ImageC<RealHSVValueC> RavlImageN::Convert(const ImageC<RealRGBValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RealHSVImageCT2RealRGBImageCT,1,
			       "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<RealHSVValueC> &)");

}
