// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_YPBPRBT601VALUE_HEADER
#define RAVL_YPBPRBT601VALUE_HEADER
////////////////////////////////////////////////////////////////////
//! file="Ravl/Image/Base/YPbPrValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos, Bill Christmas"
//! docentry="Ravl.API.Images.Pixel Types.YPbPr"

#include "Ravl/TFVector.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::TFVectorC;
#endif
  
  template<class PixelT> class RGBValueC;
  template<typename CompT> class YPbPrBT601ValueC;
  
  void RGBFloat2YPbPrBT601Float(const RGBValueC<float> &value,YPbPrBT601ValueC<float> &outValue);
  //: Convert a pixel type from RGB float to YPbPrBT601 float
 
  void YPbPrBT601Float2RGBFloat(const YPbPrBT601ValueC<float> &value,RGBValueC<float> &outValue);
  //: Convert a pixel type from YPbPrBT601 float to RGB float
  
  //:-

  //! userlevel=Normal
  //: YPbPr Pixel base class.
  // Note CompT must be a signed value, its assumed that in practice it will be either float or double.
  // http://en.wikipedia.org/wiki/YPbPr
  
  template<typename CompT>
  class YPbPrBT601ValueC 
    : public TFVectorC<CompT,3>
  {
  public:
    YPbPrBT601ValueC()
    {}
    //: Default constructor.
    // Creates an undefined value.
    
    YPbPrBT601ValueC(const CompT &y,const CompT &Pb,const CompT &Pr) {
      this->data[0] = y;
      this->data[1] = Pb;
      this->data[2] = Pr;
    }
    //: Construct from component values.
    
    YPbPrBT601ValueC(const TFVectorC<CompT,3> &v)
      : TFVectorC<CompT,3>(v)
    {}
    //: Constructor from base class.
    
    template<class OCompT>
    YPbPrBT601ValueC(const YPbPrBT601ValueC<OCompT> &oth) {
      this->data[0] = oth.Y();
      this->data[1] = oth.Pb();
      this->data[2] = oth.Pr();
    }
    //: Construct from another component type.

    void Set(const CompT &y,const CompT &pb,const CompT &pr) {
      this->data[0] = y;
      this->data[1] = pb;
      this->data[2] = pr;
    }
    //: Set the values.
    
    void LimitYPbPr(const CompT &minY,const CompT &maxY,const CompT &minBR,const CompT &maxBR) {
      if(this->data[0] < minY)
        this->data[0] = minY;
      if(this->data[0] > maxY)
        this->data[0] = maxY;
      
      if(this->data[1] < minBR)
        this->data[1] = minBR;
      if(this->data[1] > maxBR)
        this->data[1] = maxBR;
      
      if(this->data[2] < minBR)
        this->data[2] = minBR;
      if(this->data[2] > maxBR)
        this->data[2] = maxBR;
    }
    //: Limit colour values.
    
    inline const CompT & Y() const
    { return this->data[0]; }
    //: Returns the level of the Y component.
    
    inline const CompT & Pb() const
    { return this->data[1]; }
    //: Returns the level of the U component.
    
    inline const CompT & Pr() const
    { return this->data[2]; }
    //: Returns the level of the V component.
    
    inline CompT & Y() 
    { return this->data[0]; }
    //: Returns the level of the Y component.
    
    inline CompT & Pb()
    { return this->data[1]; }
    //: Returns the level of the Cb component.
    
    inline CompT & Pr()
    { return this->data[2]; }
    //: Returns the level of the Cr component.

  };

  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,YPbPrBT601ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const YPbPrBT601ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,YPbPrBT601ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const YPbPrBT601ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Binary stream output

  
  template<class CompT>
  inline
  YPbPrBT601ValueC<CompT> Average(const YPbPrBT601ValueC<CompT> &d1,const YPbPrBT601ValueC<CompT> &d2,
                             const YPbPrBT601ValueC<CompT> &d3,const YPbPrBT601ValueC<CompT> &d4) { 
    typedef typename NumericalTraitsC<CompT>::AccumT AccumT;
    return YPbPrBT601ValueC<CompT>(((AccumT) d1.Y() + (AccumT) d2.Y() + (AccumT) d3.Y() + (AccumT) d4.Y())/4,
                              ((AccumT) d1.Pb() + (AccumT) d2.Pb() + (AccumT) d3.Pb() + (AccumT) d4.Pb())/4,
                              ((AccumT) d1.Pr() + (AccumT) d2.Pr() + (AccumT) d3.Pr() + (AccumT) d4.Pr())/4
                              );
  }
  //: Average of 4 values.
  
  template<class CompT>
  inline
  YPbPrBT601ValueC<CompT> Average(const YPbPrBT601ValueC<CompT> &d1,const YPbPrBT601ValueC<CompT> &d2) { 
    typedef typename NumericalTraitsC<CompT>::AccumT AccumT;
    return YPbPrBT601ValueC<CompT>(((AccumT) d1.Y() + (AccumT) d2.Y())/2,
                              ((AccumT) d1.Pb() + (AccumT) d2.Pb())/2,
                              ((AccumT) d1.Pr() + (AccumT) d2.Pr())/2
                              );
  }
  //: Average of 2 values.
  
}


namespace RavlN {
  

  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::YPbPrBT601ValueC<PixelT> > {
    typedef RavlImageN::YPbPrBT601ValueC<typename NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YPbPrBT601ValueC<typename NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YPbPrBT601ValueC<typename NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
