// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlExtImgIO

#include "Ravl/Image/CompressedImageJPEG.hh"
#include "Ravl/Image/ImgIOJPegB.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/BufStream.hh"

namespace RavlImageN {

  //: Write to standard stream.
  
  std::ostream &operator<<(std::ostream &strm,const CompressedImageJPEGC &data) {
    strm << data.Size() << " ";
    for(SArray1dIterC<char> it(data);it;it++)
      strm << ((int) *it) << " ";
    return strm;
  }
  
  //: Read from standard stream
  
  std::istream &operator>>(std::istream &strm,CompressedImageJPEGC &data) {
    UIntT size = 0;
    strm >> size;
    SArray1dC<char> bdata(size);
    for(SArray1dIterC<char> it(data);it;it++) {
      IntT v;
      strm >> v;
      *it = v;
    }
    data = CompressedImageJPEGC(bdata);
    return strm;
  }
  
  //: Write to standard stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const CompressedImageJPEGC &data) {
    strm << data.Size();
    if(data.Size() > 0)
      strm.OBuff(&(data[0]),data.Size());
    return strm;
  }
  
  //: Read from standard stream
  
  BinIStreamC &operator>>(BinIStreamC &strm,CompressedImageJPEGC &data) {
    UIntT size = 0;
    strm >> size;
    SArray1dC<char> buff(size);
    if(size > 0)
      strm.IBuff(&(buff[0]),size);
    data = CompressedImageJPEGC(buff);
    return strm;
  }
  
  //: Convert image to CompressedImage
  
  CompressedImageJPEGC RGBImage2CompressedImageJPEG(const ImageC<ByteRGBValueC> &dat) {
    BufOStreamC bufOStrm;
    DPOImageJPegByteRGBC imgJPEG(bufOStrm,100);
    if(!imgJPEG.Put(dat))
      throw DataNotReadyC("CompressedImageJ2K2RGBImage, Failed to encode image ");
    imgJPEG.Invalidate();
    return CompressedImageJPEGC(bufOStrm.Data());
  }
  
  
  //: Convert compressed buffer to a RGB image
  
  ImageC<ByteRGBValueC> CompressedImageJPEG2RGBImage(const CompressedImageJPEGC &data) {
    ImageC<ByteRGBValueC> ret;
    BufIStreamC bufIStrm(data);
    DPIImageJPegByteRGBC imgJPEG(bufIStrm);
    if(!imgJPEG.Get(ret)) 
      throw DataNotReadyC("CompressedImageJ2K2RGBImage, Failed to decode block ");
    return ret;
  }
  

  static TypeNameC type1(typeid(CompressedImageJPEGC),"RavlImageN::CompressedImageJPEGC");
  
  DP_REGISTER_CONVERSION_NAMED(RGBImage2CompressedImageJPEG ,1.1,
			       "CompressedImageJPEGC RavlImageN::Convert(const ImageC<ByteRGBValueC> &)");
  
  DP_REGISTER_CONVERSION_NAMED(CompressedImageJPEG2RGBImage ,1,
			       "ImageC<ByteRGBValueC> RavlImageN::Convert(const CompressedImageJPEGC &)");

  FileFormatStreamC<CompressedImageJPEGC> FileFormatStream_CompressedImageJPEGC;
  FileFormatBinStreamC<CompressedImageJPEGC> FileFormatBinStream_CompressedImageJPEGC;

  // Must link func.
  
  void InitJPEGCompressConv() 
  {}
  
  bool DPIImageJPEGCompressedBodyC::IsGetEOS() const
  {
    return m_byteFile.IsValid() && m_byteFile.IsGetEOS();
  }

  bool DPIImageJPEGCompressedBodyC::Get(CompressedImageJPEGC& data)
  {
    if (!m_byteFile.IsValid())
      return false;

    int size = m_byteFile.Size();
    SArray1dC<ByteT> byteData(size);
    if (m_byteFile.GetArray(byteData) != size)
      return false;

    char* byteDataPtr = reinterpret_cast<char*>(byteData.DataStart());
    SArray1dC<char> tempData(byteDataPtr, size, false);
    data = CompressedImageJPEGC(tempData.Copy());

    return true;
  }

  bool DPOImageJPEGCompressedBodyC::Put(const CompressedImageJPEGC& data)
  {
    if (!m_byteFile.IsValid())
      return false;

    int size = data.Size();
    unsigned char* byteDataPtr = reinterpret_cast<unsigned char*>(data.DataStart());
    SArray1dC<ByteT> byteData(byteDataPtr, size, false);

    return (m_byteFile.PutArray(byteData) == size);
  }

  bool DPOImageJPEGCompressedBodyC::IsPutReady() const
  {
    return m_byteFile.IsValid() && m_byteFile.IsPutReady();
  }

  void DPOImageJPEGCompressedBodyC::PutEOS()
  {
    if (m_byteFile.IsValid())
      m_byteFile.PutEOS();
  }
  
}
