// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioFeatures
//! docentry="Ravl.API.Audio.Feature Extraction"
//! userlevel=Develop
//! author="Charles Galambos"
//! file="Ravl/Audio/Features/testAudioFeatures.cc"

#include "Ravl/Audio/MelSpectrum.hh"
#include "Ravl/Audio/MelCepstrum.hh"
#include "Ravl/Audio/FeatureMFCC.hh"
#include "Ravl/Audio/VectorDelta012.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/DP/ListIO.hh"
#include "Ravl/Random.hh"

using namespace RavlAudioN;

int testMelSpectrum();
int testMelCepstrum();
int testFeatureMFCC();
int testVectorDelta012();

int main() {
  int ln;
  if((ln = testMelSpectrum()) != 0) {
    cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testMelCepstrum()) != 0) {
    cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testFeatureMFCC()) != 0) {
    cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  if((ln = testVectorDelta012()) != 0) {
    cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  return 0;
}

int testMelSpectrum() {
  int specSize = 512;
  
  RealRangeC freqRange(133.333,6855.4976);
  MelSpectrumC mel(16000,specSize,freqRange,40);
  
  //: Check Lin2Mel & Mel2Lin.
  
  for(RealT t = 10;t < 2000;t += 10) {
    RealT v = mel.Lin2Mel(mel.Mel2Lin(t));
    if(Abs(v - t) > 0.001) {
      cerr << "v=" << v << " t=" << t << "\n";
      return __LINE__;
    }
  }
  
  // Test response to flat spectrum.
  
  SArray1dC<RealT> spec(specSize);
  spec.Fill(1);
  SArray1dC<RealT> melspec = mel.Apply(spec);
  //cerr << "Mel=" << melspec << "\n";
  
  // Should be unit spectrum.
  
  for(SArray1dIterC<RealT> it(melspec);it;it++)
    if(Abs(*it - 1) > 0.01) return __LINE__;
  
  return 0;
}

int testMelCepstrum() {
  MelCepstrumC cep(13,40);
  SArray1dC<RealT> spec(40);
  RealT i = 0;
  
  for(SArray1dIterC<RealT> it(spec);it;it++,i++) {
    *it = Cos(i * RavlConstN::pi / 20) * 100 + 101;
    cerr << " " << *it ;
  }
  cerr << "\n";
  
  SArray1dC<RealT> cspec = cep.Apply(spec);
  cerr << "Cepstrum=" << cspec << "\n";
  
  return 0;
}

int testFeatureMFCC() {
  cerr << "testFeatureMFCC(), Called. \n";
  cerr.precision(2);
  for(int i = 0;i < 3;i++) {
    DListC<Int16T> list;
    FeatureMFCCC fextract(16000);
    switch(i) {
    case 0: {
      cerr << "Sine wave.\n";
      for(int i= 0;i < 32000;i++)
        list.InsLast(Round(Sin(i *(200 / RavlConstN::pi)) * 32000));
    } break;
    case 1: {
      cerr << "Noise.\n";
      for(int i= 0;i < 32000;i++)
        list.InsLast((RandomInt() % 64000)-32000);
    } break;
    case 2: {
      cerr << "Zero.\n";
      for(int i= 0;i < 32000;i++)
        list.InsLast(0);
    } break;
    }
    DPISListC<Int16T> ip(list);
    
    fextract.Input() = ip;
    VectorC v1;
    for(int i = 0;i < 20;i++) {
      fextract.Get(v1);
      for(SArray1dIterC<RealT> it(v1);it;it++)
        if(*it < 0.001)
          *it = 0;
      cerr << "v1=" << v1 << "\n";
    }
  }
  return 0;
}

int testVectorDelta012() {
  cerr << "testVectorDelta012(), Called. \n";
  VectorDelta012C vecDelta(3);
  for(int i = 0;i < 10;i++) {
    VectorC vec(3);
    vec[0] = 1;
    vec[1] = i;
    vec[2] = Sqr(i);
    VectorC delta = vecDelta.Apply(vec);
    if(delta.Size() != (vec.Size() * 3))
      return __LINE__;
    cerr << delta << "\n";
  }
  return 0;
}

