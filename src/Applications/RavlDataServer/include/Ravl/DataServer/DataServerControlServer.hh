// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLSERVER_H
#define	RAVL_DATASERVERCONTROLSERVER_H
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlInterface.hh"
#include "Ravl/DataServer/DataServer.hh"
#include "Ravl/RCBodyV.hh"
#include "Ravl/String.hh"
#include "Ravl/Service.hh"

namespace RavlN
{

  //! userlevel = Develop
  //: Data server control interface server.

  class DataServerControlServerBodyC
  : public ServiceC
  {
  public:
    DataServerControlServerBodyC(const XMLFactoryContextC &factory);
    //: XMLFactory constructor

    DataServerControlServerBodyC(const StringC& controlAddress, const DataServerC& dataServer);
    //: Ctor.
    //!param: controlAddress - Network address for the controller to listen on.
    //!param: dataServer - The data server to be controlled.

    bool Listen();
    //: Start the server.
    // If the server starts successfully, this call will block (currently permanently).
    //!return: False if the server fails to start.

    //! Start service.
    virtual bool Start();

    //! Shutdown service
    virtual bool Shutdown();

    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<DataServerControlServerBodyC> RefT;

    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<DataServerControlServerBodyC> CBRefT;

  private:
    StringC m_controlAddress;
    DataServerC m_dataServer;
    bool m_started;
    volatile bool m_terminated;
    SocketC m_socketServer;

    //! Called when owner handles drop to zero.
    virtual void ZeroOwners();

  };

  //! userlevel = Normal
  //: Data server control interface server.

  class DataServerControlServerC
  : public RCLayerC<DataServerControlServerBodyC>
  {
  public:
    DataServerControlServerC()
    {}
    //! Default ctor.

    DataServerControlServerC(const XMLFactoryContextC &factory)
     : RCLayerC<DataServerControlServerBodyC>(*new DataServerControlServerBodyC(factory))
    {}
    //: XMLFactory constructor

    DataServerControlServerC(const StringC& controlAddress, const DataServerC& dataServer)
     : RCLayerC<DataServerControlServerBodyC>(*new DataServerControlServerBodyC(controlAddress, dataServer))
    {}
    //! Ctor.

    bool Listen()
    { return Body().Listen(); }
    //: Start the server.
    // If the server starts successfully, this call will block (currently permanently).
    //!return: False if the server fails to start.

    const DataServerControlServerBodyC* BodyPtr() const
    { return &RCLayerC<DataServerControlServerBodyC>::Body(); }
  };

}

#endif
