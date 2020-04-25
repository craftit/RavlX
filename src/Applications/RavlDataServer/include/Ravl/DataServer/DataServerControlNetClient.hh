// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLNETCLIENT_H
#define	RAVL_DATASERVERCONTROLNETCLIENT_H
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlInterface.hh"
#include "Ravl/String.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/OS/NetRequestManager.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlN
{

  //! userlevel = Develop
  //: Data server control network client.

  class DataServerControlNetClientBodyC
  : public DataServerControlInterfaceBodyC
  {
  public:
    DataServerControlNetClientBodyC();
    //: Ctor.

    virtual ~DataServerControlNetClientBodyC();
    //: Dtor.

    bool Open(const StringC& serverAddress);
    //: Open the network control client.
    //!param: serverAddress - The address of the data server controller to connect to.

    bool Close();
    //: Close the network client.

    bool IsOpen();
    //: Is the connection open?

    virtual bool AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options);
    //: Add a node to the live data server.
    //!param: path - The virtual path to add to the tree.
    //!param: nodeType - The node type name (matching the 'NodeType' config entry).
    //!param: options - Addition options (matching the format used for node config entries).
    //!return: True on success.

    virtual bool RemoveNode(const StringC& path, bool removeFromDisk = false);
    //: Remove a node from the live data server.
    //!param: path - The virtual path to remove from the tree.
    //!param: removeFromDisk - If a corresponding real file exists, remove it from the disk once all ports have closed.
    //!return: True on success.

    virtual Signal1C<StringC>& SignalNodeClosed()
    { return m_signalNodeClosed; }
    //: Connect to this signal to be informed when all ports to a target path have closed.
    //!return: Signal to a string containing the target path.

    virtual Signal1C<StringC>& SignalNodeRemoved()
    { return m_signalNodeRemoved; }
    //: Connect to this signal to be informed when all ports to a removed target path have closed and it has been removed.
    //!return: Signal to a string containing the target path.

    virtual bool QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available);
    //: Query the space info for a node, if applicable.
    //!param: total - Returns the space allocated for footage in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used for stored footage in bytes. -1 if not applicable.
    //!param: available - Returns the space available for uploading footage in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.

  private:
    bool ConnectionBroken();
    //: Called when the end point connection is broken.

    bool OnAddNode(UIntT requestId, bool result);
    //: Called when the AddNode result is received.

    bool OnRemoveNode(UIntT requestId, bool result);
    //: Called when the RemoveNode result is received.

    bool OnSignalNodeClosed(const StringC& nodePath);
    //: Called when the NodeClosed signal is received.

    bool OnSignalNodeRemoved(const StringC& nodePath);
    //: Called when the NodeRemoved signal is received.

    bool OnQueryNodeSpace(UIntT requestId, bool result, SArray1dC<Int64T> values);
    //: Called when the QueryNodeSpace result is received.

    StringC m_serverAddress;
    Signal1C<StringC> m_signalNodeClosed;
    Signal1C<StringC> m_signalNodeRemoved;
    NetEndPointC m_netEndPoint;
    NetRequestManagerC m_netRequestManager;
    SignalConnectionSetC m_signalConnectionSet;
    MutexC m_netLock;
  };

  //! userlevel = Normal
  //: Data server control network client.

  class DataServerControlNetClientC
  : public DataServerControlInterfaceC
  {
  public:
    DataServerControlNetClientC();
    //: Default ctor.

    DataServerControlNetClientC(bool)
    : DataServerControlInterfaceC(*new DataServerControlNetClientBodyC())
    {}
    //: Ctor.

    DataServerControlNetClientBodyC& Body()
    { return static_cast<DataServerControlNetClientBodyC&>(DataServerControlInterfaceC::Body()); }
    //: Body access.

    const DataServerControlNetClientBodyC& Body() const
    { return static_cast<const DataServerControlNetClientBodyC&>(DataServerControlInterfaceC::Body()); }
    //: Body access.

    bool Open(const StringC& serverAddress)
    { return Body().Open(serverAddress); }
    //: Open the network control client.
    //!param: serverAddress - The address of the data server controller to connect to.

    bool Close()
    { return Body().Close(); }
    //: Close the network client.

    bool IsOpen()
    { return Body().IsOpen(); }
    //: Is the connection open?

  };

}

#endif
