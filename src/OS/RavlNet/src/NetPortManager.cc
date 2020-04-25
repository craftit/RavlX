// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetPortManager.cc"

#include "Ravl/OS/Date.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/OS/NetPortClient.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {


  //: Default constructor.

  NetPortManagerBodyC::NetPortManagerBodyC()
    : name("*anon*"),
      managerOpen(false),
      ready(0),
      terminate(false),
      conIdAlloc(0),
      unregisterOnDisconnect(false)
  {}

  //: XML Factory constructor.

  NetPortManagerBodyC::NetPortManagerBodyC(const XMLFactoryContextC &factory)
   : name(factory.AttributeString("name",factory.Path())),
     address(factory.AttributeString("address","localhost:4143")),
     managerOpen(false),
     ready(0),
     terminate(false),
     conIdAlloc(0),
     unregisterOnDisconnect(factory.AttributeBool("unregisterOnDisconnect",false))
  {}

  //: Constructor.
  
  NetPortManagerBodyC::NetPortManagerBodyC(const StringC &nname,bool nUnregisterOnDisconnect)
    : name(nname),
      managerOpen(false),
      ready(0),
      terminate(false),
      conIdAlloc(0),
      unregisterOnDisconnect(nUnregisterOnDisconnect)
  {}
  
  //: Open manager at address.
  
  bool NetPortManagerBodyC::Open(const StringC &addr) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Open(), Called for '%s' on manager '%s' ",addr.c_str(),name.c_str()));
    
    RWLockHoldC hold(access,false);
    if(managerOpen) {
      RavlWarning("NetPortManagerBodyC::Open(), Attempt to re-open port manager at '%s'",addr.c_str());
      return false; 
    }
    
    sktserv = SocketC(addr,true);
    if(!sktserv.IsOpen()) {
      RavlError("NetPortManagerBodyC::Run(), Failed to open server socket. '%s' ",addr.data());
      return false;
    }

    address = addr;
    managerOpen = true;
    NetPortManagerC manager(*this);
    LaunchThread(manager,&NetPortManagerC::Run);
    ready.Wait();
    Sleep(0.1);
    return true;
  }
  
  //: Close down manager.
  
  bool NetPortManagerBodyC::Close() {
    terminate = true;
    sktserv.Close();
    return true;
  }
  
  //: Run port manager.
  
  bool NetPortManagerBodyC::Run() {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Run(), Called. "));
    RavlAssert(sktserv.IsOpen());
    NetPortManagerC manager(*this);
    ready.Post();
    while(!terminate) {
      SocketC skt = sktserv.Listen();
      if(!skt.IsValid())
	continue; // Listen or bind failed for some reason.
      // When a socket connects, make and end point and send a 'hello' message.
      NetPortClientC cl(skt,manager);
      // Register client somewhere ?
    }
    //cerr << "NetPortManagerBodyC::Run(), Done. \n";
    ready.Post();
    return true;
  }
  
  //: Wait until the server has exited.
  
  bool NetPortManagerBodyC::WaitForTerminate() {
    if(!managerOpen) {
      RavlError("WaitForTerminate, Called before Open(). ");
      return false;
    }
    while(!terminate) {
      if(!managerOpen) {
        Sleep(1);
        continue;
      }
      ready.Wait(10);
    } 
    return true;
  }

  //: Search for port in table.
  
  bool NetPortManagerBodyC::Lookup(const StringC &name,
                                   const StringC &dataType,
                                   NetISPortServerBaseC &isport,
                                   bool attemptCreate)
  {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup(NetISPortServerBaseC),  Called. Port='%s' ",name.c_str()));
    RWLockHoldC hold(access,RWLOCK_READONLY);
    if(iports.Lookup(name,isport))
      return true;
    if(!attemptCreate || !requestIPort.IsValid())
      return false;
    CallFunc3C<StringC,StringC,NetISPortServerBaseC &,bool> rp = requestIPort; // Make a copy of handle to ensure its thread safe.
    hold.Unlock();
    // Request new connection.
    if(!rp.Call(const_cast<StringC &>(name),const_cast<StringC &>(dataType),isport))
      return false;
    // Register connection in table.
    hold.LockWr();
    
    // Create a new name, checking it doesn't already exist.
    StringC conName;
    do {
      UInt64T newId = conIdAlloc++;
      conName = StringC("#") + StringC(newId) + ':' + name + ':' + dataType ;
    } while(iports.IsElm(conName)) ;
    
    // Register port.
    isport.SetName(conName);
    iports[conName] = isport;    
    return true;
  }
  
  bool NetPortManagerBodyC::Lookup(const StringC &name,
                                   const StringC &dataType,
                                   NetOSPortServerBaseC &osport,
                                   bool attemptCreate)
  {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup(NetOSPortServerBaseC),  Called. Port='%s' ",name.c_str()));
    RWLockHoldC hold(access,RWLOCK_READONLY);
    if(oports.Lookup(name,osport)) {
      ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup, found port "));
      return true;
    }
    // Do we have a request handler ?
    if(!attemptCreate || !requestOPort.IsValid()) {
      ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup, giving up "));
      return false;
    }
    // Request new connection.
    CallFunc3C<StringC,StringC,NetOSPortServerBaseC &,bool> rp = requestOPort; // Make a copy of handle to ensure its thread safe.
    hold.Unlock();
    // Register connection in table.
    if(!rp.Call(const_cast<StringC &>(name),const_cast<StringC &>(dataType),osport)) {
      ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup, Create failed. "));
      return false;
    }
    
    // Register connection in table.
    hold.LockWr();
    
    // Create a new name, checking it doesn't already exist.
    StringC conName;
    do {
      UInt64T newId = conIdAlloc++;
      conName = StringC("#") + StringC(newId) + ':' + name + ':' + dataType ;
    } while(oports.IsElm(conName)) ;
    
    // Register port.
    osport.SetName(conName);
    oports[conName] = osport;    
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Lookup, completed ok. "));
    return true;
  }
  
  //: Register new port.
  
  bool NetPortManagerBodyC::Register(const StringC &name,NetISPortServerBaseC &ips) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Register(),  Called. Port='%s'",name.c_str()));
    RWLockHoldC hold(access,RWLOCK_WRITE);
    if(iports.IsElm(name)) 
      return false; // Already registered.
    iports[name] = ips;
    return true;
  }

  bool NetPortManagerBodyC::Register(const StringC &name,NetOSPortServerBaseC &ops) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Register(),  Called. Port='%s' ",name.c_str()));
    RWLockHoldC hold(access,RWLOCK_WRITE);
    if(oports.IsElm(name)) 
      return false; // Already registered.
    oports[name] = ops;
    return true;
  }
  
  //: Unregister port.
  
  bool NetPortManagerBodyC::Unregister(const StringC &name,bool isInput) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::Unregister(), Called. Name='%s' ",name.c_str()));
    RWLockHoldC hold(access,RWLOCK_WRITE);
    if(isInput)
    {
      RavlAssert(iports.IsElm(name));
      return iports.Del(name);
    }
    RavlAssert(oports.IsElm(name));
    return oports.Del(name);
  }
  
  //: Called when a connection is established.
  
  bool NetPortManagerBodyC::RegisterConnection(NetISPortServerBaseC &isport) {
    if(unregisterOnDisconnect) 
      Connect(isport.NetEndPoint().SigConnectionBroken(),NetPortManagerC(*this),&NetPortManagerC::ConnectionDroppedI,isport);
    
    return true;
  }
  
  //: Called when a connection is established.
  
  bool NetPortManagerBodyC::RegisterConnection(NetOSPortServerBaseC &osport) {
    if(unregisterOnDisconnect)
      Connect(osport.NetEndPoint().SigConnectionBroken(),NetPortManagerC(*this),&NetPortManagerC::ConnectionDroppedO,osport);
    return true;
  }
  
  //: Register IPort request manager.
  // Return false if this replaces another request manager.
  
  bool NetPortManagerBodyC::RegisterIPortRequestManager(const CallFunc3C<StringC,StringC,NetISPortServerBaseC &,bool> &nRequestIPort) {
    bool ret = !requestIPort.IsValid();
    requestIPort = nRequestIPort;
    return ret;
  }
  
  //: Register OPort request manager.
  // Return false if this replaces another request manager.
  
  bool NetPortManagerBodyC::RegisterOPortRequestManager(const CallFunc3C<StringC,StringC,NetOSPortServerBaseC &,bool> &nRequestOPort) {
    bool ret = !requestOPort.IsValid();
    requestOPort = nRequestOPort;
    return ret;    
  }
  
  //: Called when connection to port is dropped.
  
  bool NetPortManagerBodyC::ConnectionDroppedI(NetISPortServerBaseC &sp) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::ConnectionDroppedI, Called "));
    // Hold handle to object to stop it being deleted before we're finished.
    NetISPortServerBaseC sph = sp;
    
    // Remove from register.
    Unregister(sph.Name(),true);
    return true;
  }
  
  //: Called when connection to port is dropped.
  
  bool NetPortManagerBodyC::ConnectionDroppedO(NetOSPortServerBaseC &sp) {
    ONDEBUG(RavlDebug("NetPortManagerBodyC::ConnectionDroppedO, Called "));
    // Hold handle to object to stop it being deleted before we're finished.
    NetOSPortServerBaseC sph = sp;
    
    // Remove from register.
    Unregister(sph.Name(),false);    
    return true;
  }
  
  //! Start service.
  bool NetPortManagerBodyC::Start() {
    if(!address.IsEmpty()) {
      while(!terminate) {
        if(Open(address))
          break;
        RavlWarning("Failed to open server socket, will retry in 10 seconds.");
        RavlN::Sleep(10);
      }
    }
    return true;
  }

  //! Shutdown service
  bool NetPortManagerBodyC::Shutdown()
  {
    Close();
    return true;
  }

  //: Owning handles has dropped to zero.
  void NetPortManagerBodyC::ZeroOwners()
  {
    ServiceC::ZeroOwners();
  }

  //: Access global net port manager.

  NetPortManagerC &GlobalNetPortManager() {
    static NetPortManagerC npm("global");
    return npm;
  }


  //: Open net port manager.
  
  bool NetPortOpen(const StringC &addr) 
  { return GlobalNetPortManager().Open(addr); }

  bool NetPortClose() { 
    if(!GlobalNetPortManager().IsValid())
      return false;
    GlobalNetPortManager().Close();
    return true;
  }
  //: Close down net port manager.

  NetPortManagerBodyC::RefT NetPortManager2Service(const NetPortManagerC &npm)
  { return &npm.Body(); }

  DP_REGISTER_CONVERSION(NetPortManager2Service,1.0);

  ServiceC::RefT NetPortManagerBody2Service(const NetPortManagerBodyC::RefT &npm)
  { return npm.BodyPtr(); }

  DP_REGISTER_CONVERSION(NetPortManagerBody2Service,1.0);

  XMLFactoryRegisterHandleC<NetPortManagerC> g_registerXMLFactoryNetPortManager("RavlN::NetPortManagerC");
}
