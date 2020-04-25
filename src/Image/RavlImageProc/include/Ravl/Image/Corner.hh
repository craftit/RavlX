// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CORNER_HEADER
#define RAVLIMAGE_CORNER_HEADER 1
///////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! userlevel=Normal
//! date="18/12/1995"
//! docentry="Ravl.API.Images.Corner Detection"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! example=exCorner.cc
//! file="Ravl/Image/Processing/Corners/Corner.hh"

#include "Ravl/Index2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Math.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::Point2dC;
  using RavlN::RealT;
  using RavlN::ByteT;
  using RavlN::Vector2dC;
  using RavlN::Abs;
#endif

  //! userlevel=Normal
  //: Corner descriptor.
  
  class CornerC {
  public:
    CornerC() 
    {}
    //: Default constructor.
    // Contents of class are undefined.
    
    CornerC(const Point2dC &location,RealT ndV,RealT ndH,ByteT nlevel) 
      : loc(location),
	grad(ndV,ndH),
	level(nlevel)
    {}
    //: Constructor.
    
    CornerC(const Point2dC &location,const Vector2dC &ngrad,ByteT nlevel) 
      : loc(location),
	grad(ngrad),
	level(nlevel)
    {}
    //: Constructor.
    
    Point2dC &Location() 
    { return loc; }
    //: Get location of corner.
    
    const Point2dC &Location() const
    { return loc; }
    //: Get location of corner.
    
    Vector2dC &Gradient() 
    { return grad; }
    // Get gradient.
    
    const Vector2dC &Gradient() const 
    { return grad; }
    // Get gradient.
    
    RealT &DVert() { return grad[0]; }
    // Vertical component of gradient.
    
    RealT &DHor() { return grad[1]; }
    // Horizontal component of gradient.
    
    ByteT &Level() { return level; }
    // Grey level of pixel.

    const ByteT &Level() const 
    { return level; }
    // Grey level of pixel.
    
    RealT Distance(const Point2dC &oth) 
    { return loc.CityBlockDistance(oth); }
    //: City block distance from another pixel.
    
    inline RealT Distance(const CornerC &Oth) const;
    // A somewhat arbitrary distance measure between two corners.
    // Suggestions for a better measure are welcome.
    
  private:
    Point2dC loc;       // Location of corner.
    Vector2dC grad;     // gradient of point.
    ByteT level;        // Intensity of point.
  };
  
  ostream &operator<<(ostream &out,const CornerC &corn);
  //: Write corner to a stream.
  
  istream &operator>>(istream &in,CornerC &corn);
  //: Read corner from a stream.
  
  BinOStreamC &operator<<(BinOStreamC &out,const CornerC &corn);
  //: Write corner to a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &in,CornerC &corn);
  //: Read corner from a binary stream.
  
  //////////////////////////////////////
  // A somewhat arbitrary distance measure between two corners.

  inline RealT CornerC::Distance(const CornerC &oth) const {
    return loc.CityBlockDistance(oth.loc) + 
      grad.CityBlockDistance(oth.grad) + 
      Abs(level - oth.level);
  }

}


#endif
