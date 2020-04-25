// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERVFSNODE_HEADER
#define RAVL_DATASERVERVFSNODE_HEADER 1
//! lib=RavlDataServer

#include "Ravl/String.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/DList.hh"
#include "Ravl/Text/ConfigFile.hh"
#include "Ravl/Threads/Signal1.hh"

namespace RavlN {
  
  class XMLFactoryContextC;
  class NetOSPortServerBaseC;
  class NetISPortServerBaseC;
  
  //! userlevel=Develop
  //: Virtual File System node for DataServerC.
  
  class DataServerVFSNodeBodyC 
    : public RCBodyVC
  {
  public:
    DataServerVFSNodeBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor

    DataServerVFSNodeBodyC(const StringC &nname,const StringC& npath,bool ncanWrite,bool isDir);
    //: Constructor.
    
    virtual bool Configure(const ConfigFileC &config);
    //: Configure node with given setup.
    
    const StringC &Name() const
    { return name; }
    //: Get name of node.

    const StringC Path() const
    { return path; }
    //: Get the node path.

    const StringC AbsoluteName();
    //: Get the absolute name of the node.
    
    virtual bool CanWrite() const
    { return canWrite; }
    //: Can write.
    
    virtual void SetWrite(bool v)
    { canWrite = v; }
    //: Set writable flag.
    
    virtual bool IsDirectory() const
    { return isDirectory; }
    //: Is this a directory ?

    virtual bool OpenIPort(DListC<StringC> &remainingPath,const StringC &dataType,NetISPortServerBaseC &port);
    //: Open input port.
    
    virtual bool OpenOPort(DListC<StringC> &remainingPath,const StringC &dataType,NetOSPortServerBaseC &port);
    //: Open output port.

    virtual bool PortsOpen()
    { return false; }
    //: Does this node contain ports that are still open?

    virtual bool Delete();
    //: Delete the physical media associated with the node.
    //!return: True if successfully deleted.

    virtual bool Delete(const DListC<StringC>& remainingPath);
    //: Delete the physical media of the target path within the node.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!return: True if successfully deleted.

    virtual bool DeletePending()
    { return deletePending; }
    //: Has this node been marked for deletion?

    virtual void SetRemovePending(const bool nremovePending)
    { removePending = nremovePending; }
    //: Mark the node for removal

    virtual bool RemovePending()
    { return removePending; }
    //: Has this node been marked for removal?

    virtual bool QueryNodeSpace(const StringC& remainingPath, Int64T& total, Int64T& used, Int64T& available);
    //: Query physical media details for the target path within the node.
    // Currently only supported by directory nodes, and returns details for the partition containing the target path.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!param: total - Returns the space allocated for the partition containing the target path in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used on the partition containing the target path in bytes. -1 if not applicable.
    //!param: available - Returns the space available on the partition containing the target node in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.

    void SetCloseSignal(Signal1C<StringC>& sigOnClose_)
    { sigOnClose = sigOnClose_; }
    //: Set the signal to be called when the target path is closed.

    void SetDeleteSignal(Signal1C<StringC>& sigOnDelete_)
    { sigOnDelete = sigOnDelete_; }
    //: Set the signal to be called when the target path is deleted.

    virtual bool OnClose(DListC<StringC>& remainingPath)
    { return true; }
    //: Called when a target path is closed.
    //!param: remainingPath - The closed target path within the node.

    virtual bool OnDelete(DListC<StringC>& remainingPath)
    { return true; }
    //: Called when a target path is deleted.
    //!param: remainingPath - The deleted target path within the node.

    void SetVerbose(bool useVerbose)
    { verbose = useVerbose; }
    //: Set verbose flag

  protected:
    StringC name;
    StringC path;
    bool isDirectory; // Is this a directory.
    bool canWrite;    // Write permission enabled ?
    bool verbose;
    Signal1C<StringC> sigOnClose;
    Signal1C<StringC> sigOnDelete;
    bool deletePending;
    bool removePending;
  };
  
  //! userlevel=Normal
  //: Virtual File System node for DataServerC. 
  //!cwiz:author
  
  class DataServerVFSNodeC
    : public RCHandleVC<DataServerVFSNodeBodyC>
  {
  public:
    DataServerVFSNodeC(const XMLFactoryContextC &factory)
      : RCHandleVC<DataServerVFSNodeBodyC>(*new DataServerVFSNodeBodyC(factory))
    {}
    //: Factory constructor

    DataServerVFSNodeC(const StringC & nname,const StringC& npath,bool ncanWrite,bool isDir)
      : RCHandleVC<DataServerVFSNodeBodyC>(*new DataServerVFSNodeBodyC(nname,npath,ncanWrite,isDir))
    {}
    //: Constructor. 
    //!cwiz:author
    
    DataServerVFSNodeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    bool Configure(const ConfigFileC &config)
    { return Body().Configure(config); }
    //: Configure node with given setup.
    
    const StringC & Name() const
    { return Body().Name(); }
    //: Get name of node. 
    //!cwiz:author

    const StringC Path() const
    { return Body().Path(); }
    //: Get the node path.

    const StringC AbsoluteName()
    { return Body().AbsoluteName(); }
    //: Get the absolute name of the node.

    bool CanWrite() const
    { return Body().CanWrite(); }
    //: Can write. 
    //!cwiz:author
    
    void SetWrite(bool v) 
    { Body().SetWrite(v); }
    //: Set writable flag. 
    //!cwiz:author
    
    bool IsDirectory() const
    { return Body().IsDirectory(); }
    //: Is this a directory ? 
    //!cwiz:author

    bool OpenIPort(DListC<StringC> &remainingPath,const StringC &dataType,NetISPortServerBaseC &port)
    { return Body().OpenIPort(remainingPath,dataType,port); }
    //: Open input port.
    
    bool OpenOPort(DListC<StringC> &remainingPath,const StringC &dataType,NetOSPortServerBaseC &port)
    { return Body().OpenOPort(remainingPath,dataType,port); }
    //: Open output port.
    
    bool PortsOpen()
    { return Body().PortsOpen(); }
    //: Does this node contain ports that are still open?

    bool Delete()
    { return Body().Delete(); }
    //: Delete the physical media associated with the node.
    //!return: True if successfully deleted.

    bool Delete(const DListC<StringC> &remainingPath)
    { return Body().Delete(remainingPath); }
    //: Delete the physical media of the target path within the node.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!return: True if successfully deleted.

    bool DeletePending()
    { return Body().DeletePending(); }
    //: Has this node been marked for deletion?

    void SetRemovePending(const bool nremovePending)
    { Body().SetRemovePending(nremovePending); }
    //: Mark the node for removal

    bool RemovePending()
    { return Body().RemovePending(); }
    //: Has this node been marked for removal?

    bool QueryNodeSpace(const StringC& remainingPath, Int64T& total, Int64T& used, Int64T& available)
    { return Body().QueryNodeSpace(remainingPath, total, used, available); }
    //: Query physical media details for the target path within the node.
    // Currently only supported by directory nodes, and returns details for the partition containing the target path.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!param: total - Returns the space allocated for the partition containing the target path in bytes (both free and used). -1 if not applicable.
    //!param: used - Returns the space used on the partition containing the target path in bytes. -1 if not applicable.
    //!param: available - Returns the space available on the partition containing the target node in bytes. -1 if not applicable.
    //!return: True if the query executed successfully.
    
    void SetCloseSignal(Signal1C<StringC>& sigOnClose_)
    { Body().SetCloseSignal(sigOnClose_); }
    //: Set the signal to be called when the target path is closed.

    void SetDeleteSignal(Signal1C<StringC>& sigOnDelete_)
    { Body().SetDeleteSignal(sigOnDelete_); }
    //: Set the signal to be called when the target path is deleted.

    bool OnClose(DListC<StringC>& remainingPath)
    { return Body().OnClose(remainingPath); }
    //: Called when a target path is closed.
    //!param: remainingPath - The closed target path within the node.

    bool OnDelete(DListC<StringC>& remainingPath)
    { return Body().OnDelete(remainingPath); }
    //: Called when a target path is deleted.
    //!param: remainingPath - The deleted target path within the node.

    void SetVerbose(bool useVerbose)
    { return Body().SetVerbose(useVerbose); }
    //: Use verbose logging.

  protected:
    DataServerVFSNodeC(DataServerVFSNodeBodyC &bod)
     : RCHandleVC<DataServerVFSNodeBodyC>(bod)
    {}
    //: Body constructor. 
    
    DataServerVFSNodeBodyC& Body()
    { return static_cast<DataServerVFSNodeBodyC &>(RCHandleC<DataServerVFSNodeBodyC>::Body()); }
    //: Body Access. 
    
    const DataServerVFSNodeBodyC& Body() const
    { return static_cast<const DataServerVFSNodeBodyC &>(RCHandleC<DataServerVFSNodeBodyC>::Body()); }
    //: Body Access. 
    
  };
  
}

#endif
