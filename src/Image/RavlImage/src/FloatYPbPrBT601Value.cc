// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage
//! file="Ravl/Image/Base/FloatYPbPrBT601Value.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/FloatYPbPrBT601Value.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  /*ByteRGBValueC UInt16YCbCrBT601ValueC::ByteRGB() const {
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

  }*/ // not needed any more as only deal with floats in YPbPr
  //: Convert to 8 bit RGB.  
    
  RGBValueC<FloatT> FloatYPbPrBT601ValueC::FloatRGB() const {
    RGBValueC<float> ret;
    YPbPrBT601Float2RGBFloat(FloatYPbPr(),ret);
    return ret;
  }
  
  
  static TypeNameC type2(typeid(FloatYPbPrBT601ValueC),"RavlImageN::FloatYPbPrBT601ValueC");
  
  ////////////////////////////////////////////////////////////
  // Accelerated IO routines...
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<FloatYPbPrBT601ValueC> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<FloatYPbPrBT601ValueC> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<FloatYPbPrBT601ValueC>(rect);
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  

    return in;
  }
  
  BinOStreamC &operator<<(BinOStreamC &out,const FloatYPbPrBT601ValueC &img) {
    out << img.Y() << img.Pb() << img.Pr();
    return out;
  }
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,FloatYPbPrBT601ValueC &img) {
    in >> img.Y() >> img.Pb() >> img.Pr();
    return in;
  }
  //: Load pixel from binary stream
  
}
