// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Line2dIter.cc"

#include "Ravl/Line2dIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#include "Ravl/Stream.hh"
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor.
  
  Line2dIterC::Line2dIterC(const Index2dC &start,const Index2dC &end)
  { First(start,end); }
  

  //: Start line again.
  
  void Line2dIterC::First(const Index2dC &start,const Index2dC &end) {
    dx = (end.Row() - start.Row()).V();
    dy = (end.Col() - start.Col()).V();    
    if(dx >= 0) {
      xc = 1;
    } else {
      xc = -1;
      dx *= -1;
    }
    if(dy >= 0)
      yc = 1;
    else {
      yc = -1;
      dy *= -1;
    }
    if(dx >= dy) {
      d = 2 *dy - dx;
      incrE = 2 * dy;
      incrNE = 2 * (dy - dx);
    } else {
      d = 2 * dx - dy;
      incrE = 2 * dx;
      incrNE = 2 * (dx - dy);
    }
    x = start.Row().V();
    y = start.Col().V();
    xe = end.Row().V();
    ye = end.Col().V();
    isElm = true;
    ONDEBUG(cerr << "dx=" << dx <<" dy=" << dy << " incrE=" << incrE << " incrNE=" << incrNE << "\n");
  }


  //: Goto next point.
  
  bool Line2dIterC::Next() {
    if(x == xe && y == ye) {
      isElm = false;
      return false;
    }
    if(dx >= dy) {
      if(d <= 0) {
	d += incrE;
	x += xc;
      } else {
	d += incrNE;
	x += xc;
	y += yc;
      }
    } else {
      if(d <= 0) {
	d += incrE;
	y += yc;
      } else {
	d += incrNE;
	x += xc;
	y += yc;
      }
    }
    return true;
  }
}
