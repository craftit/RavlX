// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLBYTERGBAVALUE_HEADER
#define RAVLBYTERGBAVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteRGBAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RGBAValue.hh"

namespace RavlImageN {
  
  //: Byte RGBA value class.
  
  class ByteRGBAValueC
    : public RGBAValueC<ByteT>
  {
  public:
    ByteRGBAValueC()
      {}
    //: Default constructor.
    // creates an undefined RGBA pixel.

    ByteRGBAValueC(ByteT r,ByteT g,ByteT b,ByteT a)
      : RGBAValueC<ByteT>(r,g,b,a)
      {}
    //: Construct from components.
    
    template<typename PixelT>
    ByteRGBAValueC(const TFVectorC<PixelT,4> &oth)
      : RGBAValueC<ByteT>(static_cast<ByteT>(oth[0]),
			  static_cast<ByteT>(oth[1]),
			  static_cast<ByteT>(oth[2]),
                          static_cast<ByteT>(oth[3]))
    {}
    //: Construct from another value type.
    
    ByteRGBAValueC(const RGBAValueC<ByteT> &oth)
      : RGBAValueC<ByteT>(oth)
      {}
    //: Default constructor.
    // creates an undefined RGBA pixel.
    
    ByteT Y() const
      { return (ByteT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.

    RGBAValueC<RealT> operator*(RealT val) const {
      RGBAValueC<RealT> ret;
      ret[0] = ((RealT)(*this)[0] * val);
      ret[1] = ((RealT)(*this)[1] * val);
      ret[2] = ((RealT)(*this)[2] * val);
      ret[3] = ((RealT)(*this)[3] * val);
      return ret;
    }
    //: Multiply by a real.

  };
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteRGBAValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteRGBAValueC> &img);  
  //: Load byte image from binary stream 
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteRGBAValueC> {
    typedef RavlImageN::RGBAValueC<UInt16T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RGBAValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RGBAValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
