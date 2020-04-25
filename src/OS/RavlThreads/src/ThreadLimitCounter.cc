// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/*
 * ThreadLimitCounter.cc
 *
 *  Created on: 9 Aug 2011
 *      Author: charles
 */
//! lib=RavlThreads

#include "Ravl/Threads/ThreadLimitCounter.hh"

namespace RavlN {

  //: Constructor
  ThreadLimitCounterC::ThreadLimitCounterC(UIntT count)
   : m_count(count),
     m_maxCount(count),
     m_waiting(0)
  {
    RavlAssert(m_maxCount > 0);
    if(m_maxCount <= 0)
      throw RavlN::ExceptionOutOfRangeC("Count to large.");
  }

  //: Release a number of units back to the pool.
  void ThreadLimitCounterC::Release(UIntT units)
  {
    m_access.Lock();
    m_count += units;
    RavlAssert(m_count <= m_maxCount);
    m_access.Unlock();
    m_cond.Broadcast();
  }

  bool ThreadLimitCounterC::Request(UIntT units)
  {
    RavlAssert(((IntT) units) >= 0); // Alert us if we're being passed silly numbers
    if (units > (UIntT) m_maxCount)
      return false;
    m_access.Lock();
    m_waiting++;
    while (m_count < (IntT) units)
      m_cond.Wait(m_access);
    m_count -= units;
    RavlAssert(m_count >= 0);
    UIntT waiting = --m_waiting;
    m_access.Unlock();
    if(waiting == 0)
      m_cond.Broadcast();
    return true;
  }

  bool ThreadLimitCounterC::RequestWait(UIntT units, float maxTime)
  {
    bool ret = true;
    RavlAssert(((IntT) units) >= 0);// Alert us if we're being passed silly numbers
    m_access.Lock();
    m_waiting++;
    DateC deadline = DateC::NowUTC() + maxTime;
    while (m_count < (IntT) units)
      ret = m_cond.WaitUntil(m_access,deadline);
    if (ret) {
      m_count -= units;
      RavlAssert(m_count >= 0);
    }
    UIntT waiting = --m_waiting;
    m_access.Unlock();
    if(waiting == 0)
      m_cond.Broadcast();
    return ret;
  }

  //: Wait for lock to be free of all waiters.
  bool ThreadLimitCounterC::WaitForFree() const
  {
    if (m_waiting == 0)
      return true;
    m_access.Lock();
    while (m_waiting != 0)
      m_cond.Wait(m_access);
    m_access.Unlock();
    return true;
  }

}
