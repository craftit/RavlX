// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CONVOLHORZ2D_HEADER
#define RAVLIMAGE_CONVOLHORZ2D_HEADER 1
///////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Filtering"
//! file="Ravl/Image/Processing/Filters/Convolve/ConvolveHorz2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Rectangle2dIter.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Horizontal convolution of a 2-D image with a 1-D filter.
  // <b>Template args:</b> <br>
  // InPixelT = Type of pixel in input image. <br>
  // OutPixelT = Type of pixel in output image. (Default = InPixelT) <br>
  // KernelPixelT = Type of pixel in convolution kernel. (Default = InPixelT)  <br>
  // SumTypeT = A type appropriate for summing the product of KernelPixelT and InPixelT. (Default = KernelPixelT <br>
  // There are two main issues when choosing these types.<br>
  // 1. Underflow and overflow of the sums and product operations. <br>
  // 2. Handing multi-channel images. (Such as RGB.) <br>
  // The exact requirements of these types depends on the gain and type of the filter being used.
  // In multi-channel filters SumPixelT should be a multi-channel value as well. e.g.
  // to filter an ImageC<ByteRGBValueC> you may use:
  // InPixelT=ByteRGBValueC, OutPixelT=ByteRGBValueC,KernelPixelT=RealT,SumType=RealRGBValueC
  
  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = InPixelT,class SumTypeT = KernelPixelT>
  class ConvolveHorz2dC {
  public:
    ConvolveHorz2dC()
    {}
    //: Default constructor.

    ConvolveHorz2dC(const Array1dC<KernelPixelT> &nrowKernel)
    { SetKernel(nrowKernel); }
    //: Default constructor.
    
    void SetKernel(const Array1dC<KernelPixelT> &nrowKernel) { 
      rowKernel = nrowKernel.Copy();
      rowKernel.Reverse(); // Reverse mask as Appy method actually does correlation.
    }
    //: Set the convolution kernel.
    // Note: Currently this rotates the mast around the central element,
    // not about the 0 index as other convolution methods in RAVL.
    
    void Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const;
    //: Do convolution on image 'in'; put the output in 'result' 
    
    ImageC<OutPixelT> Apply(const ImageC<InPixelT> &in) const {
      ImageC<OutPixelT> result;
      ConvolveHorz2dC::Apply(in, result);
      return result;
    }
    //: Do convolution on image 'in'; return result in new image 

    void operator()(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Apply(in,result); }
    //: () Style apply.
    
    ImageC<OutPixelT> operator()(const ImageC<InPixelT> &in) const
    { return Apply(in); }
    //: Do convolution on image 'in'; return result in new image
    
  protected:
    Array1dC<KernelPixelT> rowKernel;
  };
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  void ConvolveHorz2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const {
    RavlAssertMsg(rowKernel.Size() > 0,"Convolution kernel too small. ");
    ImageRectangleC resRect = in.Rectangle();
    resRect.LCol() -= rowKernel.IMin();
    resRect.RCol() -= rowKernel.IMax();
    RavlAssertMsg(resRect.Area() > 0,"ConvoleHorz2dC::Apply(), ERROR: Input rectangle too small.");
    if(resRect.RCol() < resRect.LCol()) { // Check we've got something left.
      result = ImageC<OutPixelT>(); 
      return ;
    }
    if(!result.Rectangle().Contains(resRect)) // Check the result rectangle is large enough.
      result = ImageC<OutPixelT>(resRect); // If its not make another.
    UIntT ksize = rowKernel.Range().Size();
    const KernelPixelT *ikp = &(rowKernel[rowKernel.IMin()]);
    const KernelPixelT *eol = &ikp[ksize];
    for(Array2dIter2C<OutPixelT,InPixelT> it(result,in,resRect);it;it++) { // ,resRect
      const KernelPixelT *kp = ikp;
      const InPixelT *ip = &((&it.Data2())[rowKernel.IMin().V()]);
      SumTypeT sum = (SumTypeT) ((*ip) * (*kp));
      ip++;
      kp++;
      for(;kp != eol;kp++,ip++)
	sum += (SumTypeT) ((*ip) * (*kp));
      it.Data1() = (OutPixelT) sum;
    }
  }

  
}

#endif
