// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetIPort.cc"

#include "Ravl/OS/NetIPort.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/StrStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  NetISPortBaseC::NetISPortBaseC(const StringC &server,const StringC &nPortName,const std::type_info &ndataType)
    : NetPortBaseC(server),
      portName(nPortName),
      dataType(TypeName(ndataType)),
      start(0),
      size((UIntT) -1),
      at(0),
      gotEOS(false),
      recieved(0),
      flag(0)
  {}
  
  //: Destructor.
  
  NetISPortBaseC::~NetISPortBaseC() { 
    ONDEBUG(RavlDebug("NetISPortBaseC::~NetISPortBaseC(), Called. "));
  }
  
  bool NetISPortBaseC::Init() {
    ONDEBUG(RavlDebug("NetISPortBaseC::Init(), Called for '%s'",portName.data()));
    if(!ep.IsOpen()) {
      RavlWarning("No connection. ");
      gotEOS = true;
      return false;
    }
    if(!NetPortBaseC::Connect(ep))
      return false;
    
    ep.LocalInfo().ProtocolName("IPortClient");
    ep.LocalInfo().ProtocolVersion("1.1");
    ep.RegisterR(NPMsg_StreamInfo,"StreamInfo",*this,&NetISPortBaseC::RecvState);
    ep.RegisterR(NPMsg_ReqFailed,"ReqFailed",*this,&NetISPortBaseC::ReqFailed);
    ep.Ready();
    if(!ep.WaitSetupComplete()) {
      RavlError("ERROR: Failed to complete connection. ");
      ep.Close();
      return false;
    }
    
    if(ep.PeerInfo().ProtocolName() != "PortServer") {
      RavlError("Unexpected connection protocol '%s' ",ep.PeerInfo().ProtocolName().data());
      ep.Close();
      return false;
    }
    
    if(ep.PeerInfo().ProtocolVersion() != ep.LocalInfo().ProtocolVersion()) {
      RavlError("Unexpected protocol version '%s' (Local version %s) ",
          ep.PeerInfo().ProtocolVersion().data(),
          ep.LocalInfo().ProtocolVersion().data());
      ep.Close();
      return false;
    }
    
    ep.Send(NPMsg_ReqConnection,portName,dataType,true);  // Request connection.
    if(!WaitForConnect())
      return false;
    netAttr.UpdateAttributeTypes(); 
    ep.Send(NPMsg_ReqInfo); // Request info about the stream.
    return true;
  }
  
  //: Wait for stream info to arrive.
  
  bool NetISPortBaseC::WaitForInfo() const {
    do {
      if(!ep.IsValid()) return false;
      if(!ep.IsOpen()) return false;
    } while(!gotStreamInfo.Wait(2));
    return true;
  }
  
  //: Handle incoming state info.
  
  bool NetISPortBaseC::RecvState(Int64T &nat,Int64T &nstart,Int64T &nsize) {
    ONDEBUG(RavlDebug("NetISPortBaseC::RecvState(), At=%s Start=%s Size=%s ",
        RavlN::StringOf(nat).c_str(),RavlN::StringOf(nstart).c_str(),RavlN::StringOf(nsize).c_str()));
    RWLockHoldC hold(rwlock,RWLOCK_WRITE);
    at = nat;
    start = nstart;
    size = nsize;
    gotStreamInfo.Post();
    return true;
  }
  
  //: Handle request failed.
  
  bool NetISPortBaseC::ReqFailed(IntT &nflag) {
    ONDEBUG(RavlDebug("NetISPortBaseC::ReqFailed(), Error flag=%d ",nflag));
    if(nflag == 1) gotEOS = true;
    flag = nflag;
    recieved.Post();
    return true;
  }

}
