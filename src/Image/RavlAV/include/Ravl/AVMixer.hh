// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AVMIXER_HEADER
#define RAVLIMAGE_AVMIXER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAV
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video"
//! file="Ravl/Image/AV/AVMixer.hh"

#include "Ravl/AVFrame.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/StreamProcess.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Mix Audio and Video data into an AVFrame.
  
  class AVMixerBodyC
    : public DPStreamOpBodyC,
      public DPISPortBodyC<AVFrameC>
  {
  public:
    AVMixerBodyC(const DPISPortC<ImageC<ByteRGBValueC> > &vidPort,const DPISPortC<SampleElemC<2,Int16T> > &audioPort);
    //: Constructor.
    
    AVMixerBodyC();
    //: Default constructor.
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const;
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DListC<DPIPortBaseC> IPorts() const;
    //: List input ports.
    
    virtual DListC<DPIPlugBaseC> IPlugs() const;
    //: List input plugs.
    
    virtual AVFrameC Get();
    //: Process next piece of data.
    
    virtual bool Get(AVFrameC &outbuff);
    //: Process some data.
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    
    virtual bool DSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const; 
    //: Find current location in stream.
    
    virtual UIntT Size() const; 
    //: Find the total size of the stream. (assuming it starts from 0)
    
    virtual UIntT Start() const; 
    //: Find the offset where the stream begins, normally zero.
    
  protected:    
    bool Setup();
    //: Setup rates.
    
    DPISPortC<ImageC<ByteRGBValueC> > in1;
    DPISPortC<SampleElemC<2,Int16T> > in2;
    RealT audioSamplesPerFrame;
    RealT audioFreq;
    RealT videoFrameRate;
  };
  
  //! userlevel=Normal
  //: Mix Audio and Video data into an AVFrame.
  
  class AVMixerC
    : public DPStreamOpC,
      public DPISPortC<AVFrameC>
  {
  public:
    AVMixerC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Create an invalid object.
    
    AVMixerC(const DPISPortC<ImageC<ByteRGBValueC> > &vidPort,const DPISPortC<SampleElemC<2,Int16T> > &audioPort)
      : DPEntityC(*new AVMixerBodyC(vidPort,audioPort))
    {}
    //: Constructor.
    
    
  };
  
  
}

#endif
