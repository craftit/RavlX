// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SCALEVALUES_HEADER
#define RAVLIMAGE_SCALEVALUES_HEADER 1
//! author="Charles Galambos"
//! date="8/1/2003"
//! docentry="Ravl.API.Images"
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ScaleValues.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN {
  
  template<class PixelT>
  ImageC<PixelT> ScaleOffsetValues(const ImageC<PixelT> &original,const PixelT &min,const PixelT &max,ImageC<PixelT> &result) {
    if(result.Frame() != original.Frame())
      result = ImageC<PixelT>(original.Frame());
    if(result.Frame().Area() < 1)
      return result;
    Array2dIterC<PixelT> it(original);
    PixelT omin = *it;
    PixelT omax = *it;
    for(it++;it;it++) {
      if(*it < omin)
	omin = *it;
      if(*it > omax)
	omax = *it;
    }
    RealT scale = ((RealT) max - (RealT) min) / ((RealT) omax - (RealT) omin);
    for(Array2dIter2C<PixelT,PixelT> itr(result,original);itr;itr++)
      itr.Data1() = (itr.Data2() - omin) * scale + min;
    return result;
  }
  //! userlevel=Normal
  //: Scale and offset values in image to lie between 'min' and 'max'.
  // Note input and output images may be the same.
  // A refrence to 'result' is returned.
  
  template<class PixelT>
  ImageC<PixelT> ScaleValues(const ImageC<PixelT> &original,const PixelT &max,ImageC<PixelT> &result) {
    if(result.Frame() != original.Frame())
      result = ImageC<PixelT>(original.Frame());
    if(result.Frame().Area() < 1)
      return result;
    Array2dIterC<PixelT> it(original);
    PixelT omax = *it;
    for(it++;it;it++) {
      if(*it > omax)
	omax = *it;
    }
    RealT scale = ((RealT) max) / ((RealT) omax);
    for(Array2dIter2C<PixelT,PixelT> it(result,original);it;it++)
      it.Data1() = it.Data2() * scale;
    return result;
  }
  //! userlevel=Normal
  //: Scale image to that the maximum value in the image is 'max'.
  // Note input and output images may be the same.
  // A refrence to 'result' is returned.
  
}



#endif
