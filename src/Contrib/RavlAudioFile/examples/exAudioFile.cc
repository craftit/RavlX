// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioFile
//! author="Lee Gregory"
//! docentry="Ravl.API.Audio.IO"
//! file="Ravl/Contrib/Audio/AudioFile/exAudioFile.cc"

#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Audio/Types.hh"
#include "Ravl/DP/PrintIOInfo.hh"
#include "Ravl/DP/AttributeType.hh"
using namespace RavlN;
using namespace RavlAudioN ; 

// This example reads from a wav file and outputs to a sound device.
// This example needs to be linked against RavlDevAudio as well as RavlAudioFile.


int main(int nargs,char **argv) {
  
  // get some options
  OptionC opt(nargs,argv);
  StringC idev = opt.String("i","test.wav","Input  device.");
  StringC odev = opt.String("o","@DEVAUDIO:/dev/dsp","Output device.");
  StringC format = opt.String("f","","Format.");
  bool stereo = opt.Boolean("s",false,"Stereo. ");
  bool verbose = opt.Boolean("v",false,"Verbose ");
  bool listFormats = opt.Boolean("lf",false,"List formats ");
  opt.Check();

  if(listFormats) {
    PrintIOFormats();
    return 0;
  }
  if(!stereo) {
    // open the input port
    DPIPortC<SampleElemC<1,Int16T> > in;
    if(!OpenISequence(in,idev,format,verbose)) {
      cerr << "Failed to open input : " << idev << "\n";
      return 1;
    }


    // now lets setup an output port
    DPOPortC<SampleElemC<1,Int16T> > out;
    if(!odev.IsEmpty()) {
      if(!OpenOSequence(out,odev,format,verbose)) {
        cerr << "Failed to open output : " << odev << "\n";
        return 1;
      }
    }


    // lets look at the attributes available
#if 0
    DListC<AttributeTypeC> attrList ;
    in.GetAttrTypes(attrList) ;
    cout << "\nAvailable Attributes are :\n" << attrList ;
#endif

    // lets get some attributes
    RealT sampleRate ;
    IntT  sampleBits ;
    in.GetAttr("samplerate", sampleRate) ;
    in.GetAttr("samplebits", sampleBits) ;
    cout << "\nSample rate is " << sampleRate << " and sample bits is " << sampleBits << "\n\n" ;

    // now lets read data from file and play to device
    if(out.IsValid())
      out.SetAttr("samplerate",sampleRate) ;
    SampleElemC<1,Int16T>  sample;
    while (in.Get(sample)) {
      if(out.IsValid())
        out.Put(sample) ; // play sample
    }
  } else {
    // open the input port
    DPIPortC<SampleElemC<2,Int16T> > in;
    if(!OpenISequence(in,idev,format,verbose)) {
      cerr << "Failed to open input : " << idev << "\n";
      return 1;
    }

    DPOPortC<SampleElemC<2,Int16T> > out;
    if(!odev.IsEmpty()) {
    // now lets setup an output port
      if(!OpenOSequence(out,odev,format,verbose)) {
        cerr << "Failed to open output : " << odev << "\n";
        return 1;
      }
    }


    // lets look at the attributes available
#if 0
    DListC<AttributeTypeC> attrList ;
    in.GetAttrTypes(attrList) ;
    cout << "\nAvailable Attributes are :\n" << attrList ;
#endif
    
    // lets get some attributes
    RealT sampleRate ;
    IntT  sampleBits ;
    in.GetAttr("samplerate", sampleRate) ;
    in.GetAttr("samplebits", sampleBits) ;
    cout << "\nSample rate is " << sampleRate << " and sample bits is " << sampleBits << "\n\n" ;

    // now lets read data from file and play to device
    if(out.IsValid())
      out.SetAttr("samplerate",sampleRate) ;
    SampleElemC<2,Int16T>  sample;
    while (in.Get(sample)) {
      if(out.IsValid())
        out.Put(sample) ; // play sample
    }
  }
  
  return 0;
}
