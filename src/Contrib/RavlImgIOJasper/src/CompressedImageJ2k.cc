// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOJasper

#include "Ravl/Image/CompressedImageJ2k.hh"
#include "Ravl/Image/ImgIOJasper.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  //: Write to standard stream.
  
  ostream &operator<<(ostream &strm,const CompressedImageJ2kC &data) {
    strm << data.Size() << " ";
    for(SArray1dIterC<char> it(data);it;it++)
      strm << ((int) *it) << " ";
    return strm;
  }
  
  //: Read from standard stream
  
  istream &operator>>(istream &strm,CompressedImageJ2kC &data) {
    UIntT size = 0;
    strm >> size;
    SArray1dC<char> bdata(size);
    for(SArray1dIterC<char> it(data);it;it++) {
      IntT v;
      strm >> v;
      *it = v;
    }
    data = CompressedImageJ2kC(bdata);
    return strm;
  }
  
  //: Write to standard stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const CompressedImageJ2kC &data) {
    strm << data.Size();
    if(data.Size() > 0)
      strm.OBuff(&(data[0]),data.Size());
    return strm;
  }
  
  //: Read from standard stream
  
  BinIStreamC &operator>>(BinIStreamC &strm,CompressedImageJ2kC &data) {
    UIntT size = 0;
    strm >> size;
    SArray1dC<char> buff(size);
    if(size > 0)
      strm.IBuff(&(buff[0]),size);
    data = CompressedImageJ2kC(buff);
    return strm;
  }
  
  //: Convert image to CompressedImage
  
  CompressedImageJ2kC RGBImage2CompressedImageJ2K(const ImageC<ByteRGBValueC> &dat) {
    DPImageIOJasperBaseC imgIOJasper;
    imgIOJasper.OpenWriteMem(); // Open write buffer.
    imgIOJasper.SetDefaultFmt("jp2");
    jas_image_t *img = imgIOJasper.Ravl2Jas(dat);
    if(img == 0)
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Failed convert image to jasper format. ");
    bool ret = imgIOJasper.SaveImage(img);
    imgIOJasper.FreeImage(img); // Free jasper image.
    if(!ret) 
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Failed to code image. ");
    SArray1dC<char> data;
    if(!imgIOJasper.WriteBuffer(data))
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Failed to retrieve write buffer. ");
    imgIOJasper.Close();
    return CompressedImageJ2kC(data);
  }
  
  
  //: Convert compressed buffer to a RGB image
  
  ImageC<ByteRGBValueC> CompressedImageJ2K2RGBImage(const CompressedImageJ2kC &data) {
    DPImageIOJasperBaseC imgIOJasper;
    if(!imgIOJasper.OpenReadMem(data)) 
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Conversion failed to open buffer. ");
    jas_image_t *img = imgIOJasper.LoadImage();    
    if(img == 0) 
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Decode failed. ");
    ImageC<ByteRGBValueC> ret;
    if(!imgIOJasper.Jas2Ravl(img,ret)) 
      throw DataNotReadyC("RGBImage2CompressedImageJ2K, Failed to convert jasper image to Ravl image. ");
    imgIOJasper.Close();
    return ret;
  }
  

  static TypeNameC type1(typeid(CompressedImageJ2kC),"RavlImageN::CompressedImageJ2kC");
  
  DP_REGISTER_CONVERSION_NAMED(RGBImage2CompressedImageJ2K ,1.1,
			       "CompressedImageJ2kC RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(CompressedImageJ2K2RGBImage ,1,
			       "ImageC<ByteRGBValueC> RavlImageN::Convert(const CompressedImageJ2kC &)");

  FileFormatStreamC<CompressedImageJ2kC> FileFormatStream_CompressedImageJ2kC;
  FileFormatBinStreamC<CompressedImageJ2kC> FileFormatBinStream_CompressedImageJ2kC;

  // Must link func.
  
  void InitJasperCompressConv() 
  {}
  
  
}
