// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRGBVALUE_HEADER
#define RAVL_REALRGBVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/RealRGBValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  
  class RealYUVValueC;
  
  //: Real RGB value class.
  
  class RealRGBValueC
    : public RGBValueC<RealT>
  {
  public:
    RealRGBValueC()
    {}
    //: Default constructor.
    // creates an undefined RGB pixel.

    inline RealRGBValueC(const RealYUVValueC &);
    //: Convert from a YUV pixel.
    // Implementation in Ravl/Image/RGBcYUV.hh include
    // this header when needed.
    
    RealRGBValueC(RealT r,RealT g, RealT b)
      : RGBValueC<RealT>(r,g,b)
    {}
    //: Construct from components.
    
    RealRGBValueC(const RGBValueC<float> &oth)
      : RGBValueC<RealT>(oth)
    {}
    //: Construct from float values.
    
    RealRGBValueC(const RGBValueC<ByteT> &oth)
      : RGBValueC<RealT>(oth.Red(),oth.Green(),oth.Blue())
    {}
    //: Construct from a byte RGB value.
    
    RealRGBValueC(const TFVectorC<RealT,3> &oth)
      : RGBValueC<RealT>(oth)
    {}
    //: Base class constructor.
    
    
  };

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::RealRGBValueC> {
    typedef RavlImageN::RealRGBValueC AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RealRGBValueC RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RealRGBValueC LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
