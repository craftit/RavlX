// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEYUV422VALUE_HEADER
#define RAVL_BYTEYUV422VALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteYUV422Value.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YCbCr"

#include "Ravl/Image/YUV422Value.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
#endif
  
  //! userlevel=Normal
  //: Byte YUV 422 value class.
  // Pixels even image columns contain U and odd columns V
  
  class ByteYUV422ValueC
    : public YUV422ValueC<ByteT>
  {
  public:
    ByteYUV422ValueC()
    {}
    //: Default constructor.
    // The value is left undefined.
    
    ByteYUV422ValueC(ByteT xuv,ByteT xy)
      : YUV422ValueC<ByteT>(xuv,xy)
    {}
    //: Construct from components.
    
    template<class OCompT>
    ByteYUV422ValueC(YUV422ValueC<OCompT> &oth) {
      this->data[0] = oth.UV();
      this->data[1] = oth.Y();
    }
    //: Construct from another component type.
  };
  
  inline
  std::istream &operator>>(std::istream &strm,ByteYUV422ValueC &val) { 
    int uv,y;
    // Mess about so it works on SGI.
    strm >> uv >> y;
    val.UV() = uv;
    val.Y() = y;
    return strm;
  }
  //: Stream input.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const ByteYUV422ValueC &val) 
  { return strm << ((int) val.UV()) << ' ' << ((int) val.Y()); }
  //: Stream output.
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteYUV422ValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteYUV422ValueC> &img);  
  //: Load byte image from binary stream 

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteYUV422ValueC> {
    typedef RavlImageN::YUV422ValueC<UInt16T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YUV422ValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YUV422ValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}
#endif
