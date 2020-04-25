// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioFeatures
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! userlevel=Normal
//! file="Ravl/Audio/Features/exFeatureMFCC.cc"
#include "Ravl/config.h" 
#include "Ravl/Option.hh"
#include "Ravl/Audio/FeatureMFCC.hh"
#include "Ravl/DP/SequenceIO.hh"

#if RAVL_COMPILER_MIPSPRO 
#pragma instantiate RavlN::DPOPortBodyC<RavlN::VectorC>
#endif

using namespace RavlN;
using namespace RavlAudioN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  RealT sampleRate = opts.Int("sr",0,"Sample rate. ");
  StringC inFile = opts.String("","@DEVAUDIO:/dev/dsp","Audio input. ");
  StringC outFile = opts.String("","","Output for features. ");
  opts.Check();
  
  DPIPortC<Int16T> inp;
  if(!OpenISequence(inp,inFile)) {
    std::cerr << "Failed to open input file '" << inFile << "'\n";
    return 1;
  }
  
  DPOPortC<VectorC> outp;
  if(!outFile.IsEmpty()) {
    if(!OpenOSequence(outp,outFile)) {
      std::cerr << "Failed to open output file '" << outFile << "'\n";
      return 1;
    }
  }
  RealT reportedRate;
  if(sampleRate == 0) {
    inp.GetAttr("samplerate",sampleRate);
  } else {
    inp.SetAttr("samplerate",sampleRate);
  }
  FeatureMFCCC fextract(sampleRate);
  fextract.Input() = inp;
  VectorC vec;
  while(1) {
    if(!fextract.Get(vec))
      break;
#if 1
    for(UIntT i = 0;i < vec.Size();i++)
      cout << Round(vec[i] * 20) << " ";
    cout << "\n";
#else
    //cout << vec << "\n";
#endif
    if(outp.IsValid())
      outp.Put(vec);
  }
  
  return 0;
}
