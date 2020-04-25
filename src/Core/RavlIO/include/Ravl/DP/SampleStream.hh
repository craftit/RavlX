// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAM_HEADER
#define RAVL_SAMPLESTREAM_HEADER  1
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Data Processing"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="3/12/2003"
//! userlevel=Normal
//! file="Ravl/Core/IO/SampleStream.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/SPortAttach.hh"

namespace RavlN {
  
  template<class DataT>
  class DPISampleStreamBodyC
    : public DPIStreamOpBodyC<DataT,DataT>,
      public DPSeekCtrlBodyC
  {
  public:
    DPISampleStreamBodyC()
      : startFrame(0),
	sampleRate(1)
    {}
    //: Constructor

    DPISampleStreamBodyC(const DPIPortC<DataT> &nin,Int64T nSampleRate,Int64T nStartFrame = 0)
      : DPIStreamOpBodyC<DataT,DataT>(nin),
	sinput(SPort(nin)),
	startFrame(nStartFrame),
	sampleRate(nSampleRate)
    {
      sinput.Seek64(startFrame); // Make sure sequence is aligned with start frame.
    }
    //: Constructor

    DPISampleStreamBodyC(Int64T nSampleRate,Int64T nStartFrame = 0)
      : startFrame(nStartFrame),
	sampleRate(nSampleRate)
    {}
    //: Constructor

    virtual bool Save(std::ostream &out) const 
    { return DPIStreamOpBodyC<DataT,DataT>::Save(out); }
    //: Save to std::ostream.

    virtual bool Seek(UIntT off) 
    { return sinput.Seek64(startFrame + (Int64T) off * sampleRate); }
    //: Seek to position in stream.
    
    inline bool DSeek(IntT off) 
    { return sinput.DSeek64((Int64T) off * sampleRate); }
    //: Delta seek
    
    virtual UIntT Tell() const { 
      UIntT pos = sinput.Tell();
      if(pos == ((UIntT) -1))
	return pos;
      return static_cast<UIntT>((pos - startFrame) / sampleRate); 
    }
    //: Get current position in stream.
    
    virtual UIntT Size() const { 
      UIntT size = sinput.Size();
      if(size == ((UIntT) -1))
	return size;
      return static_cast<UIntT>((size - startFrame) / sampleRate);
    }
    //: Get size of stream
    
    virtual UIntT Start() const 
    { return static_cast<UIntT>((sinput.Start64() - startFrame) / sampleRate); }
    //: First frame.
    
    virtual bool Seek64(Int64T off) 
    { return sinput.Seek64(startFrame + off * sampleRate); }
    //: Seek to position in stream.
    
    inline bool DSeek64(Int64T off) 
    { return sinput.DSeek64(off * sampleRate); }
    //: Delta seek
    
    virtual Int64T Tell64() const { 
      Int64T pos = sinput.Tell64();
      if(pos == streamPosUnknown)
	return streamPosUnknown;
      return (pos - startFrame) / sampleRate; 
    }
    //: Get current position in stream.
    
    virtual Int64T Size64() const { 
      Int64T size = sinput.Size64();
      if(size == streamPosUnknown)
	return streamPosUnknown;
      return (size - startFrame) / sampleRate; 
    }
    //: Get size of stream
    
    virtual Int64T Start64() const 
    { return (sinput.Start64() - startFrame) / sampleRate; }
    //: First frame.
    
    virtual DataT Get()  {
      DataT ret = sinput.Get();
      if(!sinput.DSeek64(sampleRate-1)) {
	if(sampleRate < 1) {
	  cerr << "WARNING: SampleStreamC, Can't use negative sample rates on a non-seekable stream. \n";
	  return ret;
	}
	// If seeking doesn't work, just discard some frames.
	for(Int64T i = 1;i < sampleRate;i++)
	  this->input.Discard();
      }
      return ret;
    }
    //: Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      if(!sinput.Get(buff))
	return false;
      if(!sinput.DSeek64(sampleRate-1)) {
	if(sampleRate < 1) {
	  cerr << "WARNING: SampleStreamC, Can't use negative sample rates on a non-seekable stream. \n";
	  return false;
	}
	// If seeking doesn't work, just discard some frames.
	for(Int64T i = 1;i < sampleRate;i++)
	  if(!this->input.Discard())
	    cerr << "WARNING: SampleStreamC, Stream sync lost. \n";
      }
      return true;
    }
    //: Try and get next piece of data.
    
    void Input(const DPIPortC<DataT> &ins) { 
      DPIStreamOpBodyC<DataT,DataT>::Input(ins);
      sinput = SPort(ins);
      sinput.Seek64(startFrame); // Make sure sequence is aligned with start frame.
    }
    // Setup input port.

    Int64T StartFrame() const
    { return startFrame; }
    //: Access start frame.
    
    Int64T SampleRate() const
    { return sampleRate; }
    //: Sample rate.
    
    void StartFrame(Int64T nStartFrame) { 
      startFrame = nStartFrame;
      if(sinput.IsValid()) 
	sinput.Seek64(startFrame);      
    }
    //: Access start frame.
    // Note this will reset the position to zero.
    
    void SampleRate(Int64T nSampleRate) { 
      sampleRate = nSampleRate; 
      if(sinput.IsValid()) 
	sinput.Seek64(startFrame);
    }
    //: Sample rate.
    // Note this will reset the position to zero.
    
  protected:
    DPISPortC<DataT> sinput;
    Int64T startFrame;
    Int64T sampleRate;
 };

  template<class DataT>
  class DPISampleStreamC
    : public DPIStreamOpC<DataT,DataT>,
      public DPSeekCtrlC
  {
  public:
    DPISampleStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPISampleStreamC(const DPIPortC<DataT> &nin,Int64T nSampleRate,Int64T nStartFrame = 0)
      : DPEntityC(*new DPISampleStreamBodyC<DataT>(nin,nSampleRate,nStartFrame))
    {}
    //: Constructor.
    
    DPISampleStreamC(Int64T nSampleRate,Int64T nStartFrame = 0)
      : DPEntityC(*new DPISampleStreamBodyC<DataT>(nSampleRate,nStartFrame))
    {}
    //: Constructor.
    
  protected:
    DPISampleStreamBodyC<DataT> &Body()
    { return dynamic_cast<DPISampleStreamBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPISampleStreamBodyC<DataT> &Body() const
    { return dynamic_cast<const DPISampleStreamBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    DPISampleStreamC<DataT> &operator=(const DPISampleStreamC<DataT> &other)
    { DPEntityC::operator=(other); return *this; }
    //: Assigment 
    
    Int64T StartFrame() const
    { return Body().StartFrame(); }
    //: Access start frame.
    
    Int64T SampleRate() const
    { return Body().SampleRate(); }
    //: Sample rate.
    
    void StartFrame(Int64T nStartFrame)
    { Body().StartFrame(nStartFrame); }
    //: Access start frame.
    // Note: this will reset the position to zero.
    
    void SampleRate(Int64T nSampleRate)
    { Body().SampleRate(nSampleRate); }
    //: Sample rate.
    // Note: this will reset the position to zero.
    
    
  };
  
}



#endif
