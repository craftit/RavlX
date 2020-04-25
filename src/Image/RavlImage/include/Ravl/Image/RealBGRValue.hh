// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALBGRVALUE_HEADER
#define RAVL_REALBGRVALUE_HEADER 1
/////////////////////////////////////////////////////
//! lib=RavlImage
//! userlevel=Normal
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/BGRValue.hh"

namespace RavlImageN {
  
  //: Real BGR value class.
  
  class RealBGRValueC
    : public BGRValueC<RealT>
  {
  public:
    RealBGRValueC()
    {}
    //: Default constructor.
    // creates an undefined BGR pixel.
    
    RealBGRValueC(RealT r,RealT g,RealT b)
      : BGRValueC<RealT>(r,g,b)
    {}
    //: Construct from components.
    
    RealBGRValueC(const BGRValueC<RealT> &oth)
      : BGRValueC<RealT>(oth)
    {}
    //: Default constructor.
    // creates an undefined BGR pixel.
    
    RealT Y() const
    { return (RealT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.
  };
  
}



namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::RealBGRValueC > {
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<RealT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<RealT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<RealT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
