// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLREALYUVVALUE_HEADER
#define RAVLREALYUVVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/RealYUVValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YUV"

#include "Ravl/Image/YUVValue.hh"

namespace RavlImageN {
  
  using namespace RavlN;

  class RealRGBValueC;
  
  //! userlevel=Normal
  //: Real YUV value class.
  
  class RealYUVValueC
    : public YUVValueC<RealT>
  {
  public:
    RealYUVValueC()
      {}
    //: Default constructor.
    // creates an undefined YUV pixel.
    
    RealYUVValueC(RealT y,RealT u, RealT v)
      : YUVValueC<RealT>(y,u,v)
      {}
    //: Construct from components.

    inline RealYUVValueC(const RealRGBValueC &v);
    //: Construct from a RGB pixel.
    
    RealYUVValueC(const YUVValueC<RealT> &oth)
      : YUVValueC<RealT>(oth)
      {}
    //: Default constructor.
    // creates an undefined YUV pixel.

    RealYUVValueC(const YUVValueC<float> &oth)
      : YUVValueC<RealT>(oth.Y(),oth.U(),oth.V())
      {}
    //: Default constructor.
    // creates an undefined YUV pixel.


  };

}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::RealYUVValueC > {
    typedef RavlImageN::RealYUVValueC AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RealYUVValueC RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RealYUVValueC LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
