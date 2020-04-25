// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_UINT16RGBVALUE_HEADER
#define RAVL_UINT16RGBVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/UInt16RGBValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  class RealRGBValueC;
  
  //: UInt16 RGB value class.
  
  class UInt16RGBValueC
    : public RGBValueC<UInt16T>
  {
  public:
    UInt16RGBValueC()
    {}
    //: Default constructor.
    // creates an undefined RGB pixel.

    UInt16RGBValueC(UInt16T r,UInt16T g, UInt16T b)
      : RGBValueC<UInt16T>(r,g,b)
    {}
    //: Construct from components.
    
    template<typename PixelT>
    UInt16RGBValueC(const TFVectorC<PixelT,3> &oth)
      : RGBValueC<UInt16T>(static_cast<UInt16T>(oth[0]),static_cast<UInt16T>(oth[1]),static_cast<UInt16T>(oth[2]))
    {}
    //: Construct from another value type.
    
    template<typename PixelT>
    const UInt16RGBValueC &operator+=(const TFVectorC<PixelT,3> &oth) { 
      data[0] += oth[0];
      data[1] += oth[1];
      data[2] += oth[2];
      return *this;
    }
    //: Add another value to this pixel.
    
    UInt16T Y() const
    { return (UInt16T)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.
    
    template<typename PixelT>
    UInt16RGBValueC operator+(const TFVectorC<PixelT,3> &val) const
    { return UInt16RGBValueC(data[0] + val[0],data[1] + val[1],data[2] + val[2]); }
    //: Add two pixels
    
    template<typename PixelT>
    UInt16RGBValueC operator-(const TFVectorC<PixelT,3> &val) const
    { return UInt16RGBValueC(data[0] - val[0],data[1] - val[1],data[2] - val[2]); }
    //: Add two pixels
    
    UInt16RGBValueC operator*(UIntT val) const
    { return UInt16RGBValueC(data[0] * val,data[1] * val,data[2] * val); }
    //: Multiply by a constant
    
    UInt16RGBValueC operator/(UIntT val) const
    { return UInt16RGBValueC(data[0] / val,data[1] / val,data[2] / val); }
    //: Divide by a constant
  };
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::UInt16RGBValueC> {
    typedef RavlImageN::RGBValueC<UIntT> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RealRGBValueC  RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RGBValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
