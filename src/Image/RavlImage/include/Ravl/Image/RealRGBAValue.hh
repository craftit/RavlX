// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLREALRGBAVALUE_HEADER
#define RAVLREALRGBAVALUE_HEADER 1
/////////////////////////////////////////////////////
//! file="Ravl/Image/Base/RealRGBAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RGBAValue.hh"

namespace RavlImageN {
  
  //: Real RGBA value class.
  
  class RealRGBAValueC
    : public RGBAValueC<RealT>
  {
  public:
    RealRGBAValueC()
      {}
    //: Default constructor.
    // creates an undefined RGBA pixel.

    RealRGBAValueC(RealT r,RealT g,RealT b,RealT a)
      : RGBAValueC<RealT>(r,g,b,a)
      {}
    //: Construct from components.
    
    template<typename PixelT>
    RealRGBAValueC(const TFVectorC<PixelT,4> &oth)
      : RGBAValueC<RealT>(static_cast<RealT>(oth[0]),
			  static_cast<RealT>(oth[1]),
			  static_cast<RealT>(oth[2]),
                          static_cast<RealT>(oth[3]))
    {}
    //: Construct from another value type.
    
    RealRGBAValueC(const RGBAValueC<RealT> &oth)
      : RGBAValueC<RealT>(oth)
      {}
    //: Default constructor.
    // creates an undefined RGBA pixel.
    
    RealT Y() const
      { return (RealT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
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
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::RealRGBAValueC> {
    typedef RavlImageN::RGBAValueC<RealT> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RGBAValueC<RealT>   RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RGBAValueC<UInt64T> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
