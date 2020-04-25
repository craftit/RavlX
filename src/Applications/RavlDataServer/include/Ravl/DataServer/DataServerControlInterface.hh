// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERCONTROLINTERFACE_H
#define	RAVL_DATASERVERCONTROLINTERFACE_H
//! lib=RavlDataServer

#include "Ravl/RCBodyV.hh"
#include "Ravl/String.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Threads/Signal1.hh"

namespace RavlN
{

  //! userlevel = Develop
  //: Data server control interface.

  class DataServerControlInterfaceBodyC
  : public RCBodyVC
  {
  public:
    DataServerControlInterfaceBodyC();
    //: Ctor.
    
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

  protected:
    Signal1C<StringC> m_signalNodeClosed;
    Signal1C<StringC> m_signalNodeRemoved;
  };

  //! userlevel = Normal
  //: Data server control interface.

  class DataServerControlInterfaceC
  : public RCHandleVC<DataServerControlInterfaceBodyC>
  {
  public:
    DataServerControlInterfaceC() {}
    //: Default ctor.

    DataServerControlInterfaceC(bool)
    : RCHandleVC<DataServerControlInterfaceBodyC>(*new DataServerControlInterfaceBodyC())
    {}
    //: Ctor.

    DataServerControlInterfaceC(DataServerControlInterfaceBodyC& body)
    : RCHandleVC<DataServerControlInterfaceBodyC>(body)
    {}
    //: Body ctor.

    bool AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options)
    { return Body().AddNode(path, nodeType, options); }
    //: Add a node to the live data server.
    //!param: path - The virtual path to add to the tree.
    //!param: nodeType - The node type name (matching the 'NodeType' config entry).
    //!param: options - Addition options (matching the format used for node config entries).
    //!return: True on success.

    bool RemoveNode(const StringC& path, bool removeFromDisk = false)
    { return Body().RemoveNode(path, removeFromDisk); }
    //: Remove a node from the live data server.
    //!param: path - The virtual path to remove from the tree.
    //!param: removeFromDisk - If a corresponding real file exists, remove it from the disk once all ports have closed.
    //!return: True on success.

    Signal1C<StringC>& SignalNodeClosed()
    { return Body().SignalNodeClosed(); }
    //: Connect to this signal to be informed when all ports to a target path have closed.
    //!return: Signal to a string containing the target path.

    Signal1C<StringC>& SignalNodeRemoved()
    { return Body().SignalNodeRemoved(); }
    //: Connect to this signal to be informed when all ports to a removed target path have closed and it has been removed.
    //!return: Signal to a string containing the target path.

    bool QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available)
    { return Body().QueryNodeSpace(path, total, used, available); }
    //: Query the space info for a node, if applicable.
    //!param: total - Returns the space allocated for footage in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used for stored footage in bytes. -1 if not applicable.
    //!param: available - Returns the space available for uploading footage in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.
  };

}

#endif
