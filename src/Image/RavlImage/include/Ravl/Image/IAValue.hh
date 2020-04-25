// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIAVALUE_HEADER
#define RAVLIAVALUE_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/IAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.Grey Level"

#include "Ravl/TFVector.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //: Image & Alpha Pixel.
  
  template<class CompT>
  class IAValueC 
    : public TFVectorC<CompT,2>
  {
  public:
    IAValueC()
      {}
    //: Default constructor.
    // Creates an undefined value.
    
    IAValueC(const CompT &i,const CompT &a) {
      this->data[0] =i;
      this->data[1] =a;
    }
    //: Construct from component values.
    
    inline const CompT & Intensity() const
      { return this->data[0]; }
    // Returns the level of the intensity component.
    
    inline const CompT & Alpha() const
      { return this->data[1]; }
    // Returns the level of the alpha component.

    inline CompT & Intensity()
      { return this->data[0]; }
    // Returns the level of the intensity component.
    
    inline CompT & Alpha()
      { return this->data[1]; }
    // Returns the level of the alpha component.
    
  };
}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::IAValueC<PixelT> > {
    typedef RavlImageN::IAValueC<typename RavlN::NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::IAValueC<typename RavlN::NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::IAValueC<typename RavlN::NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}


#endif
