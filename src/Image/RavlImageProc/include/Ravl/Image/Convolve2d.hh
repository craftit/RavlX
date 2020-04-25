// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CONVOLVE2D_HEADER
#define RAVLIMAGE_CONVOLVE2D_HEADER 1
/////////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Convolve/Convolve2d.hh"
//! docentry="Ravl.API.Images.Filtering"

#include "Ravl/Image/Correlate2d.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: 2D Convolution
  //
  // <h3>Template arguments</h3>
  // InPixelT = Type of pixel in input image. <br>
  // OutPixelT = Type of pixel in output image. (Default = InPixelT) <br>
  // KernelPixelT = Type of pixel in convolution kernel. (Default = InPixelT)  <br>
  // SumTypeT = A type appropriate for summing the product of KernelPixelT and InPixelT. (Default = KernelPixelT) <br>
  // <p>
  // There are two main issues when choosing these arguments:<ol>
  // <li> Underflow and overflow of the sums and product operations. </li>
  // <li> Handing multi-channel images (such as RGB). </li></ol>
  // The exact requirements of these types depends on the gain and type of the filter being used.
  // In multi-channel filters SumPixelT should be a multi-channel value as well.
  // E.g. to filter an ImageC&lt;ByteRGBValueC&gt; you might use:
  // InPixelT=ByteRGBValueC, OutPixelT=ByteRGBValueC, KernelPixelT=RealT, SumType=RealRGBValueC</p>
  //
  // <p> N.B. This class rotates the kernel by 180&deg; and then uses
  // Correlate2dC::Apply().
  // (For rotationally symmetric non-separable kernels, it is therefore
  // marginally faster to use <a // href="RavlImageN.Correlate2dC.html">
  // Correlate2dC</a> directly.)</p>

  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = InPixelT,class SumTypeT = KernelPixelT>
  class Convolve2dC 
    : protected Correlate2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>
  {
  public:
    Convolve2dC()
    {}
    //: Default constructor.
    // Use SetKernel() to setup a kernel before calling Apply().
    
    Convolve2dC(const ImageC<KernelPixelT> &nkernel)
    { SetKernel(nkernel); }
    //: Construct using kernel.
    
    void SetKernel(const ImageC<KernelPixelT> &nkernel) { 
      // The apply method actualy does correlation.
      Correlate2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::SetKernel(nkernel.Rotate180());
    }
    //: Set the convolution kernel.
    
    void Apply(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Correlate2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(in,result); }
    //: Do convolution on image 'in'; put the output in 'result' 

    ImageC<OutPixelT> Apply(const ImageC<InPixelT> &in) const{
      ImageC<OutPixelT> ret;
      Apply(in,ret);
      return ret;
    }
    //: Do convolution on image 'in'; return result in new image.

    
    void operator()(const ImageC<InPixelT> &in,ImageC<OutPixelT> &result) const
    { Apply(in,result); }    
    //: Do convolution on image 'in', put the output in 'result' 
  };
  
}

#endif
