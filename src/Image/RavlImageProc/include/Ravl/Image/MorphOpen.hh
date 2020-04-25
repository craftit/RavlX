// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_MORPHOPEN_HEADER
#define RAVLIMAGE_MORPHOPEN_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Morphology"
//! file="Ravl/Image/Processing/Morphology/MorphOpen.hh"

#include "Ravl/Image/Dilate.hh"
#include "Ravl/Image/Erode.hh"

namespace RavlImageN {
  
  template<class DataT> 
  void MorphBinaryOpen2d(const ImageC<DataT>& image, const ImageC<DataT>& kernel,ImageC<DataT>& result,DataT inRegion = 1) {
    ImageC<DataT> tmp;
    BinaryErode(image,kernel,tmp,inRegion);
    BinaryDilate(tmp,kernel,result,inRegion);
  }
  //: Perform a morphological 'open' on the image.  This first erodes then dilates the regions in 'image'.
  // Note: The result image is the result of two morphological operations with the given kernel and will 
  // be smaller than the original image.

  
}
#endif
