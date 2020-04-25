// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTERGBMEDIAN_HEADER
#define RAVL_BYTERGBMEDIAN_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ByteRGBMedian.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/01/2001"
//! docentry="Ravl.API.Images.Pixel Types.RGB"

#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Collection.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //: RGB + grey value class
  // It stores the Y component of an RGB pixel along with the R, G and B components, to avoid repeated evaluation of Y

  class ByteRGBGreyValueC
    : public ByteRGBValueC
  {
  public:
    ByteRGBGreyValueC()
    {}
    //: Default constructor.
    // creates an undefined RGB + grey value pixel.

    ByteRGBGreyValueC(ByteT r,ByteT g,ByteT b)
      : ByteRGBValueC(r,g,b)
    { grey = Y(); }
    //: Constructor
    // Builds an RGB + grey pixel from RGB components
    
    ByteRGBGreyValueC(const ByteRGBValueC &pix)
      : ByteRGBValueC(pix)
    { grey = Y(); }
    //: Constructor
    // Builds an RGB + grey pixel from an RGB pixel.

    ByteRGBGreyValueC(const RavlN::TFVectorC<RealT, 3> &value)
      : ByteRGBValueC((ByteT) value[0],(ByteT) value[1],(ByteT) value[2])
    { grey = Y(); }
    //: Constructor.
    
    ByteT Grey() const
    { return grey; }
    // Grey-level value for pixel

    bool operator<(const ByteRGBGreyValueC &v) 
    { return(Grey() < v.Grey()); }
    //: Comparison operator

    bool operator>(const ByteRGBGreyValueC &v) 
    { return(Grey() > v.Grey()); }
    //: Comparison operator

  private:
    ByteT grey;
  };

  //: RGB median value class.
  // All of the values needed to compute the median are stored, to facilitate fast updating of the median as new data is added
  class ByteRGBMedianC
    : public ByteRGBValueC
  {
  public:
    ByteRGBMedianC()
      : ByteRGBValueC(0,0,0),
	pixels(10)
    {} 
    //: Default constructor.
    // creates an undefined median pixel, with RGB value arbitrarily set to black.

    ByteRGBMedianC(ByteRGBValueC &pix)
      : ByteRGBValueC(pix),
	pixels(10)
    {}
    //: Construct from RGB components
    
    ByteRGBMedianC(const RavlN::TFVectorC<RealT, 3> &value)
      : ByteRGBValueC((ByteT) value[0],(ByteT) value[1],(ByteT) value[2]),
	pixels(10)
    {}
    //: Constructor.
    
    void Combine(const ByteRGBValueC &pix) {
      Add(pix) ; // add pixel 
      Update() ; // update median 
    }

    //: Mix operator
    // Adds a new pixel to the collection and finds the new median value

    ByteRGBMedianC& operator+=(const ByteRGBValueC &pix) {
      Combine(pix);
      return *this;
    }
    //: Mix operator
    // Adds a new pixel to the collection and finds the new median value

    void Add ( const ByteRGBValueC & pix ) {
      ByteRGBGreyValueC rgb_grey(pix) ; 
      pixels.Insert(rgb_grey) ; 
    }

    //: Adds another value to the collection but does NOT update the median. 
    // returns true on success 

    void Update (void) { 
      if(pixels.Size() < 1) // Check if we have any info at all.
        return ;
      ByteRGBGreyValueC rgb_grey = pixels.KthHighest(pixels.Size()/2) ; 
      Set( rgb_grey.Red(), rgb_grey.Green(), rgb_grey.Blue() ) ; 
    }
    //: Computes the median and updates the median value. 
    
  public:
    CollectionC<ByteRGBGreyValueC> pixels;
  };

}

#endif
