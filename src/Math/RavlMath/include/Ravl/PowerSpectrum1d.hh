// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POWERSPECTRUM1D_HEADER
#define RAVL_POWERSPECTRUM1D_HEADER
//////////////////////////////////////////////////////////
//! userlevel=Normal
//! docentry="Ravl.API.Math.Signals.1D"
//! example=exPowerSpectrum.cc
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/PowerSpectrum1d.hh"

#include "Ravl/StdMath.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/FFT1d.hh"
#include "Ravl/DP/Process.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Power Spectrum 
  
  class PowerSpectrum1dC
   : public DPProcessBodyC<Array1dC<RealT>,Array1dC<RealT> >
  {
  public:
    PowerSpectrum1dC(IntT size,bool useWindow = true);
    //: Constructor.

    PowerSpectrum1dC(const XMLFactoryContextC &factory);
    //: Constructor.

    Array1dC<RealT> Apply(const Array1dC<RealT> &data);
    //: Compute the power spectrum of data.
    // Compute the power spectrum using Bartlett window (simple triangle).
    
    Array1dC<RealT> Apply(const Array1dC<RealT> &d1,const Array1dC<RealT> &d2);
    //: Compute the power spectrum of data in two arrays.
    // Compute the power spectrum using Bartlett window (simple triangle).
    // The arrays are expected to be of equal size, so the first
    // array will be multiplied by an increasing ramp, the second
    // by a decreasing one.
    
    typedef RavlN::SmartPtrC<PowerSpectrum1dC> RefT;
    //: Handle to class
  protected:
    bool m_useWindow;
    FFT1dC fft;  
  };
  
  Array1dC<RealT> PowerSpectrumSimple(const Array1dC<RealT> &data,int smooth = 0);
  //: Compute the power spectrum of data with no windowing..
}



#endif
