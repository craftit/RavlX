// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlConnection.hh"
#include "Ravl/OS/Socket.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/DataServer/DataServerControlMessages.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  DataServerControlConnectionBodyC::DataServerControlConnectionBodyC(SocketC& socket, DataServerControlInterfaceC& controller)
  : NetEndPointBodyC(socket, false),
    m_controller(controller),
    m_initialised(false)
  {
    RavlAssert(socket.IsValid());
    RavlAssert(socket.IsOpen());

    ConnectR(SigConnectionBroken(), *this, &DataServerControlConnectionBodyC::OnConnectionBroken);
    m_connectionSet += Connect(m_controller.SignalNodeClosed(), DataServerControlConnectionC(*this), &DataServerControlConnectionC::OnSignalNodeClosed);
    m_connectionSet += Connect(m_controller.SignalNodeRemoved(), DataServerControlConnectionC(*this), &DataServerControlConnectionC::OnSignalNodeRemoved);
  }

  bool DataServerControlConnectionBodyC::Initialise()
  {
    if (m_initialised)
      return true;

    RegisterR(static_cast<UIntT>(DATASERVERCONTROL_ADDNODE), StringC("AddNode"), *this, &DataServerControlConnectionBodyC::OnAddNode);
    RegisterR(static_cast<UIntT>(DATASERVERCONTROL_REMOVENODE), StringC("RemoveNode"), *this, &DataServerControlConnectionBodyC::OnRemoveNode);
    RegisterR(static_cast<UIntT>(DATASERVERCONTROL_QUERYNODESPACE), StringC("QueryNodeSpace"), *this, &DataServerControlConnectionBodyC::OnQueryNodeSpace);

    Ready();
    if (!WaitSetupComplete())
    {
      Close();
      return false;
    }

    m_initialised = true;

    return true;
  }

  bool DataServerControlConnectionBodyC::OnConnectionBroken()
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnConnectionBroken" << endl);
    m_connectionSet.DisconnectAll();
    
    return true;
  }

  bool DataServerControlConnectionBodyC::OnAddNode(UIntT requestId, const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options)
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnAddNode requestId(" << requestId << ") path(" << path << ") nodeType(" << nodeType << ")" << endl);
    bool result = false;
    if (m_controller.IsValid())
    {
      result = m_controller.AddNode(path, nodeType, options);
    }
    Send(DATASERVERCONTROL_ADDNODE_RESULT, requestId, result);

    return result;
  }

  bool DataServerControlConnectionBodyC::OnRemoveNode(UIntT requestId, const StringC& path, bool removeFromDisk)
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnRemoveNode requestId(" << requestId << ") path(" << path << ") removeFromDisk(" << (removeFromDisk ? "Y" : "N") << ")" << endl);
    bool result = false;
    if (m_controller.IsValid())
    {
      result = m_controller.RemoveNode(path, removeFromDisk);
    }
    Send(DATASERVERCONTROL_REMOVENODE_RESULT, requestId, result);

    return result;
  }

  bool DataServerControlConnectionBodyC::OnSignalNodeClosed(StringC& nodePath)
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnNodeClosed" << endl);

    Send(DATASERVERCONTROL_NODECLOSED, nodePath);

    return true;
  }

  bool DataServerControlConnectionBodyC::OnSignalNodeRemoved(StringC& nodePath)
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnSignalNodeRemoved" << endl);

    Send(DATASERVERCONTROL_NODEREMOVED, nodePath);

    return true;
  }
  
  bool DataServerControlConnectionBodyC::OnQueryNodeSpace(UIntT requestId, const StringC& path)
  {
    ONDEBUG(cerr << "DataServerControlConnectionBodyC::OnQueryNodeSpace requestId(" << requestId << ") path(" << path << ")" << endl);
    bool result = false;
    SArray1dC<Int64T> values(3);
    if (m_controller.IsValid())
    {
      Int64T total, used, available;
      result = m_controller.QueryNodeSpace(path, total, used, available);

      if (result)
      {
        values[0] = total;
        values[1] = used;
        values[2] = available;
      }
    }
    Send(DATASERVERCONTROL_QUERYNODESPACE_RESULT, requestId, result, values);

    return result;
  }
  
}
