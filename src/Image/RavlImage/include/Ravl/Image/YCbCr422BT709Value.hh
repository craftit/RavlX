// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_YCBCR422BT709VALUE_HEADER
#define RAVLIMAGE_YCBCR422BT709VALUE_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id: YCbCr422BT709Value.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YCbCr"
//! file="Ravl/Image/Base/YCbCr422BT709Value.hh"

#include "Ravl/TFVector.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::TFVectorC;
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
#endif
  
  //! userlevel=Normal
  //: YCbCr422 Pixel base class.
  // Even columns in an image contain the Cb component, odd columns contain Cr.
  
  template<class CompT>
  class YCbCr422BT709ValueC 
    : public TFVectorC<CompT,2>
  {
  public:
    YCbCr422BT709ValueC()
    {}
    //: Default constructor.
    // Contents of pixels are left undefined.
    
    YCbCr422BT709ValueC(const CompT &Cbr,const CompT &y)
    { this->data[0] =Cbr; this->data[1] = y; }
    //: Construct from components.
    
    inline const CompT &Cbr() const
    { return (*this)[0]; }
    //: Access b or r component.
    
    inline CompT &Cbr()
    { return (*this)[0]; }
    //: Access b or r component.
    
    inline const CompT &Y() const
    { return (*this)[1]; }
    //: Access Y component.
    
    inline CompT &Y()
    { return (*this)[1]; }
    //: Access Y component.
    
  };

  
  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,YCbCr422BT709ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,2> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const YCbCr422BT709ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,2> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,YCbCr422BT709ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,2> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const YCbCr422BT709ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,2> &)(val)); }
  //: Binary stream output

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::YCbCr422BT709ValueC<PixelT> > {
    typedef RavlImageN::YCbCr422BT709ValueC<typename NumericalTraitsC<PixelT>::AccumT> AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YCbCr422BT709ValueC<typename NumericalTraitsC<PixelT>::RealAccumT> RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YCbCr422BT709ValueC<typename NumericalTraitsC<PixelT>::LongAccumT> LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
