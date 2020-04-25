// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! lib=RavlThreads

#include "Ravl/Threads/ThreadState.hh"

namespace RavlN
{
  //: Constructor
  ThreadStateBaseC::ThreadStateBaseC()
   : m_waiting(0)
  {}

  //: Destructor.
  ThreadStateBaseC::~ThreadStateBaseC()
  {
    if(m_waiting != 0)
      std::cerr << "ThreadStateBaseC::~ThreadStateBaseC, WARNING: Called while threads waiting. \n";
  }

  //: Wait for lock to be free of all waiters.

  bool ThreadStateBaseC::WaitForFree() {
    if(m_waiting == 0)
      return true;
    m_access.Lock();
    while(m_waiting != 0)
      m_cond.Wait(m_access);
    m_access.Unlock();
    return true;
  }

}
