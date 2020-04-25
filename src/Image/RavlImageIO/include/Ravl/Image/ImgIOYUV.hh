// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMGIOYUV_HEADER
#define RAVLIMGIOYUV_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! file="Ravl/Image/ImageIO/ImgIOYUV.hh"
//! lib=RavlImageIO
//! author="Bill Christmas"
//! docentry="Ravl.API.Images.IO.Formats"
//! example=exImgIO.cc
//! date="4/11/2013"

#include "Ravl/DP/Port.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Hash.hh"

namespace RavlImageN {



  //: Body for loading a YUV image.
  
  class DPIImageYUVBodyC : 
    public DPIPortBodyC<ImageC<ByteYUVValueC> >
  {
  public:
    DPIImageYUVBodyC(const StringC & fn);
    //: Constructor from filename.
  
    DPIImageYUVBodyC(const IStreamC &in);
    //: Constructor from stream.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    
    virtual ImageC<ByteYUVValueC> Get();
    //: Get next piece of data.
    
    virtual bool Get(ImageC<ByteYUVValueC> &buff);
    //: Get image from stream.
    
  protected:
    IStreamC inf; // In file.
    bool done;
  };
  
  //: Body for saving a ByteYUV image.
  
  class DPOImageYUVBodyC : 
    public DPOPortBodyC<ImageC<ByteYUVValueC> >
  {
  public:
    DPOImageYUVBodyC(const StringC & fn);
    //: Constructor from filename.
    
    DPOImageYUVBodyC(const OStreamC &strm);
    //: Constructor from stream.
    
    virtual bool Put(const ImageC<ByteYUVValueC> &dat);
    //: Put data.
    
    virtual bool IsPutReady() const ;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf; // Out file.
    bool done;
  };
  






  
  //: Load a YUV image from YUV 422format.
  // Use via function found in Ravl/StdType/DataProc/FileFormatIO.hh
  
  class DPIImageYUVC 
    : public DPIPortC<ImageC<ByteYUVValueC> >
  {
  public:
    DPIImageYUVC(const StringC &fn);
    //: Constructor from filename.  
    
    DPIImageYUVC(const IStreamC &strm);
    //: Constructor from stream 

    static HashC<StreamSizeT,Index2dC> ImgParams();
    //: Returns allowable image parameters for YUV format files
  };
  
  //: Save a YUV image in YUV 422 format.
  
  class DPOImageYUVC 
    : public DPOPortC<ImageC<ByteYUVValueC> >
  {
  public:
    DPOImageYUVC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOImageYUVC(const OStreamC &strm);
    //: Constructor from stream.  
  };
  
}



#endif
