// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDevAudio
//! author="Charles Galambos"
//! file="Ravl/Contrib/Audio/DevAudio/DevAudioIO.cc"

#if defined(__linux__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#endif

#include "Ravl/Audio/DevAudioIO.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {

  DevAudioBaseC::DevAudioBaseC() 
    : audiofd(-1),
      mixerfd(-1)
  {}
  
  //: Default constructor.
  
  DevAudioBaseC::DevAudioBaseC(const StringC &fn,int channel,bool forInput,const type_info &dtype) 
    : audiofd(-1),
      mixerfd(-1)
  {
    if(forInput)
      IOpen(fn,channel,dtype);
    else
      OOpen(fn,channel,dtype);
  }
  
  //: Destructor .
  
  DevAudioBaseC::~DevAudioBaseC() {
    if(audiofd >= 0)
      close(audiofd);
    if(mixerfd >= 0)
      close(mixerfd);
  }
  
  //: Set number of bits to use in samples.
  
  bool DevAudioBaseC::SetSampleBits(IntT bits) {
    long sample_size = bits;
    ioctl(audiofd, SOUND_PCM_WRITE_BITS, &sample_size); 
    return true;
  }
  
  //: Set sample rate in hertz.
  
  bool DevAudioBaseC::SetSampleRate(RealT hertz) {
    long sample_rate = Round(hertz);
    ioctl(audiofd, SOUND_PCM_WRITE_RATE, &sample_rate); 
    ONDEBUG(cout <<  "Actual sound rate: " << sample_rate << endl);
    return true;
  }
  
  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool DevAudioBaseC::GetSampleBits(IntT &bits) {
    long sample_size;
    ioctl(audiofd, SOUND_PCM_READ_BITS, &sample_size);
    bits = sample_size;
    return true;
  }
  
  //: Get frequency of samples
  // Returns actual frequency.
  
  bool DevAudioBaseC::GetSampleRate(RealT &rate) {
    long sample_rate;
    ioctl(audiofd, SOUND_PCM_READ_RATE, &sample_rate);
    ONDEBUG(cout <<  "DevAudioBaseC::GetSampleRate(), sound rate: " << sample_rate << endl);
    rate = (RealT) sample_rate;
    return true;
  }

  //: Check device capabilities.
  
  void DevAudioBaseC::ReadCaps() {
    long caps = 0;
    cout << "Sound caps: ";
    ioctl(audiofd, SNDCTL_DSP_GETCAPS, &caps); 
    if(caps & DSP_CAP_DUPLEX)
      cout << "duplex ";
    if(caps & DSP_CAP_REALTIME)
      cout << "realtime ";
    if(caps & DSP_CAP_BATCH)
      cout << "batch ";
    if(caps & DSP_CAP_COPROC)
      cout << "co-processor ";
    if(caps & DSP_CAP_TRIGGER)
      cout << "trigger ";
    if(caps & DSP_CAP_MMAP)
      cout << "memmap ";
    cout << endl;
    //printf("%x \n",caps);    
  }

  //: Setup number of channels
  
  bool DevAudioBaseC::SetupChannels(const type_info &dtype) {
    int channels = 0;
    if(dtype == typeid(SampleElemC<1,Int16T>)) {
      channels = 1;
    } else  if(dtype == typeid(SampleElemC<2,Int16T>)) {
      channels = 2;
    } else {
      cerr << "DevAudioBaseC::IOpen(), ERROR: Unrecognised sample type:" << dtype.name() << "\n";
      return false;
    }
    int setChan = channels;
    ioctl(audiofd, SOUND_PCM_WRITE_CHANNELS, &channels); 
    if(channels != setChan) {
      cerr << "Unable to set channels to " << channels << " current setting " << setChan << "\n";
      return false;
    }
    ONDEBUG(cerr << "DevAudioBaseC::SetupChannels(), Channels set to " << setChan << "\n");
    return true;
  }
  
  //: Open audio device.
  
  bool DevAudioBaseC::IOpen(const StringC &fn,int channel,const type_info &dtype) {
#if defined(__linux__)
    if((audiofd = open(fn.chars(),O_RDONLY)) < 0)
      perror("Failed to open /dev/dsp");
#if 0
    if((mixerfd = open("/dev/mixer",O_RDWR)) < 0)
      perror("Failed to open /dev/mixer");
#else
    mixerfd = -1;
#endif
    SetSampleBits(16);
    if(!SetupChannels(dtype))
      return false;
#endif
    return true;
  }
  
  //: Open audio device.
  
  bool DevAudioBaseC::OOpen(const StringC &fn,int channel,const type_info &dtype) {
    ONDEBUG(cerr << "DevAudioBaseC::OOpen(), Called. \n");
#if defined(__linux__)
    if((audiofd = open(fn.chars(),O_WRONLY)) < 0)
      perror("Failed to open audio device ");
#if 0
    if((mixerfd = open("/dev/mixer",O_RDWR)) < 0)
      perror("Failed to open /dev/mixer");
#else
    mixerfd = -1;
#endif
    SetSampleBits(16);
    if(!SetupChannels(dtype))
      return false;
#endif
    ONDEBUG(cerr << "DevAudioBaseC::OOpen(), Done. \n");
    return true;
  }
  
  
  //: Read bytes from audio stream.
  // Returns false if error occured.
  
  bool DevAudioBaseC::Read(void *buf,IntT &len) {
    if(!IsOpen()) {
      cerr << "DevAudioBaseC::Read(), WARNING: Called on invalid stream. \n";
      return false;
    }
    int ret;
    int wlen = len;;
    char *bufPtr = (char *) buf;
    do {
      ret = read(audiofd,bufPtr,wlen);
      if(ret < 0) {
	if(errno == EAGAIN)
	  continue;
	if(errno == EINTR)
	  continue;
	cerr << "DevAudioBaseC::Read(), Error while writting:" << errno << "\n"; 
	return false;
      }
      bufPtr += ret;
      wlen -= ret;
    } while(wlen > 0) ;
    return true;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occured.
  
  bool DevAudioBaseC::Write(const void *buf,IntT len) {
    if(!IsOpen()) {
      cerr << "DevAudioBaseC::Write(), WARNING: Called on invalid stream. \n";
      return false;
    }
    int ret;
    char *bufPtr = (char *) buf;
    do {
      ret = write(audiofd,bufPtr,len);
      if(ret < 0) {
	if(errno == EAGAIN)
	  continue;
	if(errno == EINTR)
	  continue;
	cerr << "DevAudioBaseC::Write(), Error while writting:" << errno << "\n"; 
	return false;
      }
      bufPtr += ret;
      len -= ret;
    } while(len > 0) ;
    return true;
  }
  
}
