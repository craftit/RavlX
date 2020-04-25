// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioFeatures
//! file="Ravl/Audio/Features/FeatureMFCC.cc"

#include "Ravl/Audio/FeatureMFCC.hh"
#include "Ravl/PowerSpectrum1d.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {

  //: Constructor.
  
  FeatureMFCCBodyC::FeatureMFCCBodyC(RealT sampleRate,    // Sample rate in Hz.
				     RealT frameRate,     // Frames a second.
				     IntT frameSize,      // Size of a frame.
				     RealRangeC freqRange,// Range of frequencies.
				     IntT numMelFilters,  // Number of mel spectrum filters.
				     IntT numCep,         // Number of ceptstrum co-efficents to compute.
				     RealT preEmphasisAlpha
				     )
    : preEmphasis(preEmphasisAlpha),
      window(RAWHanning,frameSize,Round(sampleRate / frameRate))
  {
    ONDEBUG(cerr << "Windowing signal. frameSep=" << frameSep << " \n");
    melSpectrum.InitFilters(sampleRate,frameSize,freqRange,numMelFilters);
    melCepstrum.Init(numCep,numMelFilters);
  }
  
  //: Constructor.
  
  FeatureMFCCBodyC::FeatureMFCCBodyC()
  {}
  
  //: Get next sample
  
  VectorC FeatureMFCCBodyC::Get() {
    VectorC ret;
    if(!Get(ret))
      throw DataNotReadyC("Failed to compute features. ");
    return ret;
  }
  
  //: Get next sample
  
  bool FeatureMFCCBodyC::Get(VectorC &buff) {
    DPIPortC<Int16T> inp = Input();
    preEmphasis.Input() = inp;
    window.Input() = preEmphasis;
    SArray1dC<RealT> samp;
    ONDEBUG(cerr << "Windowing signal. \n");
    if(!window.Get(samp))
      return false;
    ONDEBUG(cerr << "Computing spectrum. \n");
    SArray1dC<RealT> pspec = PowerSpectrumSimple(samp).SArray1d();
    ONDEBUG(cerr << "Computing mel spectrum. \n");
    SArray1dC<RealT> mspec = melSpectrum.Apply(pspec);
#if 1
    ONDEBUG(cerr << "Computing mel cepstrum. \n");
    SArray1dC<RealT> mcep = melCepstrum.Apply(mspec);
    buff = mcep;
#else
    buff = mspec;
#endif
    return true;
  }
  
  
}
