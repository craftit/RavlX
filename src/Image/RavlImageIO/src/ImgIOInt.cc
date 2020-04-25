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
//! file="Ravl/Image/ImageIO/ImgIOInt.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/FunctionRegister.hh"

namespace RavlImageN {
  
  static TypeNameC type1(typeid(ImageC<IntT>),"ImageC<IntT>");  
  static TypeNameC type2(typeid(ImageC<UIntT>),"ImageC<UIntT>");  
  static TypeNameC type3(typeid(RavlN::Array2dC<IntT>),"RavlN::Array2dC<RavlN::IntT>");  
  static TypeNameC type4(typeid(RavlN::Array2dC<UIntT>),"RavlN::Array2dC<RavlN::UIntT>");  
  
  void InitImgIOInt()
  {}
  
  ImageC<IntT> Array2dInt2ImageInt(const Array2dC<IntT> &dat)   
  { return dat; }
  
  Array2dC<IntT> ImageInt2Array2dInt(const ImageC<IntT> &dat)   
  { return dat; }
  
  DP_REGISTER_CONVERSION_NAMED(Array2dInt2ImageInt,1,
			       "RavlImageN::ImageC<RavlN::IntT> Array2dInt2ImageInt(const RavlN::Array2dC<RavlN::IntT> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ImageInt2Array2dInt,1,
			       "RavlN::Array2dC<RavlN::IntT> ImageInt2Array2dInt(const RavlImageN::ImageC<RavlN::IntT> &)");
  
  
  // Byte to double image.

  ImageC<RealT> UIntTImageCT2DoubleImageCT(const ImageC<UIntT> &dat) { 
    ImageC<RealT> ret(dat.Rectangle());
    for(Array2dIter2C<RealT,UIntT> it(ret,dat);it;it++) 
      it.Data1() = (RealT) it.Data2();
    return ret;
  }
  
  ImageC<UIntT> UInt16TImageCT2UIntTImageCT(const ImageC<UInt16T> &dat) { 
    ImageC<UIntT> ret(dat.Rectangle());
    for(Array2dIter2C<UIntT,UInt16T> it(ret,dat);it;it++)
      it.Data1() = it.Data2();
    return ret;
  }

  ImageC<UIntT> IntTImageCT2UIntTImageCT(const ImageC<IntT> &dat) { 
    ImageC<UIntT> ret(dat.Rectangle());
    for(Array2dIter2C<UIntT,IntT> it(ret,dat);it;it++)
      it.Data1() = it.Data2();
    return ret;
  }

  ImageC<IntT> UIntTImageCT2IntTImageCT(const ImageC<UIntT> &dat) { 
    ImageC<IntT> ret(dat.Rectangle());
    for(Array2dIter2C<IntT,UIntT> it(ret,dat);it;it++)
      it.Data1() = it.Data2();
    return ret;
  }
  
  ImageC<UInt16T> UIntTImageCT2UInt16TImageCT(const ImageC<UIntT> &dat) { 
    ImageC<UInt16T> ret(dat.Rectangle());
    for(Array2dIter2C<UInt16T,UIntT> it(ret,dat);it;it++)
      it.Data1() = it.Data2();
    return ret;
  }
  
  DP_REGISTER_CONVERSION_NAMED(UIntTImageCT2DoubleImageCT,1,
			       "ImageC<RealT> RavlImageN::Convert(const ImageC<UIntT> &)");
  DP_REGISTER_CONVERSION_NAMED(UInt16TImageCT2UIntTImageCT,1,
			       "ImageC<UIntT> RavlImageN::Convert(const ImageC<UInt16T> &)");
  DP_REGISTER_CONVERSION_NAMED(UIntTImageCT2UInt16TImageCT,2,
			       "ImageC<UInt16T> RavlImageN::Convert(const ImageC<UIntT> &)");
  DP_REGISTER_CONVERSION_NAMED(IntTImageCT2UIntTImageCT,1.03125,
			       "ImageC<UIntT> RavlImageN::Convert(const ImageC<IntT> &)");
  DP_REGISTER_CONVERSION_NAMED(UIntTImageCT2IntTImageCT,1.03125,
			       "ImageC<IntT> RavlImageN::Convert(const ImageC<UIntT> &)");
  
  FileFormatStreamC<ImageC<IntT> > FileFormatStream_ImageC_IntT;
  FileFormatBinStreamC<ImageC<IntT> > FileFormatBinStream_ImageC_IntT;
  FileFormatStreamC<ImageC<UIntT> > FileFormatStream_ImageC_UIntT;
  FileFormatBinStreamC<ImageC<UIntT> > FileFormatBinStream_ImageC_UIntT;
  
}
