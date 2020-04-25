// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALCEPSTRAL
#define RAVL_REALCEPSTRAL 1
/////////////////////////////////////////////
//! userlevel=Normal
//! docentry="Ravl.API.Math.Signals.1D"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/RealCepstral.hh"

#include "Ravl/Math.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Types.hh"
#include "Ravl/FFT1d.hh"
#include "Ravl/DP/Process.hh"

namespace RavlN {
  
  //: Compute the real cepstral 
  
  class RealCepstralC
    : public DPProcessBodyC<Array1dC<RealT>,Array1dC<RealT> >
  {
  public:
    RealCepstralC(const XMLFactoryContextC &factory);
    //: Factory constructor.

    RealCepstralC(SizeT size);
    //: Constructor.

    virtual Array1dC<RealT> Apply(const Array1dC<RealT> &data);
    //: Compute the real cepstral of time series data.

    typedef SmartPtrC<RealCepstralC> RefT;
    //: Handle to this process.
  protected:
    FFT1dC fft;
    FFT1dC ifft;
  };


}  


#endif
