// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAudioFile
//! file="Ravl/Contrib/Audio/AudioFile/AudioFileIO.cc"

#include "Ravl/Audio/AudioFileIO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/TypeName.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  using namespace RavlN;
  
  //: Constructor.
  
  AudioFileBaseC::AudioFileBaseC(const StringC &fileName,int channel,bool nforInput,const type_info &ndtype)
    : handle(0),
      setup(0),
      frameSize(0),
      channel(1),
      dtype(0),
      sampleRate(16000),
      forInput(nforInput)
  {
    if(forInput)
      IOpen(fileName,channel,ndtype);
    else
      OOpen(fileName,channel,ndtype);      
  }

  //: Destructor.
  
  AudioFileBaseC::~AudioFileBaseC() {
    if(handle != 0)
      afCloseFile(handle);
    if(setup != 0)
      afFreeFileSetup(setup);
  }
  
  //: Build Attributes 
  bool AudioFileBaseC::BuildAttributes ( AttributeCtrlBodyC & attributes ) 
{
  // build parent attributes 
  AudioIOBaseC::BuildAttributes( attributes ) ; 
  
  // cant set attributes when reading from a file 
  if ( forInput) {  
    AttributeTypeC 
      sampleRate = attributes.GetAttrType("samplerate") , 
      sampleBits  = attributes.GetAttrType("samplebits") ; 
    if ( sampleRate.IsValid() ) sampleRate.CanWrite(false) ; 
    if ( sampleBits.IsValid() ) sampleBits.CanWrite(false) ;
  }
  return true ; 
}


  //: Setup IO
  bool AudioFileBaseC::SetupChannel(int channel,const type_info &ndtype) {
    if(ndtype == typeid(SampleElemC<1,UByteT>)) {
      afSetVirtualChannels(handle,AF_DEFAULT_TRACK,1);
      afSetVirtualSampleFormat(handle,AF_DEFAULT_TRACK,AF_SAMPFMT_TWOSCOMP, 8);
    } else if(ndtype == typeid(SampleElemC<1,Int16T>)) {
      afSetVirtualChannels(handle,AF_DEFAULT_TRACK,1);
      afSetVirtualSampleFormat(handle,AF_DEFAULT_TRACK,AF_SAMPFMT_TWOSCOMP, 16);
    } else if(ndtype == typeid(SampleElemC<2,Int16T>)) {
      afSetVirtualChannels(handle,AF_DEFAULT_TRACK,2);
      afSetVirtualSampleFormat(handle,AF_DEFAULT_TRACK,AF_SAMPFMT_TWOSCOMP, 16);
    } else if(ndtype == typeid(SampleElemC<2,float>)) {
      afSetVirtualChannels(handle,AF_DEFAULT_TRACK,2);
      afSetVirtualSampleFormat(handle,AF_DEFAULT_TRACK,AF_SAMPFMT_FLOAT, 32);
    } else if(ndtype == typeid(SampleElemC<1,float>)) {
      afSetVirtualChannels(handle,AF_DEFAULT_TRACK,1);
      afSetVirtualSampleFormat(handle,AF_DEFAULT_TRACK,AF_SAMPFMT_FLOAT, 32);
    } else {
      cerr << "AudioFileBaseC::IOpen(), Don't know to handle type " << TypeName(ndtype) << "\n";
      return false;
    }
    return true;
  }
  
  //: Open audio device.
  
  bool AudioFileBaseC::IOpen(const StringC &fn,int nchannel,const type_info &ndtype) {
    ONDEBUG(cerr << "AudioFileBaseC::IOpen(), Called. \n");
    if(ndtype == typeid(SampleElemC<1,UByteT> )) {
      frameSize = sizeof(SampleElemC<1,UByteT> );
    } else if  (ndtype == typeid(SampleElemC<2,UByteT> ) ) {
      frameSize = sizeof (SampleElemC<2,UByteT> ) ;  
    } else if(ndtype == typeid(SampleElemC<1,Int16T>)) {
      frameSize = sizeof(SampleElemC<1,Int16T>);
    } else if(ndtype == typeid(SampleElemC<2,Int16T>)) {
      frameSize = sizeof(SampleElemC<2,Int16T>);
    } else if(ndtype == typeid(SampleElemC<8,Int16T>)) {
      frameSize = sizeof(SampleElemC<8,Int16T>);
    } else if(ndtype == typeid(SampleElemC<1,float>)) {
      frameSize = sizeof(SampleElemC<1,float>);
    } else if(ndtype == typeid(SampleElemC<2,float>)) {
      frameSize = sizeof(SampleElemC<2,float>);
    } else {
      cerr << "AudioFileBaseC::IOpen(), ERROR: Unrecognised sample type:" << ndtype.name() << "\n";
      return false;
    }
    dtype = &ndtype;
    channel = nchannel;
    fileName = fn;
    handle = afOpenFile(fn.chars(),"r", AF_NULL_FILESETUP);
    if(handle != 0)
      return false;
    return SetupChannel(channel,ndtype);
  }
  
  //: Open audio device.
  
  bool AudioFileBaseC::OOpen(const StringC &fn,int nchannel,const type_info &ndtype) {
    ONDEBUG(cerr << "AudioFileBaseC::OOpen(), Called. \n");
    dtype = &ndtype;
    channel = nchannel;
    setup = afNewFileSetup();
    fileName = fn;
    if(setup == 0)
      return false;

    StringC tfn(fn);
    StringC ext = tfn.after('.');
    if(ext == "wav") {
      afInitFileFormat(setup, AF_FILE_WAVE);
    } else if(ext == "aiff") {
      afInitFileFormat(setup, AF_FILE_AIFF);
    } else if(ext == "aiffc") {
      afInitFileFormat(setup, AF_FILE_AIFFC);
    } else if(ext == "bicsf") {
      afInitFileFormat(setup, AF_FILE_BICSF);
    } else if(ext == "nextsnd") {
      afInitFileFormat(setup, AF_FILE_NEXTSND);
    } else if(ext == "au") {
      afInitFileFormat(setup, AF_FILE_RAWDATA);
    }

    IntT channels = 2 ;

    int format = 0;
    int width = 0;

    if(ndtype == typeid(SampleElemC<2,Int16T>)) {
      frameSize = sizeof(SampleElemC<2,Int16T>);
      channels = 2 ;
      format = AF_SAMPFMT_TWOSCOMP;
      width = 16;
    } else if(ndtype == typeid(SampleElemC<1,Int16T> )) {
      frameSize = sizeof(SampleElemC<1,Int16T> ) ;
      channels = 1 ;
      format = AF_SAMPFMT_TWOSCOMP;
      width = 16;
    } else if(ndtype == typeid(SampleElemC<2,float> )) {
      frameSize = sizeof(SampleElemC<2,float> ) ;
      channels = 2 ;
      format = AF_SAMPFMT_FLOAT;
      width = 32;
    } else if(ndtype == typeid(SampleElemC<1,float> )) {
      frameSize = sizeof(SampleElemC<1,float> ) ;
      channels = 1 ;
      format = AF_SAMPFMT_FLOAT;
      width = 32;
    } else {
      RavlError("ERROR: Unrecognised sample type: %s ",RavlN::TypeName(ndtype.name()));
      return false;
    }

    afInitSampleFormat(setup, AF_DEFAULT_TRACK, format,width);
    afInitChannels(setup, AF_DEFAULT_TRACK, channels);
    ONDEBUG(RavlDebug("OOpen done. Channels=%d Format=%d width=%d Type=%s ",channels,format,width,TypeName(ndtype)));
    return true;
  }
  
  //: Set number of bits to use in samples.
  // returns actual number of bits.
  
  bool AudioFileBaseC::SetSampleBits(IntT bits) {
    ONDEBUG(cerr << "AudioFileBaseC::SetSampleBits(IntT bits)\n") ;
    return false;
  }
  
  //: Set frequency of samples
  // Returns actual frequency.
  
  bool AudioFileBaseC::SetSampleRate(RealT rate) {
    ONDEBUG(cerr << "AudioFileBaseC::SetSampleRate(RealT rate)\n") ;
    if(forInput) {
      cerr << "AudioFileBaseC::SetSampleRate(), WARNING: Can't set sample rate for input. \n";
      return false;
    }
    sampleRate = rate ;
    return true;
  }
  
  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool AudioFileBaseC::GetSampleBits(IntT &bits) {
    ONDEBUG(cerr << "AudioFileBaseC::GetSampleBits(IntT &bits)\n");
    IntT rate ;
    afGetSampleFormat ( handle, AF_DEFAULT_TRACK, &rate, &bits) ;
    return true;
  }

  //: Get frequency of samples
  // Returns actual frequency.
  
  bool AudioFileBaseC::GetSampleRate(RealT &rate) {
    ONDEBUG(cerr << "AudioFileBaseC::GetSampleRate(RealT &rate)\n" ) ;
    if(!forInput) {
      rate = sampleRate;
      return true;
    }
    if(handle == 0)
      return false;
    rate = Round(afGetRate (handle,AF_DEFAULT_TRACK));
    return true;
  }
  
  //: Read bytes from audio stream.
  // Returns false if error occured.
  
  bool AudioFileBaseC::Read(void *buf,IntT &len) {
    RavlAssert((len % frameSize) == 0);
    len = afReadFrames(handle,AF_DEFAULT_TRACK,buf,len / frameSize);
    if(len < 0) {
      //...
      cerr << "AudioFileBaseC::Read(), Error reading data." << len << "\n";
      len = 0;
      return false;
    }
    len *= frameSize;
    return true;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occured.
  
  bool AudioFileBaseC::Write(const void *buf,IntT len) {
    //ONDEBUG(cerr << "AudioFileBaseC::Write(), Called. \n");
    if(handle == 0) {
      ONDEBUG (cerr << "\n handle is null, doing some setup, sample rate is  :" << sampleRate  ) ;
      RavlAssert(setup != 0);
      afInitRate(setup, AF_DEFAULT_TRACK, sampleRate);
      handle = afOpenFile(fileName.chars(),"w",setup);
      if(handle == 0)
        throw DataNotReadyC("AudioFileBaseC::Write(), Failed to open file for writing. ");
      SetupChannel(channel,*dtype);
    }
    RavlAssert((len % frameSize) == 0);
    IntT ret = afWriteFrames(handle,AF_DEFAULT_TRACK,buf,len / frameSize);
    if(ret < 0) {
      //...
      cerr << "AudioFileBaseC::Write(), Error writing data." << ret << "\n";
      return false;
    }
    //ONDEBUG(cerr << "AudioFileBaseC::Write(), Done. \n");
    return true;
  }


  //: Seek to location in stream.
  
  bool AudioFileBaseC::Seek(UIntT off) {
    return afSeekFrame (handle,AF_DEFAULT_TRACK,off) == off;
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT AudioFileBaseC::Tell() const {
    return afTellFrame (handle,AF_DEFAULT_TRACK);
  }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT AudioFileBaseC::Size() const {
    return afGetFrameCount (handle,AF_DEFAULT_TRACK);
  }
  
  //: Seek to location in stream.
  bool AudioFileBaseC::Seek64(StreamPosT off) {
    return afSeekFrame(handle,AF_DEFAULT_TRACK,off) == off;
  }

  StreamPosT AudioFileBaseC::Tell64() const
  {
    return afTellFrame (handle,AF_DEFAULT_TRACK);
  }

  StreamPosT AudioFileBaseC::Size64() const
  {
    return afGetFrameCount (handle,AF_DEFAULT_TRACK);
  }

}
