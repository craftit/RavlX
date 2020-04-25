// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SPATIALDIFFERENCE_HEADER
#define RAVLIMAGE_SPATIALDIFFERENCE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Filtering"
//! author="Charles Galambos, based on code written by Bill Chrismas"
//! userlevel=Normal
//! file="Ravl/Image/Processing/Filters/Convolve/SpatialDifference.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Array2dSqr31Iter2.hh"
#include "Ravl/Array2dSqr51Iter2.hh"
#include "Ravl/Array2dSqr71Iter2.hh"

namespace RavlImageN {

  template<class PixelT>
  void SpatialDifferenceOrder1(const Array2dC<PixelT> &img,Array2dC<Vector2dC> &grad) {
    RavlAssert(img.Frame().Area() > 0);
    if(grad.IsEmpty()) {
      ImageRectangleC ir = img.Rectangle();
      grad = ImageC<Vector2dC>(ir.Shrink(1));
    }
    for(Array2dSqr31Iter2C<PixelT,Vector2dC> it(img,grad);it;it++) {
      it.Data2() = Vector2dC((RealT) (it.DataBM1() - it.DataTM1()) / 2.0,
			     (RealT) (it.DataMR1() - it.DataML1()) / 2.0);
    }
  }
  //: Estimates horizontal or vertical spatial derivatives, using central differences of order 1. 
  
  
  template<class PixelT>
  void SpatialDifferenceOrder2(const Array2dC<PixelT> &img,Array2dC<Vector2dC> &grad) {
    RavlAssert(img.Frame().Area() > 0);
    if(grad.IsEmpty()) {
      ImageRectangleC ir = img.Rectangle();
      grad = ImageC<Vector2dC>(ir.Shrink(2));
    }
    const RealT t1 = 2.0/3.0; // Was 2.0/3.0
    const RealT t2 = 1.0/12.0;
    for(Array2dSqr51Iter2C<PixelT,Vector2dC> it(img,grad);it;it++) {
      it.Data2()[0] = (RealT) (it.DataD1()[0] - it.DataU1()[0]) * t1 -
	(it.DataD2()[0] - it.DataU2()[0]) * t2;
      it.Data2()[1] = (RealT) (it.DataM0()[1] - it.DataM0()[-1]) * t1 -
	(it.DataM0()[2] - it.DataM0()[-2]) * t2;
    }
  }
  //: Estimates horizontal or vertical spatial derivatives, using central differences of order 2. 
  
  template<class PixelT>
  void SpatialDifferenceOrder3(const Array2dC<PixelT> &img,Array2dC<Vector2dC> &grad) {
    RavlAssert(img.Frame().Area() > 0);
    if(grad.IsEmpty()) {
      ImageRectangleC ir = img.Rectangle();
      grad = ImageC<Vector2dC>(ir.Shrink(3));
    }
    const RealT t1 = 3.0/4.0; // FIXME:- Not sure the co-efficents are correct.
    const RealT t2 = 3.0/20.0;
    const RealT t3 = 1.0/60.0;
    for(Array2dSqr71Iter2C<PixelT,Vector2dC> it(img,grad);it;it++) {
      it.Data2()[0] = (RealT) (it.DataD1()[0] - it.DataU1()[0]) * t1 -
	(it.DataD2()[0] - it.DataU2()[0]) * t2 +
	(it.DataD3()[0] - it.DataU3()[0]) * t3;
      it.Data2()[1] = (RealT) (it.DataM0()[1] - it.DataM0()[-1]) * t1 -
	(it.DataM0()[2] - it.DataM0()[-2]) * t2 +
	(it.DataM0()[3] - it.DataM0()[-3]) * t3;
    }
  }
  //: Estimates horizontal or vertical spatial derivatives, using central differences of order 3. 
  
  
  template<class PixelT>
  void SpatialDifference(UIntT order,const Array2dC<PixelT> &img,Array2dC<Vector2dC> &grad) {
    RavlAssert(img.Frame().Area() > 0);
    switch(order) {
    case 1: SpatialDifferenceOrder1(img,grad); break;
    case 2: SpatialDifferenceOrder2(img,grad); break;
    case 3: SpatialDifferenceOrder3(img,grad); break;
    default: 
      RavlAlwaysAssertMsg(0,"Image gradient of given order is not supported. ");
      break;
    }
  }
  //: Estimates horizontal or vertical spatial derivatives, 
  // Using central differences of order 1..3. 


}


#endif
