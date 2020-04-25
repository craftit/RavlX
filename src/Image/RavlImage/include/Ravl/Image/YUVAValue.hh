// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_YUVAVALUE_HEADER
#define RAVL_YUVAVALUE_HEADER
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/YUVAValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.YUV"

#include "Ravl/TFVector.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::TFVectorC;
#endif
  
  //! userlevel=Normal
  //: YUVA Pixel base class.
  
  template<class CompT>
  class YUVAValueC 
    : public TFVectorC<CompT,4>
  {
  public:
    YUVAValueC()
      {}
    //: Default constructor.
    // Creates an undefined value.

    YUVAValueC(const CompT &y,const CompT &u,const CompT &v,const CompT &a) {
      this->data[0] =y;
      this->data[1] =u;
      this->data[2] =v;
      this->data[3] =a;
    }
    //: Construct from component values.
    
    YUVAValueC(const TFVectorC<CompT,4> &v)
      : TFVectorC<CompT,4>(v)
    {}
    //: Constructor from base class.
    
    template<class OCompT>
    YUVAValueC(YUVAValueC<OCompT> &oth) {
      this->data[0] = oth.Y();
      this->data[1] = oth.U();
      this->data[2] = oth.V();
    }
    //: Construct from another component type.
    
    inline const CompT & Y() const
    { return this->data[0]; }
    // Returns the level of the Y component.
    
    inline const CompT & U() const
    { return this->data[1]; }
    // Returns the level of the U component.
    
    inline const CompT & V() const
    { return this->data[2]; }
    // Returns the level of the V component.
    
    inline const CompT & A() const
    { return this->data[3]; }
    // Returns the level of the V component.
    
    inline CompT & Y() 
    { return this->data[0]; }
    // Returns the level of the Y component.
    
    inline CompT & U()
    { return this->data[1]; }
    // Returns the level of the U component.
    
    inline CompT & V()
    { return this->data[2]; }
    // Returns the level of the V component.

    inline CompT & A()
    { return this->data[3]; }
    // Returns the level of the V component.

  };

  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,YUVAValueC<CompT> &val) 
    { return strm >> ((TFVectorC<CompT,4> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const YUVAValueC<CompT> &val) 
    { return strm << ((const TFVectorC<CompT,4> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,YUVAValueC<CompT> &val) 
    { return strm >> ((TFVectorC<CompT,4> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const YUVAValueC<CompT> &val) 
    { return strm << ((const TFVectorC<CompT,4> &)(val)); }
  //: Binary stream output

}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::YUVAValueC<PixelT> > {
    typedef RavlImageN::YUVAValueC<typename RavlN::NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YUVAValueC<typename RavlN::NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YUVAValueC<typename RavlN::NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
