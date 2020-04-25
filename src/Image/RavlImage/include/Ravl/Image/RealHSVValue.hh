// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HSVVALUE_HEADER
#define RAVL_HSVVALUE_HEADER 1
////////////////////////////////////////////////////////////////////
//! file="Ravl/Image/Base/RealHSVValue.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Lee Gregory"
//! date="12/10/2002"
//! docentry="Ravl.API.Images.Pixel Types.HSV"
//! rcsid="$Id$"

#include "Ravl/TFVector.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"

//class RGBValueC ; 

namespace RavlImageN {
  
  using namespace RavlN ;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
  using RavlN::TFVectorC;
  using RavlN::ByteT;
#endif

  //! userlevel=Normal
  //: Real HSV pixel class
  // H: [0-360) where red = 0, green = 120, blue = 240<br>
  // S = 1 - min(R,G,B) / max(R,G,B) &rarr; S: [0-1] <br>
  // V takes on the range of the pixel type it was constructed from.<br>
  
  class RealHSVValueC : public TFVectorC<RealT,3>
  {
  public: 
    
    inline RealHSVValueC(void) {} 
    //: Default Constructor 
    // creates an undefined YUV pixel.
    
    inline RealHSVValueC(const RealT & h, const RealT & s, const RealT & v); 
    //: Constructor from component values  
    
    inline RealHSVValueC(const RealRGBValueC & rgb) ;  
    //: Constructor from an RGB Value 
    // This function expects the range of RGB values to be 0-1
    
    inline RealHSVValueC(const RealHSVValueC & hsv) 
      : TFVectorC<RealT,3> ( hsv ) {}
    //: Copy Constructor 
    
    inline void Set (const RealT & h, const RealT & s, const RealT & v) 
    { data[0] = h ; data[1] = s ; data[2] = v ; } 
    //: Set the value of the pixel. 

    inline RealRGBValueC  RealRGBValue (void) const;
    //: Convert this value to an RGB value

#if !RAVL_OS_WINDOWS
    inline RealRGBValueC  RGB (void) const
    { return RealRGBValue(); }
    //: Convert this value to an RGB value
    // This method is depreicated as it conflicts with a windows macro definition.
#endif

    inline const RealT & H() const 
    { return data[0] ; }
    //: Const access to the Hue value

    inline const RealT & S() const 
    { return data[1] ; }
    //: Const access to the Saturation value
    
    inline const RealT & V() const 
    { return data[2] ; }
    //: Const access to the 'value' (i.e. brightness)
    
   
    inline RealT & H() 
    { return data[0] ; }
    //: Access to the Hue value

    inline RealT & S() 
    { return data[1] ; }
    //: Access to the Saturation value
    
    inline RealT & V()
    { return data[2] ; }
    //: Access to the 'value' (i.e. brightness)

  };


  


  RealHSVValueC::RealHSVValueC(const RealT & h, const RealT & s, const RealT & v) 
    : TFVectorC<RealT,3> ()
  { 
    data[0] = h ; 
    data[1] = s ; 
    data[2] = v ;
  }
  //: Constructor from component values 

  
  RealHSVValueC::RealHSVValueC( const RealRGBValueC & comp ) 
    : TFVectorC<RealT,3> () 
  {
    RealT max = Max ( comp.Red(), comp.Green(), comp.Blue() ) ;
    RealT min = Min ( comp.Red(), comp.Green(), comp.Blue() ) ;
    RealT delta = max-min ;
    
    // value 
    data[2] = max ;         // v

    // saturation 
    if ( max != 0 ) 
      data[1] = delta/max ; // s
    else // i.e. black
      {
	data[1] = 0 ;       // s = undefined 
	data[0] = 0 ;       // h = undefined
	return ;
      }
    
    // hue 
    if (delta == 0.0)  // i.e. grey
      data[0] = 0 ;       // h = undefined
    else if( comp.Red() == max )
      data[0] = ( comp.Green() - comp.Blue() ) / delta;		// between yellow & magenta
    else if( comp.Green() == max )
      data[0] = 2 + ( comp.Blue() - comp.Red() ) / delta;	// between cyan & yellow
    else  //  comp.Blue() == max
      data[0] = 4 + ( comp.Red() - comp.Green() ) / delta;	// between magenta & cyan
    
    // convert to degrees 
    data[0] *= 60;			  
    if( data[0] < 0 )
      data[0] += 360;
  }
  // Constructor From RGB Value 



   inline RealRGBValueC  RealHSVValueC::RealRGBValue (void) const
   {
    RealRGBValueC rgb ;
    
    // achromatic case 
    if( data[1] == 0 ) {
      rgb.Red() = rgb.Green() = rgb.Blue() = data[2] ;
      return rgb;
    }
    

    RealT h = data[0] / 60 ; 
    IntT i = Floor(h) ; 
    RealT f = h - i;	 
    RealT p = data[2] * ( 1 - data[1] );
    RealT q = data[2] * ( 1 - data[1] * f );
    RealT t = data[2] * ( 1 - data[1] * ( 1 - f ) );

    switch( i ) {
    case 0:
      rgb.Red()    = data[2];
      rgb.Green()  = t;
      rgb.Blue()   = p;
      break;

    case 1:
      rgb.Red()    = q;
      rgb.Green()  = data[2];
      rgb.Blue()   = p;
      break;
    
    case 2:
      rgb.Red()    = p;
      rgb.Green()  = data[2];
      rgb.Blue()   = t;
      break;
    
    case 3:
      rgb.Red()    = p;
      rgb.Green()  = q;
      rgb.Blue()   = data[2];
      break;
    
    case 4:
      rgb.Red()    = t;
      rgb.Green()  = p;
      rgb.Blue()   = data[2];
      break;
    
    default:		// case 5:
      rgb.Red()    = data[2];
      rgb.Green()  = p;
      rgb.Blue()   = q;
      break;
	} 
    return rgb ; 
   } 
    //: Convert this value into RGB 
 
};


namespace RavlN {
  
  //! userlevel=Advanced
  //: Traits for type
  
  template<>
  struct NumericalTraitsC<RavlImageN::RealHSVValueC> {
    typedef RavlImageN::RealHSVValueC AccumT;    //: Type to use for accumulator, guarantees at least 2x no bits for integer types.
    typedef RavlImageN::RealHSVValueC RealAccumT; //: Type to use for a floating point accumulator.
    typedef RavlImageN::RealHSVValueC LongAccumT; //: Type to use for accumulators that can take large sums.(10000's of elements at least.)
  };
}

#endif
