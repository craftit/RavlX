// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_DEVAUDIOIO_HEADER
#define RAVLAUDIO_DEVAUDIOIO_HEADER 1
//! rcsid="$Id$"
//! lib=RavlDevAudio
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO.Unix"
//! file="Ravl/Contrib/Audio/DevAudio/DevAudioIO.hh"

#include "Ravl/DP/Port.hh"
#include "Ravl/Audio/AudioIO.hh"

namespace RavlAudioN {

  //! userlevel=Advanced
  //: /dev/audio IO base class.
  
  class DevAudioBaseC 
    : public AudioIOBaseC
  {
  public:
    DevAudioBaseC();
    //: Default constructor.
    
    DevAudioBaseC(const StringC &fn,int channel,bool forInput,const type_info &dtype);
    //: Default constructor.
    
    ~DevAudioBaseC();
    //: Destructor .
    
    void ReadCaps();
    //: Check device capabilities.
    
    bool IOpen(const StringC &fn,int channel,const type_info &dtype);
    //: Open audio device.
    
    bool OOpen(const StringC &fn,int channel,const type_info &dtype);
    //: Open audio device.
    
    virtual bool SetSampleBits(IntT bits);
    //: Set number of bits to use in samples.
    // returns actual number of bits.
 
    virtual bool SetSampleRate(RealT rate);
    //: Set frequency of samples
    // Returns actual frequency.
    
    virtual bool GetSampleBits(IntT &bits);
    //: Get number of bits to use in samples.
    // returns actual number of bits.
    
    virtual bool GetSampleRate(RealT &rate);
    //: Get frequency of samples
    // Returns actual frequency.
    
    bool SetupChannels(const type_info &dtype);
    //: Setup number of channels
    
    bool Read(void *buf,IntT &len);
    //: Read bytes from audio stream.
    // Returns false if error occured.
    
    bool Write(const void *buf,IntT len);
    //: Write bytes to audio stream.
    // Returns false if error occured.
    
    bool IsOpen() const
    { return audiofd >= 0; }
    //: Is stream open ?
    
  protected:
    int audiofd; // File handle for sound channel.
    int mixerfd; // File handle for mixer.
  };
  

}


#endif
