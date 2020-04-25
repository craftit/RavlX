// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOJASPER_HEADER
#define RAVLIMAGE_IMGIOJASPER_HEADER 1 
//! rcsid="$Id$"
//! lib=RavlImgIOJasper

#include "Ravl/Image/Image.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/String.hh"

#include "jasper/jasper.h"

namespace RavlImageN {
  using namespace RavlN;
  
  //: Jasper IO base class.
  
  class DPImageIOJasperBaseC {
  public:
    DPImageIOJasperBaseC(const RealT compressionRate = 1.0);
    //: Default constructor.
    
    virtual ~DPImageIOJasperBaseC();
    //: Destructor.
    
    bool OpenRead(const StringC &filename);
    //: Open stream for reading
    
    bool OpenReadMem(const SArray1dC<char> &data);
    //: Open a memory buffer for reading.

    bool OpenIStream(IStreamC &is);
    //: Open a memory buffer for reading.
    
    bool OpenWrite(const StringC &filename);
    //: Open stream for write

    bool OpenWriteMem(void);
    //: Open a memory buffer for reading.

    bool OpenOStream(OStreamC &os);
    //: Open a memory buffer for reading.
    
    bool WriteBuffer(SArray1dC<char> &data);
    //: Access current write buffer.
    // Returns false if one is not found.
    
    bool CanReadImage();
    //: Test if the current stream can be read.
    
    jas_image_t *LoadImage();
    //: Load image from current stream.
    
    jas_image_t *ConvertToRGB(jas_image_t *originalImage);
    //: Convert image to RGB colour space.
    // Note the original image is free's in the process, even on error.
    
    bool Jas2Ravl(jas_image_t *img,ImageC<ByteRGBValueC> &img2);
    //: Convert an image into RAVL form.
    // img is free'd by operation.
    // Returns false if failes.
    
    bool SaveImage(jas_image_t *img);
    //: save an image to the current stream.
    
    jas_image_t *Ravl2Jas(const ImageC<ByteRGBValueC> &img);
    //: Convert a Ravl image into a Jasper one.
    
    bool FreeImage(jas_image_t *image);
    //: Free an old image.
    
    bool IsOpen() const
    { return iostream != 0; }
    //: Check we're ready for io.
    
    static IntT FindFormatByFilename(const StringC &filename);
    //: Look for format by filename.
    // returns -1 if not found.
    
    void SetDefaultFmt(IntT fmt)
    { defaultFmt = fmt; }
    //: Setup default format.
    
    void SetDefaultFmt(const StringC &fmtName);
    //: Set format by name
    
    void Close();
    //: Close the stream.

    static int  str_read(jas_stream_obj_t *obj, char *buf, int cnt);
    static int str_write(jas_stream_obj_t *obj, char *buf, int cnt);
    static long str_seek(jas_stream_obj_t *obj, long offset, int origin);
    static int str_close(jas_stream_obj_t *obj);
    
  protected:
    jas_stream_t *iostream;
    IntT defaultFmt;
    SArray1dC<char> buff;
    
    
    IStreamC istrm;
    OStreamC ostrm;
    bool useStrmI;
    bool useStrmO;
    const RealT compressionRate;
    bool useCompressionRate;
  };
  
  
  //: Load a JPEG image.
  
  template<class PixelT>
  class DPIImageJasperBodyC 
    : public DPIPortBodyC<ImageC<PixelT> >,
      public DPImageIOJasperBaseC
  {
  public:
    DPIImageJasperBodyC(const StringC &fn) 
    { OpenRead(fn); }
    //: Constructor from filename.

    DPIImageJasperBodyC(const SArray1dC<char> &buffData)
    { OpenReadMem(buffData); }
    //: Constructor from buffer.
    
    DPIImageJasperBodyC(IStreamC &is) 
    { OpenIStream(is); }
    //: Constructor from IStream.
    
    virtual bool IsGetEOS() const
    { return IsOpen(); }
    //: Is valid data ?
    
    virtual ImageC<PixelT> Get() {
      ImageC<PixelT> img;
      if(!Get(img))
        throw DataNotReadyC("Failed to load image. ");
      return img;
    }
    //: Get next piece of data.
    
    virtual bool Get(ImageC<PixelT> &buff) {
      jas_image_t *img = LoadImage();
      if(img == 0) return false;
      bool ret = Jas2Ravl(img,buff);
      Close();
      return ret;
    }
    //: Get next piece of data.
    
  protected:
    bool Read(ImageC<PixelT> &buf);
    //: Read image data.
  };
  
  //: Save a JPEG image.
  
  template<class PixelT>
  class DPOImageJasperBodyC : 
    public DPOPortBodyC<ImageC<PixelT> >,
    public DPImageIOJasperBaseC
  {
  public:
    DPOImageJasperBodyC(const StringC &fn,const RealT compressionRate = 1.0) :
      DPImageIOJasperBaseC(compressionRate)
    { 
      OpenWrite(fn); 
      SetDefaultFmt(DPImageIOJasperBaseC::FindFormatByFilename(fn));
    }
    //: Constructor from filename.
    
    DPOImageJasperBodyC(OStreamC &os,const StringC &fmtName,const RealT compressionRate = 1.0) : 
      DPImageIOJasperBaseC(compressionRate)
    {
      OpenOStream(os);
      SetDefaultFmt(fmtName);
    }
    //: Constructor an output stream.
    
    virtual bool Put(const ImageC<PixelT> &dat) {
      jas_image_t *img = Ravl2Jas(dat);
      if(img == 0) return false;
      bool ret = SaveImage(img);
      Close();
      FreeImage(img); // Free jasper image.
      return ret;
    }
    //: Put data.
    
    virtual bool IsPutReady() const 
    { return IsOpen(); }
    //: Is port ready for data ?
    
  protected:
    bool Write(const ImageC<PixelT> &buf);
    //: Write image data.
    
  };
  
  //: Load an RGB image in JPEG format.
  // Use via function found in amma/StdType/DataProc/FileFormatIO.hh
  
  class DPIImageJasperByteRGBC 
    : public DPIPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPIImageJasperByteRGBC(const StringC &fn)
      : DPEntityC(*new DPIImageJasperBodyC<ByteRGBValueC>(fn))
    {}
    //: Constructor from filename.  
    
    DPIImageJasperByteRGBC(IStreamC &is)
      : DPEntityC(*new DPIImageJasperBodyC<ByteRGBValueC>(is))
    {}
    //: Constructor from filename.  
    
  };
  
  //: Save an RGB image in JPEG format.
  
  class DPOImageJasperByteRGBC 
    : public DPOPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPOImageJasperByteRGBC(const StringC &fn,const RealT compressionRate = 1.0)
      : DPEntityC(*new DPOImageJasperBodyC<ByteRGBValueC>(fn,compressionRate))
    {}
    //: Constructor from filename. 
    
    DPOImageJasperByteRGBC(OStreamC &os,const StringC &fmtName,const RealT compressionRate = 1.0)
      : DPEntityC(*new DPOImageJasperBodyC<ByteRGBValueC>(os,fmtName,compressionRate))
    {}
    //: Constructor from filename. 
    
  };
 
}



#endif
