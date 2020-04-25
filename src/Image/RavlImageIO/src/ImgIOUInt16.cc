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
//! file="Ravl/Image/ImageIO/ImgIOUInt16.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
//#include "Ravl/Array2dIter2.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  void InitImgIOUInt16()
    {}
  
  static TypeNameC type1(typeid(ImageC<Int16T>),"ImageC<Int16T>");  
  static TypeNameC type2(typeid(ImageC<UInt16T>),"ImageC<UInt16T>");
  static TypeNameC type3(typeid(Array2dC<Int16T>),"RavlN::Array2dC<RavlN::Int16T>");  
  static TypeNameC type4(typeid(Array2dC<UInt16T>),"RavlN::Array2dC<RavlN::UInt16T>");
  
  ImageC<Int16T> Array2dInt162ImageInt16(const Array2dC<Int16T> &dat)   
  { return dat; }
  
  Array2dC<Int16T> ImageInt162Array2dInt16(const ImageC<Int16T> &dat)   
  { return dat; }
  
  DP_REGISTER_CONVERSION_NAMED(Array2dInt162ImageInt16,1,
			       "RavlImageN::ImageC<RavlN::Int16T> Array2dInt2ImageInt(const RavlN::Array2dC<RavlN::Int16T> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ImageInt162Array2dInt16,1,
			       "RavlN::Array2dC<RavlN::Int16T> ImageInt2Array2dInt(const RavlImageN::ImageC<RavlN::Int16T> &)");
  


  static FileFormatStreamC<ImageC<UInt16T> > FileFormatStream_ImageC_UInt16T;
  static FileFormatBinStreamC<ImageC<UInt16T> > FileFormatBinStream_ImageC_UInt16T;
  
}
