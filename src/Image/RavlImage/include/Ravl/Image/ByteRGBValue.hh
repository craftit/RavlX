// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTERGBVALUE_HEADER
#define RAVL_BYTERGBVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteRGBValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  using namespace RavlN;
  class UInt16RGBValueC;
  class RealRGBValueC;
  
  //: Byte RGB value class.
  
  class ByteRGBValueC
    : public RGBValueC<ByteT>
  {
  public:
    ByteRGBValueC()
    {}
    //: Default constructor.
    // creates an undefined RGB pixel.
    
    ByteRGBValueC(ByteT r,ByteT g, ByteT b)
      : RGBValueC<ByteT>(r,g,b)
    {}
    //: Construct from components.
    
    template<typename PixelT>
    ByteRGBValueC(const TFVectorC<PixelT,3> &oth)
      : RGBValueC<ByteT>(static_cast<ByteT>(oth[0]),
			 static_cast<ByteT>(oth[1]),
			 static_cast<ByteT>(oth[2]))
    {}
    //: Construct from another value type.
    
    ByteT Y() const
    { return (ByteT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.
    
    RGBValueC<RealT> operator*(RealT val) const {
      RGBValueC<RealT> ret;
      ret[0] = ((RealT)(*this)[0] * val);
      ret[1] = ((RealT)(*this)[1] * val);
      ret[2] = ((RealT)(*this)[2] * val);
      return ret;
    }
    //: Multiply by a real.
  };
  
  
  inline
  ByteRGBValueC Average(const ByteRGBValueC &d1,const ByteRGBValueC &d2,
			const ByteRGBValueC &d3,const ByteRGBValueC &d4) { 
    return ByteRGBValueC(((IntT) d1.Red() + (IntT) d2.Red() + (IntT) d3.Red() + (IntT) d4.Red())/4,
			 ((IntT) d1.Green() + (IntT) d2.Green() + (IntT) d3.Green() + (IntT) d4.Green())/4,
			 ((IntT) d1.Blue() + (IntT) d2.Blue() + (IntT) d3.Blue() + (IntT) d4.Blue())/4
			 );
  }
  //: Average of 4 values.
  
  inline
  ByteRGBValueC Average(const ByteRGBValueC &d1,const ByteRGBValueC &d2) { 
    return ByteRGBValueC(((IntT) d1.Red() + (IntT) d2.Red())/2,
			 ((IntT) d1.Green() + (IntT) d2.Green())/2,
			 ((IntT) d1.Blue() + (IntT) d2.Blue())/2
			 );
  }
  //: Average of 2 values.

  template<class DataT> class ImageC;
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<ByteRGBValueC> &img);
  //: Output image to a binary stream.
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<ByteRGBValueC> &img);
  //: Load image from a binary stream.
  
}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteRGBValueC> {
    typedef RavlImageN::UInt16RGBValueC AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RealRGBValueC  RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RGBValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
