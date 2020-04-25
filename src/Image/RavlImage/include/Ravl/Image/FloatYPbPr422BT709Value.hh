// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEYPBPR422BT709VALUE_HEADER
#define RAVL_BYTEYPBPR422BT709VALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id: ByteYUV422Value.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YPbPr"

#include "Ravl/Image/YPbPr422BT709Value.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  //! userlevel=Normal
  //: 16-bit YPbPr 422 value class.
  
  class FloatYPbPr422BT709ValueC
    : public YPbPr422BT709ValueC<FloatT>
  {
  public:
    FloatYPbPr422BT709ValueC()
    {}
    //: Default constructor.
    // The value is left undefined.
    
    FloatYPbPr422BT709ValueC(FloatT xbr,FloatT xy)
      : YPbPr422BT709ValueC<FloatT>(xbr,xy)
    {}
    //: Construct from components.
    
    FloatYPbPr422BT709ValueC(const YPbPr422BT709ValueC<FloatT> &oth) {
      this->data[0] = oth.Pbr();  //<< 8; dont think i need this any longer as b4 we had 10 bits in a 16 bit container hence it was left shifted but this dosent apply to floats ??? 
      this->data[1] = oth.Y();   // << 8;
    }
    //: Construct from byte component type.
    
  };
  
  inline
  std::istream &operator>>(std::istream &strm,FloatYPbPr422BT709ValueC &val) { 
    int pbr,y;
    // Mess about so it works on SGI.
    strm >> pbr >> y;
    val.Pbr() = pbr;
    val.Y() = y;
    return strm;
  }
  //: Stream input.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const FloatYPbPr422BT709ValueC &val) 
  { return strm << ((FloatT) val.Pbr()) << ' ' << ((FloatT) val.Y()); }
  //: Stream output.
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<FloatYPbPr422BT709ValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<FloatYPbPr422BT709ValueC> &img);  
  //: Load byte image from binary stream 

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::FloatYPbPr422BT709ValueC> {
    typedef RavlImageN::YPbPr422BT709ValueC<UInt16T> AccumT;    //: UInt16T Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YPbPr422BT709ValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YPbPr422BT709ValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}
#endif
