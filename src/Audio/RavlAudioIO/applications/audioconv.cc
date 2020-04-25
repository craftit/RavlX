// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio;Ravl.Applications.Audio"
//! userlevel=Normal
//! file="Ravl/Audio/IO/audioconv.cc"

//: Audio file converter
// This program is designed to convert audio files between various format.
// It is more efficient than "conv" for this, and understands audio
// stream attributes like sample rate.

#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Audio/Types.hh"

using namespace RavlN;
using namespace RavlAudioN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  IntT bufSize = opt.Int("bs",512,"Buffer size. ");
  StringC ifmt = opt.String("if","","Input format.");
  StringC ofmt = opt.String("of","","Output format.");
  bool verbose = opt.Boolean("v",false,"Verbose. ");
  RealT len    = opt.Real("l",-1,"Length of track to copy. -1=Unlimited ");
  bool stereo = opt.Boolean("s",false,"Enable stereo. ");
  RealT reqSampleRate = opt.Real("sr",-1,"Sample rate. ");
  StringC idev = opt.String("","@DEVAUDIO:/dev/audio","Input  device.");
  StringC odev = opt.String("","@DEVAUDIO:/dev/audio","Output device.");
  opt.Check();
  
  if(!stereo) {
    SArray1dC<Int16T> buff(bufSize);
    
    DPIPortC<Int16T> in;
    if(!OpenISequence(in,idev,ifmt,verbose)) {
      cerr << "Failed to open input : " << idev << "\n";
      return 1;
    }
    
    DPOPortC<Int16T> out;
    if(!OpenOSequence(out,odev,ofmt,verbose)) {
      cerr << "Failed to open output : " << odev << "\n";
      return 1;
    }
    if(reqSampleRate > 0){
      if(!in.SetAttr("samplerate",reqSampleRate))
	cerr << "WARNING: Failed to set input sample rate. \n";
    }
    RealT sampleRate = reqSampleRate;
    if(!in.GetAttr("samplerate",sampleRate))
      cerr << "WARNING: Failed to get input sample rate. \n";
    out.SetAttr("samplerate",sampleRate);
    IntT samples = Round(sampleRate * len/ ((RealT) bufSize));
    cerr << "Sample rate = " << sampleRate << " Samples=" << samples << "\n";
    for(;samples != 0;samples--) {
      int len = in.GetArray(buff);
      if(len == 0)
        break;
      if(!out.PutArray(buff.From(0,len))) {
	cerr << "Failed to write array of data. \n";
	break;
      }
    }
  } else {
    SArray1dC<SampleElemC<2,Int16T> > buff(bufSize);
    
    DPIPortC<SampleElemC<2,Int16T> > in;
    if(!OpenISequence(in,idev,ifmt,verbose)) {
      cerr << "Failed to open input : " << idev << "\n";
      return 1;
    }
    
    DPOPortC<SampleElemC<2,Int16T> > out;
    if(!OpenOSequence(out,odev,ofmt,verbose)) {
      cerr << "Failed to open output : " << odev << "\n";
      return 1;
    }
    if(reqSampleRate > 0){
      if(!in.SetAttr("samplerate",reqSampleRate))
	cerr << "WARNING: Failed to set input sample rate. \n";
    }
    RealT sampleRate = reqSampleRate;
    if(!in.GetAttr("samplerate",sampleRate))
      cerr << "WARNING: Failed to find input sample rate. \n";
    out.SetAttr("samplerate",sampleRate);
    IntT samples = Round(sampleRate * len / ((RealT) bufSize));
    cerr << "Sample rate = " << sampleRate << " No=" << samples << "\n";
    for(;samples != 0;samples--) {
      int len = in.GetArray(buff);
      if(len == 0)
	break;
      if(!out.PutArray(buff.From(0,len))) {
	cerr << "Failed to write array of data. \n";
	break;
      }
    }
  }
  
  return 0;
}
