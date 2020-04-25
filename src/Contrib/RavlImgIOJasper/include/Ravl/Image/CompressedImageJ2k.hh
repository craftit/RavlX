// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_COMPRESSEDIMAGEJ2K_HEADER
#define RAVLIMAGE_COMPRESSEDIMAGEJ2K_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlImgIOJasper

#include "Ravl/Array1d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Buffer containing a compressed image.
  
  class CompressedImageJ2kC 
    : public SArray1dC<char>
  {
  public:
    CompressedImageJ2kC(const SArray1dC<char> &buff)
      : SArray1dC<char>(buff)
    {}
    //: Construct from char buffer
    
    CompressedImageJ2kC()
    {}
    //: Default constructor.
    
  };
  
  ostream &operator<<(ostream &strm,const CompressedImageJ2kC &data);
  //: Write to standard stream.
  
  istream &operator>>(istream &strm,CompressedImageJ2kC &data);
  //: Read from standard stream
  
  BinOStreamC &operator<<(BinOStreamC &strm,const CompressedImageJ2kC &data);
  //: Write to standard stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,CompressedImageJ2kC &data);
  //: Read from standard stream
  
  CompressedImageJ2kC RGBImage2CompressedImageJ2K(const ImageC<ByteRGBValueC> &img);
  //: Convert RGB image to a compressed buffer.
  
  ImageC<ByteRGBValueC> CompressedImageJ2K2RGBImage(const CompressedImageJ2kC &img);
  //: Convert compressed buffer to a RGB image
  
}


#endif
