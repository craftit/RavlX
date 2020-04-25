// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FIXED_POINT_HSV_VALUE_HEADER
#define RAVL_FIXED_POINT_HSV_VALUE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImage
//! docentry="Ravl.API.Images.Pixel Types.HSV"
//! author="Joel Mitchelson"
//! file="Ravl/Image/Base/FixedPointHSVValue.hh"

#include "Ravl/Stream.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"

namespace RavlImageN {
  
  using namespace RavlN ;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::UInt16T;
  using RavlN::ByteT;
#endif

  //! userlevel=Normal
  //: Fixed point representation of HSV colour values
  //: Value ranges:
  //: H [0-3072]  (hue)
  //: S [0-255]   (saturation)
  //: V [0-255]   (brightness value)
  
  class FixedPointHSVValueC
  {
  public: 
    
    FixedPointHSVValueC()
    {
    } 
    //: Default constructor - uninitialised values
    
    FixedPointHSVValueC(UInt16T nh, ByteT ns, ByteT nv) :
      h(nh), s(ns), v(nv)
    {
    }
    //: Construct from HSV component values  
    
    FixedPointHSVValueC(const ByteRGBValueC & rgb)
    { SetRGB(rgb); }
    //: Construct from RGB values 
    
  public:

    inline void SetRGB(const ByteRGBValueC& rgb);
    //: Set RGB values
    
    // inline GetRGB(ByteRGBValueC& rgb);
    // ! not implemented

    const UInt16T & H() const 
    { return h; }
    //: Hue (const)

    const ByteT & S() const 
    { return s; }
    //: Saturation (const)
    
    const ByteT & V() const 
    { return v ; }
    //: Brightness value (const)

    UInt16T & H()
    { return h; }
    //: Hue

    ByteT & S()
    { return s; }
    //: Saturation
    
    ByteT & V()
    { return v ; }
    //: Brightness value

  protected:
    UInt16T h;
    ByteT s;
    ByteT v;

  };

  inline void FixedPointHSVValueC::SetRGB(const ByteRGBValueC & rgb)
  {
    int r = rgb[0];
    int g = rgb[1];
    int b = rgb[2];

    if (r == g && g == b)
    {
      h = 0;
      s = 0;
      v = r;
    }
    else if (r > g)
    {
      if (g > b)
      {
	// rgb
	h = ((g-b)<<10) / (r+g-(b<<1));
	s = (255*(r-b))/r;
	v = r;
      }
      else if (r > b)
      { 
	// rbg
	h = 2048 + ((r-g)<<10) / (r+b-(g<<1));
	s = (255*(r-g))/r;
	v = r;
      }
      else
      {
	// brg
	h = 2048 + ((r-g)<<10) / (r+b-(g<<1));
	s = (255*(b-g))/b;
	v = b;
      }
    }
    else if (r > b)
    {
      // grb
      h = ((g-b)<<10) / (r+g-(b<<1));
      s = (255*(g-b))/g;
      v = g;
    }
    else if (b > g)
    {
      // bgr
      h = 1024 + ((b-r)<<10) / (b+g-(r<<1));
      s = (255*(b-r))/b;
      v = b;
    }
    else
    {
      // gbr
      h = 1024 + ((b-r)<<10) / (b+g-(r<<1));
      s = (255*(g-r))/g;
      v = g;
    }    

  }
 
};

#endif
