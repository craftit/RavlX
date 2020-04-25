// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVER_HEADER
#define RAVL_DATASERVER_HEADER 1
//! lib=RavlDataServer

#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/HashTree.hh"
#include "Ravl/DataServer/DataServerVFSNode.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"

namespace RavlN {

  class DataServerC;
  
  //! userlevel=Develop
  //: Data server body class.
  
  class DataServerBodyC
  : public NetPortManagerBodyC
  {
  public:
    DataServerBodyC(const XMLFactoryContextC &factory);
    //: XML Factory constructor.

    DataServerBodyC(const StringC &name);
    //: Constructor.
    
    bool Open(const StringC &addr);
    //: Open server connection.

    bool ReadConfig(const XMLFactoryContextC &factory);
    //: Read XML configuration.

    bool ReadConfigFile(const StringC &filename, bool useAddress = true);
    //: Read a new config file.
    //!param: filename - Name of the config file to load.
    //!param: useAddress - Open the server connection with a config provided address.
    // Build Virtual File System appropriately.

    bool AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options);
    //: Add a node to the live data server.
    //!param: path - The virtual path to add to the tree.
    //!param: nodeType - The node type name (matching the 'NodeType' config entry).
    //!param: options - Addition options (matching the format used for node config entries).
    //!return: True on success.

    bool RemoveNode(const StringC& path, bool removeFromDisk = false);
    //: Remove a node from the live data server.
    //!param: path - The virtual path to remove from the tree.
    //!param: removeFromDisk - If a corresponding real file exists, remove it from the disk once all ports have closed.
    //!return: True on success.

    Signal1C<StringC>& SignalNodeClosed()
    { return m_signalNodeClosed; }
    //: Connect to this signal to be informed when all ports to a target path have closed.
    //!return: Signal to a string containing the target path.

    Signal1C<StringC>& SignalNodeRemoved()
    { return m_signalNodeRemoved; }
    //: Connect to this signal to be informed when all ports to a removed target path have closed and it has been removed.
    //!return: Signal to a string containing the target path.

    bool QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available);
    //: Query the space info for a node, if applicable.
    //!param: total - Returns the space allocated for footage in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used for stored footage in bytes. -1 if not applicable.
    //!param: available - Returns the space available for uploading footage in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.

    bool OnClose(StringC& path);
    //: Called when a target path is closed.
    // Used to inform directory nodes that a file has been closed.

    bool OnDelete(StringC& path);
    //: Called when a target path is deleted.
    // Used to inform directory nodes that a file marked for deletion has actually been deleted.

    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<DataServerBodyC> RefT;

    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<DataServerBodyC> CBRefT;

  protected:
    bool HandleRequestIPort(StringC name,StringC dataType,NetISPortServerBaseC &port);
    //: Handle a request for an input port.
    
    bool HandleRequestOPort(StringC name,StringC dataType,NetOSPortServerBaseC &port);
    //: Handle a request for an output port.
    
    bool FindVFSNode(const StringC &vfilename,HashTreeNodeC<StringC,DataServerVFSNodeC> &vfs,DListC<StringC> &remainingPath);
    //: Find a virtual file system node.
    // This breaks the filename into strings seperated by '/', it follows the path as far as possible
    // throught the virtual filesystem. If any strings remain in the path they are stored in 'remainingPath'.
    //!param:vfilename - Full virtual filename.
    //!param:vfs - Virtual file system node.
    //!param:remainingPath - Remainder of path after last node has been found.
    // Returns true if node found successfully, false if the path corresponds to no known entry in the filesystem.

    virtual void ZeroOwners();
    //: Owning handles has dropped to zero.

    bool OnCloseOrDelete(StringC &path, bool onDelete);
    //: Process close and delete signals

    bool PruneRemovedNodes(StringC &path);
    //: Walk the path and prune nodes marked for removal that no longer have open ports.

    MutexC m_access;
    HashTreeC<StringC,DataServerVFSNodeC> m_vfs; // Virtual file system.

    Signal1C<StringC> m_signalNodeClosed;
    Signal1C<StringC> m_signalNodeRemoved;

    SignalConnectionSetC m_connectionSet;

    friend class DataServerC;
  };
  
  //! userlevel=Normal
  //: Data server class.
  //!cwiz:author
  
  class DataServerC
  : public NetPortManagerC
  {
  public:
    DataServerC()
    {}
    //: Constructor.
    // Creates an invalid handle.

    DataServerC(const XMLFactoryContextC &factory)
     : NetPortManagerC(*new DataServerBodyC(factory))
    {}
    //: XML Factory constructor.

    DataServerC(const StringC & name) 
     : NetPortManagerC(*new DataServerBodyC(name))
    {}
    //: Constructor. 
    //!cwiz:author
    
    bool Open(const StringC & addr)
    { return Body().Open(addr); }
    //: Open server connection. 
    //!cwiz:author
    
    bool ReadConfigFile(const StringC &filename, bool useAddress = true)
    { return Body().ReadConfigFile(filename, useAddress); }
    //: Read a new config file.
    //!param: filename - Name of the config file to load.
    //!param: useAddress - Open the server connection with a config provided address.
    // Build Virtual File System appropriately.
    
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

    bool OnClose(StringC& path)
    { return Body().OnClose(path); }
    //: Called when a target path is closed.
    // Used to inform directory nodes that a file has been closed.

    bool OnDelete(StringC& path)
    { return Body().OnDelete(path); }
    //: Called when a node is physically deleted.
    // Used to inform directory nodes that a file marked for deletion has actually been deleted.

  protected:
    DataServerC(DataServerBodyC& body, RCLayerHandleT handleType = RCLH_OWNER)
    : NetPortManagerC(body, handleType)
    {}
    //: Body constructor. 
    
    DataServerBodyC& Body()
    { return static_cast<DataServerBodyC &>(NetPortManagerC::Body()); }
    //: Body Access. 
    
    const DataServerBodyC& Body() const
    { return static_cast<const DataServerBodyC &>(NetPortManagerC::Body()); }
    //: Body Access. 
    
    friend class DataServerBodyC;
  };
}

#endif
