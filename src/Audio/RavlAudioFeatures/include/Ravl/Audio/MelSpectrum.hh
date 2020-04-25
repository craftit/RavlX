// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_MELSPECTRUM_HEADER
#define RAVLAUDIO_MELSPECTRUM_HEADER 1
//! lib=RavlAudioFeatures
//! docentry="Ravl.API.Audio.Feature Extraction"
//! author="Charles Galambos"
//! file="Ravl/Audio/Features/MelSpectrum.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/RealRange1d.hh"
#include "Ravl/Math.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DP/Process.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Compute the mel spectrum from a power spectrum.
  
  class MelSpectrumC
    : public DPProcessBodyC<SArray1dC<RealT>,SArray1dC<RealT> >
  {
  public:
    MelSpectrumC();
    //: Default constructor.
    // This does not construct any filters. InitFilters must be called
    // before Apply.

    MelSpectrumC(const XMLFactoryContextC &factory);
    //: XMLFactory constructor.

    MelSpectrumC(RealT sampleRate,IntT frameSize,RealRangeC freqRange,IntT numFilters = 40);
    //: Constructor
    // Frequency's is in Hz.
    
    SArray1dC<RealT> Apply(const SArray1dC<RealT> &powerSpectrum);
    //: Compute mel spectrum from power spectrum

    SArray1dC<float> Apply(const SArray1dC<float> &powerSpectrum);
    //: Compute mel spectrum from power spectrum

    bool InitFilters(RealT sampleRate,IntT frameSize,RealRangeC freqRange,IntT numFilters,bool verbose = false);
    //: Initialise filters.
    // Frequency's is in Hz.
    
    RealT Lin2Mel(RealT f)
    { return 2595.0 * Log10(1 + (f / 700.0)); } 
    //: Map linear frequencies to mel frequencies.

    RealT Mel2Lin(RealT f)
    { return 700.0 * (Pow(10.0,f / 2595.0) - 1.0); } 
    //: Map mel frequencies to linear frequencies.
    
    typedef SmartPtrC<MelSpectrumC> RefT;
    //: Handle to this class.

  protected:
    bool m_expand;
    SArray1dC<Array1dC<float> > filters; // Filter bank.
  };
  
}

#endif
