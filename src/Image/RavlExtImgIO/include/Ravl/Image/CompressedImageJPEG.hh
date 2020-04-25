// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_COMPRESSEDIMAGEJPEG_HEADER
#define RAVLIMAGE_COMPRESSEDIMAGEJPEG_HEADER 1
//! lib=RavlExtImgIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"

#include "Ravl/Array1d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/DP/ByteFileIO.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Buffer containing a compressed image.
  
  class CompressedImageJPEGC 
    : public SArray1dC<char>
  {
  public:
    CompressedImageJPEGC(const SArray1dC<char> &buff)
      : SArray1dC<char>(buff)
    {}
    //: Construct from char buffer
    
    CompressedImageJPEGC()
    {}
    //: Default constructor.
    
  };
  
  std::ostream &operator<<(std::ostream &strm,const CompressedImageJPEGC &data);
  //: Write to standard stream.
  
  std::istream &operator>>(std::istream &strm,CompressedImageJPEGC &data);
  //: Read from standard stream
  
  BinOStreamC &operator<<(BinOStreamC &strm,const CompressedImageJPEGC &data);
  //: Write to standard stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,CompressedImageJPEGC &data);
  //: Read from standard stream
  
  CompressedImageJPEGC RGBImage2CompressedImageJPEG(const ImageC<ByteRGBValueC> &img);
  //: Convert RGB image to a compressed buffer.
  
  ImageC<ByteRGBValueC> CompressedImageJPEG2RGBImage(const CompressedImageJPEGC &img);
  //: Convert compressed buffer to a RGB image

  class DPIImageJPEGCompressedBodyC
  : public DPIPortBodyC<CompressedImageJPEGC>
  {
  public:
    DPIImageJPEGCompressedBodyC(const StringC &filename)
    : m_byteFile(filename)
    {}

    DPIImageJPEGCompressedBodyC(IStreamC &stream)
    : m_byteFile(stream)
    {}

    virtual ~DPIImageJPEGCompressedBodyC()
    {}

    virtual bool IsGetEOS() const;

    virtual bool Get(CompressedImageJPEGC& data);

  private:
    DPIByteFileC m_byteFile;
  };

  class DPOImageJPEGCompressedBodyC
  : public DPOPortBodyC<CompressedImageJPEGC>
  {
  public:
    DPOImageJPEGCompressedBodyC(const StringC &filename)
    : m_byteFile(filename)
    {}

    DPOImageJPEGCompressedBodyC(OStreamC &stream)
    : m_byteFile(stream)
    {}

    virtual ~DPOImageJPEGCompressedBodyC()
    {}

    virtual bool Put(const CompressedImageJPEGC& data);

    virtual bool IsPutReady() const;

    virtual void PutEOS();

  private:
    DPOByteFileC m_byteFile;
  };

  class DPIImageJPEGCompressedC
  : public DPIPortC<CompressedImageJPEGC>
  {
  public:
    DPIImageJPEGCompressedC(const StringC &filename)
    : DPEntityC(*new DPIImageJPEGCompressedBodyC(filename))
    {}

    DPIImageJPEGCompressedC(IStreamC &stream)
    : DPEntityC(*new DPIImageJPEGCompressedBodyC(stream))
    {}
  };

  class DPOImageJPEGCompressedC
  : public DPOPortC<CompressedImageJPEGC>
  {
  public:
    DPOImageJPEGCompressedC(const StringC &filename)
    : DPEntityC(*new DPOImageJPEGCompressedBodyC(filename))
    {}

    DPOImageJPEGCompressedC(OStreamC &stream)
    : DPEntityC(*new DPOImageJPEGCompressedBodyC(stream))
    {}
  };

}

#endif
