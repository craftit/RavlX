// This file is part of RAVL, Recognition And Vision Library.
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDataServer

#include "Ravl/DataServer/DataServerControlInterface.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  DataServerControlInterfaceBodyC::DataServerControlInterfaceBodyC()
  : m_signalNodeClosed(StringC()),
    m_signalNodeRemoved(StringC())
  {}

  bool DataServerControlInterfaceBodyC::AddNode(const StringC& path, const StringC& nodeType, const HashC<StringC, StringC>& options)
  {
    RavlAssertMsg(false,"Not implemented");
    return false;
  }

  bool DataServerControlInterfaceBodyC::RemoveNode(const StringC& path, bool removeFromDisk)
  {
    RavlAssertMsg(false,"Not implemented");
    return false;
  }

  bool DataServerControlInterfaceBodyC::QueryNodeSpace(const StringC& path, Int64T& total, Int64T& used, Int64T& available)
  {
    RavlAssertMsg(false,"Not implemented");
    return false;
  }

}
