// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage
//! file="Ravl/Image/Base/UInt16YCbCrBT709Value.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/UInt16YCbCrBT709Value.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  ByteRGBValueC UInt16YCbCrBT709ValueC::ByteRGB() const {
    double y = Y()/256.0;
    double cb = Cb()/256.0;
    double cr = Cr()/256.0;
    double r = ( 298.082 * y + 458.942 * cr                  ) / 256.0 - 248.101;
    double g = ( 298.081 * y - 54.5917 * cb - 136.425 * cr ) / 256.0 + 76.8783;
    double b = ( 298.083 * y + 540.776 * cb                  ) / 256.0 - 289.018;
    //std::cerr << "r=" << r << " g=" << g << " b=" << b <<"\n";
    return ByteRGBValueC(ClipRange(Round(r),0,255),
                         ClipRange(Round(g),0,255),
                         ClipRange(Round(b),0,255)
                         );

  }
  //: Convert to 8 bit RGB.  
    
  RGBValueC<FloatT> UInt16YCbCrBT709ValueC::FloatRGB() const {
    RGBValueC<float> ret;
    YCbCrBT709Float2RGBFloat(FloatYCbCr(),ret);
    return ret;
  }
  
  
  static TypeNameC type2(typeid(UInt16YCbCrBT709ValueC),"RavlImageN::UInt16YCbCrBT709ValueC");
  
  ////////////////////////////////////////////////////////////
  // Accelerated IO routines...
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<UInt16YCbCrBT709ValueC> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<UInt16YCbCrBT709ValueC> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<UInt16YCbCrBT709ValueC>(rect);
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  

    return in;
  }
  
  BinOStreamC &operator<<(BinOStreamC &out,const UInt16YCbCrBT709ValueC &img) {
    out << img.Y() << img.Cb() << img.Cr();
    return out;
  }
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,UInt16YCbCrBT709ValueC &img) {
    in >> img.Y() >> img.Cb() >> img.Cr();
    return in;
  }
  //: Load pixel from binary stream
  
}
