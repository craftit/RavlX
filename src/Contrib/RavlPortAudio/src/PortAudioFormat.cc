// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! docentry="Ravl.Audio.Audio IO.PortAudio"
//! lib=RavlPortAudio
//! file="Ravl/Contrib/Audio/PortAudio/PortAudioFormat.cc"

#include "Ravl/Audio/PortAudioFormat.hh"
#include "Ravl/Audio/PortAudioIO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SysLog.hh"
#include <portaudio.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  //: Access port audio mutex.
  RavlN::MutexC &PortAudioMutex() {
    static RavlN::MutexC *mutex = new RavlN::MutexC();
    return *mutex;
  }

  void InitPortAudioFormat()
  {}
  
  // PortAudio ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatPortAudioBodyC::FileFormatPortAudioBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("PortAudio driver.")),
      m_isOpen(false)
  {
    ONDEBUG(std::cerr << "Registering format. \n");
  }
  
  //: Destructor.
  FileFormatPortAudioBodyC::~FileFormatPortAudioBodyC()
  {
    RavlN::MutexLockC lock(PortAudioMutex());
    if(m_isOpen) {
      m_isOpen = false;
      Pa_Terminate();
    }
    lock.Unlock();
  }

  const type_info &FileFormatPortAudioBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }

  
  const type_info &
  FileFormatPortAudioBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(RavlDebug("ProbeLoad(), Checking file type. %s for '%s' ",RavlN::TypeName(obj_type),filename.c_str()));

    StringC devType = ExtractDevice(filename);
    ONDEBUG(RavlDebug("Got type %s ",devType.c_str()));
    if(devType != "PORTAUDIO") {
      ONDEBUG(RavlDebug("Not recognised."));
      return typeid(void);
    }
    StringC device = ExtractParams(filename);
    ONDEBUG(RavlDebug("Got device '%s' ",device.c_str()));
    int devId = 0;
    const PaDeviceInfo *devInfo = FindDevice(device,false,devId);

    // Device found ?
    if(devInfo == 0) {
      RavlError("Requested device %s not found. ",device.c_str());
      return typeid(void);
    }
    int channels = devInfo->maxInputChannels;
    if(channels == 0) {
      RavlError("No channels found. Input:%d Output:%d",(int) devInfo->maxInputChannels,(int) devInfo->maxOutputChannels);
      return typeid(void);
    }
    
    int reqChannels = Channels(obj_type);

    if(reqChannels > channels)
      reqChannels = channels;

    bool isFloat = IsFloat(obj_type);

    return SampleType(isFloat,reqChannels);
  }

  const type_info &
  FileFormatPortAudioBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    ONDEBUG(RavlDebug("ProbeSave(), Checking file type. %s for '%s' ",RavlN::TypeName(obj_type),filename.c_str()));

    StringC devType = ExtractDevice(filename);
    ONDEBUG(RavlDebug("Got type %s ",devType.c_str()));
    if(devType != "PORTAUDIO") {
      ONDEBUG(RavlDebug("Not recognised."));
      return typeid(void);
    }
    StringC device = ExtractParams(filename);
    ONDEBUG(RavlDebug("Got device '%s' ",device.c_str()));
    int devId = 0;
    const PaDeviceInfo *devInfo = FindDevice(device,false,devId);

    // Device found ?
    if(devInfo == 0)
      return typeid(void);
    int channels = devInfo->maxOutputChannels;
    if(channels == 0)
      return typeid(void);

    // FIXME:- Handle requested types better.

    int reqChannels = Channels(obj_type);
    if(reqChannels > channels)
      reqChannels = channels;

    bool isFloat = IsFloat(obj_type);
    return SampleType(isFloat,reqChannels);
  }

  //: Work out number of channels from type.
  int FileFormatPortAudioBodyC::Channels(const type_info &obj_type) {
    if ( (obj_type == typeid ( SampleElemC<1,Int16T> )) || (obj_type == typeid(SampleElemC<1,RealT>)) ||
         (obj_type == typeid ( SampleElemC<1,UByteT>)) || (obj_type == typeid(SampleElemC<1,IntT>)) ||
         (obj_type == typeid(SampleElemC<1,float>) ) ||
         obj_type == typeid(float) || obj_type == typeid(Int16T))
         return 1;
    if ( (obj_type == typeid ( SampleElemC<2,Int16T> )) || (obj_type == typeid(SampleElemC<2,RealT>)) ||
         (obj_type == typeid ( SampleElemC<2,UByteT>)) || (obj_type == typeid(SampleElemC<2,IntT>)) ||
         (obj_type == typeid(SampleElemC<2,float>) ) )
         return 2;
    if ( (obj_type == typeid ( SampleElemC<8,Int16T> )) || (obj_type == typeid(SampleElemC<8,RealT>)) ||
         (obj_type == typeid ( SampleElemC<8,UByteT>)) || (obj_type == typeid(SampleElemC<8,IntT>)) ||
         (obj_type == typeid(SampleElemC<8,float>) ) )
         return 8;
    RavlError("Can't find the number of channels from %s ",TypeName(obj_type));
    RavlAssertMsg(0,"Unknown type");
    return 0;
  }

  //: Work out if we want float or Int16 samples.
  bool FileFormatPortAudioBodyC::IsFloat(const type_info &obj_type) {
    if( (obj_type == typeid(SampleElemC<1,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<2,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<3,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<4,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<5,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<6,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<7,Int16T>) ) ||
        (obj_type == typeid(SampleElemC<8,Int16T>) ) ||
        (obj_type == typeid(Int16T) )
        )
      return false;
    return true;
  }

  const type_info &FileFormatPortAudioBodyC::SampleType(bool isFloat, int channels) {
    if(isFloat) {
      switch(channels)
      {
        case 1: return typeid(SampleElemC<1,float>);
        case 2: return typeid(SampleElemC<2,float>);
        case 8: return typeid(SampleElemC<8,float>);
      }
    } else {
      switch(channels)
      {
        case 1: return typeid(SampleElemC<1,Int16T>);
        case 2: return typeid(SampleElemC<2,Int16T>);
        case 8: return typeid(SampleElemC<8,Int16T>);
      }
    }
    RavlError("Format isFloat=%d Channels=%d not supported",(int)isFloat,channels);
    RavlAssertMsg(0,"Unknown type");
    return typeid(void);
  }

  //: How big is a frame.

  size_t FileFormatPortAudioBodyC::FrameSize(const type_info &obj_type) {
    if(obj_type == typeid(SampleElemC<8,Int16T>))
      return sizeof(SampleElemC<8,Int16T>);
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return sizeof(SampleElemC<2,Int16T>);
    if(obj_type == typeid(SampleElemC<1,Int16T>))
      return sizeof(SampleElemC<1,Int16T>);

    if(obj_type == typeid(SampleElemC<8,float>))
      return sizeof(SampleElemC<8,float>);
    if(obj_type == typeid(SampleElemC<2,float>))
      return sizeof(SampleElemC<2,float>);
    if(obj_type == typeid(SampleElemC<1,float>))
      return sizeof(SampleElemC<1,float>);
    RavlError("Format %s not supported",RavlN::TypeName(obj_type));
    RavlAssertMsg(0,"Unknown type");
    return 0;
  }

  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatPortAudioBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPortAudioBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatPortAudioBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatPortAudioBodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    int devId = 0;
    StringC device = ExtractParams(filename);
    if(FindDevice(device,false,devId) == 0)
      return DPIPortBaseC();

    // 1 channel
    if(obj_type == typeid(SampleElemC<1,float> ))
      return DPIAudioC<SampleElemC<1,float>,PortAudioBaseC>(filename,devId);

    // 2 channels
    if(obj_type == typeid(SampleElemC<2,float> ))
      return DPIAudioC<SampleElemC<2,float>,PortAudioBaseC>(filename,devId);

    // 8 channel
    if(obj_type == typeid(SampleElemC<8,float> ))
      return DPIAudioC<SampleElemC<8,float>,PortAudioBaseC>(filename,devId);

    // 1 channel 16 bit 
    if(obj_type == typeid(SampleElemC<1,Int16T> ))
      return DPIAudioC<SampleElemC<1,Int16T>,PortAudioBaseC>(filename,devId);
    
    // 2 channels 16 bit 
    if(obj_type == typeid(SampleElemC<2,Int16T> ))
      return DPIAudioC<SampleElemC<2,Int16T>,PortAudioBaseC>(filename,devId);

    // 8 channel 16 bit 
    if(obj_type == typeid(SampleElemC<8,Int16T> ))
      return DPIAudioC<SampleElemC<8,Int16T>,PortAudioBaseC>(filename,devId);
   
    // unknown 
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatPortAudioBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { 
    ONDEBUG(RavlDebug("FileFormatPortAudioBodyC::CreateOutput(const StringC &,const type_info &), Called. "));
    int devId = 0;
    StringC device = ExtractParams(filename);
    if(FindDevice(device,false,devId) == 0) {
      RavlError("Failed to find device '%s' ",device.c_str());
      return DPOPortBaseC();
    }

    if(obj_type == typeid(SampleElemC<8,Int16T>))
      return DPOAudioC<SampleElemC<8,Int16T>,PortAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return DPOAudioC<SampleElemC<2,Int16T>,PortAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<1,Int16T>))
      return DPOAudioC<SampleElemC<1,Int16T>, PortAudioBaseC> (filename,devId);

    if(obj_type == typeid(SampleElemC<8,float>))
      return DPOAudioC<SampleElemC<8,float>,PortAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<2,float>))
      return DPOAudioC<SampleElemC<2,float>,PortAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<1,float>))
      return DPOAudioC<SampleElemC<1,float>, PortAudioBaseC> (filename,devId) ;

    RavlError("Sample type '%s' not known.",TypeName(obj_type));

    return DPOPortBaseC();
  }
  
  //: Get preferred IO type.
  
  const type_info &FileFormatPortAudioBodyC::DefaultType() const
  { return typeid ( SampleElemC<2,float>) ; }
  

  //: Find a named device
  const PaDeviceInfo *FileFormatPortAudioBodyC::FindDevice(const StringC &devName,bool forInput,int &devId) const {
    RavlN::MutexLockC lock(PortAudioMutex());
    if(!m_isOpen) {
      PaError errCode = Pa_Initialize();
      if(errCode != paNoError) {
        RavlError("Error initialising port audio %d ",errCode);
        return 0;
      }
      m_isOpen = true;
    }

    if(devName.IsEmpty()) {
      PaDeviceIndex ind;
      if(forInput) {
        ind = Pa_GetDefaultInputDevice();
        if(ind < 0) {
          RavlWarning("AudioPort failed to find default input device");
          // Failed to find appropriate device.
          return 0;
        }
      } else {
        ind = Pa_GetDefaultOutputDevice();
        if(ind < 0) {
          RavlWarning("AudioPort failed to find default output device");
          // Failed to find appropriate device.
          return 0;
        }
      }
      devId = ind;
      ONDEBUG(RavlDebug("Using default device %d ",(int) devId));
      return Pa_GetDeviceInfo(ind);
    }

    int numDevices = Pa_GetDeviceCount();
    for(int i = 0;i < numDevices;i++) {
      const PaDeviceInfo *xdevInfo = 0;
      xdevInfo = Pa_GetDeviceInfo(i);
      // Does it match the device number ?
      if(StringC(i) == devName || devName == xdevInfo->name) {
        devId = i;
        return xdevInfo;
      }
    }

    RavlInfo("Failed to find device '%s'. Available devices are: ",devName.c_str());
    for(int i = 0;i < numDevices;i++) {
      const PaDeviceInfo *xdevInfo = 0;
      xdevInfo = Pa_GetDeviceInfo(i);
      RavlInfo(" %d '%s' Inputs:%d Outputs:%d Default sample rate:%f ",
          i,
          xdevInfo->name,
          xdevInfo->maxInputChannels,
          xdevInfo->maxOutputChannels,
          xdevInfo->defaultSampleRate);
    }

    return 0;
  }

  static FileFormatPortAudioC RegisterFileFormatPortAudio("PortAudio");
}
