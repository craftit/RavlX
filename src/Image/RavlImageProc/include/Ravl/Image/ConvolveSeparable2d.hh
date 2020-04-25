// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CONVOLVESEPARABLE2D_HEADER
#define RAVLIMAGE_CONVOLVESEPARABLE2D_HEADER 1
/////////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Charles Galambos, Bill Christmas"
//! docentry="Ravl.API.Images.Filtering"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Convolve/ConvolveSeparable2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ConvolveHorz2d.hh"
#include "Ravl/Image/ConvolveVert2d.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Separable 2D Convolution
  //
  // <h3>Template args:</h3>
  // InPixelT = Type of pixel in input image. <br>
  // OutPixelT = Type of pixel in output image. (Default = InPixelT) <br>
  // KernelPixelT = Type of pixel in convolution kernel. (Default = InPixelT)  <br>
  // SumTypeT = A type appropriate for summing the product of KernelPixelT and InPixelT. (Default = KernelPixelT <br>
  // <p>
  // There are two main issues when choosing these types.<br>
  // 1. Underflow and overflow of the sums and product operations. <br>
  // 2. Handing multi-channel images. (Such as RGB.) <br>
  // The exact requirements of these types depends on the gain and type of the filter being used.
  // In multi-channel filters SumPixelT should be a multi-channel value as well. e.g.
  // to filter an ImageC&lt;ByteRGBValueC&gt; you might use:
  // InPixelT=ByteRGBValueC, OutPixelT=ByteRGBValueC, KernelPixelT=RealT, SumType=RealRGBValueC</p>
  
  
  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = InPixelT,class SumTypeT = KernelPixelT>
  class ConvolveSeparable2dC {
  public:
    ConvolveSeparable2dC()
    {}
    //: Default constructor.

    ConvolveSeparable2dC(const Array1dC<KernelPixelT> &VertKernel,const Array1dC<KernelPixelT> &HorzKernel)
    { SetKernel(VertKernel,HorzKernel); }
    //: Constructor from vertical and horizontal kernels respectively.
    
    ConvolveSeparable2dC(const Array1dC<KernelPixelT> &Kernel)
    { SetKernel(Kernel,Kernel); }
    //: Constructor for filter using the same 1-D kernel for vertical and horizontal.
    
    void SetKernel(const Array1dC<KernelPixelT> &VertKernel,const Array1dC<KernelPixelT> &HorzKernel) { 
      vert.SetKernel(VertKernel);
      horz.SetKernel(HorzKernel);
    }
    //: Set the convolution kernel.
    
    void Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const;
    //: Do convolution on image 'in'; put the output in 'result' 
    
    ImageC<OutPixelT> Apply(const ImageC<InPixelT> &in) const{
      ImageC<OutPixelT> ret;
      Apply(in,ret);
      return ret;
    }
    //: Do convolution on image 'in'; return result in new image.
    
    void operator()(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Apply(in,result); }
    //: Do convolution on image 'in'; put the output in 'result' 
    
    ImageC<OutPixelT> operator()(const ImageC<InPixelT> &in) const
    { return Apply(in); }
    //: Do convolution on image 'in'; return result in new image
    
  protected:
    ConvolveVert2dC<InPixelT,SumTypeT,KernelPixelT,SumTypeT>  vert;
    ConvolveHorz2dC<SumTypeT,OutPixelT,KernelPixelT,SumTypeT> horz;
  };
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  void ConvolveSeparable2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const {
    ImageC<SumTypeT> tmp;
    vert.Apply(in,tmp);
    horz.Apply(tmp,result);
  }
  //: Do convolution.
  
}

#endif
