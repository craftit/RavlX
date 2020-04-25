// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerVFSRealDir.hh"
#include "Ravl/OS/NetPortManager.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/CacheIStream.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/OS/Directory.hh"

#if RAVL_HAVE_FTW_H
#include <ftw.h>

#ifndef FTW_CONTINUE
#define FTW_CONTINUE 0
#endif
#ifndef FTW_STOP
#define FTW_STOP 1
#endif

#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace
{

  const int g_directoryPermissions = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // Default directory permissions.
  const int g_childProcessWait = 10; // Number of seconds to wait for the process to finish.

}

namespace RavlN {

#if RAVL_HAVE_FTW_H
  int removeFTW(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
  {
    FilenameC filename(fpath);
    ONDEBUG(cerr << "removeFTW deleting (" << filename << ")" << endl);

    return filename.Remove() ? FTW_CONTINUE : FTW_STOP;
  }
#endif
  
  //: Factory constructor

  DataServerVFSRealDirBodyC::DataServerVFSRealDirBodyC(const XMLFactoryContextC &factory)
   : DataServerVFSNodeBodyC(factory),
     realDirname(factory.AttributeString("realDirname","")),
     defaultFileFormat(factory.AttributeString("defaultFileFormat","")),
     canCreate(factory.AttributeBool("canCreate",false)),
     m_syncBeforeRead(factory.AttributeBool("syncBeforeRead",true))
  {

  }

  //: Constructor.
  
  DataServerVFSRealDirBodyC::DataServerVFSRealDirBodyC(const StringC &nname,const StringC& npath,const StringC &nRealDirname,bool ncanWrite,bool ncanCreate)
    : DataServerVFSNodeBodyC(nname,npath,ncanWrite,true),
      realDirname(nRealDirname),
      canCreate(ncanCreate),
      m_syncBeforeRead(false)
  {
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::DataServerVFSRealDirBodyC nname (" << nname << ")" << endl);

    // Shall we create?
    if (canCreate)
    {
      // Doesn't exist, so go for it.
      if (!realDirname.Exists())
      {
        FilePermissionC permissions(g_directoryPermissions);
        if (!realDirname.MakeDir(permissions))
        {
          cerr << "DataServerVFSRealDirBodyC::DataServerVFSRealDirBodyC failed to create directory (" << nRealDirname << ")" << endl;
          // TODO(WM) Throw exception on failure?
        }
      }
    }
  }
  
  //: Destructor.
  
  DataServerVFSRealDirBodyC::~DataServerVFSRealDirBodyC()
  {
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::~DataServerVFSRealDirBodyC" << endl);

    RavlAssert(name2file.IsEmpty() && nameDeletePending.IsEmpty());
  }
  
  //: Configure node with given setup.
  
  bool DataServerVFSRealDirBodyC::Configure(const ConfigFileC &config)
  {
    // Lock server while we're setting up.
    // No other threads should be running here, but better safe than sorry.
    MutexLockC lock(access);
    
    // Configure basic attributes.
    DataServerVFSNodeBodyC::Configure(config);
    
    // Setup filename 
    realDirname = config["Filename"];

    // File format
    defaultFileFormat = config["FileFormat"];
    
    return true;
  }
  
  //: Open VFS file.
  
  bool DataServerVFSRealDirBodyC::OpenVFSFile(DListC<StringC> &remainingPath, DataServerVFSRealFileC &rfile, bool forWrite)
  {
    if (deletePending)
    {
      RavlWarning("OpenVFSFile refusing to open file in soon to be deleted path (%s)",name.c_str());
      return false;
    }

    StringC vfile = StringListC(remainingPath).Cat("/");
    if (vfile.IsEmpty())
    {
      RavlWarning("OpenVFSFile no filename specified in path (%s)", name.c_str());
      return false;
    }
    FilenameC absFile = realDirname + "/" + vfile;
    ONDEBUG(RavlDebug("OpenVFSFile asked to open '%s' from '%s'",absFile.c_str(),realDirname.c_str()));

    MutexLockC lock(access);

    if (nameDeletePending.IsMember(vfile))
    {
      RavlWarning("OpenVFSFile refusing to open soon to be deleted file (%s)",absFile.c_str());
      return false;
    }

    if (!name2file.Lookup(vfile, rfile))
    {
      if(!absFile.Exists() && !forWrite && m_syncBeforeRead) {
        // Wait for sync of directory to disk, just to check.
        DirectoryC blobDir = absFile.PathComponent();
        blobDir.Sync();
      }

      if (!absFile.Exists() && !forWrite)
      {
        RavlWarning("OpenVFSFile failed to open file '%s' with default format '%s' ",absFile.c_str(),defaultFileFormat.c_str());
        return false;
      }
      
      rfile = DataServerVFSRealFileC(vfile, AbsoluteName(), absFile, canWrite);
      rfile.SetVerbose(verbose);
      rfile.SetFileFormat(defaultFileFormat);
      rfile.SetCloseSignal(sigOnClose);
      rfile.SetDeleteSignal(sigOnDelete);
		  DataServerVFSRealDirC ref(*this);
		  rfile.SetParent(ref);

      name2file[vfile] = rfile;
    }

    return true;
  }

  //: Open input port.
  
  bool DataServerVFSRealDirBodyC::OpenIPort(DListC<StringC> &remainingPath,const StringC &dataType,NetISPortServerBaseC &port) {
    DataServerVFSRealFileC rfile;
    if(!OpenVFSFile(remainingPath,rfile,false))
      return false;
    DListC<StringC> emptyList;
    return rfile.OpenIPort(emptyList,dataType,port);
  }
  
  //: Open output port.
  
  bool DataServerVFSRealDirBodyC::OpenOPort(DListC<StringC> &remainingPath,const StringC &dataType,NetOSPortServerBaseC &port) {
    // Check we can write to dir.
    if(!CanWrite())
      return false;
    
    // Setup output port.
    DataServerVFSRealFileC rfile;
    if(!OpenVFSFile(remainingPath,rfile,true))
      return false;
    DListC<StringC> emptyList;
    return rfile.OpenOPort(emptyList,dataType,port);
  }
  


  bool DataServerVFSRealDirBodyC::PortsOpen()
  {
    MutexLockC lock(access);

    return !name2file.IsEmpty() || !nameDeletePending.IsEmpty();
  }



  bool DataServerVFSRealDirBodyC::Delete(const DListC<StringC>& remainingPath)
  {
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::Delete name (" << name << ") remainingPath (" << StringListC(remainingPath).Cat("/") << ")" << endl);
    if (!canWrite)
    {
      cerr << "DataServerVFSRealDirBodyC::Delete attempting to delete on read-only directory (" << name << ")" << endl;
      return false;
    }

    StringC targetFilename = StringListC(remainingPath).Cat("/");
    if (targetFilename.IsEmpty())
    {
      ONDEBUG(cerr << "DataServerVFSRealDirBodyC::Delete delete pending (" << name << ")" << endl);
    	deletePending = true;

      return true;
    }

    FilenameC targetFile = realDirname + "/" + targetFilename;
    DataServerVFSRealFileC targetFileNode;

    MutexLockC lock(access);
    
    if (!name2file.Lookup(targetFilename, targetFileNode))
    {
      if(!targetFile.Exists())
      {
        cerr << "DataServerVFSRealDirBodyC::Delete failed to find file '" << targetFilename << "' for '" << name << "'" << endl;
        return false;
      }

      targetFileNode = DataServerVFSRealFileC(targetFilename, AbsoluteName(), targetFile, canWrite);
      targetFileNode.SetCloseSignal(sigOnClose);
      targetFileNode.SetDeleteSignal(sigOnDelete);
      DataServerVFSRealDirC ref(*this);
      targetFileNode.SetParent(ref);
    }

    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::Delete delete pending of (" << targetFilename << ") on (" << name << ")" << endl);

    nameDeletePending.Insert(targetFilename);

    RavlAssert(targetFileNode.IsValid());
    return targetFileNode.Delete();
  }


  
  bool DataServerVFSRealDirBodyC::QueryNodeSpace(const StringC& remainingPath, Int64T& total, Int64T& used, Int64T& available)
  {
    StringC targetFilename =  StringListC(remainingPath).Cat("/");
    FilenameC targetFile = realDirname + "/" + targetFilename;

    if (!targetFile.Exists() || !targetFile.IsReadable())
    {
      cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace target does not exist or is unreadable '" << targetFile << "'" << endl;
      return false;
    }

    if (!targetFile.IsDirectory())
    {
      if (targetFile.IsRegular())
      {
        total = -1;
        available = -1;
        used = targetFile.FileSize();

        return true;
      }

      cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace cannot query size of irregular file '" << targetFile << "'" << endl;
      return false;
    }

    StringC commandLine ="df -P " + targetFile;

    ChildOSProcessC childProcess(commandLine, true);
    if (!childProcess.IsRunning() && !childProcess.ExitedOk())
    {
      cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace command failed for '" << name << "'" << endl;
      return false;
    }

    StringC childOutput;
    if (childProcess.StdOut())
    {
      readline(childProcess.StdOut(), childOutput);
      if (childOutput.contains("df:"))
      {
        cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace command returned error for '" << name << "' file '" << targetFile << "'" << endl;
        return false;
      }

      ONDEBUG(cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace discarding command output '" << childOutput << "'" << endl);
    }

    readline(childProcess.StdOut(), childOutput);
    StringListC childOutputList(childOutput);
    if (childOutputList.Size() != 6)
    {
      cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace command output size failed for '" << name << "'" << endl;
      return false;
    }

    DLIterC<StringC> listIter(childOutputList);
    listIter++; // Skip device

    total = (*listIter).Int64Value();
    listIter++;
    used = (*listIter).Int64Value();
    listIter++;
    available = (*listIter).Int64Value();

    if (childProcess.IsRunning())
    {
      if (!childProcess.Wait(g_childProcessWait))
      {
        cerr << "DataServerVFSRealDirBodyC::QueryNodeSpace waiting for process to end" << endl;
        // TODO(WM) Add a kill -9 here?
      }
    }

    return true;
  }

  
  
  bool DataServerVFSRealDirBodyC::OnClose(DListC<StringC>& remainingPath)
  {
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::OnClose path (" << StringListC(remainingPath).Cat("/") << ")" << endl);

    MutexLockC lock(access);

    RavlAssert(!remainingPath.IsEmpty());
    StringC targetFilename = StringListC(remainingPath).Cat("/");
    name2file.Del(targetFilename);

    if (ReadyToDelete())
    {
      lock.Unlock();

      DoDelete();
    }

    return true;
  }



  bool DataServerVFSRealDirBodyC::OnDelete(DListC<StringC>& remainingPath)
  {
#if RAVL_HAVE_FTW_H
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::OnDelete path (" << StringListC(remainingPath).Cat("/") << ")" << endl);

    MutexLockC lock(access);

    StringC targetFilename = StringListC(remainingPath).Cat("/");
    if (!targetFilename.IsEmpty())
    {
      RavlAssert(nameDeletePending.Contains(targetFilename));
      nameDeletePending.Remove(targetFilename);

      if (ReadyToDelete())
      {
        lock.Unlock();

        DoDelete();
      }
    }

    return true;
#else
    return false;
#endif
  }



  bool DataServerVFSRealDirBodyC::ReadyToDelete()
  {
    ONDEBUG(cerr << "DataServerVFSRealDirBodyC::ReadyToDelete pending (" << (deletePending ? "Y" : "N") << ") " << \
                          "name2file (" << name2file.Size() << ") " << \
                          "nameDeletePending (" << nameDeletePending.Size() << ")" << endl);

    return deletePending && name2file.IsEmpty() && nameDeletePending.IsEmpty();
  }



  void DataServerVFSRealDirBodyC::DoDelete()
  {
#if RAVL_HAVE_FTW_H
    ONDEBUG(cerr << "DataServerVFSRealFileBodyC::DoDelete deleting (" << realDirname << ")" << endl);

    if (nftw(realDirname, removeFTW, 64, FTW_DEPTH | FTW_PHYS) == 0)
    {
      if (sigOnDelete.IsValid())
        sigOnDelete(AbsoluteName());
    }
    else
    {
      cerr << "DataServerVFSRealFileBodyC::DoDelete failed to delete (" << realDirname << ")" << endl;
    }
#else
    RavlAssert(false);	
#endif
  }

  XMLFactoryRegisterHandleConvertC<DataServerVFSRealDirC,DataServerVFSNodeC> g_registerXMLFactoryDataServerVFSRealDir("RavlN::DataServerVFSRealDirC");

}
