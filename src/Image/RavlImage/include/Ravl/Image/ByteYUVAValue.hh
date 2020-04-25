// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEYUVAVALUE_HEADER
#define RAVL_BYTEYUVAVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteYUVAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YUV"

#include "Ravl/Image/YUVAValue.hh"

namespace RavlImageN {
  
  //: Byte YUVA value class.
  
  class ByteYUVAValueC
    : public YUVAValueC<ByteT>
  {
  public:
    ByteYUVAValueC()
      {}
    //: Default constructor.
    // creates an undefined YUVA pixel.
    
    ByteYUVAValueC(ByteT y,ByteT u, ByteT v,ByteT a)
      : YUVAValueC<ByteT>(y,u,v,a)
      {}
    //: Construct from components.
    
    template<typename PixelT>
    ByteYUVAValueC(const YUVAValueC<PixelT> &oth)
      : YUVAValueC<ByteT>(oth)
    {}
    //: Copy constructor.
    
  };
  
  template<class DataT> class ImageC;
  
  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteYUVAValueC> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteYUVAValueC> &img);  
  //: Load byte image from binary stream 

}

namespace RavlN {
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteYUVAValueC> {
    typedef RavlImageN::YUVAValueC<UInt16T> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YUVAValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YUVAValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
