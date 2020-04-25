// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/PowerSpectrum1d.cc"

#include "Ravl/StdMath.hh"
#include "Ravl/PowerSpectrum1d.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "ccmath/ccmath.h"

namespace RavlN {

  //: Compute the power spectrum of data with no windowing.
  
  Array1dC<RealT> PowerSpectrumSimple(const Array1dC<RealT> &data,int smooth) {
    Array1dC<RealT> ret = data.Copy();
    int os = pwspec((double *) &(ret[0]),data.Size(),smooth);
    return Array1dC<RealT>(ret,IndexRangeC(os));
  }
  
  //: Constructor
  
  PowerSpectrum1dC::PowerSpectrum1dC(IntT size,bool useWindow)
    : m_useWindow(useWindow),
      fft(size)
  {}

  //: Constructor.
  PowerSpectrum1dC::PowerSpectrum1dC(const XMLFactoryContextC &factory)
   : m_useWindow(factory.AttributeBool("useWindow",true))
  {}

  //: Compute the power spectrum of data.
  
  Array1dC<RealT> PowerSpectrum1dC::Apply(const Array1dC<RealT> &data) {
    if(!m_useWindow)
      return PowerSpectrumSimple(data);
    if(fft.IsValid())
      fft = FFT1dC(data.Size());
    UIntT size = data.Size();
    IndexC mid = data.IMin() + (size/2);
    //cerr << "Size=" << size <<" Mid=" << mid << " Rem=" << rem << "\n";
    Array1dC<RealT> d1(data,IndexRangeC(data.IMin(),mid));
    Array1dC<RealT> d2(data,IndexRangeC(mid+1,data.IMax()));
    return Apply(d1,d2);
  }
  
  //: Compute the power spectrum of data in two arrays.
  // Compute the power spectrum using Bartlett window (simple triangle).
  // The arrays are expected to be of equal size, so the first
  // array will be multiplied by an increasing ramp, the second
  // by a decreasing one.
  
  Array1dC<RealT> PowerSpectrum1dC::Apply(const Array1dC<RealT> &d1,const Array1dC<RealT> &d2) {
    UIntT size = d1.Size() + d2.Size();
    RavlAssert(Abs((int) d1.Size() - (int) d2.Size()) <= 1);    
    if(size == 0)
      return Array1dC<RealT>();
    Array1dC<RealT> work(size);
    Array1dC<RealT> ret(size);
    
    RealT wss = 0; // This should be computed analytically.
    // Ramp up
    int mid = d1.Size();
    RealT frac = 0,inc = 1/((RealT)size);
    for(Array1dIter2C<RealT,RealT> it(d1,work);it;it++) {
      frac += inc;
      it.Data2() = it.Data1() * frac;
      wss += Sqr(frac); 
    }
    
    // Ramp down
    int rem = d2.Size();
    inc = 1/(RealT) rem; // Make sure its accurate.
    Array1dC<RealT> t2(work,IndexRangeC(mid,d2.Size()));
    for(Array1dIter2C<RealT,RealT> it2(t2,d2);it2;it2++) {
      frac -= inc;
      it2.Data1() = it2.Data2() * frac;
      wss += Sqr(frac);
    }
    
    // Do the fft.
    
    Array1dC<ComplexC> fftres = fft.Apply(work);
    
    wss *= size/2; // Because we only sum half the spectrum.
    //cerr << "wss:" << wss << "\n";
    // Compute the magnitude.
    Array1dC<RealT> mag(fftres.Size() / 2);
    Array1dIter2C<RealT,ComplexC> ita(mag,fftres);
    ita.Data1() = (Sqr(ita.Data2().Re()) + Sqr(ita.Data2().Im())) / (wss * 2);
    ita++;
    for(;ita;ita++)
      ita.Data1() = (Sqr(ita.Data2().Re()) + Sqr(ita.Data2().Im())) / wss;
    
    // Return the results.
    return mag;
  }

  static XMLFactoryRegisterConvertC<PowerSpectrum1dC,DPProcessBodyC<Array1dC<RealT>,Array1dC<RealT> > > g_regiserPowerSpectrum1d("RavlN::PowerSpectrum1dC");

  void LinkPowerSpectrum1d()
  {}
}
