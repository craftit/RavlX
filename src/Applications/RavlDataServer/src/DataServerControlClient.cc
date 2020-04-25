// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlClient.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  DataServerControlClientBodyC::DataServerControlClientBodyC(DataServerC& dataServer)
  : m_dataServer(dataServer)
  {}

  bool DataServerControlClientBodyC::AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options)
  {
    if (m_dataServer.IsValid())
      return m_dataServer.AddNode(path, nodeType, options);
    return false;
  }

  bool DataServerControlClientBodyC::RemoveNode(const StringC& path, bool removeFromDisk)
  {
    if (m_dataServer.IsValid())
      return m_dataServer.RemoveNode(path, removeFromDisk);
    return false;
  }

  Signal1C<StringC>& DataServerControlClientBodyC::SignalNodeClosed()
  {
    RavlAssert(m_dataServer.IsValid());
    return m_dataServer.SignalNodeClosed();
  }

  Signal1C<StringC>& DataServerControlClientBodyC::SignalNodeRemoved()
  {
    RavlAssert(m_dataServer.IsValid());
    return m_dataServer.SignalNodeRemoved();
  }

  bool DataServerControlClientBodyC::QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available)
  {
    if (m_dataServer.IsValid())
      return m_dataServer.QueryNodeSpace(path, total, used, available);
    return false;
  }

}
