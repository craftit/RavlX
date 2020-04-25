// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlServer.hh"
#include "Ravl/DataServer/DataServerControlClient.hh"
#include "Ravl/DataServer/DataServerControlConnection.hh"
#include "Ravl/OS/Socket.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  //: XMLFactory constructor

  DataServerControlServerBodyC::DataServerControlServerBodyC(const XMLFactoryContextC &factory)
   : ServiceC(factory),
     m_controlAddress(factory.AttributeString("address","localhost:6447")),
     m_started(false),
     m_terminated(false)
  {
    if(!factory.UseChildComponent("DataServer",m_dataServer,false,typeid(DataServerC))) {
      RavlError("No data server provided in '%s' ",factory.Path().data());
    }
  }

  DataServerControlServerBodyC::DataServerControlServerBodyC(const StringC& controlAddress, const DataServerC& dataServer)
  : m_controlAddress(controlAddress),
    m_dataServer(dataServer),
    m_started(false),
    m_terminated(false)
  {}

  //! Start service.
  bool DataServerControlServerBodyC::Start() {
    RavlDebug("Starting DataServerControl. %d %d ",m_terminated,m_started);
    if(m_terminated || m_started)
      return true;
    m_started = true;
    RavlN::LaunchThread(TriggerPtr(CBRefT(this),&DataServerControlServerBodyC::Listen));
    return true;
  }

  //! Shutdown service
  bool DataServerControlServerBodyC::Shutdown() {
    m_terminated = true;
    if(m_socketServer.IsValid())
      m_socketServer.Close();
    return true;
  }

  static bool startClient(SocketC socketConnection, DataServerC dataServer)
  {
    DataServerControlClientC controlClient(dataServer);
    DataServerControlConnectionC controlConnection(socketConnection, controlClient);
    
    return controlConnection.Initialise();
  }

  bool DataServerControlServerBodyC::Listen()
  {
    RavlDebug("DataServerControl Listening on '%s' ",m_controlAddress.data());
    m_started = true;

    while (!m_terminated) {
      m_socketServer = SocketC(m_controlAddress, true);
      if (m_socketServer.IsOpen())
        break;
      RavlWarning("DataServerControlServerBodyC::Listen failed to open socket (%s), will retry in 10 seconds.",m_controlAddress.data());
      RavlN::Sleep(10);
    }

    //TODO(WM) Add a kill signal to this loop?
    while (!m_terminated)
    {
      try
      {
        ONDEBUG(cerr << "DataServerControlServerBodyC::Listen listening on (" << m_controlAddress << ")" << endl);
        SocketC socketConnection = m_socketServer.Listen(true, 20);
        if (!socketConnection.IsValid())
        {
          RavlError("DataServerControlServerBodyC::Listen opened invalid socket ");
          break;
        }

        if (!socketConnection.IsOpen())
        {
          if (!m_socketServer.IsOpen())
          {
            RavlWarning("DataServerControlServerBodyC::Listen server socket was closed on us, shutting down.");
            break;
          }

          RavlWarning("DataServerControlServerBodyC::Listen connection not opened, looping again");
          continue;
        }

        LaunchThread(&startClient, socketConnection, m_dataServer);
      }
      catch(ExceptionC& e)
      {
        RavlError("DataServerControlServerBodyC::Listen RAVL exception (%s)",e.Text());
        break;
      }
      catch( ... )
      {
        RavlError("DataServerControlServerBodyC::Listen unknown exception ");
        break;
      }
    }

    ONDEBUG(RavlDebug("DataServerControlServerBodyC::Listen loop finished (so something went wrong)"));
    return false;
  }

  //! Called when owner handles drop to zero.
  void DataServerControlServerBodyC::ZeroOwners() {
    ServiceC::ZeroOwners();
  }

  void LinkDataServerControlServer()
  {}

  ServiceC::RefT DataServerControlServer2Service(const DataServerControlServerC &ds)
  { return ds.BodyPtr(); }

  DP_REGISTER_CONVERSION(DataServerControlServer2Service,1.0);

  static XMLFactoryRegisterHandleC<DataServerControlServerC> g_registerXMLFactoryDataServerControlServer("RavlN::DataServerControlServerC");

}
