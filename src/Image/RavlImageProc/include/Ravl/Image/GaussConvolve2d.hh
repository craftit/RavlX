// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_GAUSSCONVOLVE2D_HEADER
#define RAVLIMAGE_GAUSSCONVOLVE2D_HEADER 1
//! userlevel=Normal
//! author="Kieron J Messer"
//! docentry="Ravl.API.Images.Filtering"
//! example=exGaussConvolve.cc
//! lib=RavlImageProc
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Filters/Convolve/GaussConvolve2d.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/GenerateSignal1d.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Low-pass-filters an image with a finite-width approximation to a Gaussian mask
  //
  // <p>This class just wraps <a href="RavlImageN.ConvolveSeparable2dC.html">ConvolveSeparable2dC</a> and
  // <a href="RavlN.GenerateBinomialObUIntT_bool_bool_UIntTCb.html">GenerateBinomial()</a> calls in a convenient form.</p>

  //<p> The filter design is based on a normalised row from Pascal's triangle
  // (i.e. binomial coefficients).  E.g. a 5th order filter has mask of:</p>
  // <pre> (1 4 6 4 1) / 16 </pre>

  //<p> The "&sigma", or effective half-width of such a filter, with <i>n</i> coefficients, is thus &frac12;&radic;(<i>n</i>-1).  For more details, see <a href="RavlN.GenerateBinomialObSizeT_bool_bool_UIntTCb.html">GenerateBinomial()</a>.</p>

  //<p> This filter design becomes expensive for large values of n.  Instead, use repeated applications of <a href="RavlImageN.Averaging2dC.html">Averaging2dC</a> as described there.

  // <p>The default form of this function is setup to handle single channel images. (e.g. byte, real, int)
  // if you want to convolve multi channel images you should change the 'SumTypeT' template argument to
  // a type that can handle sums and multiplications without overflowing 
  // e.g. If you wish to convolve a <a href="RavlImageN.ByteRGBValueC.html">ByteRGBValueC</a> SumTypeT should be set to RealRGBValueC. </p>

  // <p>Template args:</p>
  // InPixelT = Type of pixel in input image. <br>
  // OutPixelT = Type of pixel in output image. (Default = InPixelT) <br>
  // KernelPixelT = Type of pixel in convolution kernel. (Default = RealT; integer types currently will not work, as the normalisation is done in the kernel generation)  <br>
  // SumTypeT = A type appropriate for summing the product of KernelPixelT and InPixelT. (Default = KernelPixelT) <br>
  // There are two main issues when choosing these types.<br>
  // 1. Underflow and overflow of the sums and product operations. <br>
  // 2. Handing multi-channel images. (Such as RGB.) <br>
  // The exact requirements of these types depends on the gain and type of the filter being used.
  // In multi-channel filters SumPixelT should be a multi-channel value as well. e.g.
  // to filter an ImageC<ByteRGBValueC> you may use:
  // InPixelT=ByteRGBValueC, OutPixelT=ByteRGBValueC,KernelPixelT=RealT,SumType=RealRGBValueC

  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = RealT,class SumTypeT = KernelPixelT>
  class GaussConvolve2dC 
  : public ConvolveSeparable2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> {
  public:
    GaussConvolve2dC()
    {}
    //: Default constructor.
    
    GaussConvolve2dC(UIntT order) 
      : binomial(GenerateBinomial<KernelPixelT>(order, true, true))
      { this->SetKernel(binomial, binomial); }
    //: Construct Gaussian with the given size.
    
    ImageC<OutPixelT> Apply (const ImageC<InPixelT> &in) const {
      ImageC<OutPixelT> op;
      ConvolveSeparable2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(in, op);
      return op;
    }
    //: Performs Gaussian filter on image 'in'.
    // Returns a new filtered image.

    ImageC<OutPixelT> Apply (const ImageC<InPixelT> &in,ImageC<OutPixelT> &op) const {
      ConvolveSeparable2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT>::Apply(in, op);
      return op;
    }
    //: Performs Gaussian filter on image 'in', put the result in op.
    // Returns a new filtered image.
    
    Array1dC<KernelPixelT> &Filter()
    { return binomial; }
    //: Access current filter coefficients.
    // Note the save/load routines of this class do NOT save
    // the filter coefficients, just its order. So modifying
    // them and saving this class is pointless.
    
    const Array1dC<KernelPixelT> &Filter() const
    { return binomial; }
    //: Access current filter.
    
    UIntT Order() const
    { return binomial.Size(); }
    //: Get size of Gaussian.
    
  protected:
    Array1dC<KernelPixelT> binomial;
  };
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  ostream &operator<<(ostream &s, const GaussConvolve2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &out) {
    s << out.Filter().Size() << endl;
    return s;
  }

  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  istream &operator>>(istream &s, GaussConvolve2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &in) {
    UIntT sz;
    s >> sz;
    in = GaussConvolve2dC<InPixelT,OutPixelT,SumTypeT>(sz);
    return s;
  }
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  BinOStreamC &operator<<(BinOStreamC &s, const GaussConvolve2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &out) {
    s << ((UIntT) out.Filter().Size());
    return s;
  }
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  BinIStreamC &operator>>(BinIStreamC &s, GaussConvolve2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &in) {
    UIntT sz;
    s >> sz;
    in = GaussConvolve2dC<InPixelT,OutPixelT,SumTypeT>(sz);
    return s;
  }

}




#endif
