// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LINE2DITER_HEADER
#define RAVL_LINE2DITER_HEADER    1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Geometry.2D"
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/2D/Line2dIter.hh"

#include "Ravl/Point2d.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/LinePP2d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through integer grid points along a 2d line.
  // Uses a version of the midpoint algorithm to iterate all
  // 8-connected grid points on a line between two positions.
  
  class Line2dIterC {
  public:
    Line2dIterC(const Index2dC &start,const Index2dC &end);
    //: Constructor.

    Line2dIterC(const LinePP2dC &line)
    { *this = Line2dIterC(line.P1(), line.P2()); }
    //: Constructor.
    
    void First(const Index2dC &start,const Index2dC &end);
    //: Start line again.
    
    bool IsElm() const
    { return isElm; }
    //: Still in line.
    
    operator bool() const
    { return isElm; }
    //: At a pixel in line ?
    
    bool Next();
    //: Goto next point on line.
    
    void operator++(int)
    { Next(); }
    //: Goto next point on line.
    
    Index2dC Data() const
    { return Index2dC(x,y); }
    //: Access position.
    
    Index2dC operator*() const
    { return Index2dC(x,y); }
    //: Access position.
    
  protected:
    int dx,dy;
    int d;
    int incrE,incrNE;
    int x,y;
    int xe,ye;
    int xc,yc;
    bool isElm;
  };
}


#endif
