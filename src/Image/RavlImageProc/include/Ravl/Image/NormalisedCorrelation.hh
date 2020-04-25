// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_NORMALISEDCORRELATION_HEADER
#define RAVLIMAGE_NORMALISEDCORRELATION_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Correlation"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Matching/NormalisedCorrelation.hh"

#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Math.hh"

namespace RavlImageN {
  
  //! userlevel=Normal

  template<typename Data1T,typename Data2T>
  RealT NormalisedCorrelation(const ImageC<Data1T> &img1,const ImageC<Data2T> &img2) {
    RavlAssert(img2.Frame().Size() == img1.Frame().Size());
    RealT area = img2.Frame().Area();
    RealT mean1 = 0.0;
    RealT mean2 = 0.0;
    
    Array2dIter2C<Data1T,Data2T> it(img1,img2);
    for(;it;it++) {
      mean1 += (RealT) it.Data1();
      mean2 += (RealT) it.Data2();
    }
    
    mean1 /= area;
    mean2 /= area;
    
    RealT top = 0.0,botr = 0.0,botl = 0.0;
    
    for(it.First();it;it++) {
      RealT n2 = ((RealT) it.Data2())-mean2;
      RealT n1 = ((RealT) it.Data1())-mean1;
      top +=  n1 * n2;
      botr += Sqr(n1);
      botl += Sqr(n2);
    }
    if(botl == 0 || botr == 0) // One of the images has zero variance.
      return botl == 0 && botr == 0; // If both have, call it a match.
    RealT bot = botl * botr;
    RavlAssert(bot >= 0);
    return (top/Sqrt(bot));
  }
  //: Compute the normalised correlation of two images.
  
}
#endif
