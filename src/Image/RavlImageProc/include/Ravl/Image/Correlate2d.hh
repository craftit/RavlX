// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CORRELATION_HEADER
#define RAVLIMAGE_CORRELATION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Correlation"
//! author="Charles Galambos"
//! userlevel=Normal
//! file="Ravl/Image/Processing/Filters/Matching/Correlate2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Rectangle2dIter.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: 2D Correlation
  //
  // See <a href="RavlImageN.Convolve2dC.html">Convolve2dC</a> for information on setting template arguments
  
  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = InPixelT,class SumTypeT = KernelPixelT>
  class Correlate2dC {
  public:
    Correlate2dC()
    {}
    //: Default constructor.

    Correlate2dC(const ImageC<KernelPixelT> &nkernel)
    { SetKernel(nkernel); }
    //: Construct using "kernel" image.
    
    void SetKernel(const ImageC<KernelPixelT> &nkernel) 
    { kernel = nkernel; }
    //: Set the correlation kernel image.
    
    void Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const;
    //: Do correlation on image 'in', put the output in 'result' 
    
    void operator()(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Apply(in,result); }
    //: Do correlation on image 'in', put the output in 'result' 

  protected:
    ImageC<KernelPixelT> kernel;
  };
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  void Correlate2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const {
    RavlAssertMsg(kernel.Frame().Area() > 0,"Convolution / correlation kernel has zero size. ");
    ImageRectangleC resRect = in.Rectangle();
    resRect -= kernel.Rectangle();
    RavlAssertMsg(resRect.Area() > 0,"Convolution / correlation: kernel is too big for input rectangle.");
    if(!result.Rectangle().Contains(resRect)) // Check the result rectangle is large enough.
      result = ImageC<OutPixelT>(resRect); // If its not make another.
      
    Array2dIterC<OutPixelT> res(result,resRect);
    for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
      BufferAccess2dIter2C<KernelPixelT,InPixelT> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
      SumTypeT sum = (SumTypeT) (it.Data2() * it.Data1());
      it++;
      for(;it;it++)
	sum +=  (SumTypeT) (it.Data2() * it.Data1());
      *res =(OutPixelT) sum;
    }
  }
  //: Do correlation.

#if RAVL_USE_MMX
  template <> void Correlate2dC<short,short,short,short>::Apply(const ImageC<short> &in,ImageC<short> &result) const;
  //: Use some MMX code to speed this up.
  
  template <> void Correlate2dC<short,short,short,int>::Apply(const ImageC<short> &in,ImageC<short> &result) const;
  //: Use some MMX code to speed this up.
#endif

}

#endif
