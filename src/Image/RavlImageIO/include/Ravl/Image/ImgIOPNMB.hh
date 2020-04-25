// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DPIMAGEIOPPM_HEADER
#define RAVLIMAGE_DPIMAGEIOPPM_HEADER
///////////////////////////////////////////////////
//! userlevel=Develop
//! rcsid="$Id$"
//! file="Ravl/Image/ImageIO/ImgIOPNMB.hh"
//! lib=RavlImageIO
//! docentry="Ravl.API.Images.IO.Formats"
//! author="Charles Galambos"
//! date="29/10/1998"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  //: PNM Utilities
  
  class DPImageIOPNMBaseC {
  public:    
    bool LoadHeader(IStreamC &in,ByteT &header,IntT &x,IntT &y);
    //: Load a PNM header.
    
    bool SkipComment(IStreamC &in);
    //: Skip rest of comment.
    
    bool GetNumber(IStreamC &in,IntT &dig);
    //: Get a number.
    
    bool GetBit(IStreamC &inf,ByteT &into);
    //: Read next bit from pbm
  
    char SkipWhite(IStreamC &in);
    //: Skip white space.
  };
  
  //: Load a PPM image.
  
  class DPIImagePNMByteRGBBodyC : 
    public DPIPortBodyC<ImageC<ByteRGBValueC> >,
    public DPImageIOPNMBaseC
  {
  public:
    DPIImagePNMByteRGBBodyC(const StringC & fn);
    //: Constructor from filename.
  
    DPIImagePNMByteRGBBodyC(const IStreamC &in);
    //: Constructor from stream.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    
    virtual ImageC<ByteRGBValueC> Get();
    //: Get next piece of data.
    
    virtual bool Get(ImageC<ByteRGBValueC> &buff);
    //: Get image from stream.
    
  protected:
    IStreamC inf; // Infile.
    bool done;
  };
  
  //: Save a ByteRGB image.
  
  class DPOImagePNMByteRGBBodyC : 
    public DPOPortBodyC<ImageC<ByteRGBValueC> >,
    public DPImageIOPNMBaseC
  {
  public:
    DPOImagePNMByteRGBBodyC(const StringC & fn);
    //: Constructor from filename.
    
    DPOImagePNMByteRGBBodyC(const OStreamC &strm);
    //: Constructor from filename.
    
    virtual bool Put(const ImageC<ByteRGBValueC> &dat);
    //: Put data.
    
    virtual bool IsPutReady() const ;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf; // Infile.
    bool done;
  };
  
  //: Load a pgm image.
  
  class DPIImagePNMByteGreyBodyC : 
    public DPIPortBodyC<ImageC<ByteT> >,
    public DPImageIOPNMBaseC
  {
  public:
    DPIImagePNMByteGreyBodyC(const StringC & fn);
    //: Constructor from filename.
    
    DPIImagePNMByteGreyBodyC(const IStreamC &strm);
    //: Constructor from filename.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?

    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    
    virtual ImageC<ByteT> Get();
    //: Get next piece of data.
    
    virtual bool Get(ImageC<ByteT> &buff);
    //: Get image from stream.
    
  protected:
    IStreamC inf; // Infile.
    bool done;
  };
  
  //: Save a pgm image.
  
  class DPOImagePNMByteGreyBodyC :
    public DPOPortBodyC<ImageC<ByteT> >,
    public DPImageIOPNMBaseC
  {
  public:
    DPOImagePNMByteGreyBodyC(const StringC & fn);
    //: Constructor from filename.
    
    DPOImagePNMByteGreyBodyC(const OStreamC &strm);
    //: Constructor from filename.
    
    virtual bool Put(const ImageC<ByteT> &dat);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf; // Infile.
    bool done;
  };

}
#endif
