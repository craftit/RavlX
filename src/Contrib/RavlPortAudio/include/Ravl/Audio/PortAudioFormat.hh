// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PORTAUDIOFORMAT_HEADER
#define RAVL_PORTAUDIOFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO.PortAudio"
//! example=exAudioIO.cc
//! lib=RavlPortAudio
//! file="Ravl/Contrib/Audio/PortAudio/PortAudioFormat.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Threads/Mutex.hh"
#include <portaudio.h>

namespace RavlAudioN {
  using namespace RavlN;

  //: Access port audio mutex.
  RavlN::MutexC &PortAudioMutex();
  
  //! userlevel=Develop
  //: PortAudio File format information.
  
  class FileFormatPortAudioBodyC
    : public FileFormatBodyC 
  {
  public:
    FileFormatPortAudioBodyC(const StringC &vName);
    //: Constructor.
    
    ~FileFormatPortAudioBodyC();
    //: Destructor.

    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain PortAudio stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain PortAudio stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

    static int Channels(const type_info &obj_type);
    //: Work out number of channels from type.

    static bool IsFloat(const type_info &obj_type);
    //: Work out if we want float or Int16 samples.

    static size_t FrameSize(const type_info &obj_type);
    //: How big is a frame.
  protected:

    static const type_info &SampleType(bool isFloat, int channels);

    const PaDeviceInfo *FindDevice(const StringC &devName,bool forInput,int &inputId) const;
    //: Find a named device

    mutable bool m_isOpen;
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a PortAudio File Format.
  
  class FileFormatPortAudioC
    : public FileFormatC<Int16T > 
  {
  public:
    FileFormatPortAudioC(const StringC &vName)
      : FileFormatC<Int16T>(*new FileFormatPortAudioBodyC(vName))
    {}
  };
  
}
#endif
