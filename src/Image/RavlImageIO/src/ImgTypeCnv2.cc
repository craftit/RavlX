// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv2.cc"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Array2dIter2.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteYUVAValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/RGBcYUV.hh"


namespace RavlImageN
{
  void InitStdImageCnv2()
  {}
  
  void InitYUVImageCnv()
  {}
  

  DP_REGISTER_CONVERSION_NAMED(ByteYUVAImageCT2ByteYUVImageCT,1.25,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<ByteYUVAValueC> &)");
  DP_REGISTER_CONVERSION_NAMED(ByteYUVImageCT2ByteYUVAImageCT,1,
			       "ImageC<ByteYUVAValueC> RavlImageN::Convert(const ImageC<ByteYUVValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteYUVImageCT2RealYUVImageCT ,1,
			       "ImageC<RealYUVValueC> RavlImageN::Convert(const ImageC<ByteYUVValueC> &)");
  DP_REGISTER_CONVERSION_NAMED(RealYUVImageCT2ByteYUVImageCT ,8,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<RealYUVValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RealYUVImageCT2RealRGBImageCT,1,
			       "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<RealYUVValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(RealRGBImageCT2RealYUVImageCT,1,
			       "ImageC<RealYUVValueC> RavlImageN::Convert(const ImageC<RealRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(ByteYUVImageCT2ByteImageCT   ,3,
			       "ImageC<ByteT> RavlImageN::Convert(const ImageC<ByteYUVValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RGBImageCT2ByteImageCT       ,3.1,
			       "ImageC<ByteT> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
    
  DP_REGISTER_CONVERSION_NAMED(RGBImageCT2DoubleImageCT     ,3,
			       "ImageC<RealT> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(YUVImageCT2RGBImageCT        ,1.3,
			       "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<ByteYUVValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RGBImageCT2YUVImageCT        ,1.3,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  


}
