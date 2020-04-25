// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/ThreadEvent.cc"

#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/OS/Date.hh"

namespace RavlN
{

  ThreadEventC::ThreadEventC()
    : m_occurred(false),
      m_waiting(0)
  {}

  bool ThreadEventC::Post() {
    m_access.Lock();
    if(m_occurred) {
      m_access.Unlock();
      return false;
    }
    m_occurred = true;
    m_access.Unlock();
    m_cond.Broadcast();
    return true;
  }
  //: Post an event.
  // Returns true, if event has been posted by this thread.

  ThreadEventC::~ThreadEventC()  {
    if(!m_occurred)
      Post();
    if(m_waiting != 0)
      std::cerr << "PThread::~ThreadEvent(), WARNING: Called while threads waiting. \n";
  }
  //: Destructor.

  //: Wait indefinitely for an event to be posted.
  void ThreadEventC::Wait() {
    if(m_occurred) // Check before we bother with locking.
      return ;
    m_access.Lock();
    m_waiting++;
    while(!m_occurred)
      m_cond.Wait(m_access);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast(); // If something is waiting for it to be free...
      m_access.Unlock();
      return ;
    }
    m_access.Unlock();
  }

  //: Wait for an event.
  // Returns false if timed out.
  
  bool ThreadEventC::Wait(RealT maxTime) {
    if(m_occurred) // Check before we bother with locking.
      return true;
    bool ret = true;
    m_access.Lock();
    m_waiting++;
    DateC deadline = DateC::NowUTC() + maxTime;
    while(!m_occurred && ret) 
      ret = m_cond.WaitUntil(m_access,deadline);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast(); // If something is waiting for it to be free...
      m_access.Unlock();
      return ret;
    }
    m_access.Unlock();
    return ret;
  }

  //: Wait for an event.
  // Returns false if timed out.

  bool ThreadEventC::WaitUntil(const DateC &deadline) {
    if(m_occurred) // Check before we bother with locking.
      return true;
    bool ret(true);
    m_access.Lock();
    m_waiting++;
    while(!m_occurred && ret)
      ret = m_cond.WaitUntil(m_access,deadline);
    m_waiting--;
    if(m_waiting == 0) {
      m_condWaiting.Broadcast(); // If something is waiting for it to be free...
      m_access.Unlock();
      return ret;
    }
    m_access.Unlock();
    return ret;
  }

  //: Wait for lock to be free.
  // NB. This is only guaranteed to work for one thread.
  
  bool ThreadEventC::WaitForFree() {
    if(m_waiting == 0)
      return true;
    m_access.Lock();
    while(m_waiting != 0) 
      m_condWaiting.Wait(m_access);
    m_access.Unlock();
    return true;
  }
  
  
}
