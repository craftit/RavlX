// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MEDIANFILTER_HEADER
#define RAVL_MEDIANFILTER_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Filtering"
//! lib=RavlImageProc

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dSqr31Iter2.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlN {
  using namespace RavlImageN;
  
  template<class PixelT>
  void MedianFilter33(const ImageC<PixelT> &in,ImageC<PixelT> &out) {
    if(out.Frame().Area() == 0)
      out = ImageC<PixelT>(in.Frame().Expand(-1));
    CollectionC<PixelT> pixels(9,9);
    for(Array2dSqr31Iter2C<PixelT,PixelT> it(in,out);it;it++) {
      pixels[0] = it.DataTL1();
      pixels[1] = it.DataTM1();
      pixels[2] = it.DataTR1();
      pixels[3] = it.DataML1();
      pixels[4] = it.DataMM1();
      pixels[5] = it.DataMR1();
      pixels[6] = it.DataBL1();
      pixels[7] = it.DataBM1();
      pixels[8] = it.DataBR1();
      
      it.Data2() = pixels.KthHighest(4);
    }
  }
  //: Median filter for single channel images.
  
  
  void MedianFilter33(const ImageC<ByteRGBValueC> &in,ImageC<ByteRGBValueC> &out);
  //: Median filter for byte rgb images.
  // This filter takes a median of the grey value, and
  // uses the colour of the associated pixel.
  
}



#endif
