// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlNetClient.hh"
#include "Ravl/DataServer/DataServerControlMessages.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  DataServerControlNetClientBodyC::DataServerControlNetClientBodyC()
  : m_signalNodeClosed(StringC()),
    m_signalNodeRemoved(StringC()),
    m_netRequestManager(false)
  {}

  DataServerControlNetClientBodyC::~DataServerControlNetClientBodyC()
  {
    Close();
  }
  
  bool DataServerControlNetClientBodyC::Open(const StringC& serverAddress)
  {
    MutexLockC netLock(m_netLock);

    if (m_netEndPoint.IsValid())
      return true;

    m_netEndPoint = NetEndPointC(serverAddress, false);
    if (!m_netEndPoint.IsOpen())
    {
      RavlError("Open end point failed to open (%s)",serverAddress.data());
      m_netEndPoint.Close();
      m_netEndPoint.Invalidate();
      netLock.Unlock();

      ConnectionBroken();

      return false;
    }

    m_signalConnectionSet += ConnectR(m_netEndPoint.SigConnectionBroken(), *this, &DataServerControlNetClientBodyC::ConnectionBroken);

    m_netEndPoint.RegisterR(static_cast<UIntT>(DATASERVERCONTROL_ADDNODE_RESULT), StringC("AddNodeResult"), *this, &DataServerControlNetClientBodyC::OnAddNode);
    m_netEndPoint.RegisterR(static_cast<UIntT>(DATASERVERCONTROL_REMOVENODE_RESULT), StringC("RemoveNodeResult"), *this, &DataServerControlNetClientBodyC::OnRemoveNode);
    m_netEndPoint.RegisterR(static_cast<UIntT>(DATASERVERCONTROL_NODECLOSED), StringC("NodeClosed"), *this, &DataServerControlNetClientBodyC::OnSignalNodeClosed);
    m_netEndPoint.RegisterR(static_cast<UIntT>(DATASERVERCONTROL_NODEREMOVED), StringC("NodeRemoved"), *this, &DataServerControlNetClientBodyC::OnSignalNodeRemoved);
    m_netEndPoint.RegisterR(static_cast<UIntT>(DATASERVERCONTROL_QUERYNODESPACE_RESULT), StringC("QueryNodeSpaceResult"), *this, &DataServerControlNetClientBodyC::OnQueryNodeSpace);

    netLock.Unlock();

    m_netEndPoint.Ready();

    if (!m_netEndPoint.WaitSetupComplete())
    {
      RavlError("EndPoint failed to complete setup (%s)",serverAddress.data());
      return false;
    }

    m_serverAddress = serverAddress;
    ONDEBUG(cerr << "DataServerControlNetClientBodyC::Open connection opened (" << serverAddress << ")" << endl);

    return true;
  }

  bool DataServerControlNetClientBodyC::Close()
  {
    MutexLockC netLock(m_netLock);

    if (!m_netEndPoint.IsValid())
      return false;

    ONDEBUG(RavlDebug("DataServerControlNetClientBodyC::Close closing"));
    m_netEndPoint.Close();
    m_netEndPoint.Invalidate();
    m_signalConnectionSet.DisconnectAll();
    
    return true;
  }

  bool DataServerControlNetClientBodyC::IsOpen()
  {
    MutexLockC netLock(m_netLock);
    return m_netEndPoint.IsValid();
  }

  bool DataServerControlNetClientBodyC::AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options)
  {
    MutexLockC netLock(m_netLock);

    if (!m_netEndPoint.IsValid() || !m_netEndPoint.IsOpen())
    {
      netLock.Unlock();
      ConnectionBroken();
      return false;
    }

    bool result = false;
    try
    {
      UIntT requestId;
      m_netRequestManager.CreateReq(requestId);
      ONDEBUG(cerr << "DataServerControlNetClientBodyC::AddNode sending request (" << requestId << ")" << endl);
      m_netEndPoint.Send(DATASERVERCONTROL_ADDNODE, requestId, path, nodeType, options);

      if (!m_netRequestManager.WaitForReq(requestId, result))
      {
        ONDEBUG(cerr << "DataServerControlNetClientBodyC::AddNode wait for request failed (" << requestId << ")" << endl);
        return false;
      }
    }
    catch(ExceptionC &e)
    {
      RavlError("DataServerControlNetClientBodyC::AddNode exception(%s)",e.Text());
    }
    catch(...)
    {
      RavlError("DataServerControlNetClientBodyC::AddNode exception");
    }

    return result;
  }

  bool DataServerControlNetClientBodyC::RemoveNode(const StringC& path, bool removeFromDisk)
  {
    MutexLockC netLock(m_netLock);

    if (!m_netEndPoint.IsValid() || !m_netEndPoint.IsOpen())
    {
      netLock.Unlock();
      ConnectionBroken();
      return false;
    }

    bool result = false;
    try
    {
      UIntT requestId;
      m_netRequestManager.CreateReq(requestId);
      ONDEBUG(cerr << "DataServerControlNetClientBodyC::RemoveNode sending request (" << requestId << ")" << endl);
      m_netEndPoint.Send(DATASERVERCONTROL_REMOVENODE, requestId, path, removeFromDisk);

      if (!m_netRequestManager.WaitForReq(requestId, result))
      {
        ONDEBUG(cerr << "DataServerControlNetClientBodyC::RemoveNode wait for request failed (" << requestId << ")" << endl);
        return false;
      }
    }
    catch(ExceptionC &e)
    {
      RavlError("DataServerControlNetClientBodyC::RemoveNode exception(%s)",e.Text());
    }
    catch(...)
    {
      RavlError("DataServerControlNetClientBodyC::RemoveNode exception");
    }

    return result;
  }

  bool DataServerControlNetClientBodyC::QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available)
  {
    MutexLockC netLock(m_netLock);

    if (!m_netEndPoint.IsValid() || !m_netEndPoint.IsOpen())
    {
      netLock.Unlock();
      ConnectionBroken();
      return false;
    }

    bool result = false;
    try
    {
      UIntT requestId;
      m_netRequestManager.CreateReq(requestId);
      ONDEBUG(cerr << "DataServerControlNetClientBodyC::QueryNodeSpace sending request (" << requestId << ")" << endl);
      m_netEndPoint.Send(DATASERVERCONTROL_QUERYNODESPACE, requestId, path);

      SArray1dC<Int64T> values;
      if (!m_netRequestManager.WaitForReq(requestId, result, values))
      {
        ONDEBUG(cerr << "DataServerControlNetClientBodyC::QueryNodeSpace wait for request failed (" << requestId << ")" << endl);
        return false;
      }

      if (result && values.IsValid() && values.Size() == 3)
      {
        total = values[0];
        used = values[1];
        available = values[2];

        return true;
      }
    }
    catch(ExceptionC &e)
    {
      RavlError("DataServerControlNetClientBodyC::QueryNodeSpace exception(%s)",e.Text());
    }
    catch(...)
    {
      RavlError("DataServerControlNetClientBodyC::QueryNodeSpace exception");
    }


    return false;
  }

  bool DataServerControlNetClientBodyC::ConnectionBroken()
  {
    ONDEBUG(RavlDebug("DataServerControlNetClientBodyC::ConnectionBroken"));

    Close();

    return true;
  }

  bool DataServerControlNetClientBodyC::OnAddNode(UIntT requestId, bool result)
  {
    ONDEBUG(RavlDebug("DataServerControlNetClientBodyC::OnAddNode requestId(%u)",requestId));

    m_netRequestManager.DeliverReq(requestId, result);

    return true;
  }

  bool DataServerControlNetClientBodyC::OnRemoveNode(UIntT requestId, bool result)
  {
    ONDEBUG(cerr << "DataServerControlNetClientBodyC::OnRemoveNode requestId("<< requestId << ")" << endl);

    m_netRequestManager.DeliverReq(requestId, result);

    return true;
  }

  bool DataServerControlNetClientBodyC::OnSignalNodeClosed(const StringC& nodePath)
  {
    ONDEBUG(cerr << "DataServerControlNetClientBodyC::OnSignalNodeClosed nodePath("<< nodePath << ")" << endl);

    RavlAssert(m_signalNodeClosed.IsValid());
    m_signalNodeClosed(nodePath);

    return true;
  }

  bool DataServerControlNetClientBodyC::OnSignalNodeRemoved(const StringC& nodePath)
  {
    ONDEBUG(cerr << "DataServerControlNetClientBodyC::OnSignalNodeRemoved nodePath("<< nodePath << ")" << endl);

    RavlAssert(m_signalNodeRemoved.IsValid());
    m_signalNodeRemoved(nodePath);

    return true;
  }

  bool DataServerControlNetClientBodyC::OnQueryNodeSpace(UIntT requestId, bool result, SArray1dC<Int64T> values)
  {
    ONDEBUG(cerr << "DataServerControlNetClientBodyC::OnQueryNodeSpace requestId("<< requestId << ")" << endl);

    m_netRequestManager.DeliverReq(requestId, result, values);

    return true;
  }

}
