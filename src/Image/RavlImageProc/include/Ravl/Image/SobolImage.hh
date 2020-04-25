// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SOBOLIMAGE_HEADER
#define RAVL_SOBOLIMAGE_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Misc"
//! file="Ravl/Image/Processing/Tools/SobolImage.hh"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="07/01/1999"

#include "Ravl/Image/Image.hh"

namespace RavlImageN {
  
  ImageC<UIntT> SobolImage(const ImageRectangleC &rect);
  //! userlevel=Normal
  //: Generate a Sobol image of a given size.
  // Each pixel image is set to its place in a Sobol sequence.
  
}
#endif
