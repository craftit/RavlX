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
//! file="Ravl/Image/ImageIO/ImgIOFloat.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  void InitImgIOFloat()
  {}
  
  static TypeNameC type1(typeid(ImageC<FloatT>),"RavlImageN::ImageC<FloatT>");  
  static TypeNameC type2(typeid(Array2dC<FloatT>),"RavlN::Array2dC<FloatT>");  
  
  FileFormatStreamC<ImageC<FloatT> > FileFormatStream_ImageC_FloatT;
  FileFormatBinStreamC<ImageC<FloatT> > FileFormatBinStream_ImageC_FloatT;
  
  //: Convert a real image into an array2d
  
  Array2dC<FloatT> FloatImage2Array(const ImageC<FloatT> &img)
  { return img; }

  //: Convert a real image into an array2d
  
  ImageC<FloatT> FloatArray2Image(const Array2dC<FloatT> &arr)
  { return arr; }
  

  DP_REGISTER_CONVERSION_NAMED(FloatImage2Array     ,1,
			       "Array2dC<FloatT> RavlImageN::Convert(const ImageC<FloatT> &)");
  DP_REGISTER_CONVERSION_NAMED(FloatArray2Image     ,1,
			       "ImageC<FloatT> RavlImageN::Convert(const Array2dC<FloatT> &)");
  
}
