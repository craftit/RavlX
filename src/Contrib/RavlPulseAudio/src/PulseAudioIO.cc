// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPulseAudio
//! file="Ravl/Contrib/Audio/PulseAudio/PulseAudioIO.cc"

#include "Ravl/Audio/PulseAudioIO.hh"
#include "Ravl/Audio/PulseAudioFormat.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/SysLog.hh"

#include <string.h>
#include <pulse/error.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  using namespace RavlN;
  
  static pa_sample_format_t SampleFormat(const type_info &obj_type) {
    if(obj_type == typeid(SampleElemC<8,Int16T>))
      return PA_SAMPLE_S16LE;
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return PA_SAMPLE_S16LE;
    if(obj_type == typeid(SampleElemC<1,Int16T>))
      return PA_SAMPLE_S16LE;
    if(obj_type == typeid(SampleElemC<8,float>))
      return PA_SAMPLE_FLOAT32LE;
    if(obj_type == typeid(SampleElemC<2,float>))
      return PA_SAMPLE_FLOAT32LE;
    if(obj_type == typeid(SampleElemC<1,float>))
      return PA_SAMPLE_FLOAT32LE;

    RavlError("Format %s ",RavlN::TypeName(obj_type));
    RavlAssertMsg(0,"Unknown type");
    return PA_SAMPLE_S16LE;
  }


  //: Constructor.
  
  PulseAudioBaseC::PulseAudioBaseC(const StringC &fileName,int nchannel,bool nforInput,const type_info &ndtype)
    : m_stream(0),
      m_doneSetup(false),
      m_dtype(&ndtype),
      m_forInput(nforInput),
      m_bufferSize(4096)

  {
    m_sampleSpec.channels = FileFormatPulseAudioBodyC::Channels(ndtype);
    m_sampleSpec.rate = 44100;
    m_sampleSpec.format = SampleFormat(ndtype);
  }

  //: Destructor.
  
  PulseAudioBaseC::~PulseAudioBaseC() {
    if(m_stream != 0) {
      pa_simple_free(m_stream);
      m_stream = 0;
    }
  }
  
  //: Build Attributes 
  bool PulseAudioBaseC::BuildAttributes ( AttributeCtrlBodyC & attributes )
  {
    // build parent attributes
    AudioIOBaseC::BuildAttributes( attributes ) ;
    return true ;
  }
  
  //: Open audio device.
  
  bool PulseAudioBaseC::IOpen() {
    ONDEBUG(RavlDebug("PulseAudioBaseC::IOpen(), Called. "));
    m_doneSetup = true;

    pa_buffer_attr bufferSetup;
    bufferSetup.fragsize = m_bufferSize;
    bufferSetup.maxlength = (uint32_t) -1;
    bufferSetup.minreq = (uint32_t) -1;
    bufferSetup.prebuf = (uint32_t) -1;
    bufferSetup.tlength = (uint32_t) -1;

    //RavlN::MutexLockC lock(PulseAudioMutex());

    m_stream = pa_simple_new(NULL, // Use the default server.
                            "RAVL", // Our application's name.
                            PA_STREAM_RECORD,
                            NULL, // Use the default device.
                            "Music", // Description of our stream.
                            &m_sampleSpec, // Our sample format.
                            NULL, // Use default channel map
                            &bufferSetup, // Use default buffering attributes.
                            NULL // Ignore error code.
                            );


    if(m_stream == 0) {
      RavlError("Failed to open input. ");
      return false;
    }

    return true;
  }
  
  //: Open audio device.
  
  bool PulseAudioBaseC::OOpen() {
    ONDEBUG(RavlDebug("PulseAudioBaseC::OOpen(), Called. "));
    m_doneSetup = true;

    pa_buffer_attr bufferSetup;
    //bufferSetup.fragsize = m_bufferSize;
    bufferSetup.fragsize = (uint32_t) -1;
    bufferSetup.maxlength = (uint32_t) -1;
    bufferSetup.minreq = (uint32_t) -1;
    bufferSetup.prebuf = (uint32_t) -1;
    bufferSetup.tlength = m_bufferSize;

    m_stream = pa_simple_new(NULL, // Use the default server.
                            "RAVL", // Our application's name.
                            PA_STREAM_PLAYBACK,
                            NULL, // Use the default device.
                            "Music", // Description of our stream.
                            &m_sampleSpec, // Our sample format.
                            NULL, // Use default channel map
                            NULL, // Use default buffering attributes.
                            NULL // Ignore error code.
                            );

    if(m_stream == 0) {
      RavlError("Failed to open input. ");
      return false;
    }
    return true;
  }
  
  //: Set number of bits to use in samples.
  // returns actual number of bits.
  
  bool PulseAudioBaseC::SetSampleBits(IntT bits) {
    ONDEBUG(RavlDebug("SetSampleBits called."));
    return false;
  }
  
  //: Set frequency of samples
  // Returns actual frequency.
  
  bool PulseAudioBaseC::SetSampleRate(RealT rate)
  {
    if(m_doneSetup) {
      RavlWarning("Stream already open, sample rate may not change.");
    }
    m_sampleSpec.rate = RavlN::Round(rate);
    return true;
  }
  
  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool PulseAudioBaseC::GetSampleBits(IntT &bits) {
    ONDEBUG(RavlDebug("GetSampleBits(IntT &bits)"));

    return false;
  }

  //: Get frequency of samples
  // Returns actual frequency.
  
  bool PulseAudioBaseC::GetSampleRate(RealT &rate) {
    ONDEBUG(RavlDebug("GetSampleRate(RealT &rate)")) ;
    rate = m_sampleSpec.rate;
    return true;
  }
  
  //: Read bytes from audio stream.
  // Returns false if error occurred.
  
  bool PulseAudioBaseC::Read(void *buf,IntT &len) {
    if(!m_doneSetup) {
      if(!IOpen())
        return false;
    }
    if(m_stream == 0) {
      // Error should have already been reported.
      return false;
    }

    int err = 0;
    if(pa_simple_read  (m_stream,
                          buf,
                          len,
                          &err) < 0) {
      RavlError("Failed to read audio data %s ",pa_strerror(err));
      return false;
    }

    return true;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occurred.
  
  bool PulseAudioBaseC::Write(const void *buf,IntT len) {
    if(!m_doneSetup) {
      if(!OOpen())
        return false;
    }
    if(m_stream == 0)
      return false;

    int err = 0;
    if(pa_simple_write  (m_stream,
                          buf,
                          len,
                          &err) < 0) {
      RavlError("Failed to read audio data %s ",pa_strerror(err));
      return false;
    }

    return true;
  }


  //: Seek to location in stream.
  
  bool PulseAudioBaseC::Seek(UIntT off) {
    return false;
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT PulseAudioBaseC::Tell() const {
    return ((UIntT) (-1));
  }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT PulseAudioBaseC::Size() const {
    return ((UIntT) (-1));
  }
  
}
