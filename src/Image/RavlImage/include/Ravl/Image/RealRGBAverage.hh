// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALRGBAVERAGE_HEADER
#define RAVL_REALRGBAVERAGE_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/RealRGBAverage.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //: RGB average value class.
  class RealRGBAverageC
    : public RealRGBValueC
  {
  public:
    RealRGBAverageC()
      : RealRGBValueC(0,0,0),
	count(0)
    {}
    //: Default constructor.
    // creates a zero average pixel.

    RealRGBAverageC(RealRGBValueC &naverage, RealT ncount)
      : RealRGBValueC(naverage),
	count(ncount)
    {}
    //: Construct from components

    void Combine(const ByteRGBValueC &pix)
    {
      RealT newcount=count+1.0;
      RealT div=count/newcount;
      Red()   = div*Red()   + (RealT)pix.Red()/newcount;
      Green() = div*Green() + (RealT)pix.Green()/newcount;
      Blue()  = div*Blue()  + (RealT)pix.Blue()/newcount;
      count = newcount;
    }
  public:
    RealT count;
  };
}

#endif
