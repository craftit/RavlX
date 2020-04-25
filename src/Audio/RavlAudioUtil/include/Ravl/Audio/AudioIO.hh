// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_AUDIOIO_HEADER
#define RAVLAUDIO_AUDIOIO_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO"
//! file="Ravl/Audio/Util/AudioIO.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Audio/Types.hh"
#include "Ravl/String.hh"
#include "Ravl/TypeName.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Audio IO base class.
  // This class is effectively an abstract base class which can be inherited from and used
  // with DPIAudioC<> DPOAudioC<> to create full audio IO streams.
  
  class AudioIOBaseC {
  public:
    AudioIOBaseC();
    //: Default constructor.
    
    AudioIOBaseC(const StringC &fn,int channel,bool forInput,const type_info &type);
    //: Constructor.

    virtual ~AudioIOBaseC();
    //: Destructor.
    
    virtual bool BuildAttributes( AttributeCtrlBodyC & attributes ) ; 
    //: Build the set of attributes
    
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
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    
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

    bool Read(void *buf,IntT &len);
    //: Read bytes from audio stream.
    // Returns false if error occurred.
    
    bool Write(const void *buf,IntT len);
    //: Write bytes to audio stream.
    // Returns false if error occurred.
    
    bool IsOpen() const
    { return false; }
    //: Is stream open ?
    
    bool HandleGetAttr(const StringC &attrName,StringC &attrValue);
    //: Handle get attrib.
    
    bool HandleSetAttr(const StringC &attrName,const StringC &attrValue);
    //: Handle Set attrib.
    
    bool HandleGetAttr(const StringC &attrName,IntT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as sample rate.
    
    bool HandleSetAttr(const StringC &attrName,const IntT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as sample rate.

    bool HandleGetAttr(const StringC &attrName,RealT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    bool HandleSetAttr(const StringC &attrName,const RealT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
   
    
  };
  
  //! userlevel=Develop
  //: Audio input port.
  // This class is intended to be used with a class inherited from AudioIOBaseC
  // to create full audio IO streams.
  
  template<typename DataT,typename IOClassT> 
  class DPIAudioBodyC
    : public IOClassT,
      public DPISPortBodyC<DataT>
  {
  public:
    DPIAudioBodyC()
      { this->BuildAttributes( *this ) ; }
    //: Default constructor.
    
    DPIAudioBodyC(const StringC &dev,int channel)
      : IOClassT(dev,channel,true,typeid(DataT))
      { this->BuildAttributes( *this ) ; }
    //: Default constructor.
    
    virtual bool IsGetReady() const
    { return this->IsOpen(); }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const
    { return !this->IsOpen(); }
    //: Is data available ?
    
    DataT Get() {
      DataT ret;
      IntT size = sizeof(DataT);
      if(!this->Read((void *) &ret,size))
        throw DataNotReadyC("Failed to read sample.");
      if(size != sizeof(DataT))
        throw DataNotReadyC("Failed to read sample.");
      return ret;
    }
    //: Get sample from stream.
    
    bool Get(DataT &buff)  { 
      IntT size = sizeof(DataT);
      if(!this->Read((void *) &buff,size))
        return false;
      return size == sizeof(DataT);
    }
    //: Get sample from stream.
    
    IntT GetArray(SArray1dC<DataT> &data) {
      IntT size = data.Size() * sizeof(DataT);
      if(data.Size() == 0) {
        RavlIssueError("Attempt to read 0 bytes");
        return 0;
      }
      if(!this->Read(&(data[0]),size))
        return 0;
      return size / sizeof(DataT);
    }
    //: Get an array of samples.
    
    virtual bool Seek(UIntT off)
    { return IOClassT::Seek(off); }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.
    
    virtual UIntT Tell() const
    { return IOClassT::Tell(); }
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const
    { return IOClassT::Size(); }
    //: Find the total size of the stream.  (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual bool Seek64(StreamPosT off)
    { return IOClassT::Seek64(off); }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurred (Seek returned False) then stream
    // position will not be changed.

    virtual StreamPosT Tell64() const
    { return IOClassT::Tell64(); }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.

    virtual StreamPosT Size64() const
    { return IOClassT::Size64(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) 
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) 
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
   

  };
  
  //! userlevel=Normal
  //: Audio input port helper class.
  // This class is intended to be used with a class inherited from AudioIOBaseC
  // to create full audio IO streams.
  
  template<typename DataT,typename IOClassT> 
  class DPIAudioC
    : public DPIPortC<DataT>
  {
  public:
    DPIAudioC()
      
    {}
    //: Default constructor.
    
    DPIAudioC(const StringC &dev,int channel = 0)
      : DPEntityC(*new DPIAudioBodyC<DataT,IOClassT>(dev,channel))
    {}
    //: Constructor.
    
  protected:
    DPIAudioBodyC<DataT,IOClassT> &Body()
    { return dynamic_cast<DPIAudioBodyC<DataT,IOClassT> &>(DPEntityC::Body()); }
    //: Access body.

    const DPIAudioBodyC<DataT,IOClassT> &Body() const
    { return dynamic_cast<const DPIAudioBodyC<DataT,IOClassT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    bool IsOpen() const
    { return Body().IsOpen(); }
    //: Is stream open ?
  };

  // -----------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Audio output port helper class.
  // This class is intended to be used with a class inherited from AudioIOBaseC
  // to create full audio IO streams.
  
  template<typename DataT,typename IOClassT> 
  class DPOAudioBodyC
    : public IOClassT,
      public DPOPortBodyC<DataT>
  {
  public:
    DPOAudioBodyC()
    {}
    //: Default constructor.
    
    DPOAudioBodyC(const StringC &dev,int channel)
      : IOClassT(dev,channel,false,typeid(DataT))
    {}
    //: Default constructor.
    
    virtual bool IsPutReady() const
    { return this->IsOpen(); }
    //: Is some data ready ?
    // true = yes.
    
    bool Put(const DataT &buff) 
    { return this->Write((void *) &buff,sizeof(DataT)); }
    //: Write sample to stream.
    
    IntT PutArray(const SArray1dC<DataT> &data) {
      if(!this->Write(&(data[0]),sizeof(DataT) * data.Size()))
        return 0;
      return (IntT) data.Size();
    }
    //: Get an array of samples.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue) 
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool GetAttr(const StringC &attrName,RealT &attrValue) 
    { return this->HandleGetAttr(attrName,attrValue); }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue)
    { return this->HandleSetAttr(attrName,attrValue); }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
   

  };
  
  //! userlevel=Normal
  //: Audio output port helper class
  // This class is intended to be used with a class inherited from AudioIOBaseC
  // to create full audio IO streams.
  
  template<typename DataT,typename IOClassT> 
  class DPOAudioC
    : public DPOPortC<DataT>
  {
  public:
    DPOAudioC()
      
    {}
    //: Default constructor.
    
    DPOAudioC(const StringC &dev,int channel)
      : DPEntityC(*new DPOAudioBodyC<DataT,IOClassT>(dev,channel))
    {}
    //: Constructor.
  };
  
}


#endif
