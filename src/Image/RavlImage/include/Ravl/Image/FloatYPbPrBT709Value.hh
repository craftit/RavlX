// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FLOATYPBPRBT709VALUE_HEADER
#define RAVL_FLOATYPBPRBT709VALUE_HEADER 1
/////////////////////////////////////////////////////
//! file="Ravl/Image/Base/ByteYPbPrValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Pixel Types.YPbPr"

#include "Ravl/Image/YPbPrBT709Value.hh"
#include "Ravl/Types.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  class ByteRGBValueC;
  
  //! userlevel=Normal
  //: 16-bit YPbPr value class.
  // See http://en.wikipedia.org/wiki/YPbPr for details.
  
  // Note: this class's relationship with 8 bit values is special, when 
  // converting to 16 bit values the most significant bits are used. 
  // (Or equvialently the values are divided by 256).
  
  
  class FloatYPbPrBT709ValueC
    : public YPbPrBT709ValueC<FloatT>
  {
  public:
    FloatYPbPrBT709ValueC()
    {}
    //: Default constructor.
    // creates an undefined YCbCr pixel.
    
    FloatYPbPrBT709ValueC(UInt16T y,UInt16T b,UInt16T r)
      : YPbPrBT709ValueC<FloatT>((FloatT)y,(FloatT)b,(FloatT)r)
    {}
    //: Construct from 16 bit components.
    
    FloatYPbPrBT709ValueC(FloatT y,FloatT b,FloatT r)
      : YPbPrBT709ValueC<FloatT>(y,b,r)
    {}

    FloatYPbPrBT709ValueC(UInt8T y,UInt8T b,UInt8T r)
      : YPbPrBT709ValueC<FloatT>((FloatT)y,(FloatT)b,(FloatT)r)    //<<8,b<<8,r<<8)
    {}
    //: Construct from 8 bit components.
    
    FloatYPbPrBT709ValueC(const YPbPrBT709ValueC<UInt8T> &oth)
      : YPbPrBT709ValueC<FloatT>((FloatT)oth[0],(FloatT)oth[1],(FloatT)oth[2])   //<<8,oth[1]<<8,oth[2]<<8)
    {}
    //: Convert from 8 bit values.
    
    FloatYPbPrBT709ValueC(const YPbPrBT709ValueC<float> &oth)
      : YPbPrBT709ValueC<FloatT>((FloatT)ClipRange( 16*256.0 + oth[0]*65535.0,0.0,65535.0),
                                  (FloatT)ClipRange(128*256.0 + oth[1]*65535.0,0.0,65535.0),
                                  (FloatT)ClipRange(128*256.0 + oth[2]*65535.0,0.0,65535.0))
    {}
    //: Convert from floating point values.
    
    inline YPbPrBT709ValueC<float> FloatYPbPr() const
    { return YPbPrBT709ValueC<float>((data[0] - 256.0*16.0)/65535.0,
                                     (data[1] - 256.0*128.0)/65535.0,
                                     (data[2] - 256.0*128.0)/65535.0 
                                     ); 
     //data[0] - 32768,data[1] - 32768,data[2] - 32768);
    }
    //: Convert to float.

#if !RAVL_COMPILER_LLVM
    operator YPbPrBT709ValueC<float>() const
    { return FloatYPbPr(); }
    //: Convert to floating point values.
    // FIXME:- The llvm compiler complains this will never be used, is this a problem?
#endif

    operator YPbPrBT709ValueC<UInt8T>() const
    { return YPbPrBT709ValueC<FloatT>((FloatT) data[0],(FloatT) data[1],(FloatT) data[2]);}  //>>8,(UInt16T) data[1]>>8,(UInt16T) data[2]>>8); }
    //: Convert to 16 bit.
    
    //ByteRGBValueC ByteRGB() const;   not needed any more as only deal with floats in YPbPr
    //: Convert to 8 bit RGB.  
    
    RGBValueC<FloatT> FloatRGB() const;
    //: Convert to floating point RGB.
    
  };
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<FloatYPbPrBT709ValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<FloatYPbPrBT709ValueC> &img);  
  //: Load byte image from binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &out,const FloatYPbPrBT709ValueC &img);
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,FloatYPbPrBT709ValueC &img);
  //: Load pixel from binary stream
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::FloatYPbPrBT709ValueC > {
    typedef RavlImageN::YPbPrBT709ValueC<UInt32T> AccumT;    //: UInt32T Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YPbPrBT709ValueC<RealT> RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YPbPrBT709ValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
