// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLBYTEIAVALUE_HEADER
#define RAVLBYTEIAVALUE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteIAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.Grey Level"

#include "Ravl/Image/IAValue.hh"

namespace RavlImageN {
  
  //: Byte Intensity and Alpha value class.
  
  class ByteIAValueC
    : public IAValueC<ByteT>
  {
  public:
    ByteIAValueC()
      {}
    //: Default constructor.
    // creates an undefined IA pixel.

    ByteIAValueC(const IAValueC<ByteT> &oth)
      : IAValueC<ByteT>(oth)
      {}
    //: Default constructor.
    // creates an undefined IA pixel.
    
  };
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::ByteIAValueC> {
    typedef RavlImageN::IAValueC<RavlN::NumericalTraitsC<ByteT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::IAValueC<RavlN::NumericalTraitsC<ByteT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::IAValueC<RavlN::NumericalTraitsC<ByteT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
