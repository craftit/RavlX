// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! docentry="Ravl.Audio.Audio IO.PulseAudio"
//! lib=RavlPulseAudio
//! file="Ravl/Contrib/Audio/PulseAudio/PulseAudioFormat.cc"

#include "Ravl/Audio/PulseAudioFormat.hh"
#include "Ravl/Audio/PulseAudioIO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SysLog.hh"


#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  //: Access port audio mutex.
  RavlN::MutexC &PulseAudioMutex() {
    static RavlN::MutexC *mutex = new RavlN::MutexC();
    return *mutex;
  }

  void InitPulseAudioFormat()
  {}
  
  // PulseAudio ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatPulseAudioBodyC::FileFormatPulseAudioBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("PulseAudio driver.")),
      m_isOpen(false)
  {
    ONDEBUG(std::cerr << "Registering format. \n");
  }
  
  //: Destructor.
  FileFormatPulseAudioBodyC::~FileFormatPulseAudioBodyC()
  {
    RavlN::MutexLockC lock(PulseAudioMutex());
    if(m_isOpen) {
      m_isOpen = false;
    }
    lock.Unlock();
  }

  const type_info &FileFormatPulseAudioBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }

  
  const type_info &
  FileFormatPulseAudioBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(RavlDebug("ProbeLoad(), Checking file type. %s for '%s' ",RavlN::TypeName(obj_type),filename.c_str()));

    StringC devType = ExtractDevice(filename);
    ONDEBUG(RavlDebug("Got type %s ",devType.c_str()));
    if(devType != "PULSEAUDIO") {
      ONDEBUG(RavlDebug("Not recognised."));
      return typeid(void);
    }
    StringC device = ExtractParams(filename);
    ONDEBUG(RavlDebug("Got device '%s' ",device.c_str()));
    
    int reqChannels = Channels(obj_type);
    bool isFloat = IsFloat(obj_type);

    return SampleType(isFloat,reqChannels);
  }

  const type_info &
  FileFormatPulseAudioBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    ONDEBUG(RavlDebug("ProbeSave(), Checking file type. %s for '%s' ",RavlN::TypeName(obj_type),filename.c_str()));

    StringC devType = ExtractDevice(filename);
    ONDEBUG(RavlDebug("Got type %s ",devType.c_str()));
    if(devType != "PULSEAUDIO") {
      ONDEBUG(RavlDebug("Not recognised."));
      return typeid(void);
    }
    StringC device = ExtractParams(filename);
    ONDEBUG(RavlDebug("Got device '%s' ",device.c_str()));

    int reqChannels = Channels(obj_type);
    bool isFloat = IsFloat(obj_type);
    return SampleType(isFloat,reqChannels);
  }

  //: Work out number of channels from type.
  int FileFormatPulseAudioBodyC::Channels(const type_info &obj_type) {
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
  bool FileFormatPulseAudioBodyC::IsFloat(const type_info &obj_type) {
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

  const type_info &FileFormatPulseAudioBodyC::SampleType(bool isFloat, int channels) {
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

  size_t FileFormatPulseAudioBodyC::FrameSize(const type_info &obj_type) {
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
    RavlError("Format isFloat=%d Channels=%d not supported",RavlN::TypeName(obj_type));
    RavlAssertMsg(0,"Unknown type");
    return 0;
  }

  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatPulseAudioBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatPulseAudioBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatPulseAudioBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatPulseAudioBodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    int devId = 0;
    StringC device = ExtractParams(filename);

    // 1 channel
    if(obj_type == typeid(SampleElemC<1,float> ))
      return DPIAudioC<SampleElemC<1,float>,PulseAudioBaseC>(filename,devId);

    // 2 channels
    if(obj_type == typeid(SampleElemC<2,float> ))
      return DPIAudioC<SampleElemC<2,float>,PulseAudioBaseC>(filename,devId);

    // 8 channel
    if(obj_type == typeid(SampleElemC<8,float> ))
      return DPIAudioC<SampleElemC<8,float>,PulseAudioBaseC>(filename,devId);

    // 1 channel 16 bit 
    if(obj_type == typeid(SampleElemC<1,Int16T> ))
      return DPIAudioC<SampleElemC<1,Int16T>,PulseAudioBaseC>(filename,devId);
    
    // 2 channels 16 bit 
    if(obj_type == typeid(SampleElemC<2,Int16T> ))
      return DPIAudioC<SampleElemC<2,Int16T>,PulseAudioBaseC>(filename,devId);

    // 8 channel 16 bit 
    if(obj_type == typeid(SampleElemC<8,Int16T> ))
      return DPIAudioC<SampleElemC<8,Int16T>,PulseAudioBaseC>(filename,devId);
   
    // unknown 
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatPulseAudioBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { 
    ONDEBUG(RavlDebug("FileFormatPulseAudioBodyC::CreateOutput(const StringC &,const type_info &), Called. "));
    int devId = 0;
    StringC device = ExtractParams(filename);

    if(obj_type == typeid(SampleElemC<8,Int16T>))
      return DPOAudioC<SampleElemC<8,Int16T>,PulseAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return DPOAudioC<SampleElemC<2,Int16T>,PulseAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<1,Int16T>))
      return DPOAudioC<SampleElemC<1,Int16T>, PulseAudioBaseC> (filename,devId);

    if(obj_type == typeid(SampleElemC<8,float>))
      return DPOAudioC<SampleElemC<8,float>,PulseAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<2,float>))
      return DPOAudioC<SampleElemC<2,float>,PulseAudioBaseC>(filename,devId);
    if(obj_type == typeid(SampleElemC<1,float>))
      return DPOAudioC<SampleElemC<1,float>, PulseAudioBaseC> (filename,devId) ;

    RavlError("Sample type '%s' not known.",TypeName(obj_type));

    return DPOPortBaseC();
  }
  
  //: Get preferred IO type.
  
  const type_info &FileFormatPulseAudioBodyC::DefaultType() const
  { return typeid ( SampleElemC<2,float>) ; }
  

  static FileFormatPulseAudioC RegisterFileFormatPulseAudio("PulseAudio");
}
