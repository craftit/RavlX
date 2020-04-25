// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CIRCLEITER_HEADER
#define RAVL_CIRCLEITER_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Sequence/CircleIter.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Sequences"
//! date="08/02/1999"

#include "Ravl/Types.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  //: Iterate around a circle.
  // NB. This does NOT move linearly around the circle,
  // but through octants.
  
  class CircleIterC {
  public:  
    inline CircleIterC(int nradius = 1,Index2dC nOffset = Index2dC(0,0))
      : octant(0),
	radius(nradius),
	offset(nOffset)
      { First(); }
    //: Constructor.
    
    void First();
    //: Goto first point on circle.
    
    inline bool IsElm() const { return (octant > -1); }
    //: At valid position ?

    operator bool() const
    { return IsElm(); }
    //: At a valid position ?
    
    inline const Index2dC &Data() const { return data; }
    //: Get point.
    // Largest error from radius should be less than 0.5
    
    const Index2dC &operator*() const
    { return data; }
    //: Get current point.
    
    bool Next();
    //: Goto next point.
    // Returns true if we're now at a valid point.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next point.
    // Returns true if we're now at a valid point.
  private:
    void Dummy();
    //: 
    int octant; // Current octant.
    int radius;
    int d,deltaE,deltaSE;
    int x,y;
    Index2dC offset;
    Index2dC data;
  };
}  

#endif
