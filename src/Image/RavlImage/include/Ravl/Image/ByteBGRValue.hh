// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTEBGRVALUE_HEADER
#define RAVL_BYTEBGRVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteBGRValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/BGRValue.hh"

namespace RavlImageN {
  
  //: Byte BGR value class.
  
  class ByteBGRValueC
    : public BGRValueC<ByteT>
  {
  public:
    ByteBGRValueC()
    {}
    //: Default constructor.
    // creates an undefined BGR pixel.
    
    ByteBGRValueC(ByteT r,ByteT g,ByteT b)
      : BGRValueC<ByteT>(r,g,b)
    {}
    //: Construct from components.
    
    ByteBGRValueC(const BGRValueC<ByteT> &oth)
      : BGRValueC<ByteT>(oth)
    {}
    //: Default constructor.
    // creates an undefined BGR pixel.
    
    ByteT Y() const
    { return (ByteT)( ((int) data[0] + (int)data[1] + (int)data[2])/3); }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.
  };
  
}



namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteBGRValueC > {
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<ByteT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<ByteT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::BGRValueC<RavlN::NumericalTraitsC<ByteT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
