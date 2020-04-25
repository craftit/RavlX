#ifndef RAVLIMAGE_AVERAGING2D_HEADER
#define RAVLIMAGE_AVERAGING2D_HEADER 1
// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

//! author="Bill Christmas"
//! docentry="Ravl.API.Images.Filtering"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Convolve/Convolve2d.hh"

#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"

namespace RavlImageN {
  using namespace RavlN;
  

  //! userlevel=Normal
  //: Averaging filter

  // <p>This is based on the SumRectangles function, and is included for
  // consistency.  It can also be used as a summing filter by omitting the
  // normalising step.</p>

  // This filter can be used to generate an efficient approximation to a Gaussian filter for large filter widths, by applying the filter multiple times (typically 3 or 4 times).  For an averaging filter of width/height n, applied 3 times, the resulting Gaussian approximation has a "&sigma;" of about n/2.  If applied 4 times, "&sigma;" is about n/&radic;3.

  // <p><b><font color="cc0088">Note:</font></b> This filter is not recommended  for byte pixel types.</p>

  template<class InPixelT,class OutPixelT> 
  class Averaging2dC {

  public:
    Averaging2dC()
    {}
    //: Default constructor.
    
    Averaging2dC(UIntT MaskHeight, UIntT MaskWidth, bool Norm=true)
      : mask(Index2dC(0,0), MaskHeight, MaskWidth),
        norm(Norm)
    {}
    //: Construct from mask dimensions
    //!param: Norm - if true, normalising step is performed
    
    ImageC<OutPixelT> Apply (const ImageC<InPixelT> &in) const {
      ImageC<OutPixelT> op;
      SumRectangles(in, mask, op) ;
      if (norm) 
        for (Array2dIterC<OutPixelT> i(op); i; ++i) *i /= mask.Area();
      return op;
    }
    //: Performs an averaging filter on image 'in'.
    // Returns a new filtered image.
    
  private:

    IndexRange2dC mask;
    bool norm;
    
  };
}
#endif
