// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage
//! file="Ravl/Image/Base/ByteYCbCrBT709Value.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYCbCrBT709Value.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  //: Conversion from byte RGB.
  
  ByteYCbCrBT709ValueC::ByteYCbCrBT709ValueC(const ByteRGBValueC &brgb)
  {
    const float R = brgb.Red();
    const float G = brgb.Green();
    const float B = brgb.Blue();
    this->data[0] = (UInt8T)(16  + 1.0/256.0 * (   46.742136455  * R +  157.243537124  * G +  15.873858194  * B));
    this->data[1] = (UInt8T)(128 + 1.0/256.0 * ( - 25.764878487  * R -   86.674699406  * G + 112.439  * B));
    this->data[2] = (UInt8T)(128 + 1.0/256.0 * (  112.439  * R -   102.129522617  * G -  10.310055275  * B));
  }
  
  //: Conversion from floating point RGB.
  
  ByteYCbCrBT709ValueC::ByteYCbCrBT709ValueC(const RGBValueC<float> &brgb)
  {
    const float &R = brgb.Red();
    const float &G = brgb.Green();
    const float &B = brgb.Blue();
    
    this->data[0] = (UInt8T)(16  + ( 46.559  * R + 156.6288  * G +  15.8118  * B));
    this->data[1] = (UInt8T)(128 + (-25.664151744  * R -  88.390970622  * G + 112.0    * B));
    this->data[2] = (UInt8T)(128 + (112.0    * R -  101.730251392  * G -  10.269748608  * B));
  }
  
  
  //: Convert to 8 bit RGB.
  
  ByteRGBValueC ByteYCbCrBT709ValueC::ByteRGB() const {
    double r = ( 298.082 * Y() + 458.942 * Cr()                  ) / 256.0 - 248.101;
    double g = ( 298.081 * Y() - 54.5917 * Cb() - 136.425 * Cr() ) / 256.0 + 76.8783;
    double b = ( 298.083 * Y() + 540.776 * Cb()                  ) / 256.0 - 289.018;
    //std::cerr << "r=" << r << " g=" << g << " b=" << b <<"\n";
    return ByteRGBValueC(ClipRange(Round(r),0,255),
                         ClipRange(Round(g),0,255),
                         ClipRange(Round(b),0,255)
                         );
  }

  //: Convert to a floating point RGB value.
  
  RGBValueC<float> ByteYCbCrBT709ValueC::FloatRGB() const {
    RGBValueC<float> ret;
    YCbCrBT709Float2RGBFloat(FloatYCbCr(),ret);
    return ret;
  }
  
  static TypeNameC type2(typeid(ByteYCbCrBT709ValueC),"ByteYCbCrBT709ValueC");
  
  ////////////////////////////////////////////////////////////
  // Accelerated IO routines...
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<ByteYCbCrBT709ValueC> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<ByteYCbCrBT709ValueC> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<ByteYCbCrBT709ValueC>(rect);
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  

    return in;
  }
  
  BinOStreamC &operator<<(BinOStreamC &out,const ByteYCbCrBT709ValueC &img) {
    out << img.Y() << img.Cb() << img.Cr();
    return out;
  }
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,ByteYCbCrBT709ValueC &img) {
    in >> img.Y() >> img.Cb() >> img.Cr();
    return in;
  }
  //: Load pixel from binary stream
  
}
