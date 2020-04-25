// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DRAWCIRCLE_HEADER
#define RAVLIMAGE_DRAWCIRCLE_HEADER 1
///////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="8/8/2002"
//! docentry="Ravl.API.Images.Drawing"
//! lib=RavlImage
//! userlevel=Normal
//! file="Ravl/Image/Base/DrawCircle.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Circle2d.hh"
#include "Ravl/CircleIter.hh"
#include "Ravl/Image/DrawPolygon.hh"

namespace RavlImageN {
  
  template<class DataT>
  void DrawCircle(Array2dC<DataT> &dat,const DataT &value,const Index2dC &center,IntT radius ) {
    if(dat.Frame().Contains(center + Index2dC(radius,radius)) && dat.Frame().Contains(center - Index2dC(radius,radius))) {
      // If far diagonals are inside the image, all pixels in between are.
      for(CircleIterC it(radius,center);it;it++)
	dat[*it] = value;
      return ;
    }
    for(CircleIterC it(radius,center);it;it++) {
      if(!dat.Contains(*it)) 
	continue;
      dat[*it] = value;
    }
  }
  //: Draw a circle in an image.

  template<class DataT>
  void DrawCircle(Array2dC<DataT> &dat,const DataT &value,const Point2dC &center,float radius,bool filled) {
    if(radius <= 1.0) { // Very small ?
      Index2dC at = center;
      if(dat.Frame().Contains(at))
        dat[at] = value;
      return ;
    }
    RealT step = RavlConstN::pi/(radius+2.0);
    Polygon2dC poly;
    Circle2dC circle(center,radius);
    for(RealT a = 0;a < 2*RavlConstN::pi;a += step)
      poly.InsLast(circle.Value(a));
    // Fill in...
    DrawPolygon(dat,value,poly,filled);
  }
  //: Draw a filled circle in an image from real floating point coordinates.

}

#endif
