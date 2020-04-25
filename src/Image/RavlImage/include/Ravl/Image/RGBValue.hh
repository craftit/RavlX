// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RGBVALUE_HEADER
#define RAVL_RGBVALUE_HEADER
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/RGBValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/TFVector.hh"
#include "Ravl/Traits.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::TFVectorC;
  using RavlN::ByteT;
#endif

  
  //: RGB Pixel base class.
  
  template<class CompT>
  class RGBValueC 
    : public TFVectorC<CompT,3>
  {
  public:
    RGBValueC()
      {}
    //: Default constructor.
    // Creates an undefined value.

    RGBValueC(const CompT &r,const CompT &g,const CompT &b) {
      this->data[0] =r;
      this->data[1] =g;
      this->data[2] =b;
    }
    //: Construct from component values.
    
    template<class OCompT>
    RGBValueC(const RGBValueC<OCompT> &oth) {
      this->data[0] = (CompT) oth.Red();
      this->data[1] = (CompT) oth.Green();
      this->data[2] = (CompT) oth.Blue();
    }
    //: Construct from another component type.
    
    RGBValueC(const TFVectorC<CompT,3> &v)
      : TFVectorC<CompT,3>(v)
      {}
    //: Constructor from base class.
    
    void Set(const CompT &r,const CompT &g,const CompT &b) {
      this->data[0] =r;
      this->data[1] =g;
      this->data[2] =b;
    }
    //: Set the values.
    
    inline const CompT & Red() const
      { return this->data[0]; }
    //: Returns the level of the red component.
    
    inline const CompT & Green() const
      { return this->data[1]; }
    //: Returns the level of the green component.
    
    inline const CompT & Blue() const
      { return this->data[2]; }
    //: Returns the level of the blue component.
    
    inline CompT & Red() 
      { return this->data[0]; }
    //: Returns the level of the red component.
    
    inline CompT & Green()
      { return this->data[1]; }
    //: Returns the level of the green component.
    
    inline CompT & Blue()
      { return this->data[2]; }
    //: Returns the level of the blue component.
    
    inline CompT NTSC_Y()
      { return (CompT)((RealT) 0.299 * this->data[0] + 0.587 * this->data[1] + 0.114 * this->data[2]); }
    //: Get the pixel intensity of an NTSC colour system.
    // the NTSC RGB color system.
    
    CompT Y() const
      { return (this->data[0] + this->data[1] + this->data[2])/3; }
    //: Calculate intensity of the pixel.
    // This returns the average of the red, green
    // and blue components.

  };
  
  template<class CompT>
  inline
  std::istream &operator>>(std::istream &strm,RGBValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Stream input.
  
  template<class CompT>
  inline
  std::ostream &operator<<(std::ostream &strm,const RGBValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Stream output.
  
  template<class CompT>
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,RGBValueC<CompT> &val) 
  { return strm >> ((TFVectorC<CompT,3> &)(val)); }
  //: Binary stream input.
  
  template<class CompT>
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const RGBValueC<CompT> &val) 
  { return strm << ((const TFVectorC<CompT,3> &)(val)); }
  //: Binary stream output

  inline std::istream &operator>>(std::istream &strm,RGBValueC<ByteT> &val) { 
    int r,g,b;
    strm >> r >> g >> b;
    val.Set(r,g,b);
    return strm; 
  }
  //: Stream input.
  // This is to make sure bytes are handled as numeric values.
  
  inline std::ostream &operator<<(std::ostream &strm,const RGBValueC<ByteT> &val) 
  { return strm << ((int) val.Red()) << ' ' << ((int) val.Green()) << ' ' << ((int) val.Blue()); }
  //: Stream output.
  // This is to make sure bytes are handled as numeric values.
  
}

namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<typename PixelT>
  struct NumericalTraitsC<RavlImageN::RGBValueC<PixelT> > {
    typedef RavlImageN::RGBValueC<typename RavlN::NumericalTraitsC<PixelT>::AccumT > AccumT;    //: Type to use for accumulator, guarantee's at least 2x no bits for interger types.
    typedef RavlImageN::RGBValueC<typename RavlN::NumericalTraitsC<PixelT>::RealAccumT > RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RGBValueC<typename RavlN::NumericalTraitsC<PixelT>::LongAccumT > LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
