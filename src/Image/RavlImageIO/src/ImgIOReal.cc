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
//! file="Ravl/Image/ImageIO/ImgIOReal.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  void InitImgIOReal()
  {}
  
  static TypeNameC type1(typeid(ImageC<RealT>),"ImageC<RealT>");  
  static TypeNameC type2(typeid(Array2dC<RealT>),"RavlN::Array2dC<RealT>");  
  
  static FileFormatStreamC<ImageC<RealT> > FileFormatStream_ImageC_RealT;
  static FileFormatBinStreamC<ImageC<RealT> > FileFormatBinStream_ImageC_RealT;
  
  //: Convert a real image into an array2d
  
  Array2dC<RealT> RealImage2Array(const ImageC<RealT> &img)
  { return img; }

  //: Convert a real image into an array2d
  
  ImageC<RealT> RealArray2Image(const Array2dC<RealT> &arr)
  { return arr; }
  

  DP_REGISTER_CONVERSION_NAMED(RealImage2Array     ,1,
			       "Array2dC<RealT> RavlImageN::Convert(const ImageC<RealT> &)");
  DP_REGISTER_CONVERSION_NAMED(RealArray2Image     ,1,
			       "ImageC<RealT> RavlImageN::Convert(const Array2dC<RealT> &)");

}
