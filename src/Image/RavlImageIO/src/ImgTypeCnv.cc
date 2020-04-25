// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv.cc"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN
{
  void InitStdImageCnv()
  {}
  
  void InitRGBImageCnv()
  {}

  static TypeNameC type1(typeid(ImageRectangleC),"RavlImageN::ImageRectangleC");
  
  static IndexRange2dC ImageRectangle2IndexRange(const ImageRectangleC &rect) 
  { return IndexRange2dC(rect); }
  
  static ImageRectangleC IndexRange2ImageRectangle(const IndexRange2dC &rect) 
  { return IndexRange2dC(rect); }
  
  // Put in to help the automatic conversion mechanism deal with Array2dC to ImageC conversion.
  
  ImageC<RealRGBValueC> RealRGBArray2dCT2RealRGBImageCT(const Array2dC<RealRGBValueC> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = RealRGBValueC(it.Data2().Red(),it.Data2().Green(),it.Data2().Blue());
    return ret;
  }

  DP_REGISTER_CONVERSION_NAMED(ImageRectangle2IndexRange   ,1,
			       "RavlN::IndexRange2dC RavlImageN::ImageRectangle2IndexRange(const RavlImageN::ImageRectangleC &)");

  DP_REGISTER_CONVERSION_NAMED(IndexRange2ImageRectangle   ,1,
			       "RavlImageN::ImageRectangleC RavlImageN::IndexRange2ImageRectangle(const RavlN::IndexRange2dC &)");
  
  
  // Handle converters.
  
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2ByteRGBImageCT   ,1,
			       "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<ByteT> &)");
  DP_REGISTER_CONVERSION_NAMED(RealImageCT2RealRGBImageCT   ,1,
			       "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<RealT> &)");
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2ByteYUVImageCT   ,1,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<ByteT> &)");
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2IntImageCT       ,1,
			       "ImageC<IntT> RavlImageN::Convert(const ImageC<ByteT> &)");
  DP_REGISTER_CONVERSION_NAMED(ByteImageCT2DoubleImageCT    ,1,
			       "ImageC<RealT> RavlImageN::Convert(const ImageC<ByteT> &)");
  DP_REGISTER_CONVERSION_NAMED(DoubleImageCT2ByteImageCT    ,8,
			       "ImageC<ByteT> RavlImageN::Convert(const ImageC<RealT> &)");
  DP_REGISTER_CONVERSION_NAMED(IntImageCT2DoubleImageCT     ,1,
			       "ImageC<RealT> RavlImageN::Convert(const ImageC<IntT> &)");

  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<ByteRGBAValueC>,ImageC<ByteRGBValueC>,ByteRGBAImageCT2ByteRGBImageCT,1.25,
				  "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<ByteRGBAValueC> &)");  
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<ByteRGBValueC>,ImageC<ByteRGBAValueC>,ByteRGBImageCT2ByteRGBAImageCT,1,
				  "ImageC<ByteRGBAValueC> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<RealRGBAValueC>,ImageC<RealRGBValueC>,RealRGBAImageCT2RealRGBImageCT,1.25,
				  "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<RealRGBAValueC> &)");  
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<RealRGBValueC>,ImageC<RealRGBAValueC>,RealRGBImageCT2RealRGBAImageCT,1,
				  "ImageC<RealRGBAValueC> RavlImageN::Convert(const ImageC<RealRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(Array2dC<RealRGBValueC>,ImageC<RealRGBValueC>,RealRGBArray2dCT2RealRGBImageCT,1,
                                  "ImageC<RealRGBValueC> RavlImageN::Convert(const Array2dC<RealRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<ByteRGBValueC>,ImageC<RealRGBValueC>,ByteRGBImageCT2RealRGBImageCT ,1,
				  "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<RealRGBValueC>,ImageC<ByteRGBValueC>,RealRGBImageCT2ByteRGBImageCT ,8,
				  "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<RealRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<RealRGBValueC>,ImageC<RealT>,RealRGBImageCT2DoubleImageCT ,3,
				  "ImageC<RealT> RavlImageN::Convert(const ImageC<RealRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<ByteRGBValueC>,ImageC<UInt16RGBValueC>,ByteRGBImageCT2UInt16RGBImageCT,1,
				  "ImageC<UInt16RGBValueC> RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<UInt16RGBValueC>,ImageC<ByteRGBValueC>,UInt16RGBImageCT2ByteRGBImageCT,2,
				  "ImageC<ByteRGBValueC> RavlImageN::Convert(const ImageC<UInt16RGBValueC> &dat)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<UInt16RGBValueC>,ImageC<RealRGBValueC>,UInt16RGBImageCT2RealRGBImageCT,1,
				  "ImageC<RealRGBValueC> RavlImageN::Convert(const ImageC<UInt16RGBValueC> &dat)");
  DP_REGISTER_CONVERSION_FT_NAMED(ImageC<RealRGBValueC>,ImageC<UInt16RGBValueC>,RealRGBImageCT2UInt16RGBImageCT,4,
				  "ImageC<UInt16RGBValueC> RavlImageN::Convert(const ImageC<RealRGBValueC> &dat)");
  
  
  
}
