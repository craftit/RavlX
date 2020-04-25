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
//! file="Ravl/Image/ImageIO/ImgIOByte.cc"

#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Stream.hh"
#include "Ravl/FunctionRegister.hh"


namespace RavlImageN {
  
  void InitImgIOByte()
  {}
  
  static TypeNameC type1(typeid(ImageC<ByteT>),"ImageC<ByteT>");  
  static TypeNameC type2(typeid(ImageC<SByteT>),"ImageC<SByteT>");
  static TypeNameC type3(typeid(Array2dC<ByteT>),"RavlN::Array2dC<ByteT>");  
  static TypeNameC type4(typeid(Array2dC<SByteT>),"RavlN::Array2dC<SByteT>");
  
  ImageC<ByteT>  BoolImageC2ByteImageCT(const ImageC<bool> &dat)   { 
    ImageC<ByteT> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteT,bool> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = it.Data2() ? 255 : 0;
    return ret;
  }
  
  ImageC<ByteT> Array2dByte2ImageByte(const Array2dC<ByteT> &dat)   
  { return dat; }

  Array2dC<ByteT> ImageByte2Array2dByte(const ImageC<ByteT> &dat)   
  { return dat; }
  
  DP_REGISTER_CONVERSION_NAMED(BoolImageC2ByteImageCT,1,
			       "ImageC<ByteT> RavlImageN::Convert(const ImageC<bool> &)");
  
  DP_REGISTER_CONVERSION_NAMED(Array2dByte2ImageByte,1,
			       "ImageC<ByteT> RavlImageN::Convert(const Array2dC<ByteT> &)");
  
  DP_REGISTER_CONVERSION_NAMED(ImageByte2Array2dByte,1,
			       "Array2dC<ByteT> RavlImageN::Convert(const ImageC<ByteT> &)");
  
  FileFormatStreamC<ImageC<ByteT> > FileFormatStream_ImageC_ByteT;
  FileFormatBinStreamC<ImageC<ByteT> > FileFormatBinStream_ImageC_ByteT;
  FileFormatStreamC<ImageC<SByteT> > FileFormatStream_ImageC_SByteT;
  FileFormatBinStreamC<ImageC<SByteT> > FileFormatBinStream_ImageC_SByteT;
  

}
