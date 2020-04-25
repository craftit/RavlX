// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioFeatures
//! file="Ravl/Audio/Features/MelSpectrum.cc"

#include "Ravl/Audio/MelSpectrum.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Vector.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {

  //: Default constructor.
  
  MelSpectrumC::MelSpectrumC()
   : m_expand(false)
  {}
  
  //: Constructor
  // Frequency's is in Hz.
  
  MelSpectrumC::MelSpectrumC(RealT sampleRate,IntT frameSize,RealRangeC freqRange,IntT nfilters)
   : m_expand(false)
  {
    InitFilters(sampleRate,frameSize,freqRange,nfilters);
  }

  //: XMLFactory constructor.
  
  MelSpectrumC::MelSpectrumC(const XMLFactoryContextC &factory)
   : DPProcessBodyC<SArray1dC<RealT>,SArray1dC<RealT> >(factory),
     m_expand(factory.AttributeBool("expandBins",false))
  {
    RealT sampleRate = factory.AttributeReal("sampleRate",8000);
    IntT frameSize = factory.AttributeUInt("frameSize",512);
    IntT nfilters = factory.AttributeUInt("numberOfFilter",14);
    RealRangeC freqRange(factory.AttributeReal("minFrequency",133.333334),
                         factory.AttributeReal("maxFrequency",6855.4976)
                         );
    InitFilters(sampleRate,frameSize,freqRange,nfilters);
  }

  //: Compute mel spectrum from power spectrum
  
  SArray1dC<RealT> MelSpectrumC::Apply(const SArray1dC<RealT> &powerSpectrum) {
    SArray1dC<RealT> ret(filters.Size());
    Array1dC<RealT> spec(powerSpectrum);
    for(SArray1dIter2C<Array1dC<float>,RealT > it(filters,ret);it;it++) {
      RealT sum = 0;
      for(Array1dIter2C<float,RealT> fit(it.Data1(),spec,it.Data1().Range());fit;fit++)
	sum += fit.Data1() * fit.Data2();
      it.Data2() = sum;
    }
    
    return ret;
  }
  
  //: Compute mel spectrum from power spectrum
  SArray1dC<float> MelSpectrumC::Apply(const SArray1dC<float> &powerSpectrum)
  {
    SArray1dC<float> ret(filters.Size());
    Array1dC<float> spec(powerSpectrum);
    for(SArray1dIter2C<Array1dC<float>,float> it(filters,ret);it;it++) {
      float sum = 0;
      for(Array1dIter2C<float,float> fit(it.Data1(),spec,it.Data1().Range());fit;fit++)
        sum += fit.Data1() * fit.Data2();
      it.Data2() = sum;
    }
    return ret;
  }

  //: Initialise filters.
  // Frequency's is in Hz.
  
  bool MelSpectrumC::InitFilters(RealT sampleRate,IntT frameSize,RealRangeC freqRange,IntT numFilters,bool verbose)
  {
    // Compute edge values for filters.
    
    float dfreq = sampleRate / frameSize;
    if(verbose) {
      RavlInfo("Bin frequency width %f ",dfreq);
    }
    SArray1dC<float> edges(numFilters+2);
    {
      RealRangeC melRng(Lin2Mel(freqRange.Min()),Lin2Mel(freqRange.Max()));
      RealT rStep = melRng.Size() / (RealT)(numFilters+1);
      RealT at = melRng.Min();
      // Compute edge frequency's and round them to fit nicely to results of dft
      for(SArray1dIterC<float> it(edges);it;it++,at += rStep) {
        if(m_expand) {
          *it = (RealT) Mel2Lin(at);
        } else {
          *it = (RealT) Round(Mel2Lin(at) / dfreq) * dfreq;
        }
      }
      //cerr << "Edges=" << edges << "\n";
    }
    
    // Compute values for filter banks
    
    filters = SArray1dC<Array1dC<float> >(numFilters);
    {
      int i = 0;
      for(SArray1dIterC<Array1dC<float> > it(filters);it;it++,i++) {
        Array1dC<float> &filt = *it;
        IntT imin;
        IntT imax;
        if(m_expand) {
          imin = Floor(edges[i]/dfreq)+1;
          imax = Ceil(edges[i+2]/dfreq)-1;
        } else {
          imin = Round(edges[i]/dfreq)+1;
          imax = Round(edges[i+2]/dfreq)-1;
        }
        if(imax < imin)
          imax = imin;
        filt = Array1dC<float>(imin,imax);

        float left = edges[i];
        float cent = edges[i+1];
        float right = edges[i+2];
        float width = right - left;
        float height = (2 * dfreq)/width;
        float slope1 = height / (cent - left);
        float slope2 = height / (cent - right);
        float freq = (filt.Range().Min().V()) * dfreq;

        if(verbose) {
          RavlInfo("Frequency bin range %s ",RavlN::StringOf(filt.Range()).c_str());
          RavlInfo("Cut offs:  %f %f %f ",left,cent,right);
          RavlInfo("Width:%f Slope1:%f Slope2:%f ",width,slope1,slope2);
        }

        if(filt.Size() == 1) { // Range small ?
          filt[filt.IMin()] = 1.0;
          if(verbose) {
            RavlInfo("Single bin!");
          }
        } else {
          for(Array1dIterC<float> fit(filt);fit;fit++) {
            if(freq <= cent)
              *fit = (freq - left) * slope1;
            else
              *fit = (freq - right) * slope2;
            freq += dfreq;
          }
          if(verbose)
            RavlInfo("Filter:%s",RavlN::StringOf(VectorC(filt.SArray1d(true))).c_str());
        }
      }
    }
    return true;
  }
  
  void LinkMelSpectrum()
  {}

  static RavlN::XMLFactoryRegisterConvertC<MelSpectrumC,DPProcessBodyC<SArray1dC<RealT>,SArray1dC<RealT> > > g_registerVectorDelta012("RavlAudioN::MelSpectrumC");


}
