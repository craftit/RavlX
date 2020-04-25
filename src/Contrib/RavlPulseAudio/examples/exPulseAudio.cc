// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPulseAudio
//! author="Lee Gregory"
//! docentry="Ravl.API.Audio.IO"
//! file="Ravl/Contrib/Audio/PulseAudio/exPulseAudio.cc"

#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Audio/Types.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/DP/PrintIOInfo.hh"

using namespace RavlN;
using namespace RavlAudioN ; 

// This example reads from a wav file and outputs to a sound device.
// This example needs to be linked against RavlDevAudio as well as RavlPulseAudio.


int main(int nargs,char **argv) {
  
  // get some options
  OptionC opt(nargs,argv);
  bool verbose = opt.Boolean("v",false,"Verbose ");
  StringC idev = opt.String("i","test.wav","Input  device.");
  StringC odev = opt.String("o","@PULSEAUDIO","Output device.");
  bool listFormats = opt.Boolean("lf",false,"List formats ");
  double sampleRate = opt.Real("sr",0,"Sample rate to use.");
  opt.Check();

  if(listFormats) {
    PrintIOFormats();
    return 0;
  }


  RavlN::SysLogOpen("exPulseAudio",false,true,true,-1,true);

  // open the input port 
  DPIPortC<Int16T> in;
  if(!OpenISequence(in,idev,"",verbose)) {
    RavlError("Failed to open input : %s ",idev.c_str());
    return 1;
  }
  
  
  // now lets setup an output port 
  DPOPortC<Int16T > out;
  if(!OpenOSequence(out,odev,"",verbose)) {
    RavlError("Failed to open output : %s ",odev.c_str());
    return 1;
  }
 
  
  // lets look at the attributes available 
  DListC<AttributeTypeC> attrList ; 
  in.GetAttrTypes(attrList) ; 
  //cout << "\nAvailable Attributes are :\n" << attrList ;
#if 1
  // lets get some attributes 
  if(sampleRate != 0) {
    if(!in.SetAttr("samplerate", sampleRate)) {
      RavlWarning("Failed to set sample rate.");
    }
  } else {
    in.GetAttr("samplerate", sampleRate) ;
  }
    
  // now lets read data from file and play to device
  RavlInfo("Using a sample rate of %f ",sampleRate);
  if(!out.SetAttr("samplerate",sampleRate)) {
    RavlError("Failed to set output sample rate.");
  }
#endif

#if 0
  RavlN::SArray1dC<SampleElemC<2,float> > samples(16);
  while (in.GetArray(samples)) {
    out.PutArray(samples) ; // play sample
  }
#else
  RavlN::SArray1dC<Int16T > samples(32);
  while (in.GetArray(samples)) {
    out.PutArray(samples) ; // play sample
  }
#endif
  RavlN::Sleep(1);
  return 0;
}
