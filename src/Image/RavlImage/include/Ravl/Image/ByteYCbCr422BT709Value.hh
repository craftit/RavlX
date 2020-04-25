// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEYCBCR422BT709VALUE_HEADER
#define RAVL_BYTEYCBCR422BT709VALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id: ByteYUV422Value.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YCbCr"

#include "Ravl/Image/YCbCr422BT709Value.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  //! userlevel=Normal
  //: Byte YCbCr 422 value class.
  // Even columns in an image contain the Cb component, odd columns contain Cr.
  
  class ByteYCbCr422BT709ValueC
    : public YCbCr422BT709ValueC<ByteT>
  {
  public:
    ByteYCbCr422BT709ValueC()
    {}
    //: Default constructor.
    // The value is left undefined.
    
    ByteYCbCr422BT709ValueC(ByteT cbr,ByteT xy)
      : YCbCr422BT709ValueC<ByteT>(cbr,xy)
    {}
    //: Construct from components.
    
    ByteYCbCr422BT709ValueC(const YCbCr422BT709ValueC<UInt16T> &oth) {
      this->data[0] = oth.Cbr() >> 8;
      this->data[1] = oth.Y() >> 8;
    }
    //: Construct from another component type.
  };
  
  inline
  std::istream &operator>>(std::istream &strm,ByteYCbCr422BT709ValueC &val) { 
    int cbr,y;
    // Mess about so it works on SGI.
    strm >> cbr >> y;
    val.Cbr() = cbr;
    val.Y() = y;
    return strm;
  }
  //: Stream input.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const ByteYCbCr422BT709ValueC &val) 
  { return strm << ((int) val.Cbr()) << ' ' << ((int) val.Y()); }
  //: Stream output.
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteYCbCr422BT709ValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteYCbCr422BT709ValueC> &img);  
  //: Load byte image from binary stream 

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteYCbCr422BT709ValueC> {
    typedef RavlImageN::YCbCr422BT709ValueC<UInt16T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YCbCr422BT709ValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YCbCr422BT709ValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}
#endif
