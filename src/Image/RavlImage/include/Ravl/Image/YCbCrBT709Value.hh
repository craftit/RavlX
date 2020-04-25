// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_YCBCRBT709VALUE_HEADER
#define RAVL_YCBCRBT709VALUE_HEADER
////////////////////////////////////////////////////////////////////
//! file="Ravl/Image/Base/YCbCrValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="11/09/2008"
//! docentry="Ravl.API.Images.Pixel Types.YCbCr"

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
  template<typename CompT> class YCbCrBT709ValueC;
  
  void RGBFloat2YCbCrBT709Float(const RGBValueC<float> &value,YCbCrBT709ValueC<float> &outValue);
  //: Convert a pixel type from RGB float to YCbCrBT601 float
 
  void YCbCrBT709Float2RGBFloat(const YCbCrBT709ValueC<float> &value,RGBValueC<float> &outValue);
  //: Convert a pixel type from YCbCrBT601 float to RGB float
  
  //:-

  //! userlevel=Normal
  //: YCbCr Pixel base class.
  // Note CompT must be a signed value, its assumed that in practice it will be either float or double.
  // http://en.wikipedia.org/wiki/YCbCr
  
  template<typename CompT>
  class YCbCrBT709ValueC 
    : public TFVectorC<CompT,3>
  {
  public:
    YCbCrBT709ValueC()
    {}
    //: Default constructor.
    // Creates an undefined value.
    
    YCbCrBT709ValueC(const CompT &y,const CompT &Cb,const CompT &Cr) {
      this->data[0] = y;
      this->data[1] = Cb;
      this->data[2] = Cr;
    }
    //: Construct from component values.
    
    YCbCrBT709ValueC(const TFVectorC<CompT,3> &v)
      : TFVectorC<CompT,3>(v)
    {}
    //: Constructor from base class.
    
    template<class OCompT>
    YCbCrBT709ValueC(const YCbCrBT709ValueC<OCompT> &oth) {
      this->data[0] = oth.Y();
      this->data[1] = oth.Cb();
      this->data[2] = oth.Cr();
    }
    //: Construct from another component type.

    void Set(const CompT &y,const CompT &cb,const CompT &cr) {
      this->data[0] = y;
      this->data[1] = cb;
      this->data[2] = cr;
    }
    //: Set the values.
    
    void LimitYCbCr(const CompT &minY,const CompT &maxY,const CompT &minBR,const CompT &maxBR) {
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
    
    inline const CompT & Cb() const
    { return this->data[1]; }
    //: Returns the level of the U component.
    
    inline const CompT & Cr() const
    { return this->data[2]; }
    //: Returns the level of the V component.
    
    inline CompT & Y() 
    { return this->data[0]; }
    //: Returns the level of the Y component.
    
    inline CompT & Cb()
    { return this->data[1]; }
    //: Returns the level of the Cb component.
    
    inline CompT & Cr()
    { return this->data[2]; }
    //: Returns the level of the Cr component.

  };

  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,YCbCrBT709ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const YCbCrBT709ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,YCbCrBT709ValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const YCbCrBT709ValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Binary stream output

  
  template<class CompT>
  inline
  YCbCrBT709ValueC<CompT> Average(const YCbCrBT709ValueC<CompT> &d1,const YCbCrBT709ValueC<CompT> &d2,
                             const YCbCrBT709ValueC<CompT> &d3,const YCbCrBT709ValueC<CompT> &d4) { 
    typedef typename NumericalTraitsC<CompT>::AccumT AccumT;
    return YCbCrBT709ValueC<CompT>(((AccumT) d1.Y() + (AccumT) d2.Y() + (AccumT) d3.Y() + (AccumT) d4.Y())/4,
                              ((AccumT) d1.Cb() + (AccumT) d2.Cb() + (AccumT) d3.Cb() + (AccumT) d4.Cb())/4,
                              ((AccumT) d1.Cr() + (AccumT) d2.Cr() + (AccumT) d3.Cr() + (AccumT) d4.Cr())/4
                              );
  }
  //: Average of 4 values.
  
  template<class CompT>
  inline
  YCbCrBT709ValueC<CompT> Average(const YCbCrBT709ValueC<CompT> &d1,const YCbCrBT709ValueC<CompT> &d2) { 
    typedef typename NumericalTraitsC<CompT>::AccumT AccumT;
    return YCbCrBT709ValueC<CompT>(((AccumT) d1.Y() + (AccumT) d2.Y())/2,
                              ((AccumT) d1.Cb() + (AccumT) d2.Cb())/2,
                              ((AccumT) d1.Cr() + (AccumT) d2.Cr())/2
                              );
  }
  //: Average of 2 values.
  
}


namespace RavlN {
  

  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::YCbCrBT709ValueC<PixelT> > {
    typedef RavlImageN::YCbCrBT709ValueC<typename NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::YCbCrBT709ValueC<typename NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::YCbCrBT709ValueC<typename NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
