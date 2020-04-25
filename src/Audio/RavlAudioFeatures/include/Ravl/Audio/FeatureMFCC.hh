// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_FEATUREEXTACTOR_HEADER
#define RAVLAUDIO_FEATUREEXTACTOR_HEADER 1
/////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! lib=RavlAudioFeatures
//! examples=exFeatureMFCC.cc
//! file="Ravl/Audio/Features/FeatureMFCC.hh"

#include "Ravl/Audio/WindowSignal.hh"
#include "Ravl/Audio/MelSpectrum.hh"
#include "Ravl/Audio/MelCepstrum.hh"
#include "Ravl/Audio/PreEmphasis.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/Vector.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Mel-Frequency Cepstral Coefficients feature extractor.
  
  class FeatureMFCCBodyC
    : public DPIStreamOpBodyC<Int16T,VectorC>
  {
  public:
    FeatureMFCCBodyC();
    //: Constructor.
    
    FeatureMFCCBodyC(RealT sampleRate,    // Sample rate in Hz.
		     RealT frameRate,     // Frames a second.
		     IntT frameSize,      // Size of a frame.
		     RealRangeC freqRange,// Range of frequencies.
		     IntT numMelFilters,  // Number of mel spectrum filters.
		     IntT numCep,         // Number of ceptstrum coefficients to compute.
		     RealT preEmphasis    // Amount of pre-emphasis.
		     );
    //: Constructor.
    

    virtual VectorC Get();
    //: Get next sample
    
    virtual bool Get(VectorC &buff);
    //: Get next sample
    
  protected:
    PreEmphasisC<Int16T,RealT,RealT> preEmphasis;
    WindowSignalC<RealT,RealT,RealT> window;
    MelSpectrumC melSpectrum;
    MelCepstrumC melCepstrum;
  };
  
  //! userlevel=Normal
  //: Mel-Frequency Cepstral Coefficients feature extractor.
  // Based on  Sphinx III front end signal processor.
  
  class FeatureMFCCC
    : public DPIStreamOpC<Int16T,VectorC>
  {
  public:
    FeatureMFCCC()
      : DPEntityC(true)
    {}
    //: Default constructor
    // creates an invalid handle.
    
    FeatureMFCCC(RealT sampleRate , // Sample rate in Hz.
		 RealT frameRate = 100,    // Frames a second.
		 IntT frameSize = 512,     // Size of a frame.
		 RealRangeC freqRange = RealRangeC(133.333334,6855.4976),// Range of frequencies.
		 IntT numMelFilters = 40,  // Number of mel spectrum filters.
		 IntT numCep = 13,         // Number of ceptstrum coefficients to compute.
		 RealT preEmphasis = 0.97  // Pre emphasis alpha value.
		 )
      : DPEntityC(*new FeatureMFCCBodyC(sampleRate,frameRate,frameSize,freqRange,numMelFilters,numCep,preEmphasis) )
    {}
    //: Constructor.
    
  };
  
}


#endif
