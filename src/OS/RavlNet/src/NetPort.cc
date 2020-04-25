// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetPort.cc"

#include "Ravl/OS/NetPort.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  NetPortBaseC::NetPortBaseC(const StringC &server)
    : ep(server,false),
      netAttr(0)
  {}
  
  //: Destructor.
  
  NetPortBaseC::~NetPortBaseC()
  { 
    sigConnections.DisconnectAll(true);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::~NetPortBaseC() " << (void *) this);
  }
  
  //: Wait for connection complete.
  
  bool NetPortBaseC::WaitForConnect(RealT timeOut) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::WaitForConnect(). Called. timeOut(" << timeOut << ")");
    if(!streamReady.Wait(timeOut)) {
      RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::WaitForConnect(), Connection failed to complete. ";
      return false;
    }
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::WaitForConnect(). Done. ");
    return true;
  }
  
  //: Handle incoming StreamReady message.
  
  bool NetPortBaseC::MsgStreamReady() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::MsgStreamReady(). Called. " << (void *) this);
    streamReady.Post();
    return true;
  }
  
  //: Initialise link.
  
  bool NetPortBaseC::Init() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetPortBaseC::Init(). Called. ");
    ep.RegisterR(NPMsg_StreamReady,"StreamInfo",*this,&NetPortBaseC::MsgStreamReady);
    //: Handle incoming StreamReady message.
    
    return true;
  }

  //: Setup net end point.
  
  bool NetPortBaseC::Connect(NetEndPointC &nep) {
    ep = nep;
    netAttr.Connect(nep);
    // Register method to call on connection closed.
    sigConnections += ConnectR(ep.SigConnectionBroken(),*this,&NetPortBaseC::ConnectionClosed);
    NetPortBaseC::Init();
    return true;
  }
  
  //: Handle connection broken signal.
  
  bool NetPortBaseC::ConnectionClosed() {
    if(netAttr.IsValid())
      netAttr.ConnectionClosed();
    return true;
  }
 
}
