// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERVFSREALDIR_HEADER
#define RAVL_DATASERVERVFSREALDIR_HEADER 1
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerVFSNode.hh"
#include "Ravl/DataServer/DataServerVFSRealFile.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/DP/SPortShare.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Handle file's or directories in a real filesystem.
  
  class DataServerVFSRealDirBodyC
    : public DataServerVFSNodeBodyC
  {
  public:
    DataServerVFSRealDirBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor

    DataServerVFSRealDirBodyC(const StringC &vname,const StringC& npath,const StringC &nRealDirname,bool canWrite,bool canCreate_);
    //: Constructor.
    
    ~DataServerVFSRealDirBodyC();
    //: Destructor.
    
    virtual bool Configure(const ConfigFileC &config);
    //: Configure node with given setup.
    
    const FilenameC &RealDirname() const
    { return realDirname; }
    //: Real filename.

    void SetFileFormat(const StringC& fileFormat)
    { defaultFileFormat = fileFormat; }
    //: Set the file format for all directory files.

    virtual bool OpenIPort(DListC<StringC> &remainingPath,const StringC &dataType,NetISPortServerBaseC &port);
    //: Open input port.
    
    virtual bool OpenOPort(DListC<StringC> &remainingPath,const StringC &dataType,NetOSPortServerBaseC &port);
    //: Open output port.
    
    virtual bool PortsOpen();
    //: Does this node contain ports that are still open?

    virtual bool Delete(const DListC<StringC>& remainingPath);
    //: Delete the physical media of the target path within the node.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!return: True if successfully deleted.

    virtual bool QueryNodeSpace(const StringC& remainingPath, Int64T& total, Int64T& used, Int64T& available);
    //: Query physical media details for the target path within the node.
    //!param: remainingPath - List of strings containing the path elements to the target within the node.
    //!param: total - If the target path is a directory, return the space allocated for the partition containing the target path in bytes (both free and used). -1 if the target path is a file.
    //!param: used - If the target path is a directory, the space used on the partition containing the target path is returned. If the target path is a file, the size of the file is returned. Value in bytes.
    //!param: available - If the target path is a directory, return the space available on the partition containing the target node in bytes. -1 if the target path is a file.
    //!return: True if the query executed successfully.

    virtual bool OnClose(DListC<StringC>& remainingPath);
    //: Called when a target path is closed.
    //!param: remainingPath - The closed target path within the node.

    virtual bool OnDelete(DListC<StringC>& remainingPath);
    //: Called when a child node is deleted.
    //!param: remainingPath - The deleted target path within the node.

  protected:
    bool OpenVFSFile(DListC<StringC> &remainingPath,DataServerVFSRealFileC &rfile,bool forWrite = false);
    //: Open VFS file.
    
    bool ReadyToDelete();
    //: Check if the directory is marked for deletion and no longer has open ports.

    void DoDelete();
    //: Recursively delete the directory.

    MutexC access; // Access control for object.
    HashC<StringC,DataServerVFSRealFileC> name2file;
    HSetC<StringC> nameDeletePending;
    
    FilenameC realDirname;
    StringC defaultFileFormat;

    bool canCreate;
    bool m_syncBeforeRead;
  };
  
  //! userlevel=Normal
  //: Handle file's or directories in a real filesystem. 
  //!cwiz:author
  
  class DataServerVFSRealDirC
    : public DataServerVFSNodeC
  {
  public:
    DataServerVFSRealDirC()
    {}
    //: Create invalid handle

    DataServerVFSRealDirC(const XMLFactoryContextC &factory)
     : DataServerVFSNodeC(*new DataServerVFSRealDirBodyC(factory))
    {}
    //: Factory constructor

    DataServerVFSRealDirC(const StringC & vname,const StringC& npath,const StringC & nRealDirname,bool canWrite = false, bool canCreate = false)
      : DataServerVFSNodeC(*new DataServerVFSRealDirBodyC(vname,npath,nRealDirname,canWrite,canCreate))
    {}
    //: Constructor. 
    //!cwiz:author
    
    const FilenameC & RealDirname() const
    { return Body().RealDirname(); }
    //: Real filename. 
    //!cwiz:author
    
    void SetFileFormat(const StringC& fileFormat)
    { Body().SetFileFormat(fileFormat); }
    //: Set the file format for all directory files.

  protected:
    DataServerVFSRealDirC(DataServerVFSRealDirBodyC &body)
     : DataServerVFSNodeC(body)
    {}
    //: Body constructor. 
    
    DataServerVFSRealDirBodyC& Body()
    { return static_cast<DataServerVFSRealDirBodyC &>(DataServerVFSNodeC::Body()); }
    //: Body Access. 
    
    const DataServerVFSRealDirBodyC& Body() const
    { return static_cast<const DataServerVFSRealDirBodyC &>(DataServerVFSNodeC::Body()); }
    //: Body Access. 
    
    friend class DataServerVFSRealDirBodyC;
  };
}


#endif
