// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgTypeCnv5.cc"
//! author="Charles Galambos"

#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Types.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN
{
  void InitStdImageCnv5()
  {}
  
  DP_REGISTER_CONVERSION_NAMED(FloatTImageCT2RealImageCT,1,
			       "RavlImageN::ImageC<RealT>  RavlImageN::Convert(const RavlImageN::ImageC<FloatT> &)");
  
  DP_REGISTER_CONVERSION_NAMED(RealTImageCT2FloatImageCT,1.5,
			       "RavlImageN::ImageC<FloatT>  RavlImageN::Convert(const RavlImageN::ImageC<RealT> &)");

  DP_REGISTER_CONVERSION_NAMED(ByteRGBImageCT2FloatImageCT,2.8,
			       "RavlImageN::ImageC<FloatT>  RavlImageN::Convert(const RavlImageN::ImageC<ByteRGBValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(BoolImage2ByteImage,1,
			       "RavlImageN::ImageC<ByteT>  RavlImageN::Convert(const RavlImageN::ImageC<bool> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ByteImage2BoolImage,8,
			       "RavlImageN::ImageC<bool>  RavlImageN::Convert(const RavlImageN::ImageC<ByteT> &)");
}
