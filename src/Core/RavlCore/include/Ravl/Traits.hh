// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRAITS_HEADER
#define RAVL_TRAITS_HEADER 1
//! author="Charles Galambos, based on ideas from sigc++"
//! date="27/4/2003
//! docentry="Ravl.API.Core.Misc"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Traits.hh"

#include "Ravl/Types.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: Traits for type
  // Used for decomposing types into there components.
  // i.e. const, reference and base types.
  
  template<typename DataT>
  struct TraitsC {
  public:
    typedef DataT &RefT;     //: Non-const reference to type.
    typedef DataT TypeT;     //: Unmodified type.
    typedef DataT BaseTypeT; //: Base type ignoring const and reference.
  };

  //! userlevel=Advanced
  //: Traits for type
  // Used for decomposing types into there components.
  // i.e. const, reference and base types.

  template<typename DataT>
  struct TraitsC<const DataT> {
  public:
    typedef DataT &RefT;     //: Non-const reference to type.
    typedef const DataT TypeT;     //: Unmodified type.
    typedef DataT BaseTypeT; //: Base type ignoring const and reference.
  };
  
  //! userlevel=Advanced
  //: Traits for type
  // Used for decomposing types into there components.
  // i.e. const, reference and base types.
  
  template<typename DataT >
  struct TraitsC<DataT &> {
  public:
    typedef DataT &RefT;     //: Non-const reference to type.
    typedef DataT &TypeT;    //: Unmodified type.
    typedef DataT BaseTypeT; //: Base type ignoring const and reference.
  };

  //! userlevel=Advanced
  //: Traits for type
  // Used for decomposing types into there components.
  // i.e. const, reference and base types.
  
  template<typename DataT >
  struct TraitsC<const DataT &> {
  public:
    typedef DataT &RefT;        //: Non-const reference to type.
    typedef const DataT &TypeT; //: Unmodified type.
    typedef DataT BaseTypeT;    //: Base type ignoring const and reference.
  };

  //:--------------------------------------------------------------------------------------------

  //! userlevel=Advanced
  //: Traits for type
  
  template<typename DataT>
  struct NumericalTraitsC {
    typedef DataT  AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef DataT RealAccumT; //: Type to use for a floating point accumulator.
    typedef DataT LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<ByteT> {
    typedef IntT  AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RealT RealAccumT; //: Type to use for a floating point accumulator.
    typedef Int64T LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<Int16T> {
    typedef IntT   AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RealT  RealAccumT; //: Type to use for a floating point accumulator.
    typedef Int64T LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<IntT> {
    typedef Int64T AccumT;   //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RealT  RealAccumT; //: Type to use for a floating point accumulator.
    typedef Int64T LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RealT> {
    typedef RealT AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RealT RealAccumT; //: Type to use for a floating point accumulator.
    typedef RealT LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<FloatT> {
    typedef FloatT AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef FloatT RealAccumT; //: Type to use for a floating point accumulator.
    typedef RealT  LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };

}

#endif
