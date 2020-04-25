// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMGIOPNG_HEADER
#define RAVLIMGIOPNG_HEADER 1
/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! file="Ravl/Image/ExternalImageIO/PNG/ImgIOPNG.hh"
//! lib=RavlExtImgIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! example=exImgIO.cc
//! date="29/10/98"

#include "Ravl/DP/Port.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  
  //: Load an RGB image in PNG format.
  // Use via function found in amma/StdType/DataProc/FileFormatIO.hh
  
  class DPIImagePNGByteRGBC : public DPIPortC<ImageC<ByteRGBValueC> > {
  public:
    DPIImagePNGByteRGBC(StringC fn);
    //: Constructor from filename.  
    
    DPIImagePNGByteRGBC(const IStreamC &strm);
    //: Constructor from stream 
  };
  
  //: Save an RGB image in PNG format.
  
  class DPOImagePNGByteRGBC : public DPOPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPOImagePNGByteRGBC(StringC fn);
    //: Constructor from filename.  
    
    DPOImagePNGByteRGBC(const OStreamC &strm);
    //: Constructor from stream.  
  };
  
  //: Load a byte image in PNG format.
  // Use via function found in amma/StdType/DataProc/FileFormatIO.hh
  
  class DPIImagePNGByteGreyC 
    :  public DPIPortC<ImageC<ByteT> >
  {
  public:
    DPIImagePNGByteGreyC(StringC fn);
    //: Constructor from filename.  
    
    DPIImagePNGByteGreyC(const IStreamC &strm);
    //: Constructor from stream 
  };
  
  //: Save a byte image in PNG format.
  
  class DPOImagePNGByteGreyC 
    : public DPOPortC<ImageC<ByteT> >
  {
  public:
    DPOImagePNGByteGreyC(StringC fn);
    //: Constructor from filename.  
    
    DPOImagePNGByteGreyC(const OStreamC &strm);
    //: Constructor from stream.  
  };

}

#endif
