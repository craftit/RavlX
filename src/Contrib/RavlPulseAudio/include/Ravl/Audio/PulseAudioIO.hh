// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PulseAudioIO_HEADER
#define RAVL_PulseAudioIO_HEADER 1
//! docentry="Ravl.API.Audio.IO.PulseAudio"
//! author="Charles Galambos"
//! lib=RavlPulseAudio
//! file="Ravl/Contrib/Audio/PulseAudio/PulseAudioIO.hh"
//! example =exPulseAudio.cc

#include "Ravl/DP/Port.hh"
#include "Ravl/Audio/AudioIO.hh"

#include <pulse/simple.h>

namespace RavlAudioN {
  
  //! userlevel=Advanced
  //: PulseAudio library driver class.
  
  class PulseAudioBaseC
    : public AudioIOBaseC
  {
  public:
    PulseAudioBaseC(const StringC &fileName,int channel,bool forInput,const type_info &dtype);
    //: Constructor
    
    ~PulseAudioBaseC();
    //: Destructor.
    
    virtual bool BuildAttributes ( AttributeCtrlBodyC & attributes ) ; 
    //: Build the list of attributes 

    bool IOpen();
    //: Open audio device.
    
    bool OOpen();
    //: Open audio device.
    
    bool SetupChannel(int channel,const type_info &dtype);
    //: Setup IO.
    
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
    
    bool Read(void *buf,IntT &len);
    //: Read bytes from audio stream.
    // Returns false if error occurred.
    
    bool Write(const void *buf,IntT len);
    //: Write bytes to audio stream.
    // Returns false if error occurred.
    
    bool IsOpen() const
    { return m_stream != 0; }
    //: Is stream open ?
    
    bool Seek(UIntT off);
    //: Seek to location in stream.
    
    UIntT Tell() const;
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    UIntT Size() const;
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
  protected:
    pa_simple *m_stream;
    pa_sample_spec m_sampleSpec;

    bool m_doneSetup;

    const std::type_info *m_dtype;
    bool m_forInput;
    int m_bufferSize;
  };
  

  
}

#endif
