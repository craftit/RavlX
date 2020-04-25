// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_YUVVALUE_HEADER
#define RAVL_YUVVALUE_HEADER
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/YUVValue.hh"
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
  //: YUV Pixel base class.
  
  template<class CompT>
  class YUVValueC 
    : public TFVectorC<CompT,3>
  {
  public:
    YUVValueC()
    {}
    //: Default constructor.
    // Creates an undefined value.

    YUVValueC(const CompT &y,const CompT &u,const CompT &v) {
      this->data[0] =y;
      this->data[1] =u;
      this->data[2] =v;
    }
    //: Construct from component values.
    
    YUVValueC(const TFVectorC<CompT,3> &v)
      : TFVectorC<CompT,3>(v)
    {}
    //: Constructor from base class.
    
    template<class OCompT>
    YUVValueC(YUVValueC<OCompT> &oth) {
      this->data[0] = oth.Y();
      this->data[1] = oth.U();
      this->data[2] = oth.V();
    }
    //: Construct from another component type.

    void Set(const CompT &y,const CompT &u,const CompT &v) {
      this->data[0] =y;
      this->data[1] =u;
      this->data[2] =v;
    }
    //: Set the values.
    
    void LimitYUV(const CompT &minY,const CompT &maxY,const CompT &minUV,const CompT &maxUV) {
      if(this->data[0] < minY)
        this->data[0] = minY;
      if(this->data[0] > maxY)
        this->data[0] = maxY;
      
      if(this->data[1] < minUV)
        this->data[1] = minUV;
      if(this->data[1] > maxUV)
        this->data[1] = maxUV;
      
      if(this->data[2] < minUV)
        this->data[2] = minUV;
      if(this->data[2] > maxUV)
        this->data[2] = maxUV;
    }
    //: Limit colour values.
    
    inline const CompT & Y() const
    { return this->data[0]; }
    //: Returns the level of the Y component.
    
    inline const CompT & U() const
    { return this->data[1]; }
    //: Returns the level of the U component.
    
    inline const CompT & V() const
    { return this->data[2]; }
    //: Returns the level of the V component.
    
    inline CompT & Y() 
    { return this->data[0]; }
    //: Returns the level of the Y component.
    
    inline CompT & U()
    { return this->data[1]; }
    //: Returns the level of the U component.
    
    inline CompT & V()
    { return this->data[2]; }
    //: Returns the level of the V component.

  };

  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,YUVValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const YUVValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,YUVValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const YUVValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Binary stream output

}


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::YUVValueC<PixelT> > {
    typedef RavlImageN::YUVValueC<typename NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YUVValueC<typename NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YUVValueC<typename NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
