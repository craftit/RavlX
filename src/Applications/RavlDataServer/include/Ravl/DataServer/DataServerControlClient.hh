// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLCLIENT_H
#define	RAVL_DATASERVERCONTROLCLIENT_H
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlInterface.hh"
#include "Ravl/DataServer/DataServer.hh"

namespace RavlN
{

  //! userlevel = Develop
  //: Data server control client (interface proxy class).

  class DataServerControlClientBodyC
  : public DataServerControlInterfaceBodyC
  {
  public:
    DataServerControlClientBodyC(DataServerC& dataServer);
    //: Ctor.
    //!param: dataServer - The data server to be controlled.
    
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

    virtual Signal1C<StringC>& SignalNodeClosed();
    //: Connect to this signal to be informed when all ports to a target path have closed.
    //!return: Signal to a string containing the target path.

    virtual Signal1C<StringC>& SignalNodeRemoved();
    //: Connect to this signal to be informed when all ports to a removed target path have closed and it has been removed.
    //!return: Signal to a string containing the target path.

    virtual bool QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available);
    //: Query the space info for a node, if applicable.
    //!param: total - Returns the space allocated for footage in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used for stored footage in bytes. -1 if not applicable.
    //!param: available - Returns the space available for uploading footage in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.

  private:
    DataServerC m_dataServer;
  };

  //! userlevel = Normal
  //: Data server control client (interface proxy class).

  class DataServerControlClientC
  : public DataServerControlInterfaceC
  {
  public:
    DataServerControlClientC();
    //: Default ctor.

    DataServerControlClientC(DataServerC& dataServer)
    : DataServerControlInterfaceC(*new DataServerControlClientBodyC(dataServer))
    {}
    //: Ctor.
    //!param: dataServer - The data server to be controlled.

    DataServerControlClientBodyC& Body()
    { return static_cast<DataServerControlClientBodyC&>(DataServerControlInterfaceC::Body()); }
    //: Body access.

    const DataServerControlClientBodyC& Body() const
    { return static_cast<const DataServerControlClientBodyC&>(DataServerControlInterfaceC::Body()); }
    //: Body access.
  };

}

#endif
