// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASERVERVFSREALFILE_HEADER
#define RAVL_DATASERVERVFSREALFILE_HEADER 1
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerVFSNode.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/DP/SPortShare.hh"
#include "Ravl/RCWrap.hh"

namespace RavlN {
  
  class DataServerVFSRealFileC;
  
  //! userlevel=Develop
  //: Handle file's or directories in a real filesystem.
  
  class DataServerVFSRealFileBodyC
    : public DataServerVFSNodeBodyC
  {
  public:
    DataServerVFSRealFileBodyC(const XMLFactoryContextC &factory);
    //: XMLFactory Constructor

    DataServerVFSRealFileBodyC(const StringC &vname,const StringC& npath,const StringC &nRealFilename,bool canWrite);
    //: Constructor.
    
    ~DataServerVFSRealFileBodyC();
    //: Destructor.
    
    virtual bool Configure(const ConfigFileC &config);
    //: Configure node with given setup.

    const FilenameC &RealFilename() const
    { return realFilename; }
    //: Real filename.
    
    void SetDefaultType(const StringC &typeName)
    { defaultDataType = typeName; }
    //: Set default type name.

    void SetFileFormat(const StringC &fileFormat)
    { defaultFileFormat = fileFormat; }
    //: Set default file format.

    void SetCacheSize(UIntT size)
    { cacheSize = size; }
    //: Set cache size in number of items to cache.
    
    bool CanSeek() const
    { return canSeek; }
    //: Is stream seekable ?

    void SetParent(DataServerVFSNodeC &parent)
    { m_parent = parent; }
    //: Set the parent node.
    
    virtual bool OpenIPort(DListC<StringC> &remainingPath,const StringC &dataType,NetISPortServerBaseC &port);
    //: Open input port.
    
    virtual bool OpenOPort(DListC<StringC> &remainingPath,const StringC &dataType,NetOSPortServerBaseC &port);
    //: Open output port.
    
    virtual bool Delete();
    //: Delete the physical media associated with the node.
    //!return: True if successfully deleted.

    virtual bool QueryNodeSpace(const StringC& remainingPath, Int64T& total, Int64T& used, Int64T& available);
    //: Query physical media details for the target path within the node.
    //!param: remainingPath - List of strings containing the path elements to the target within the node. Should be empty for a file node.
    //!param: total - Set to -1 as the node is a file and total is not applicable.
    //!param: used - The size of the physical media associated is returned, in bytes.
    //!param: available - Set to -1 as the node is a file and available is not applicable.
    //!return: True if the query executed successfully.

  protected:
    bool OpenFileReadAbstract(const StringC &dataType, NetISPortServerBaseC& netPort);
    //: Open file and setup cache for reading.

    bool OpenFileReadByte(const StringC &dataType, NetISPortServerBaseC& netPort);
    //: Open file and setup cache for reading.

    bool OpenFileWriteAbstract(const StringC &dataType, NetOSPortServerBaseC& netPort);
    //: Open file and setup cache for writing.

    bool OpenFileWriteByte(const StringC &dataType, NetOSPortServerBaseC& netPort);
    //: Open file and setup cache for writing.

    bool CloseIFileAbstract();
    //: Close input file and discard cache.

    bool CloseIFileByte();
    //: Close input file and discard cache.

    bool CloseOFileAbstract();
    //: Close output file

    bool CloseOFileByte();
    //: Close output file

    bool AddIPortTypeConversion(const StringC &dataType, DPIPortBaseC& iPort);
    //: Add any required type conversion.

    bool AddOPortTypeConversion(const StringC &dataType, DPOPortBaseC& oPort);
    //: Add any required type conversion.

    bool DisconnectIPortClientByte();
    //: Called when input file client disconnects.

    bool DisconnectOPortClientAbstract();
    //: Called when output file client disconnects.

    bool DisconnectOPortClientByte();
    //: Called when output file client disconnects.

    bool ZeroIPortClientsAbstract();
    //: Called when input file stops being used.

    bool DoDelete();
    //: Delete the file.
    //!return: True if successfully deleted.
    
    MutexC access; // Access control for object.
    
    StringC defaultDataType;
    StringC defaultFileFormat;

    DPTypeInfoC iTypeInfo; // Type info for input stream.
    
    DPISPortShareC<RCWrapAbstractC> iSPortShareAbstract; // Share for abstract input port.
    UIntT iSPortByteCount;
    UIntT cacheSize; // Size of cache to use.

    DPTypeInfoC oTypeInfo; // Type info for input stream.
    DPOPortC<RCWrapAbstractC> oPortAbstract; // Abstract output port.
    DPOPortC<ByteT> oPortByte; // Byte output port.
    
    FilenameC realFilename;
    
    bool canSeek;
    bool multiWrite; // Can multiple clients write to the same file ?

    DataServerVFSNodeC m_parent;

    friend class DataServerVFSRealFileC;
  };
  
  //! userlevel=Normal
  //: Handle file's or directories in a real filesystem. 
  //!cwiz:author

  class DataServerVFSRealFileC
    : public DataServerVFSNodeC
  {
  public:
    DataServerVFSRealFileC(const XMLFactoryContextC &factory)
      : DataServerVFSNodeC(*new DataServerVFSRealFileBodyC(factory))
    {}
    //: XMLFactory Constructor

    DataServerVFSRealFileC(const StringC & vname,const StringC& npath,const StringC & nRealFilename,bool canWrite = false)
      : DataServerVFSNodeC(*new DataServerVFSRealFileBodyC(vname,npath,nRealFilename,canWrite))
    {}
    //: Constructor. 
    //!cwiz:author
    
    DataServerVFSRealFileC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    const FilenameC & RealFilename() const
    { return Body().RealFilename(); }
    //: Real filename. 
    //!cwiz:author
    
    void SetDefaultType(const StringC &typeName)
    { return Body().SetDefaultType(typeName); }
    //: Set default type to be used on node.
    
    void SetFileFormat(const StringC &fileFormat)
    { Body().SetFileFormat(fileFormat); }
    //: Set file format.

    void SetParent(DataServerVFSNodeC &parent)
    { Body().SetParent(parent); }
    //: Set the parent node.

  protected:
    DataServerVFSRealFileC(DataServerVFSRealFileBodyC &bod)
     : DataServerVFSNodeC(bod)
    {}
    //: Body constructor. 
    
    DataServerVFSRealFileBodyC& Body()
    { return static_cast<DataServerVFSRealFileBodyC &>(DataServerVFSNodeC::Body()); }
    //: Body Access. 
    
    const DataServerVFSRealFileBodyC& Body() const
    { return static_cast<const DataServerVFSRealFileBodyC &>(DataServerVFSNodeC::Body()); }
    //: Body Access. 
    
    bool ZeroIPortClientsAbstract()
    { return Body().ZeroIPortClientsAbstract(); }
    //: Called when input file stops being used.

    bool DisconnectIPortClientByte()
    { return Body().DisconnectIPortClientByte(); }
    //: Called when input file client disconnects.

    bool DisconnectOPortClientAbstract()
    { return Body().DisconnectOPortClientAbstract(); }
    //: Called when output file client disconnects.

    bool DisconnectOPortClientByte()
    { return Body().DisconnectOPortClientByte(); }
    //: Called when output file client disconnects.

    friend class DataServerVFSRealFileBodyC;
  };
}

#endif
