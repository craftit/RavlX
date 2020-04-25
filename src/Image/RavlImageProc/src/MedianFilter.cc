// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc

#include "Ravl/Image/MedianFilter.hh"
#include "Ravl/Image/ByteRGBMedian.hh"

namespace RavlN {
  
  void MedianFilter33(const ImageC<ByteRGBValueC> &in,ImageC<ByteRGBValueC> &out)
  {
    if(out.Frame().Area() == 0)
      out = ImageC<ByteRGBValueC>(in.Frame().Expand(-1));
    CollectionC<ByteRGBGreyValueC> pixels(9,9);
    for(Array2dSqr31Iter2C<ByteRGBValueC,ByteRGBValueC> it(in,out);it;it++) {
      pixels[0] = it.DataTL1();
      pixels[1] = it.DataTM1();
      pixels[2] = it.DataTR1();
      pixels[3] = it.DataML1();
      pixels[4] = it.DataMM1();
      pixels[5] = it.DataMR1();
      pixels[6] = it.DataBL1();
      pixels[7] = it.DataBM1();
      pixels[8] = it.DataBR1();

      ByteRGBGreyValueC &best = pixels.KthHighest(4);
      it.Data2() = static_cast<ByteRGBValueC &>(best);
    }
  }

}
