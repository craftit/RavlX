// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGEL_HEADER
#define RAVLIMAGE_EDGEL_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Edges"
//! author="Charles Galambos"
//! date="23/09/1999"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/Edgel.hh"

#include "Ravl/Index2d.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Math.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::ATan2;
  using RavlN::Sqr;
  using RavlN::Sqrt;
  using RavlN::BinIStreamC;
  using RavlN::BinOStreamC;
#endif

  //! userlevel=Normal
  //: Edge Location, Direction and Magnitude information.
  
  class EdgelC 
  {
  public:
    EdgelC(Index2dC loc,RealT dir,RealT mag)
      : at(loc),
	direction(dir),
	magnitude(mag)
    {}
    //: Constructor.
    // loc = Location of edge <br>
    // dir = Direction of edge in radians. <br>
    // mag = Magnitude of edge.
    
    EdgelC(const Index2dC &loc,RealT dx,RealT dy,RealT mag) 
      : at(loc),
	magnitude(mag)
    {
      direction = ATan2(dx,dy) + RavlConstN::pi/2;
      if(direction >= RavlConstN::pi)
	direction -= RavlConstN::pi*2;
    }
    //: Construct from dx,dy values.
    // loc = Location of edge <br>
    // dx,dy = Direction of edge. <br>
    // mag = Magnitude of edge.
    
    EdgelC(RealT dx,RealT dy,Index2dC loc) 
      : at(loc),
	magnitude(Sqrt(Sqr(dx) + Sqr(dy)))
    {
      direction = ATan2(dx,dy) + RavlConstN::pi/2;
      if(direction > RavlConstN::pi)
	direction -= RavlConstN::pi*2;
      
    }
    //: Construct from dx,dy values.
    // loc = Location of edge <br>
    // dx,dy = Direction of edge. <br>
    
    EdgelC()
    {}
    //: Default constructor.
    // The contents of the edgel are left undefined.
    
    Index2dC &At()
    { return at; }
    //: Location of edge.
    
    RealT &Direction()
    { return direction; }
    //: Edge direction in radians
    
    RealT &Magnitude()
    { return magnitude; }
    //: Magnitude
    
    const Index2dC &At() const
    { return at; }
    //: Location of edge.
    
    RealT Direction() const
    { return direction; }
    //: Edge direction in Radians
    
    RealT Magnitude() const
    { return magnitude; }
    //: Magnitude
    
  protected:
    Index2dC at;
    RealT direction;
    RealT magnitude;
  };

  //! userlevel=Normal
  
  ostream &operator<<(ostream &s,const EdgelC &edgel);
  //: Write to a stream.
  
  istream &operator>>(istream &s,EdgelC &edgel);
  //: Read from a stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const EdgelC &edgel);
  //: Write to a stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,EdgelC &edgel);
  //: Read from a stream.
  
    
}
#endif
