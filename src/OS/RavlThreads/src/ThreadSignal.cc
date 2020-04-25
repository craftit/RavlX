// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/ThreadSignal.cc"

#include "Ravl/Threads/ThreadSignal.hh"
#include "Ravl/OS/Date.hh"

namespace RavlN
{

  ThreadSignalC::ThreadSignalC()
    : m_waiting(0)
  {}

  bool ThreadSignalC::WakeAll() {
    cond.Broadcast();
    return true;
  }
  //: Post an event.
  // Returns true, if event has been posted by this thread.

  bool ThreadSignalC::WakeSingle() {
    cond.Signal();
    return true;
  }
  //: Post an event.
  // Returns true, if event has been posted by this thread.

  ThreadSignalC::~ThreadSignalC()  {
    WakeAll();
    WaitForFree();
    if(m_waiting != 0)
      std::cerr << "PThread::~ThreadSignal(), WARNING: Called while threads waiting. \n";
  }
  //: Destructor.

  //: Wait indefinitely for an event to be posted.
  void ThreadSignalC::Wait() {
    m_access.Lock();
    m_waiting++;
    cond.Wait(m_access);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast();
      m_access.Unlock();
      return ;
    }
    m_access.Unlock();
  }

  //: Wait for an event.
  // Returns false if timed out.
  
  bool ThreadSignalC::Wait(RealT maxTime) {
    bool ret = true;
    m_access.Lock();
    m_waiting++;
    DateC deadline = DateC::NowUTC() + maxTime;
    ret = cond.WaitUntil(m_access,deadline);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast();
      m_access.Unlock();
      return ret;
    }
    m_access.Unlock();
    return ret;
  }

  //: Wait for an event.
  // Returns false if timed out.

  bool ThreadSignalC::WaitUntil(const DateC &deadline) {
    bool ret = true;
    m_access.Lock();
    m_waiting++;
    ret = cond.WaitUntil(m_access,deadline);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast();
      m_access.Unlock();
      return ret;
    }
    m_access.Unlock();
    return ret;
  }

  //: Wait for lock to be free.
  // NB. This is only guaranteed to work for one thread.
  
  bool ThreadSignalC::WaitForFree() {
    if(m_waiting == 0)
      return true;
    m_access.Lock();
    while(m_waiting != 0) 
      m_condWaiting.Wait(m_access);
    m_access.Unlock();
    return true;
  }
  
  
}
