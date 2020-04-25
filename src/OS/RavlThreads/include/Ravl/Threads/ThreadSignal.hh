// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THREADSIGNAL_HEADER
#define RAVL_THREADSIGNAL_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/ThreadSignal.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="25/05/2000"

#include "Ravl/config.h"
#include "Ravl/Threads/ConditionalVariable.hh"
#include "Ravl/Stream.hh"

namespace RavlN
{
  //! userlevel=Normal
  //: Broadcast, signal or wait for an event &ndash; edge-triggered.
  // <p>This class enables a thread to sleep until signalled from another thread.</p>
  // <p>In this class, the Wait() will wait until the next WakeSingle() or WakeAll() </p>
  // <p> Note: if you are implementing new threading primitives you may want to consider <a href="RavlN.ConditionalVariableC.html">ConditionalVariableC</a>
  // which also manages the lock and unlocking of a mutex. </p>

  class ThreadSignalC {

  public:
    ThreadSignalC();
    //: Constructor.
    
    ~ThreadSignalC();
    //: Destructor.

    bool WakeSingle();
    //: Wake a single waiting thread.

    bool WakeAll();
    //: Wake all waiting threads.

    void Wait();
    //: Wait indefinitely for a wake event
    
    bool WaitForFree();
    //: Wait for lock to be free of all waiters.

    IntT ThreadsWaiting() const 
    { return m_waiting; }
    //: Get approximation of number of threads waiting.
    
    bool Wait(RealT maxTime);
    //: Wait for a wake event for a limited amount of time.
    // Returns false if timed out.

    bool WaitUntil(const DateC &deadline);
    //: Wait for a wake event or until a timeout
    // Returns false if timed out.
    
  protected:
    MutexC m_access;
    ConditionalVariableC cond;
    ConditionalVariableC m_condWaiting;
    volatile IntT m_waiting; // Count of number of threads waiting on this...
  };
};

#endif
