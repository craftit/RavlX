// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETOPORT_HEADER
#define RAVL_NETOPORT_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network.NetPort"
//! file="Ravl/OS/Network/NetOPort.hh"
//! example=exNetPort.cc

#include "Ravl/DP/SPort.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/OS/NetPort.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Base class for NetIPorts.
  
  class NetOSPortBaseC 
    : public NetPortBaseC 
  {
  public:
    NetOSPortBaseC(const StringC &server,const StringC &portName,const type_info &ndataType);
    //: Constructor.
    // The 'server' has the format  'host:port' where port may be a
    // host name or its ip (dotted numbers) address and port is the 
    // number of the port to use. <br>
    // 'portName' is the namer of the stream on the server.
    
    virtual ~NetOSPortBaseC();
    //: Destructor.
    
    NetEndPointC &NetEndPoint()
    { return ep; }
    //: Access net end point.
    
  protected:
    bool Init();
    //: Initalise link.
    
    bool RecvState(Int64T &at,Int64T &start,Int64T &end);
    //: Handle incoming state info.
    
    bool ReqFailed(IntT &flag);
    //: Handle request failed.
    
    bool WaitForInfo() const;
    //: Wait for stream info to arrive.
    
    StringC portName,dataType;
    
    StreamPosT start,size;    
    StreamPosT at; // Position in stream.
    bool gotEOS; // Got an End Of Stream.
    
    RWLockC rwlock;
    
    UIntT flag;
    
    mutable ThreadEventC gotStreamInfo; // Have we recieved stream info yet.
  };
  
  //! userlevel=Develop
  //: Input port body
  
  template<class DataT>
  class NetOSPortBodyC
    : public DPOSPortBodyC<DataT>,
      public NetOSPortBaseC
  {
  public:
    NetOSPortBodyC(const StringC &address,const StringC &portName)
      : NetOSPortBaseC(address,portName,typeid(DataT))
    { 
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
      Init(); 
    }
    //: Constructor.
    // The 'address' has the format  'host:port' where port may be a
    // host name or its ip (dotted numbers) address and port is the 
    // number of the port to use. <br>
    // 'portName' is the namer of the stream on the server.
    
    ~NetOSPortBodyC() { 
      if(ep.IsOpen() && !gotEOS) {
	ep.Send(NPMsg_Close);
	ep.WaitTransmitQClear();
      }
      ep.Close(); 
    }
    //: Destructor.
    
    virtual bool Seek(UIntT off) { 
      //cerr << "NetOSPortBodyC()::Seek() Off=" << off << " Start=" << start << " Size=" << size << "\n";
      if(off >= size || off < start)
	return false;
      gotEOS = false; // Reset end of stream flag.
      at = off; 
      return true;
    }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.

    virtual bool Seek64(Int64T off) { 
//      cerr << "NetOSPortBodyC()::Seek() Off=" << off << " Start=" << start << " Size=" << size << "\n";
      if(off < start || (size != streamPosUnknown && off >= size))
        return false;
      gotEOS = false; // Reset end of stream flag.
      at = off; 
      return true;
    }
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off) {
      //cerr << "NetOSPortBodyC()::DSeek() Off=" << off << " At=" << at <<" Start=" << start << " Size=" << size << "\n";
      Int64T newOff = (Int64T) at + off;
      if(newOff < start || newOff >= size)
        return false;
      gotEOS = false; // Reset end of stream flag.
      at += off;
      return true;
    }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek(Tell() + off);
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.

    virtual bool DSeek64(Int64T off) {
      //cerr << "NetOSPortBodyC()::DSeek() Off=" << off << " At=" << at <<" Start=" << start << " Size=" << size << "\n";
      if (off == streamPosUnknown)
        return false;
      Int64T newOff = (Int64T) at + off;
      if(newOff < start || newOff >= size)
        return false;
      gotEOS = false; // Reset end of stream flag.
      at += off;
      return true;
    }
    //: Delta Seek, goto location relative to the current one.
    // The default behavour of this functions is :
    // Do some error checking then:
    //   Seek(Tell() + off);
    // if an error occurered (DSeek returned False) then stream
    // position will not be changed.
    
    
    virtual UIntT Tell() const { 
      WaitForInfo();
      return static_cast<UIntT>(at); 
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Size() const { 
      WaitForInfo();
      return static_cast<UIntT>(size); 
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual UIntT Start() const { 
      WaitForInfo();
      return static_cast<UIntT>(start); 
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0
    
    virtual Int64T Tell64() const  { 
      WaitForInfo();
      return at; 
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual Int64T Size64() const { 
      WaitForInfo();
      return size; 
    }
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.
    
    virtual Int64T Start64() const { 
      WaitForInfo();
      return start; 
    }
    //: Find the offset where the stream begins, normally zero.
    // Defaults to 0

    virtual void PutEOS() { 
      gotEOS = true; 
      ep.Send(NPMsg_Close);
      ep.WaitTransmitQClear();
    }
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const 
    { return !gotEOS && ep.IsValid(); }
    //: Is port ready for data ?
    
    virtual bool Put(const DataT &data) { 
      if(gotEOS || !ep.IsValid())
      	return false;
      ep.Send(NPMsg_Data,at,data);
      at++;
      if(at > size)
      	size = at;
      return true; 
    }
    //: Put data.
    // This function MUST be provided by client class.
    
    virtual IntT PutArray(const SArray1dC<DataT>& data)
    {
//      cerr << "NetOSPortBodyC()::PutArray() Data Size=" << (data.IsValid() ? data.Size() : -1) << " At=" << at << " Size=" << size << "\n";
      if (gotEOS || !data.IsValid() || !ep.IsValid())
      	return 0;
      Int64T dataSize = data.Size();
      ep.Send(NPMsg_DataArrayPut,at,dataSize,data);
      at += dataSize;
      if (at > size)
      	size = at;
      return dataSize;
    }
    //: Put an array of data to stream.
    // returns number of elements processed.
    // NB. This need NOT be overridden in client classes
    // unless fast handling of arrays of data elements is required.

    virtual AttributeCtrlC ParentCtrl() const
    { return netAttr; }
    //: Get Parent attribute control.
    
  };

  //! userlevel=Normal
  //: Input port.
  
  template<class DataT>
  class NetOSPortC
    : public DPOSPortC<DataT>
  {
  public:
    NetOSPortC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    NetOSPortC(const StringC &server,const StringC &portName)
      : DPEntityC(*new NetOSPortBodyC<DataT>(server,portName))
    {}
    //: Constructor.
    // The 'address' has the format  'host:port' where port may be a
    // host name or its ip (dotted numbers) address and port is the 
    // number of the port to use. <br>
    // 'portName' is the namer of the stream on the server.
    
  protected:
    NetOSPortC(NetOSPortBodyC<DataT> &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    NetOSPortBodyC<DataT> &Body()
    { return dynamic_cast<NetOSPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const NetOSPortBodyC<DataT> &Body() const 
    { return dynamic_cast<const NetOSPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    NetEndPointC &NetEndPoint()
    { return Body().NetEndPoint(); }
    //: Access net end point.
    
  };


  
}


#endif
