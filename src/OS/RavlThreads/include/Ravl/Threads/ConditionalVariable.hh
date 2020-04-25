// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTHREADS_CONDITIONALVARIABLE_HEADER
#define RAVLTHREADS_CONDITIONALVARIABLE_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/ConditionalVariable.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="02/07/1999"

#include "Ravl/config.h"
#if !defined(_POSIX_SOURCE) && !defined(__sgi__) && !RAVL_OS_FREEBSD
#define _POSIX_SOURCE 1
#endif

//#if defined(__sol2__)
#if RAVL_HAVE_SIGNAL_H
#include <sys/signal.h>
#endif

#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#include "Ravl/IntrDList.hh"

#endif
#if RAVL_HAVE_POSIX_THREADS
#include <pthread.h>
#endif

#include "Ravl/Types.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlN
{
  
  class DateC;

  //! userlevel=Normal
  //: Sleeping until signal arrives.
  //
  // <p>This class implements a "condition variable".  
  // It causes a thread to sleep until signalled from another thread.  </p>
  //
  // <p>ConditionalVariableC wraps the pthreads condition variable and
  // its associated mutex
  // into a single object.  See man pages on e.g. pthread_cond_init for
  // a full description.  See also SemaphoreC for an example of its
  // use.</p>
  //
  // <p>In this class, Wait() will only wake up once after each Broadcast(), which is why it does not need resetting after a Broadcast() (in contrast to <a href="RavlN.ThreadEventC.html">ThreadEventC</a>).  If this "edge-triggered" behaviour is not what is wanted, or if this class is used only for its ability to wake up other sleeping threads, <a href="RavlN.ThreadEventC.html">ThreadEventC</a> may be a better choice.</p>
  
  class ConditionalVariableC
  {
  public:
    ConditionalVariableC();
    //: Constructor.

    ~ConditionalVariableC();
    //: Destructor

    void Broadcast()
#if RAVL_HAVE_PTHREAD_COND 
    { pthread_cond_broadcast(&cond); }
    //: Broadcast a signal to all waiting threads.
    // Always succeeds.
#else
    ;
#endif
    
    void Signal() 
#if RAVL_HAVE_PTHREAD_COND 
    { pthread_cond_signal(&cond); }
    //: Signal one waiting thread.
    // Always succeeds.  The particular thread selected is arbitrary.
#else
    ;
#endif
    
    void Wait(MutexC &umutex)
#if RAVL_HAVE_PTHREAD_COND 
    { pthread_cond_wait(&cond,&umutex.mutex); }
    //: Wait for conditional.
    // <p>This unlocks the mutex and then waits for a signal
    // from either Signal or Broadcast.  When it gets the signal
    // the mutex is re-locked and control returned to the
    // program. </p>
    // <p>Always succeeds.</p>
#else
    ;
#endif
    
    bool Wait(MutexC &umutex,RealT maxTime);
    //: Wait for conditional.
    // This unlocks the mutex and then waits for a signal
    // from either Signal, Broadcast or timeout.  When it get the signal
    // the mutex is re-locked and control returned to the
    // program. <p>
    // Returns false, if timeout occurs.

    bool WaitUntil(MutexC &umutex,const DateC &deadline);
    //: Wait for conditional.
    // This unlocks the mutex and then waits for a signal
    // from either Signal, Broadcast or timeout.  When it get the signal
    // the mutex is re-locked and control returned to the
    // program. <p>
    // Returns false, if timeout occurs.

  protected:
    void Error(const char *msg);
    //: Report an error.

  private:
    ConditionalVariableC(const ConditionalVariableC &)
    {}
    //: This is just a bad idea.

#if RAVL_HAVE_PTHREAD_COND 
    pthread_cond_t cond;
#endif
#if RAVL_HAVE_WIN32_THREADS
    MutexC m_access;

    class WaiterC
      : public DLinkC
    {
    public:
      //! Constructor
      WaiterC();

      //! Destructor
      ~WaiterC();

      //! Wake the waiter.
      void Wake();

      //! Wait for something to happen
      bool Wait();

      //! Wait for something to happen
      bool Wait(float maxWait);

      //! Reset condition.
      bool Reset();

      //! Has waiter been woken ?
      bool IsWoken() const
      { return m_woken; }

      HANDLE m_sema;
      bool m_woken;
    };

    // Allocate a new waiter.
    WaiterC *GetWaiter();

    // Free a waiter.
    bool FreeWaiter(WaiterC *waiter);

    IntrDListC<WaiterC> m_free;
    IntrDListC<WaiterC> m_waiting;
#endif
  };
}

#endif
