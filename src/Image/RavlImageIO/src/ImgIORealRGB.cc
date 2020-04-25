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
//! file="Ravl/Image/ImageIO/ImgIORealRGB.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlImageN {
  
  void InitImgIORealRGB()
  {}
  
  static TypeNameC type1(typeid(ImageC<RealRGBValueC>),"ImageC<RealRGBValueC>");  
  static TypeNameC type2(typeid(ImageC<RGBValueC<float> >),"RavlImageN::ImageC<RGBValueC<float> >");  
  
  static FileFormatStreamC<ImageC<RealRGBValueC> > FileFormatStream_ImageC_RealRGBValueC;
  static FileFormatBinStreamC<ImageC<RealRGBValueC> > FileFormatBinStream_ImageC_RealRGBValueC;
  
  static FileFormatStreamC<ImageC<RGBValueC<float> > > FileFormatStream_ImageC_RGBValueC_float;
  static FileFormatBinStreamC<ImageC<RGBValueC<float> > > FileFormatBinStream_ImageC_RGBValueC_float;
  
  // Some conversions.
  
  DP_REGISTER_CONVERSION_NAMED(RealRGBImageCT2RGBImageCFloat,2.0,
			       "RavlImageN::ImageC<RavlImageN::RGBValueC<float> > RealRGBImageCT2RGBImageCFloat(const RavlImageN::ImageC<RavlImageN::RealRGBValueC> &)");

  DP_REGISTER_CONVERSION_NAMED(RGBImageCFloat2RealRGBImageCT,1.0,
			       "RavlImageN::ImageC<RavlImageN::RealRGBValueC> RGBImageCFloat2RealRGBImageCT(const RavlImageN::ImageC<RavlImageN::RGBValueC<float> > & dat )");
  
}
