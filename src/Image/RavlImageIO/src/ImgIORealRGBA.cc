// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id: ImgIOByteRGBA.cc 3700 2003-11-19 17:44:12Z craftit $"
//! lib=RavlImageIO
//! file="Ravl/Image/ImageIO/ImgIOByteRGBA.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  
  void InitImgIORealRGBA()
    {}
  
  static TypeNameC type1(typeid(ImageC<RealRGBAValueC>),"ImageC<RealRGBAValueC>");  
  
  static FileFormatStreamC<ImageC<RealRGBAValueC> > FileFormatStream_ImageC_RealRGBAValueC;
  static FileFormatBinStreamC<ImageC<RealRGBAValueC> > FileFormatBinStream_ImageC_RealRGBAValueC;
  
  

}

