// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_BGRVALUE_HEADER
#define RAVLIMAGE_BGRVALUE_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Pixel Types.RGB"
//! userlevel=Normal
//! lib=RavlImage
//! file="Ravl/Image/Base/BGRValue.hh"

#include "Ravl/Image/RGBValue.hh"

namespace RavlImageN {
  
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::TFVectorC;
#endif


  //: BGR Pixel.
  
  template<class CompT>
  class BGRValueC
    : public TFVectorC<CompT,3>
  {
  public:
    // Constructors, copies, assignment, setting, and destructor
    // --------------------------------------------------------
    
    inline BGRValueC(CompT r = 0,
		     CompT g = 0,
		     CompT b = 0) {
      this->data[2] = r;
      this->data[1] = g;
      this->data[0] = b;
    }
    //: Constructs a colour from three colour components. 
    // Note: the arguments are given in the order r,g,b.
    
    BGRValueC(const RGBValueC<CompT> & rgb) {
      this->data[2] = rgb.Red();
      this->data[1] = rgb.Green();
      this->data[0] = rgb.Blue();
    }
    //: Constructs the colour from the triple RGB colour 'rgb'
    
    inline
    RGBValueC<CompT> RGB() const
    { return RGBValueC<CompT>(Red(),Green(),Blue()); }
    //: Get an RGB pixel value.
    
    inline const CompT & Red() const
    { return this->data[2]; }
    // Returns the level of the red component.
    
    inline const CompT & Green() const
    { return this->data[1]; }
    // Returns the level of the green component.
    
    inline const CompT & Blue() const
    { return this->data[0]; }
    // Returns the level of the blue component.
    
    inline CompT & Red()
    { return this->data[2]; }
    // Access to the red component.
    
    inline CompT & Green()
    { return this->data[1]; }
    // Access to the green component.
    
    inline CompT & Blue()
    { return this->data[0]; }
    // Access to the blue component.
    
  };
  
}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::BGRValueC<PixelT> > {
    typedef RavlImageN::BGRValueC<typename RavlN::NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::BGRValueC<typename RavlN::NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::BGRValueC<typename RavlN::NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
