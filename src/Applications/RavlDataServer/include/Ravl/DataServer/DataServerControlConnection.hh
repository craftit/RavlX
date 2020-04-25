// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLCONNECTION_H
#define	RAVL_DATASERVERCONTROLCONNECTION_H
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlInterface.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"

namespace RavlN
{

  //! userlevel = Develop
  //: Data server control interface connection.

  class DataServerControlConnectionBodyC
  : public NetEndPointBodyC
  {
  public:
    DataServerControlConnectionBodyC(SocketC& socket, DataServerControlInterfaceC& controller);
    //! Ctor.

    bool Initialise();
    //: Must be called directly after the object is instantiated to initialise the connection.

    bool OnConnectionBroken();
    //: Called when the connection is broken.
    
    bool OnAddNode(UIntT requestId, const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options);
    //: Called when an AddNode request is received.

    bool OnRemoveNode(UIntT requestId, const StringC& path, bool removeFromDisk = false);
    //: Called when a RemoveNode request is received.

    bool OnSignalNodeClosed(StringC& nodePath);
    //: Called when SignalNodeClosed is signalled.

    bool OnSignalNodeRemoved(StringC& nodePath);
    //: Called when SignalNodeRemoved is signalled.

    bool OnQueryNodeSpace(UIntT requestId, const StringC& path);
    //: Called when a QueryNodeSpace request is received.

  private:
    DataServerControlInterfaceC m_controller;
    bool m_initialised;
    SignalConnectionSetC m_connectionSet;
  };

  //! userlevel = Normal
  //: Data server control interface connection.

  class DataServerControlConnectionC
  : public NetEndPointC
  {
  public:
    DataServerControlConnectionC();
    //! Default ctor.

    DataServerControlConnectionC(SocketC& socket, DataServerControlInterfaceC& controller)
    : NetEndPointC(*new DataServerControlConnectionBodyC(socket, controller))
    {}
    //! Ctor.

    DataServerControlConnectionC(DataServerControlConnectionBodyC& body)
    : NetEndPointC(body)
    {}
    //! Body ctor.

    DataServerControlConnectionBodyC& Body()
    { return static_cast<DataServerControlConnectionBodyC&>(NetEndPointC::Body()); }
    //: Body access.

    const DataServerControlConnectionBodyC& Body() const
    { return static_cast<const DataServerControlConnectionBodyC&>(NetEndPointC::Body()); }
    //: Body access.

    bool Initialise()
    { return Body().Initialise(); }
    //: Must be called directly after the object is instantiated to initialise the connection.

    bool OnSignalNodeClosed(StringC& nodePath)
    { return Body().OnSignalNodeClosed(nodePath); }
    //: Called when SignalNodeClosed is signalled.

    bool OnSignalNodeRemoved(StringC& nodePath)
    { return Body().OnSignalNodeRemoved(nodePath); }
    //: Called when SignalNodeRemoved is signalled.

  };

}

#endif
