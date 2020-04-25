// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioUtil
//! file="Ravl/Audio/Util/AudioIO.cc"

#include "Ravl/Audio/AudioIO.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  //: Constructor.
  
  AudioIOBaseC::AudioIOBaseC() {
  }
   
  //: Constructor.
  
  AudioIOBaseC::AudioIOBaseC(const StringC &fn,int channel,bool forInput,const type_info &type) {
  }
  
  //: Destructor.
  
  AudioIOBaseC::~AudioIOBaseC() {
  }
  
  //: Open audio device.
  
  bool AudioIOBaseC::IOpen(const StringC &fn,int channel,const type_info &dtype) {
    return false;
  }
  
  //: Open audio device.
  
  bool AudioIOBaseC::OOpen(const StringC &fn,int channel,const type_info &dtype) {
    return false;
  }
  
  //: Set number of bits to use in samples.
  // returns actual number of bits.
  
  bool AudioIOBaseC::SetSampleBits(IntT bits) {
    ONDEBUG(cerr << "\nAudioIOBaseC::SetSampleBits(IntT bits)" ) ; 
    return false;
  }
  
  //: Set fequence of samples
  // Returns actual frequency.
  
  bool AudioIOBaseC::SetSampleRate(RealT rate) {
    ONDEBUG(cerr << "\nAudioIOBaseC::SetSampleRate(RealT rate)") ; 
    return false;
  }

  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool AudioIOBaseC::GetSampleBits(IntT &bits) {
    ONDEBUG(cerr << "\nAudioIOBaseC::GetSampleBits(IntT &bits)") ;
    return false;
  }
  
  //: Get fequence of samples
  // Returns actual frequency.
  
  bool AudioIOBaseC::GetSampleRate(RealT &rate) {
    ONDEBUG(cerr << "\nAudioIOBaseC::GetSampleRate(RealT &rate)") ;
    return false;
  } 
  
  //: Read bytes from audio stream.
  // Returns false if error occurred.
  
  bool AudioIOBaseC::Read(void *buf,IntT &len) {
    return false;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occurred.
  
  bool AudioIOBaseC::Write(const void *buf,IntT len) {
    return false;
  }
  
  //: Handle get attrib.
  
  bool AudioIOBaseC::HandleGetAttr(const StringC &attrName,StringC &attrValue) {
      if(attrName == "samplerate") {
      RealT rval;
      if(!GetSampleRate(rval)) return false;
      attrValue = StringC(rval);
      return true;
    }

    if (attrName == "samplebits") {
      IntT ival ; 
      bool ret = false ; 
      ret = GetSampleBits(ival) ;
      if (!ret) return false ;
      else {
        attrValue = StringC(ival) ;
        return true ;
      }
    }
   return false;
  }
 
  
  //: Handle Set attrib.
  bool AudioIOBaseC::HandleSetAttr(const StringC &attrName,const StringC &attrValue) {
    ONDEBUG(cerr << "\nAudioIOBaseC::HandleSetAttr(const StringC &, const StringC & ), '" << attrName << "' = " << attrValue << "\n");
    if(attrName == "samplerate")
      return SetSampleRate(attrValue.RealValue());
    
    if(attrName == "samplebits") 
      return SetSampleBits(attrValue.IntValue() ) ; 
    return false;
} 

  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as sample rate.
  bool AudioIOBaseC::HandleGetAttr(const StringC &attrName,IntT &attrValue)
  {
    if(attrName == "samplerate") {
      RealT value;
      if(!GetSampleRate(value))
        return false;
      attrValue = Round(value);
      return true;
    }

    if (attrName == "samplebits") {
      IntT value ; 
      if (!GetSampleBits(value) ) 
        return false ;
      attrValue = value ; 
      return true ; 
    }
    return false;
  }
 

  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as sample rate.  
  bool AudioIOBaseC::HandleSetAttr(const StringC &attrName,const IntT &attrValue) {
    ONDEBUG(cerr << "\nAudioIOBaseC::HandleSetAttr(const StringC &, const IntT & ), '" << attrName << "' = " << attrValue << "\n");
    if(attrName == "samplerate")
      return SetSampleRate((RealT) attrValue);
  
    if(attrName == "samplebits")
      return SetSampleBits (attrValue) ; 
    return false;
    }
  
    
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  bool AudioIOBaseC::HandleGetAttr(const StringC &attrName,RealT &attrValue) {
    if(attrName == "samplerate") 
      return GetSampleRate(attrValue);
  
    if(attrName == "samplebits") 
      {
	IntT value ; 
	if (!GetSampleBits(value) ) return false ; 
	attrValue = value ; 
	return true ; 
      } 
    return false;
  }
 


  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool AudioIOBaseC::HandleSetAttr(const StringC &attrName,const RealT &attrValue) {  
    ONDEBUG(cerr << "\nAudioIOBaseC::HandleSetAttr(const STringC &, const RealT &), '" << attrName << "' = " << attrValue << "\n");
    if(attrName == "samplerate")
      return SetSampleRate(attrValue);

    if (attrName == "samplebits") 
      return SetSampleBits(IntT(attrValue) ) ; 
    return false;
  }


  
  //: Build the attributes 
  bool AudioIOBaseC::BuildAttributes( AttributeCtrlBodyC & attr ) 
  {
    attr.RegisterAttribute( AttributeTypeNumC<RealT> ("samplerate", "The sample rate of the audio signal",  true,  true,   0  ) ) ;
    attr.RegisterAttribute( AttributeTypeNumC<IntT>   ("samplebits", "The number of bits per sample  ", true , true, 0 ) ) ;
    return true ;
  }

  
  //: Seek to location in stream.
  // Returns false, if seek failed. (Maybe because its
  // not implemented.)
  bool AudioIOBaseC::Seek(UIntT off) 
  { return false; }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT AudioIOBaseC::Tell() const
  { return (UIntT) -1; }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT AudioIOBaseC::Size() const
  { return (UIntT) -1; }
  


  //: Seek to location in stream.
  bool AudioIOBaseC::Seek64(StreamPosT off)
  {
    if(off < 0)
      return false;
    if(off > (1<<31))
      return false;
    return Seek(off);
  }

  //: Find current location in stream.
  // May return ((UInt64T) (-1)) if not implemented.
  StreamPosT AudioIOBaseC::Tell64() const
  {
    return Tell();
  }

  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UInt64T) (-1)) if not implemented.
  StreamPosT AudioIOBaseC::Size64() const
  {
    return Size();
  }

}
