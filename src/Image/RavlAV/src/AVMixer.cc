// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAV
//! author="Charles Galambos"
#include "Ravl/config.h" 
#include "Ravl/AVMixer.hh"
//! file="Ravl/Image/AV/AVMixer.cc"

#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/Audio/Types.hh"
#pragma instantiate RavlN::DPIPortBodyC<RavlImageN::ImageC<RavlImageN::ByteRGBValueC> >
#pragma instantiate RavlN::DPIPortBodyC<RavlAudioN::SampleElemC<(unsigned int)2,short> >
#endif 

namespace RavlImageN {
  
  //: Default constructor.
  
  AVMixerBodyC::AVMixerBodyC()
    : audioSamplesPerFrame(1),
      audioFreq(1),
      videoFrameRate(1)
  {}
  
  //: Constructor.
  
  AVMixerBodyC::AVMixerBodyC(const DPISPortC<ImageC<ByteRGBValueC> > &vidPort,const DPISPortC<SampleElemC<2,Int16T> > &audioPort)
    : in1(vidPort),
      in2(audioPort),
      audioSamplesPerFrame(1),
      audioFreq(1),
      videoFrameRate(1)
  { Setup(); }
  
  //: Setup rates.
  
  bool AVMixerBodyC::Setup() {
    RealT videoFrameRate;
    if(!in1.GetAttr("framerate",videoFrameRate)) {
      cerr << "WARNING: Failed to get video frame rate, assuming 25hz \n";
      videoFrameRate = 25;
    }
    if(!in1.GetAttr("samplerate",audioFreq)) {
      cerr << "WARNING: Failed to get audio sample rate, assuming 44Khz \n";
      audioFreq = 44000;
    }
    audioSamplesPerFrame = Round(audioFreq/videoFrameRate);
    return true;
  }
  
  //: Is some data ready ?
  // true = yes.
  
  bool AVMixerBodyC::IsGetReady() const {
    if(!in1.IsValid() || !in2.IsValid())
      return false;
    return in1.IsGetReady() && in2.IsGetReady();
  }
    
  //: Has the End Of Stream been reached ?
  // true = yes.
  
  bool AVMixerBodyC::IsGetEOS() const {
    if(!in1.IsValid() || !in2.IsValid())
      return true;
    return in1.IsGetEOS() && in2.IsGetEOS();
  }
  
  //: List input ports.
  
  DListC<DPIPortBaseC> AVMixerBodyC::IPorts() const {
    DListC<DPIPortBaseC> lst = DPStreamOpBodyC::IPorts();
    lst.InsLast(DPIPortBaseC((DPIPortBaseBodyC &)*this));
    return lst;
  }
  
  //: List input plugs.
  
  DListC<DPIPlugBaseC> AVMixerBodyC::IPlugs() const {
    DListC<DPIPlugBaseC> lst = DPStreamOpBodyC::IPlugs();
    lst.InsLast(DPIPlugC<ImageC<ByteRGBValueC> >(in1,"Video",DPEntityC((DPEntityBodyC &)*this)));
    lst.InsLast(DPIPlugC<SampleElemC<2,Int16T> >(in2,"Audio",DPEntityC((DPEntityBodyC &)*this)));
    return lst;
  }
    
  //: Process next piece of data.
  
  AVFrameC AVMixerBodyC::Get()  { 
    AVFrameC ret;
    if(!Get(ret))
      throw DataNotReadyC("Get failed. ");
    return ret;
  }
  
  //: Process some data.
  
  bool AVMixerBodyC::Get(AVFrameC &outbuff) { 
    SArray1dC<SampleElemC<2,Int16T> > audio(Round(audioSamplesPerFrame));
    if(in2.GetArray(audio) <= 0)
      return false;
    ImageC<ByteRGBValueC> img;
    if(!in1.Get(img))
      return false;
    outbuff = AVFrameC(AudioFrameC(audio,audioFreq),img);
    return true;
  }
  
  //: Seek to location in stream.
  
  bool AVMixerBodyC::Seek(UIntT off) {
    in2.Seek(Round(audioSamplesPerFrame * (RealT) off));
    return in1.Seek(off);
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool AVMixerBodyC::DSeek(IntT off) {
    in2.DSeek(Round(audioSamplesPerFrame * (RealT) off));
    return in1.DSeek(off);
  }
  
  //: Find current location in stream.
  
  UIntT AVMixerBodyC::Tell() const {
    return in1.Tell();
  }
  
  //: Find the total size of the stream. (assuming it starts from 0)
  
  UIntT AVMixerBodyC::Size() const {
    return in1.Size();
  }
  
  //: Find the offset where the stream begins, normally zero.
  
  UIntT AVMixerBodyC::Start() const {
    return in1.Start();
  }

}
