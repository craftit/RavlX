// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetOPort.cc"

#include "Ravl/OS/NetOPort.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  NetOSPortBaseC::NetOSPortBaseC(const StringC &server,const StringC &nPortName,const type_info &ndataType) 
    : NetPortBaseC(server),
      portName(nPortName),
      dataType(TypeName(ndataType)),
      start(0),
      size(streamPosUnknown),
      at(0),
      gotEOS(false),
      flag(0)
  {}
  
  //: Destructor.
  
  NetOSPortBaseC::~NetOSPortBaseC() { 
    ONDEBUG(RavlDebug("NetOSPortBaseC::~NetOSPortBaseC(), Called. "));
  }
  
  bool NetOSPortBaseC::Init() {
    ONDEBUG(RavlDebug("NetOSPortBaseC::Init(), Called for '%s' ",portName.data()));
    if(!ep.IsOpen()) {
      RavlWarning("No connection. ");
      gotEOS = true;
      return false;
    }
    if(!NetPortBaseC::Connect(ep))
      return false;

    ep.LocalInfo().ProtocolName("OPortClient");
    ep.LocalInfo().ProtocolVersion("1.1");
    ep.RegisterR(NPMsg_StreamInfo,"StreamInfo",*this,&NetOSPortBaseC::RecvState);
    ep.RegisterR(NPMsg_ReqFailed,"ReqFailed",*this,&NetOSPortBaseC::ReqFailed);
    ep.Ready();
    if(!ep.WaitSetupComplete()) {
      RavlError("Failed to setup connection. ");
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
    
    ep.Send(NPMsg_ReqConnection,portName,dataType,false);  // Request connection.
    if(!WaitForConnect())
      return false;
    netAttr.UpdateAttributeTypes(); 
    ep.Send(NPMsg_ReqInfo); // Request info about the stream.
    return true;
  }
  
  bool NetOSPortBaseC::WaitForInfo() const {
    do {
      if(!ep.IsValid()) return false;
      if(!ep.IsOpen()) return false;
    } while(!gotStreamInfo.Wait(2));
    return true;
  }
  
  //: Handle incoming state info.
  
  bool NetOSPortBaseC::RecvState(Int64T &nat,Int64T &nstart,Int64T &nsize) {
    ONDEBUG(cerr << "NetOSPortBaseC::RecvState(), At=" << at << " Start=" << nstart << " Size=" << nsize << "\n");
    RWLockHoldC hold(rwlock,RWLOCK_WRITE);
    at = nat;
    start = nstart;
    size = nsize;
    gotStreamInfo.Post();
    return true;
  }
  
  //: Handle request failed.
  
  bool NetOSPortBaseC::ReqFailed(IntT &nflag) {
    ONDEBUG(cerr << "NetOSPortBaseC::ReqFailed(), Flag=" << flag << "\n");
    if(nflag == 1) gotEOS = true;
    flag = nflag;
    return true;
  }

}
