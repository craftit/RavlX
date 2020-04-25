// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPortAudio
//! file="Ravl/Contrib/Audio/PortAudio/PortAudioIO.cc"

#include "Ravl/Audio/PortAudioIO.hh"
#include "Ravl/Audio/PortAudioFormat.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/SysLog.hh"

#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  using namespace RavlN;
  
  //: Constructor.
  
  PortAudioBaseC::PortAudioBaseC(const StringC &fileName,int nchannel,bool nforInput,const std::type_info &ndtype)
    : m_stream(0),
      m_doneSetup(false),
      m_latency(0),
      m_frameSize(0),
      m_channel(nchannel),
      m_dtype(&ndtype),
      m_sampleRate(16000),
      m_forInput(nforInput),
      m_framerPerBuffer(0), // FIXME:- Should be an attribute.
      m_frameCount(0),
      m_reportedOverflowFrame(-10000)
  {
    RavlN::MutexLockC lock(PortAudioMutex());
    const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(m_channel);
    if(devInfo == 0) {
      RavlError("No information found about device!");
      // FIXME:- Throw exception?
      return ;
    }
    m_sampleRate = devInfo->defaultSampleRate;
    RavlDebug("Opening device '%s' Inputs:%u Outputs:%u SampleFormat:%s Rate:%f ",
        devInfo->name,
        devInfo->maxInputChannels,devInfo->maxOutputChannels,
        RavlN::TypeName(ndtype),m_sampleRate);
#if 0
    if(nforInput) {
      m_latency = devInfo->defaultLowInputLatency;
    } else {
      m_latency = devInfo->defaultLowOutputLatency;
    }
#else
    // Go for a medium latency by default .
    if(nforInput) {
      m_latency = (devInfo->defaultHighInputLatency + devInfo->defaultLowInputLatency)/2.0;
    } else {
      m_latency = (devInfo->defaultHighOutputLatency + devInfo->defaultLowInputLatency)/2.0;
    }
#endif
    m_frameSize = FileFormatPortAudioBodyC::FrameSize(ndtype);
    ONDEBUG(RavlDebug("Default latency %f frame size %u ",m_latency,(unsigned) m_frameSize));
  }

  //: Destructor.
  
  PortAudioBaseC::~PortAudioBaseC() {
    if(m_stream != 0) {
      RavlN::MutexLockC lock(PortAudioMutex());
      Pa_CloseStream(m_stream);
      m_stream = 0;
    }
  }
  
  //: Build Attributes 
  bool PortAudioBaseC::BuildAttributes ( AttributeCtrlBodyC & attributes )
  {
    // build parent attributes
    AudioIOBaseC::BuildAttributes( attributes ) ;

    // cant set attributes when reading from a file
    if ( m_forInput) {
      AttributeTypeC
        sampleRate = attributes.GetAttrType("samplerate") ,
        sampleBits  = attributes.GetAttrType("samplebits") ;
      if ( sampleRate.IsValid() ) sampleRate.CanWrite(false) ;
      if ( sampleBits.IsValid() ) sampleBits.CanWrite(false) ;
    }
    return true ;
  }
  
  //: Open audio device.
  
  // Setup parameters
  bool PortAudioBaseC::SetupParameters(PaStreamParameters &inputParameters) {
    memset(&inputParameters,0,sizeof(inputParameters));

    inputParameters.device = m_channel;
    inputParameters.channelCount = FileFormatPortAudioBodyC::Channels(*m_dtype);
    inputParameters.sampleFormat = FileFormatPortAudioBodyC::IsFloat(*m_dtype) ? paFloat32 : paInt16;
    inputParameters.suggestedLatency = m_latency ;

    return true;
  }

  bool PortAudioBaseC::IOpen() {
    ONDEBUG(RavlDebug("PortAudioBaseC::IOpen(), Called. "));
    m_doneSetup = true;

    PaStreamParameters inputParameters;
    SetupParameters(inputParameters);

    RavlN::MutexLockC lock(PortAudioMutex());

    PaError err = Pa_OpenStream(&m_stream,
            &inputParameters,
            0,
            m_sampleRate,
            m_framerPerBuffer,
            paNoFlag,
            0, // User callback
            0  // User data
             );

    if(err != paNoError) {
      RavlError("Failed to open output. Error: %s ",Pa_GetErrorText (err));
      //if(err == paUnanticipatedHostError) {
        const PaHostErrorInfo *lastHostError = Pa_GetLastHostErrorInfo();
        RavlError(" APIId:%u Code:%d Error text:%s ",
            (unsigned) lastHostError->hostApiType,
            (int) lastHostError->errorCode ,
            lastHostError->errorText);
      //}

      return false;
    }

    err = Pa_StartStream(m_stream);
    if(err != paNoError) {
      RavlError("Failed to start output. Error: %s ",Pa_GetErrorText (err));
      return false;
    }

    return true;
  }
  
  //: Open audio device.
  
  bool PortAudioBaseC::OOpen() {
    ONDEBUG(RavlDebug("PortAudioBaseC::OOpen(), Called. "));
    m_doneSetup = true;
    PaStreamParameters outputParameters;
    SetupParameters(outputParameters);

    RavlN::MutexLockC lock(PortAudioMutex());

    PaError err = Pa_OpenStream(&m_stream,
            0,
            &outputParameters,
            m_sampleRate,
            m_framerPerBuffer,
            paNoFlag,
            0, // User callback
            0  // User data
             );

    if(err != paNoError) {
      RavlError("Failed to open output. Error: %s ",Pa_GetErrorText (err));
      return false;
    }

    err = Pa_StartStream(m_stream);
    if(err != paNoError) {
      RavlError("Failed to start output. Error: %s ",Pa_GetErrorText (err));
      return false;
    }

    return true;
  }
  
  //: Set number of bits to use in samples.
  // returns actual number of bits.
  
  bool PortAudioBaseC::SetSampleBits(IntT bits) {
    ONDEBUG(RavlDebug("SetSampleBits called."));
    return false;
  }
  
  //: Set frequency of samples
  // Returns actual frequency.
  
  bool PortAudioBaseC::SetSampleRate(RealT rate)
  {
    if(m_doneSetup) {
      RavlWarning("Stream already open, sample rate may not change.");
    }
    ONDEBUG(RavlDebug("SetSampleRate called."));
    PaStreamParameters ioParameters;
    SetupParameters(ioParameters);
    PaError err;
    RavlN::MutexLockC lock(PortAudioMutex());
    if(m_forInput) {
      err = Pa_IsFormatSupported( &ioParameters, 0, rate);
    } else {
      err = Pa_IsFormatSupported( 0, &ioParameters, rate);
    }
    lock.Unlock();
    if(err != paNoError) {
      RavlWarning("Failed to set sample rate to %f. Error:%s ",rate,Pa_GetErrorText (err));
      return false;
    }
    m_sampleRate = rate;
    return true;
  }
  
  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool PortAudioBaseC::GetSampleBits(IntT &bits) {
    ONDEBUG(RavlDebug("GetSampleBits(IntT &bits)"));

    return false;
  }

  //: Get frequency of samples
  // Returns actual frequency.
  
  bool PortAudioBaseC::GetSampleRate(RealT &rate) {
    ONDEBUG(RavlDebug("GetSampleRate(RealT &rate)")) ;
    if(m_stream == 0) {
      rate = m_sampleRate;
      return true;
    }
    RavlN::MutexLockC lock(PortAudioMutex());
    const PaStreamInfo *streamInfo =Pa_GetStreamInfo(m_stream);
    lock.Unlock();
    if(streamInfo == 0) {
      RavlError("Failed to find information about stream.");
      return false;
    }
    rate = streamInfo->sampleRate;
    return true;
  }
  
  //: Read bytes from audio stream.
  // Returns false if error occurred.
  
  bool PortAudioBaseC::Read(void *buf,IntT &len) {
    if(!m_doneSetup) {
      if(!IOpen())
        return false;
    }
    if(m_stream == 0)
      return false;
    if((len % m_frameSize) != 0) {
      RavlError("Miss aligned read! frameSize=%d len=%d ",m_frameSize,len);
      return false;
    }
    int blocks = len / m_frameSize;
    m_frameCount++;
    PaError err = Pa_ReadStream(m_stream, buf, blocks);
    if(err == paInputOverflowed) {
      // Throttle the frequency of this message so it doesn't interfere with catching up...
      if(m_reportedOverflowFrame < (m_frameCount - 5)) {
        RavlWarning("Overflow reading from stream. Requested %d blocks with frame size %d. Error: %s ",blocks,(int) m_frameSize,Pa_GetErrorText (err));
        m_reportedOverflowFrame = m_frameCount;
      }
    } else if(err != paNoError) {
      RavlError("Problem reading from stream. Error: %s ",Pa_GetErrorText (err));
      return false;
    }
    return true;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occurred.
  
  bool PortAudioBaseC::Write(const void *buf,IntT len) {
    if(!m_doneSetup) {
      if(!OOpen())
        return false;
    }
    if(m_stream == 0)
      return false;
    if((len % m_frameSize) != 0) {
      RavlError("Miss aligned write! frameSize=%d len=%d ",m_frameSize,len);
      return false;
    }
    int blocks = len / m_frameSize;

    m_frameCount++;
    PaError err = Pa_WriteStream(m_stream, buf, blocks);
    if(err != paNoError) {
      RavlError("Error writing to stream. Error: %s ",Pa_GetErrorText (err));
      return false;
    }
    return true;
  }


  //: Seek to location in stream.
  
  bool PortAudioBaseC::Seek(UIntT off) {
    return false;
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT PortAudioBaseC::Tell() const {
    return ((UIntT) (-1));
  }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT PortAudioBaseC::Size() const {
    return ((UIntT) (-1));
  }
  
}
