// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! author="Charles Galambos"
//! file="Ravl/Audio/IO/SphereIO.cc"

#include "Ravl/Audio/SphereIO.hh"
#include "Ravl/StringList.hh"
#include "Ravl/DLIter.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {

  SphereBaseC::SphereBaseC() 
    : sampleRate(8000),
      endOfFile(false)
  {}
  
  //: Default constructor.
  
  SphereBaseC::SphereBaseC(const StringC &fn,int channel,bool forInput,const type_info &dtype) 
    : sampleRate(8000),
      endOfFile(false)
  {
    if(forInput)
      IOpen(fn,channel,dtype);
    else
      OOpen(fn,channel,dtype);
  }
  
  //: Destructor .
  
  SphereBaseC::~SphereBaseC() {
  }
  
  //: Set number of bits to use in samples.
  
  bool SphereBaseC::SetSampleBits(IntT nbits) {
    //bits = nbits;
    return false;
  }
  
  //: Set sample rate in hertz.
  
  bool SphereBaseC::SetSampleRate(RealT hertz) {
    //sampleRate = hertz;
    return false;
  }
  
  //: Get number of bits to use in samples.
  // returns actual number of bits.
  
  bool SphereBaseC::GetSampleBits(IntT &nbits) {
    nbits = bits;
    return true;
  }
  
  //: Get frequency of samples
  // Returns actual frequency.
  
  bool SphereBaseC::GetSampleRate(RealT &rate) {
    rate = sampleRate;
    return true;
  }
  
  //: Open audio device.
  
  bool SphereBaseC::IOpen(const StringC &fn,int channel,const type_info &dtype) {
    IStreamC fin(fn);
    char buf[1024];
    if(!fin.read(buf,9)) {
      cerr << "ERROR: Failed to read header. \n";
      return false; 
    }
    buf[7] = 0;
    if(StringC("NIST_1A") != buf) {
      cerr << "ERROR: Not a sphere audio file. '" << buf << "' \n";
      return false;
    }
    if(!fin.read(buf,9)) {
      cerr << "ERROR: Failed to read header. \n";
      return false; 
    }
    dataOffset = StringC(buf).IntValue();
    
    while(1) {
      fin.getline(buf,1024);
      if(buf[0] == ';')
	continue; // Skip comments
      StringC line(buf);
      StringListC lst(line);
      if(lst.Size() == 1) {
	if(lst.First() == "end_head")
	  break;
	cerr << "Unexpected line in sphere file '" << line << "'\n";
	continue;
      }
      DLIterC<StringC> it(lst);
      StringC key = *it;
      it++; 
      it++; // Skip data type.
      StringC data = *it;
      attribs[key] = data; // Store info.
      // Might as well process attributes while we're here.
      if(key == "sample_rate") {
	sampleRate = data.RealValue();
	continue;
      }
      if(key == "sample_sig_bits") {
	bits = data.IntValue();
	continue;
      }
      // Interested in anything else ?
    }
    bis = BinIStreamC(fin);
    bis.Seek(dataOffset);
    return true;
  }
  
  //: Open audio device.
  
  bool SphereBaseC::OOpen(const StringC &fn,int channel,const type_info &dtype) {
    ONDEBUG(cerr << "SphereBaseC::OOpen(), ERROR: Writing sphere files not supported. \n");
    return false;
  }
  
  
  //: Read bytes from audio stream.
  // Returns false if error occured.
  
  bool SphereBaseC::Read(void *buf,IntT &len) {
    if(!IsOpen()) {
      cerr << "SphereBaseC::Read(), WARNING: Called on invalid stream. \n";
      return false;
    }
    bis.Stream().read((char *) buf,len);
    IntT nlen = bis.Stream().gcount();
    if(nlen < len)
      endOfFile = true;
#if RAVL_BIGENDIAN
    // Swap endian.
    Int16T *ptr = (Int16T *) buf;
    Int16T *end = (Int16T *) (&((char *)buf)[len]);
    for(;ptr != end;ptr++)
      *ptr = bswap_16(*ptr);
#endif
    return true;
  }
  
  //: Write bytes to audio stream.
  // Returns false if error occured.
  
  bool SphereBaseC::Write(const void *buf,IntT len) {
    RavlAssertMsg(0,"SphereBaseC::Write(), Not implemented. ");
    return false;
  }
  
  //: Seek to location in stream.
  
  bool SphereBaseC::Seek(UIntT off) {
    streampos npos = static_cast<streampos>(off) * static_cast<streampos>(bits/8) + dataOffset;
	bis.Seek(npos);
    endOfFile = false;
    return Tell() == off; // Did it work ?
  }
  
  //: Find current location in stream.
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT SphereBaseC::Tell() const {
    return (bis.Stream().tellg() - static_cast<streampos>(dataOffset)) / static_cast<streampos>(bits/8);
  }
  
  //: Find the total size of the stream.  (assuming it starts from 0)
  // May return ((UIntT) (-1)) if not implemented.
  
  UIntT SphereBaseC::Size() const {
    StringC val;
    if(attribs.Lookup(StringC("sample_count"),val))
      return val.IntValue();
    return (UIntT) -1;
  }
  
}
