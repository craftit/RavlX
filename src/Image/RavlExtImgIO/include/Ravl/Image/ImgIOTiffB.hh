// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLDPIMAGEIOTIFFB_HEADER
#define RAVLDPIMAGEIOTIFFB_HEADER
///////////////////////////////////////////////////
//! userlevel=Develop
//! file="Ravl/Image/ExternalImageIO/Tiff/ImgIOTiffB.hh"
//! lib=RavlExtImgIO
//! docentry="Ravl.API.Images.IO.Formats"
//! author="Charles Galambos"
//! date="29/10/98"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/ImgIOTiff.hh"
#include "Ravl/Image/ByteRGBAValue.hh"

#include <tiffio.h>

namespace RavlImageN {

  //: TIFF Utilities
  
  class DPImageIOTIFFBaseC {
  public:    
    DPImageIOTIFFBaseC();
    //: Constructor.
    
    ~DPImageIOTIFFBaseC();
    //: Destructor.
    
  protected:    
    TIFF *tif;
  };
  
  //: Load a RGBA image.
  
  class DPIImageTIFFByteRGBABodyC : 
    public DPIPortBodyC<ImageC<ByteRGBAValueC> >,
    public DPImageIOTIFFBaseC
  {
  public:
    DPIImageTIFFByteRGBABodyC(StringC fn);
    //: Constructor from filename.
    
    DPIImageTIFFByteRGBABodyC(const IStreamC &in);
    //: Constructor from stream.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual ImageC<ByteRGBAValueC> Get();
    //: Get next piece of data.
    
  protected:
    static tsize_t TIFFReadProc(thandle_t, tdata_t, tsize_t);
    static tsize_t TIFFWriteProc(thandle_t, tdata_t, tsize_t);
    static toff_t  TIFFSeekProc(thandle_t, toff_t, int);
    static int TIFFCloseProc(thandle_t);
    static toff_t TIFFSizeProc(thandle_t);
    static int TIFFMapFileProc(thandle_t, tdata_t*, toff_t*);
    static void TIFFUnmapFileProc(thandle_t, tdata_t, toff_t);
    
    
    IStreamC inf; // Infile.
    bool done;
  };
  
  //: Save a ByteRGBA image.
  
  class DPOImageTIFFByteRGBABodyC : 
    public DPOPortBodyC<ImageC<ByteRGBAValueC> >,
    public DPImageIOTIFFBaseC
  {
  public:
    DPOImageTIFFByteRGBABodyC(StringC fn);
    //: Constructor from filename.
    
    DPOImageTIFFByteRGBABodyC(const OStreamC &strm);
    //: Constructor from filename.
    
    virtual bool Put(const ImageC<ByteRGBAValueC> &dat);
    //: Put data.
    
    virtual bool IsPutReady() const ;
    //: Is port ready for data ?
    
  protected:
    static tsize_t TIFFReadProc(thandle_t, tdata_t, tsize_t);
    static tsize_t TIFFWriteProc(thandle_t, tdata_t, tsize_t);
    static toff_t  TIFFSeekProc(thandle_t, toff_t, int);
    static int TIFFCloseProc(thandle_t);
    static toff_t TIFFSizeProc(thandle_t);
    static int TIFFMapFileProc(thandle_t, tdata_t*, toff_t*);
    static void TIFFUnmapFileProc(thandle_t, tdata_t, toff_t);
    
    OStreamC outf; // Infile.
    bool done;
  };

}
#endif
