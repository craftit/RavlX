// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETPORTMANAGER_HEADER
#define RAVL_NETPORTMANAGER_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network.NetPort"
//! file="Ravl/OS/Network/NetPortManager.hh"
//! example=exNetPort.cc

#include "Ravl/String.hh"
#include "Ravl/OS/Socket.hh"
#include "Ravl/RCLayer.hh"
#include "Ravl/Hash.hh"
#include "Ravl/OS/NetIPortServer.hh"
#include "Ravl/OS/NetOPortServer.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Calls.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/Service.hh"

namespace RavlN {
  
  class NetPortManagerC;
  
  //! userlevel=Develop
  //: Port server.
  
  class NetPortManagerBodyC 
    : public ServiceC
  {
  public:
    NetPortManagerBodyC();
    //: Default constructor.

    NetPortManagerBodyC(const XMLFactoryContextC &factory);
    //: XML Factory constructor.

    NetPortManagerBodyC(const StringC &name,bool nUnregisterOnDisconnect = false);
    //: Constructor.


    const StringC &Name() const
    { return name; }
    //: Get the name.

    const StringC &Address() const
    { return address; }
    //: Get the address.

    bool Lookup(const StringC &name,const StringC &dataType,NetISPortServerBaseC &isp,bool attemptCreate = true);
    //: Search for port in table.
    //!param: name - Name of connection
    //!param: dataType - Type of data requested.
    //!param: isp - Place to store new connection.
    //!param: attemptCreate - When true, use requestIPort call to attempt to create connection if its not already registered.
    
    bool Lookup(const StringC &name,const StringC &dataType,NetOSPortServerBaseC &osp,bool attemptCreate = true);
    //: Search for port in table.
    //!param: name - Name of connection
    //!param: dataType - Type of data requested.
    //!param: osp - Place to store new connection.
    //!param: attemptCreate - When true, use requestIPort call to attempt to create connection if its not already registered.
    
    bool Register(const StringC &name,NetISPortServerBaseC &ips);
    //: Register new port.
    
    bool Register(const StringC &name,NetOSPortServerBaseC &ops);
    //: Register new port.
    
    bool Unregister(const StringC &name,bool isInput);
    //: Unregister port.
    
    virtual bool Open(const StringC &addr);
    //: Open manager at address.
    
    bool Close();
    //: Close down manager.

    bool IsOpen() const
    { return managerOpen; }
    //: Is the manager open?
    
    bool WaitForTerminate();
    //: Wait until the server has exited.
    // NB. Can only be called after 'Open' has returned.
    
    virtual bool RegisterConnection(NetISPortServerBaseC &isport);
    //: Called when a connection is established.
    
    virtual bool RegisterConnection(NetOSPortServerBaseC &isport);
    //: Called when a connection is established.
    
    bool RegisterIPortRequestManager(const CallFunc3C<StringC,StringC,NetISPortServerBaseC &,bool> &requestIPort);
    //: Register IPort request manager.
    // Args: PortName,DataType,Place to open port to
    // Return false if this replaces another request manager.
    
    bool RegisterOPortRequestManager(const CallFunc3C<StringC,StringC,NetOSPortServerBaseC &,bool> &requestOPort);
    //: Register OPort request manager.
    // Args: PortName,DataType,Place to open port to
    // Return false if this replaces another request manager.
    
    virtual bool Start();
    //: Start service.

    virtual bool Shutdown();
    //: Shutdown service

    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<NetPortManagerBodyC> RefT;

    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<NetPortManagerBodyC> CBRefT;

  protected:
    bool Run();
    //: Run port manager.

    void SetUnregisterOnDisconnect(bool enable)
    { unregisterOnDisconnect = enable; }
    //: Set UnregisterOnDisconnect config flag.

    bool ConnectionDroppedI(NetISPortServerBaseC &sp);
    //: Called when connection to port is dropped.
    
    bool ConnectionDroppedO(NetOSPortServerBaseC &sp);
    //: Called when connection to port is dropped.
    
    virtual void ZeroOwners();
    //: Owning handles has dropped to zero.

    StringC name;
    StringC address;
    
    CallFunc3C<StringC,StringC,NetISPortServerBaseC &,bool> requestIPort; // Args: PortName,DataType,Place to open port to
    CallFunc3C<StringC,StringC,NetOSPortServerBaseC &,bool> requestOPort; // Args: PortName,DataType,Place to open port to
    
    HashC<StringC,NetISPortServerBaseC> iports; // List of IPorts waiting for incomming connections.
    HashC<StringC,NetOSPortServerBaseC> oports; // List of OPorts waiting for incomming connections.
    RWLockC access;
    bool managerOpen;
    SocketC sktserv;         // Server socket.
    SemaphoreC ready;        // Semaphore used to indicate the server setup is complete.
    volatile bool terminate; // Shutdown flag.
    UInt64T conIdAlloc;       // Connection id allocator.
    bool unregisterOnDisconnect; // Unregister served ports on client disconnect.
    
    friend class NetPortManagerC;
  };

  NetPortManagerBodyC::RefT NetPortManager2Service(const NetPortManagerC &npm);
  //: Convert a net port manager to a service reference.

  //:-

  //! userlevel=Advanced
  //: Port server.
  // Server side exported port manager. <br>
  // This class manages the ports to exported on a server.  The server listens for
  // connections on network socket, and does setup.
  // It should be noted registered connection's are 1 to 1, it isn't possible for
  // two clients to use a connection simultaneously.
  
  class NetPortManagerC 
    : public RCLayerC<NetPortManagerBodyC>
  {
  public:
    NetPortManagerC(const XMLFactoryContextC &factory)
    : RCLayerC<NetPortManagerBodyC>(*new NetPortManagerBodyC(factory))
    {}
    //: XML Factory constructor.

    NetPortManagerC(const StringC &name, bool nUnregisterOnDisconnect = false)
    : RCLayerC<NetPortManagerBodyC>(*new NetPortManagerBodyC(name, nUnregisterOnDisconnect))
    {}
    //: Constructor.
    
    NetPortManagerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    StringC Name() const
    { return Body().Name(); }
    //: Get the name.

    StringC Address() const
    { return Body().Address(); }
    //: Get the address.

  protected:
    NetPortManagerC(NetPortManagerBodyC& body, RCLayerHandleT handleType = RCLH_OWNER)
    : RCLayerC<NetPortManagerBodyC>(body, handleType)
    {}
    //: Body constructor.
    
    NetPortManagerBodyC &Body()
    { return RCLayerC<NetPortManagerBodyC>::Body(); }
    //: Access body.

    const NetPortManagerBodyC &Body() const
    { return RCLayerC<NetPortManagerBodyC>::Body(); }
    //: Access body.

    bool Run()
    { return Body().Run(); }
    //: Run manager thread.
    
    bool ConnectionDroppedI(NetISPortServerBaseC &sp)
    { return Body().ConnectionDroppedI(sp); }
    //: Called when connection to port is dropped.
    
    bool ConnectionDroppedO(NetOSPortServerBaseC &sp)
    { return Body().ConnectionDroppedO(sp); }
    //: Called when connection to port is dropped.
    
  public:
    
    bool Lookup(const StringC &name,const StringC &dataType,NetISPortServerBaseC &isp,bool attemptCreate = true)
    { return Body().Lookup(name,dataType,isp,attemptCreate); }
    //: Search for port in table.
    
    bool Lookup(const StringC &name,const StringC &dataType,NetOSPortServerBaseC &osp,bool attemptCreate = true)
    { return Body().Lookup(name,dataType,osp,attemptCreate); }
    //: Search for port in table.
    
    bool Register(const StringC &name,NetISPortServerBaseC &ips)
    { return Body().Register(name,ips); }
    //: Register port.
    
    bool Register(const StringC &name,NetOSPortServerBaseC &ops)
    { return Body().Register(name,ops); }
    //: Register port.
    
    bool Unregister(const StringC &name,bool isInput)
    { return Body().Unregister(name,isInput); }
    //: Unregister port.
    
    bool Open(const StringC &addr)
    { return Body().Open(addr); }
    //: Open manager at address.
    
    bool Close()
    { return Body().Close(); }
    //: Close down manager.
    
    bool WaitForTerminate()
    { return Body().WaitForTerminate(); }
    //: Wait until the server has exited.
    // NB. Can only be called after 'Open' has returned.
    
    bool RegisterConnection(NetISPortServerBaseC &isport)
    { return Body().RegisterConnection(isport); }
    //: Called when a connection is established.
    
    bool RegisterConnection(NetOSPortServerBaseC &osport)
    { return Body().RegisterConnection(osport); }
    //: Called when a connection is established.
    
    bool RegisterIPortRequestManager(const CallFunc3C<StringC,StringC,NetISPortServerBaseC &,bool> &requestIPort)
    { return Body().RegisterIPortRequestManager(requestIPort); }
    //: Register IPort request manager.
    // Args: PortName,DataType,Place to open port to
    // Return false if this replaces another request manager.
    
    bool RegisterOPortRequestManager(const CallFunc3C<StringC,StringC,NetOSPortServerBaseC &,bool> &requestOPort)
    { return Body().RegisterOPortRequestManager(requestOPort); }
    //: Register OPort request manager.
    // Args: PortName,DataType,Place to open port to
    // Return false if this replaces another request manager.
    
    friend class NetPortManagerBodyC;
    friend NetPortManagerBodyC::RefT NetPortManager2Service(const NetPortManagerC &npm);
  };
  
  NetPortManagerC &GlobalNetPortManager();
  //: Access global net port manager.
  
  //! userlevel=Normal
  
  bool NetPortOpen(const StringC &address);
  //: Open net port manager.
  // The 'address' has the format  'host:port' where port may be a
  // host name or its ip (dotted numbers) address and port is the 
  // number of the port to use.
  
  bool NetPortClose();
  //: Close down net port manager.
  
}

#endif
