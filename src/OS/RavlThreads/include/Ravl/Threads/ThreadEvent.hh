// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTHREADS_EVENT_HEADER
#define RAVLTHREADS_EVENT_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/ThreadEvent.hh"
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
  //: Post or wait for an event &ndash; level-triggered.
  // <p>This class enables a thread to sleep until signalled from another thread.</p>
  // <p>In this class, the Wait() will wait until the next Post(), whereupon it will not wait until the next Reset().  Thus it is a "level-triggerered" event, in contrast to <a href="RavlN.ThreadSignalC.html">ThreadSignalC</a>.</p>

  class ThreadEventC {

  public:
    ThreadEventC();
    
    ~ThreadEventC();
    //: Destructor.

    bool Post();
    //: Post an event.
    // Returns true, if event has been posted by this thread.
    
    void Wait();
    //: Wait indefinitely for an event to be posted.
    
    bool WaitForFree();
    //: Wait for lock to be free of all waiters.
    
    IntT ThreadsWaiting() const 
    { return m_waiting; }
    //: Get approximation of number of threads waiting.
    
    bool Wait(RealT maxTime);
    //: Wait for an event.
    // Returns false if timed out.

    bool WaitUntil(const DateC &deadline);
    //: Wait for an event.
    // Returns false if timed out.

    operator bool() const 
    { return m_occurred; }
    //: Test if the event has occurred.
    
    bool Occurred() const
    { return m_occurred; }
    //: Test if event has occurred.
    
    void Reset()
    { m_occurred = false; }
    //: Reset an event.
    
  protected:
    MutexC m_access;
    ConditionalVariableC m_cond;
    ConditionalVariableC m_condWaiting;
    volatile bool m_occurred;
    volatile IntT m_waiting; // Count of number of threads waiting on this...
  };
};

#endif
