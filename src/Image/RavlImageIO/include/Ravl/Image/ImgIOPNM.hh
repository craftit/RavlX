// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMGIOPNM_HEADER
#define RAVLIMGIOPNM_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! file="Ravl/Image/ImageIO/ImgIOPNM.hh"
//! lib=RavlImageIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! example=exImgIO.cc
//! date="29/10/98"

#include "Ravl/DP/Port.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  
  //: Load a RGB image in PPM format.
  // Use via function found in Ravl/StdType/DataProc/FileFormatIO.hh
  
  class DPIImagePNMByteRGBC 
    : public DPIPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPIImagePNMByteRGBC(const StringC &fn);
    //: Constructor from filename.  
    
    DPIImagePNMByteRGBC(const IStreamC &strm);
    //: Constructor from stream 
  };
  
  //: Save a RGB image in PPM format.
  
  class DPOImagePNMByteRGBC 
    : public DPOPortC<ImageC<ByteRGBValueC> >
  {
  public:
    DPOImagePNMByteRGBC(const StringC &fn);
    //: Constructor from filename.  
    
    DPOImagePNMByteRGBC(const OStreamC &strm);
    //: Constructor from stream.  
  };
  
  //: Load a byte image in PGM format.
  
  class DPIImagePNMByteGreyC 
    : public DPIPortC<ImageC<ByteT> >
  {
  public:
    DPIImagePNMByteGreyC(const StringC &fn);
    //: Constructor from filename.
    
    DPIImagePNMByteGreyC(const IStreamC &strm);
    //: Constructor from stream.
  };
  
  //: Save a byte image in PGM format.
  
  class DPOImagePNMByteGreyC 
    : public DPOPortC<ImageC<ByteT> >
  {
  public:
    DPOImagePNMByteGreyC(const StringC &fn);
    //: Constructor from filename.
    
    DPOImagePNMByteGreyC(const OStreamC &strm);
  //: Constructor from filename.
    
  };
}



#endif
