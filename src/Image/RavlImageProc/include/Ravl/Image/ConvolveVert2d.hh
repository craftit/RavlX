// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CONVOLVERT2D_HEADER
#define RAVLIMAGE_CONVOLVERT2D_HEADER 1
/////////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Filtering"
//! file="Ravl/Image/Processing/Filters/Convolve/ConvolveVert2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Rectangle2dIter.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Vertical convolution of a 2-D image with a 1-D filter.
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

  template<typename InPixelT,typename OutPixelT = InPixelT,typename KernelPixelT = InPixelT,typename SumTypeT = typename NumericalTraitsC<InPixelT>::RealAccumT>
  class ConvolveVert2dC {
  public:
    ConvolveVert2dC()
    {}
    //: Default constructor.

    ConvolveVert2dC(const Array1dC<KernelPixelT> &ncolKernel)
    { SetKernel(ncolKernel); }
    //: Default constructor.
    
    void SetKernel(const Array1dC<KernelPixelT> &ncolKernel) { 
      colKernel = ncolKernel.Copy();
      colKernel.Reverse();  // Reverse mask as Appy method actually does correlation.
    }
    //: Set the convolution kernel.
    // Note: Currently this rotates the mast around the central element,
    // not about the 0 index as other convolution methods in RAVL.
    
    void Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const;
    //: Do convolution on image 'in'; put the output in 'result' 

    ImageC<OutPixelT> Apply(const ImageC<InPixelT> &in) const {
      ImageC<OutPixelT> result;
      ConvolveVert2dC::Apply(in, result);
      return result;
    }
    //: Do convolution on image 'in'; return result in new image 

    void operator()(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Apply(in,result); }
    //: Do convolution on image 'in'; put the output in 'result' 
    
    ImageC<OutPixelT> operator()(const ImageC<InPixelT> &in) const
    { return Apply(in); }
    //: Do convolution on image 'in'; return result in new image 
    
  protected:
    Array1dC<KernelPixelT> colKernel;
  };
  
  template<typename InPixelT,typename OutPixelT,typename KernelPixelT,typename  SumTypeT>
  void ConvolveVert2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const {
    RavlAssertMsg(colKernel.Size() > 0,"Convolution kernel too small. ");
    ImageRectangleC resRect = in.Rectangle();
    resRect.TRow() -= colKernel.IMin();
    resRect.BRow() -= colKernel.IMax();
    RavlAssertMsg(resRect.Area() > 0,"ConvolveVert2dC::Apply(), ERROR: Input rectangle too small.");
    if(resRect.BRow() < resRect.TRow()) { // Check we've got something left.
      result = ImageC<OutPixelT>(); 
      return ;
    }
    if(!result.Rectangle().Contains(resRect)) // Check the result rectangle is large enough.
      result = ImageC<OutPixelT>(resRect); // If its not make another.
    UIntT ksize = colKernel.Range().Size();
    IntT rowSep = in.Stride();
    const KernelPixelT *ikp = &(colKernel[colKernel.IMin()]);
    const KernelPixelT *eol = &ikp[ksize];
    const IntT koff = colKernel.IMin().V() * rowSep;
    for(Array2dIter2C<OutPixelT,InPixelT> it(result,in,resRect);it;it++) { // ,resRect
      const KernelPixelT *kp = ikp;
      const InPixelT *ip = &((&it.Data2())[koff]);
      SumTypeT sum = (SumTypeT) ((*ip) * (*kp));
      ip += rowSep;
      kp++;
      for(;kp != eol;kp++,ip += rowSep)
	sum += ((*ip) * (*kp));
      it.Data1() = (OutPixelT) sum;
    }
  }
  
}

#endif
