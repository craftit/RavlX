// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_AUDIOFILEIO_HEADER
#define RAVL_AUDIOFILEIO_HEADER 1
//! docentry="Ravl.API.Audio.IO.Audio File"
//! author="Charles Galambos"
//! lib=RavlAudioFile
//! file="Ravl/Contrib/Audio/AudioFile/AudioFileIO.hh"
//! example =exAudioFile.cc

#include "Ravl/DP/Port.hh"
#include "Ravl/Audio/AudioIO.hh"

#include <audiofile.h>

namespace RavlAudioN {
  
  //! userlevel=Advanced
  //: AudioFile library driver class.
  
  class AudioFileBaseC 
    : public AudioIOBaseC
  {
  public:
    AudioFileBaseC(const StringC &fileName,int channel,bool forInput,const type_info &dtype);
    //: Constructor
    
    ~AudioFileBaseC();
    //: Destructor.
    
    virtual bool BuildAttributes ( AttributeCtrlBodyC & attributes ) ; 
    //: Build the list of attributes 

    bool IOpen(const StringC &fn,int channel,const type_info &dtype);
    //: Open audio device.
    
    bool OOpen(const StringC &fn,int channel,const type_info &dtype);
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
    { return handle != 0; }
    //: Is stream open ?
    
    bool Seek(UIntT off);
    //: Seek to location in stream.
    
    virtual UIntT Tell() const;
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const;
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    virtual bool Seek64(StreamPosT off);
    //: Seek to location in stream.

    virtual StreamPosT Tell64() const;
    //: Find current location in stream.
    // May return ((UInt64T) (-1)) if not implemented.
    
    virtual StreamPosT Size64() const;
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UInt64T) (-1)) if not implemented.

  protected:
    AFfilehandle handle;  
    AFfilesetup setup;
    UIntT frameSize;
    IntT channel;
    const type_info *dtype;
    StringC fileName;
    RealT sampleRate;
    bool forInput;
  };
  

  
}

#endif
