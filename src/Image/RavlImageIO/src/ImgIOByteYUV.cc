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
//! file="Ravl/Image/ImageIO/ImgIOByteYUV.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  
  void InitImgIOByteYUV() {
  }
  
  static TypeNameC type3(typeid(ImageC<ByteYUVValueC>),"ImageC<ByteYUVValueC>");
  
  static FileFormatStreamC<ImageC<ByteYUVValueC> > FileFormatStream_ImageC_ByteYUVValueC;
  static FileFormatBinStreamC<ImageC<ByteYUVValueC> > FileFormatBinStream_ImageC_ByteYUVValueC;
  
}

