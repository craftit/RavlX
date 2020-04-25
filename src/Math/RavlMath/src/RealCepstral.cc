// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/RealCepstral.cc"

#include "Ravl/StdMath.hh"
#include "Ravl/RealCepstral.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: Constructor.
  
  RealCepstralC::RealCepstralC(SizeT size) 
    : fft(size),
      ifft(size,true)
  {}
  
  //: Factory constructor.

  RealCepstralC::RealCepstralC(const XMLFactoryContextC &factory)
   : DPProcessBodyC<Array1dC<RealT>,Array1dC<RealT> >(factory)
  {}


  //: Compute the real cepstral of data.
  
  Array1dC<RealT> RealCepstralC::Apply(const Array1dC<RealT> &data) {
    if(!fft.IsValid()) {
      fft = FFT1dC(data.Size());
      ifft = FFT1dC(data.Size(),true);
    }
    Array1dC<ComplexC> res = fft.Apply(data); // Compute FFT
    // Log of magnitude.
    for(Array1dIterC<ComplexC> it(res);it;it++) {
      it.Data().Re() = Log(it.Data().Mag() + 0.00000000001);
      it.Data().Im() = 0;
    }
    res = ifft.Apply(res); // Inverse FFT
    Array1dC<RealT> ret(res.Size()/2);
    // Get magnitude of results.
    for(Array1dIter2C<RealT,ComplexC> ita(ret,res);ita;ita++) {
      ita.Data1() = ita.Data2().Re();
      //cerr << "Im:" << it.Data2().Re() << " " << place->Re() << "\n";
    }
    return ret;
  }

  void LinkRealCepstral()
  {}

  static XMLFactoryRegisterConvertC<RealCepstralC,DPProcessBodyC<Array1dC<RealT>,Array1dC<RealT> > > g_register("RavlN::RealCepstralC");
}
