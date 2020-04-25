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
//! file="Ravl/Image/ImageIO/ImgIOUInt16RGB.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/TypeName.hh"
//#include "Ravl/DP/Converter.hh"
//#include "Ravl/Array2dIter2.hh"

namespace RavlImageN {

  void InitImgIOUInt16RGB()
  {}
  
  static TypeNameC type1(typeid(ImageC<UInt16RGBValueC>),"ImageC<UInt16RGBValueC>");  

  static FileFormatStreamC<ImageC<UInt16RGBValueC> > FileFormatStream_ImageC_UInt16RGBValueC;
  static FileFormatBinStreamC<ImageC<UInt16RGBValueC> > FileFormatBinStream_ImageC_UInt16RGBValueC;
}
