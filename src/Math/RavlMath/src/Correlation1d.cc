// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/Correlation1d.cc"

#include "Ravl/StdMath.hh"
#include "Ravl/Correlation1d.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor.
  
  Correlation1dC::Correlation1dC(IntT size)
#if 1
    : fft(size*2,false,true),
      ifft(size*2,true)
#else
    : fft(size,false),
      ifft(size,true)
#endif
  {}
  
  //: Return the correlation of signals d1 and d2.
  
  Array1dC<RealT> Correlation1dC::Apply(const Array1dC<RealT> &d1,const Array1dC<RealT> &d2) {
    if(d1.Size() == 0)
      return Array1dC<RealT>();
    // Do a forward fft.
    Array1dC<ComplexC> fftd1 = fft.Apply(d1);
    Array1dC<ComplexC> fftd2 = fft.Apply(d2);
    ONDEBUG(std::cerr << "CRes Size1:" << fftd1.Size() << " Size2:" << fftd2.Size() << "\n");
    // Multiply by the complex conjugate and put result back in fftd1.
    for(Array1dIter2C<ComplexC,ComplexC> it(fftd1,fftd2);it;it++)
      it.Data1() = it.Data1() * it.Data2().Conj();
    // Do a reverse fft.
    Array1dC<ComplexC> cres = ifft.Apply(fftd1);
    ONDEBUG(std::cerr << "CRes Size:" << cres.Size() << "\n");
    Array1dC<RealT> ret(cres.Size());
    // Convert to real parts only.
    for(Array1dIter2C<RealT,ComplexC> itx(ret,cres);itx;itx++)
      itx.Data1() = itx.Data2().Re();
    return ret;
  }
  
  
  //: Return the auto correlation  signals d1.
  // This uses FFT to calculate the correlation d1 and itself.
  
  Array1dC<RealT> Correlation1dC::AutoCorrelation(const Array1dC<RealT> &d1) {
    if(d1.Size() == 0)
      return Array1dC<RealT>();
    // Do a forward fft.
    Array1dC<ComplexC> fftd1 = fft.Apply(d1);
    // Multiply by the complex conjugate and put result back in fftd1.
    for(Array1dIterC<ComplexC> it(fftd1);it;it++)
      it.Data1() = it.Data1() * it.Data1().Conj();
    // Do a reverse fft.
    Array1dC<ComplexC> cres = ifft.Apply(fftd1);
    Array1dC<RealT> ret(cres.Size());
    for(Array1dIter2C<RealT,ComplexC> ita(ret,cres);ita;ita++)
      ita.Data1() = ita.Data2().Re();
    return ret;
  }

}
