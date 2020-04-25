// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_SPHEREIO_HEADER
#define RAVLAUDIO_SPHEREIO_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO"
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/SphereIO.hh"

#include "Ravl/Audio/Types.hh"
#include "Ravl/Audio/AudioIO.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Hash.hh"

namespace RavlAudioN {

  //! userlevel=Advanced
  //: /dev/audio IO base class.
  
  class SphereBaseC 
    : public AudioIOBaseC
  {
  public:
    SphereBaseC();
    //: Default constructor.
    
    SphereBaseC(const StringC &fn,int channel,bool forInput,const type_info &dtype);
    //: Default constructor.
    
    ~SphereBaseC();
    //: Destructor .
    
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
    { return bis.Stream() && !endOfFile; }
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
    HashC<StringC,StringC> attribs;
    RealT sampleRate;
    IntT bits;
    UInt16T dataOffset;
    BinIStreamC bis;
    bool endOfFile;
  };
  
  
}


#endif
