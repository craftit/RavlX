// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_UINT16YCBCRBT601VALUE_HEADER
#define RAVL_UINT16YCBCRBT601VALUE_HEADER 1
/////////////////////////////////////////////////////
//! file="Ravl/Image/Base/ByteYCbCrValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Pixel Types.YCbCr"

#include "Ravl/Image/YCbCrBT601Value.hh"
#include "Ravl/Types.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  class ByteRGBValueC;
  
  //! userlevel=Normal
  //: 16-bit YCbCr value class.
  // See http://en.wikipedia.org/wiki/YCbCr for details.
  
  // Note: this class's relationship with 8 bit values is special, when 
  // converting to 16 bit values the most significant bits are used. 
  // (Or equivalently the values are divided by 256).
  
  
  class UInt16YCbCrBT601ValueC
    : public YCbCrBT601ValueC<UInt16T>
  {
  public:
    UInt16YCbCrBT601ValueC()
    {}
    //: Default constructor.
    // creates an undefined YCbCr pixel.
    
    UInt16YCbCrBT601ValueC(UInt16T y,UInt16T b,UInt16T r)
      : YCbCrBT601ValueC<UInt16T>(y,b,r)
    {}
    //: Construct from 16 bit components.
    
    UInt16YCbCrBT601ValueC(UInt8T y,UInt8T b,UInt8T r)
      : YCbCrBT601ValueC<UInt16T>(y<<8,b<<8,r<<8)
    {}
    //: Construct from 8 bit components.
    
    UInt16YCbCrBT601ValueC(const YCbCrBT601ValueC<UInt8T> &oth)
      : YCbCrBT601ValueC<UInt16T>(oth[0]<<8,oth[1]<<8,oth[2]<<8)
    {}
    //: Convert from 8 bit values.
    
    UInt16YCbCrBT601ValueC(const YCbCrBT601ValueC<float> &oth)
      : YCbCrBT601ValueC<UInt16T>((UInt16T)ClipRange( 16*256.0 + oth[0]*65535.0,0.0,65535.0),
                                  (UInt16T)ClipRange(128*256.0 + oth[1]*65535.0,0.0,65535.0),
                                  (UInt16T)ClipRange(128*256.0 + oth[2]*65535.0,0.0,65535.0))
    {}
    //: Convert from floating point values.
    
    inline YCbCrBT601ValueC<float> FloatYCbCr() const
    { return YCbCrBT601ValueC<float>((data[0] - 256.0* 16.0)/65535.0,
                                     (data[1] - 256.0*128.0)/65535.0,
                                     (data[2] - 256.0*128.0)/65535.0 
                                     ); 
    }
    //: Convert to float.
    operator YCbCrBT601ValueC<float>() const
    { return FloatYCbCr(); }
    //: Convert to floating point values.
    
    operator YCbCrBT601ValueC<UInt8T>() const
    { return YCbCrBT601ValueC<UInt8T>((UInt16T) data[0]>>8,(UInt16T) data[1]>>8,(UInt16T) data[2]>>8); }
    //: Convert to 16 bit.
    
    ByteRGBValueC ByteRGB() const;
    //: Convert to 8 bit RGB.  
    
    RGBValueC<FloatT> FloatRGB() const;
    //: Convert to floating point RGB.
    
  };
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<UInt16YCbCrBT601ValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<UInt16YCbCrBT601ValueC> &img);  
  //: Load byte image from binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &out,const UInt16YCbCrBT601ValueC &img);
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,UInt16YCbCrBT601ValueC &img);
  //: Load pixel from binary stream
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::UInt16YCbCrBT601ValueC > {
    typedef RavlImageN::YCbCrBT601ValueC<UInt32T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YCbCrBT601ValueC<RealT> RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YCbCrBT601ValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
