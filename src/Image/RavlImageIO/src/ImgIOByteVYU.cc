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
//! file="Ravl/Image/ImageIO/ImgIOByteVYU.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteVYUValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  
  void InitImgIOByteVYU() {
  }
  static TypeNameC type3(typeid(ImageC<ByteVYUValueC>),"ImageC<ByteVYUValueC>");
  
  ImageC<ByteYUVValueC> ByteVYUImageCT2ByteYUVImageCT(const ImageC<ByteVYUValueC> &dat) { 
    ImageC<ByteYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVValueC,ByteVYUValueC> it(ret,dat);it;it++) 
      it.Data1() = ByteYUVValueC(it.Data2().Y(),it.Data2().U(),it.Data2().V());
    return ret;
  }

  ImageC<ByteVYUValueC> ByteYUVImageCT2ByteVYUImageCT(const ImageC<ByteYUVValueC> &dat) { 
    ImageC<ByteVYUValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteVYUValueC,ByteYUVValueC> it(ret,dat);it;it++) 
      it.Data1() = ByteVYUValueC(it.Data2().V(),it.Data2().Y(),it.Data2().U());
    return ret;
  }
  
  DP_REGISTER_CONVERSION_NAMED(ByteVYUImageCT2ByteYUVImageCT,1,
			       "ImageC<ByteYUVValueC> RavlImageN::Convert(const ImageC<ByteVYUValueC> &)");
  DP_REGISTER_CONVERSION_NAMED(ByteYUVImageCT2ByteVYUImageCT,1,
			       "ImageC<ByteVYUValueC> RavlImageN::Convert(const ImageC<ByteYUVValueC> &)");
  
  
  static FileFormatStreamC<ImageC<ByteVYUValueC> > FileFormatStream_ImageC_ByteVYUValueC;
  static FileFormatBinStreamC<ImageC<ByteVYUValueC> > FileFormatBinStream_ImageC_ByteVYUValueC;
  
}

