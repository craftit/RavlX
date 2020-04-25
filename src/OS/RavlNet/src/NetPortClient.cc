// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetPortClient.cc"

#include "Ravl/OS/NetPortClient.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Construct from open socket.
  
  NetPortClientBodyC::NetPortClientBodyC(SocketC &skt,NetPortManagerC &nManager) 
    : NetEndPointBodyC(skt,false),
      manager(nManager)
  {
    ONDEBUG(RavlDebug("NetPortClientBodyC::NetPortClientBodyC(skt,manager) Called. "));
  }
  
  //: Destructor.
  
  NetPortClientBodyC::~NetPortClientBodyC() {
    ONDEBUG(RavlDebug("NetPortClientBodyC::~NetPortClientBodyC() Called. "));
    // Make sure connections closed before destructing.
    Close();
  }
  
  //: Initalise connection.
  
  bool NetPortClientBodyC::Init() {
    ONDEBUG(RavlDebug("NetPortClientBodyC::Init(), Called. "));
    LocalInfo().ProtocolName("PortServer");
    LocalInfo().ProtocolVersion("1.1");
    RegisterR(NPMsg_ReqConnection,"ConnectTo",*this,&NetPortClientBodyC::MsgConnectTo);
    RegisterR(NPMsg_Close,"Close",*this,&NetPortClientBodyC::MsgClose);
    Ready();
    if(!WaitSetupComplete()) {
      RavlError("NetPortClientBodyC::NetPortClientBodyC(), Connection init failed. ");
      Close();
      return false;
    }
    // Check protocol.
    
    if(PeerInfo().ProtocolName() !=  "IPortClient" && PeerInfo().ProtocolName() !=  "OPortClient") {
      RavlError("Unexpected connection protocol '%s' Expected:'OPortClient' or 'IPortClient' ",
          PeerInfo().ProtocolName().data());
      Close();
      return false;
    }
    
    if(PeerInfo().ProtocolVersion() != "1.1") {
      RavlError("Unexpected protocol version '%s' (Local version 1.1) ",PeerInfo().ProtocolVersion().data());
      Close();
      return false;
    }
    
    return true;
  }
  
  //: Handle connect to message.
  
  bool NetPortClientBodyC::MsgConnectTo(StringC &port,StringC &datatype,bool nIsIPort) {
    ONDEBUG(RavlDebug("NetPortClientBodyC::MsgConnectTo(), Called. Port=%s Type=%s IsIPort=%d",port.c_str(),datatype.c_str(),(int) nIsIPort));
    isIPort = nIsIPort;
    if(isIPort) {
      // Deal with input port.
      NetISPortServerBaseC isport;      
      if(!manager.Lookup(port,datatype,isport)) {
	RavlError("NetPortClientBodyC::MsgConnectTo(), Failed to find input port. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	// Send a failure message ?
	return true;
      }
      if(isport.PortType() != datatype) {
        RavlError("NetPortClientBodyC::MsgConnectTo(), Mismatch in data types. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	return true;
      }
      // Connect something ?
      NetPortClientC me(*this);
      if(!isport.Connect(me)) {
        RavlError("NetPortClientBodyC::MsgConnectTo(), Failed, Already connected. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	// Return a failed message ?
	return true;
      }
      connectionName = port;
      manager.RegisterConnection(isport);
    } else {
      // Deal with output port.
      
      NetOSPortServerBaseC osport;
      if(!manager.Lookup(port,datatype,osport)) {
        RavlError("NetPortClientBodyC::MsgConnectTo(), Failed to find output port. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	// Send a failure message ?
	return true;
      }
      if(osport.PortType() != datatype) {
        RavlError("NetPortClientBodyC::MsgConnectTo(), Missmatch in data types. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	return true;
      }
      // Connect something ?
      NetPortClientC me(*this);
      ONDEBUG(RavlDebug("NetPortClientBodyC::MsgConnectTo(), Connecting '%s'. ",port.data()));
      if(!osport.Connect(me)) {
	RavlError("NetPortClientBodyC::MsgConnectTo(), Failed, Already connected. ");
	Send(NPMsg_ReqFailed,1); // End of stream.
	// Return a failed message ?
	return true;
      }
      connectionName = port;
      ONDEBUG(RavlDebug("NetPortClientBodyC::MsgConnectTo(), Registering. "));
      manager.RegisterConnection(osport);
    }
    Send(NPMsg_StreamReady);
    ONDEBUG(RavlDebug("NetPortClientBodyC::MsgConnectTo(), Done. "));
    return true;
  }

  //: Handle close message.
  // Close down an established connection.
  
  bool NetPortClientBodyC::MsgClose() {
    ONDEBUG(RavlDebug("NetPortClientBodyC::MsgClose(), Called. connectionName='%s' ",connectionName.data()));
    Close();
    if(connectionName.IsEmpty())
      return true;
    StringC empty;
    if(isIPort) {
      NetISPortServerBaseC isport;
      if(!manager.Lookup(connectionName,empty,isport,false))
        return true;
      ONDEBUG(RavlDebug("NetPortClientBodyC::MsgClose(), disconnecting ISPort "));
      if(isport.IsValid())
        isport.Disconnect();
    } else {
      NetOSPortServerBaseC osport;
      if(!manager.Lookup(connectionName,empty,osport,false))
        return true;
      ONDEBUG(RavlDebug("NetPortClientBodyC::MsgClose(), disconnecting OSPort "));
      if(osport.IsValid())
        osport.Disconnect();
    }
    return true;
  }
  
}
