// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_GAUSSCONVOLVE_HEADER
#define RAVLIMAGE_GAUSSCONVOLVE_HEADER 1
//! author="Kieron J Messer"
//! docentry="Ravl.API.Images.Filtering"
//! lib=RavlImageProc
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Filters/Convolve/GaussConvolve.hh"

#if !RAVL_COMPILER_VISUALCPP
#warning "GaussConvolveC is obsolete: please use GaussConvolve2dC"
#endif
#include "Ravl/Image/Image.hh"
#include "Ravl/GenerateSignal1d.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"

namespace RavlImageN {
  
  //! userlevel=Obsolete

  template<class InPixelT,class OutPixelT = InPixelT,class KernelPixelT = RealT,class SumTypeT = KernelPixelT>
  class GaussConvolveC {
  public:
    GaussConvolveC()
    {}
    //: Default constructor.
    
    GaussConvolveC(UIntT order) {
      binomial = GenerateBinomial((KernelPixelT) 1.0, order, true,true);
      conv.SetKernel(binomial, binomial);
    }
    //: Construct Gaussian with the given size.
    
    ImageC<OutPixelT> Apply (const ImageC<InPixelT> &in) const {
      ImageC<OutPixelT> op;
      conv.Apply(in, op);
      return op;
    }
    //: Performs Gaussian filter on image 'in'.
    // Returns a new filtered image.
    
    Array1dC<KernelPixelT> &Filter()
    { return binomial; }
    //: Access current filter.
    // Note the save/load routines of this class do NOT save
    // the filter coefficients, just its order. So modifying
    // them and saving this class is pointless.
    
    const Array1dC<KernelPixelT> &Filter() const
    { return binomial; }
    //: Access current filter.
    
    UIntT Order() const
    { return binomial.Size(); }
    //: Get size of gausian.
    
  protected:
    Array1dC<KernelPixelT> binomial;
    ConvolveSeparable2dC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> conv;
  };
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  ostream &operator<<(ostream &s, const GaussConvolveC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &out) {
    s << out.Filter().Size() << endl;
    return s;
  }

  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  istream &operator>>(istream &s, GaussConvolveC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &in) {
    UIntT sz;
    s >> sz;
    in = GaussConvolveC<InPixelT,OutPixelT,SumTypeT>(sz);
    return s;
  }
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  BinOStreamC &operator<<(BinOStreamC &s, const GaussConvolveC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &out) {
    s << ((UIntT) out.Filter().Size());
    return s;
  }
  
  template<class InPixelT,class OutPixelT,class KernelPixelT,class SumTypeT>
  BinIStreamC &operator>>(BinIStreamC &s, GaussConvolveC<InPixelT,OutPixelT,KernelPixelT,SumTypeT> &in) {
    UIntT sz;
    s >> sz;
    in = GaussConvolveC<InPixelT,OutPixelT,SumTypeT>(sz);
    return s;
  }

}




#endif
