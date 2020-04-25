// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CACHEISTREAM_HEADER
#define RAVL_CACHEISTREAM_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/CacheIStream.hh"
//! docentry="Ravl.API.Core.IO.Streams"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/Cache.hh"
#include "Ravl/DP/IOJoin.hh"

namespace RavlN {
  template<class InT,class OutT> class DPProcessC;
  
  //! userlevel=Develop
  //: Cache for seekable input stream 
  
  template<class DataT>
  class CacheIStreamBodyC
    : public DPISPortBodyC<DataT>,
      public DPStreamOpBodyC
  {
  public:
    CacheIStreamBodyC(UIntT maxSize)
      : at(0),
	atIn(0),
	cache(maxSize)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Constructor.

    CacheIStreamBodyC(const DPISPortC<DataT> &newPort,UIntT maxSize)
      : at(0),
	atIn(0),
	cache(maxSize)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
      SetInput(newPort);
    }
    //: Constructor from port and size.

    virtual bool Save(std::ostream &out) const 
    { return DPISPortBodyC<DataT>::Save(out); }
    //: Save to std::ostream.

    const DPISPortC<DataT> &Input() const
    { return inPort; }
    //: Access input port.
    
    void SetInput(const DPISPortC<DataT> &newPort) { 
      inPort = newPort; 
      this->ReparentAttributeCtrl(newPort);
    }
    //: Set input port.
    
    virtual bool Get(DataT &buf) {
      if(cache.LookupR(at,buf)) {
	at++;
	return true;
      }
      if(at != atIn) {
	if(inPort.Seek64(at))
	  atIn = at;
	// If seek fails, we'll reset 'at' to an appropriate value.
      }
      if(!inPort.Get(buf))
	return false;
      cache.Insert(atIn,buf);
      atIn++;
      at = atIn;
      return true;
    }
    //: Get next element
    
    virtual DataT Get() {
      DataT ret;
      if(!Get(ret))
	throw DataNotReadyC("Data not ready. ");
      return ret;
    }
    //: Get next piece of data from the stream.
    
    virtual bool Seek(UIntT off)
    { at = off; return true; }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off)
    { at += off; return true; }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.
    
    virtual UIntT Tell() const
    { return static_cast<UIntT>(at); }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const
    { return inPort.Size(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Start() const
    { return inPort.Start(); }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0
    
    virtual bool Seek64(StreamPosT off)
    { at = off; return true; }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek64(StreamPosT off)
    { at += off; return false; }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek((UIntT)((IntT) Tell() + off));
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.
    
    virtual StreamPosT Tell64() const
    { return at; }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual StreamPosT Size64() const
    { return inPort.Size64(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual StreamPosT Start64() const
    { return inPort.Start64(); }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0
    
    virtual AttributeCtrlC ParentCtrl() const
    { return inPort; }
    //: Get Parent attribute control.
  protected:
    Int64T at; // Position.
    DPISPortC<DataT> inPort;
    Int64T atIn; // Position of last read.
    CacheC<Int64T,DataT> cache; // Cache by index.
    bool inputSeekable; // Is the input stream actually seekable?
  };
  
  //! userlevel=Normal
  //: Cache for seekable input stream 
  
  template<class DataT>
  class CacheIStreamC
    : public DPISPortC<DataT>,
      public DPStreamOpC
  {
  public:
    CacheIStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CacheIStreamC(UIntT cacheSize)
      : DPEntityC(*new CacheIStreamBodyC<DataT>(cacheSize))
    {}
    //: Constructor.
    //!param: cacheSize - Size of cache to create

    CacheIStreamC(const DPISPortC<DataT> &newPort,UIntT cacheSize)
      : DPEntityC(*new CacheIStreamBodyC<DataT>(newPort,cacheSize))
    {}
    //: Constructor.
    //!param: cacheSize - Size of cache to create
    
  protected:
    CacheIStreamC(CacheIStreamBodyC<DataT> &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    //!param: bod - Body to connect handle to.
    
    CacheIStreamBodyC<DataT> &Body()
    { return dynamic_cast<CacheIStreamBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const CacheIStreamBodyC<DataT> &Body() const
    { return dynamic_cast<const CacheIStreamBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    const DPISPortC<DataT> &Input() const
    { return Body().Input(); }
    //: Access input port.
    //!return: Current input port.
    
    void SetInput(const DPISPortC<DataT> &newPort)
    { Body().SetInput(newPort); }
    //: Set input port.
    //!param: newPort - Port to set as input.
    
  };
  
  template<class InT>
  DPISPortC<InT> operator>>(const DPIPortC<InT> &in,CacheIStreamC<InT> &dat) {
    dat.SetInput(SPort(in));
    return dat;
  }

  template<class InT>
  DPISPortC<InT> operator>>(const DPISPortC<InT> &in,CacheIStreamC<InT> &dat) {
    dat.SetInput(in);
    return dat;
  }
  
  template<class DataT,class InterT>
  DPIOPortC<DataT,InterT> operator>>(const DPIOPortC<DataT,InterT> &in,CacheIStreamC<DataT> &out) {
    out.SetInput(in);
    return DPIOPortJoin(out.NewPort(),in); 
  }
  
}

#endif
