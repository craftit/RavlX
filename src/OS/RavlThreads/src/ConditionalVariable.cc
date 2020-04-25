// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/ConditionalVariable.cc"

#include "Ravl/config.h"
#if RAVL_OS_SOLARIS
#define __EXTENSIONS__ 1
#endif

#include "Ravl/Threads/ConditionalVariable.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Math.hh"
#include "Ravl/OS/Date.hh"

#if defined(VISUAL_CPP)
#include <time.h>
#else
#include <sys/time.h>
#endif

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <iostream>

#define NANOSEC 1000000000

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  void ConditionalVariableC::Error(const char *msg)  {
    std::cerr << "ConditionalVariableC::Error() :" << msg << " \n";
    RavlAssert(0);
  }


#if RAVL_HAVE_PTHREAD_COND 

  ConditionalVariableC::ConditionalVariableC()
  {
    if(pthread_cond_init(&cond,0))
      Error("pthread_cond_init failed. \n");
  }

  //: Destructor
  
  ConditionalVariableC::~ConditionalVariableC() {
    int maxRetry = 100;
    while(pthread_cond_destroy(&cond) && --maxRetry > 0) 
      OSYield();
    if(maxRetry <= 0)
      Error("WARNING: ConditionalVariableC::~ConditionalVariableC(), destroy failed. \n");
  }
  
  bool ConditionalVariableC::Wait(MutexC &umutex,RealT maxTime) {
    if(maxTime < 0) {
      std::cerr << "ConditionalVariableC::Wait, WARNING: Negative timeout given " << maxTime << ". Returning failed. \n";
      return false;
    }
    if(maxTime == 0)
      return false;
    struct timespec ts;
    struct timeval tv;

    // Work out delay.
    long secs = Floor(maxTime);
    long nsecs = (long) ((RealT) ((RealT) maxTime - ((RealT) secs)) * NANOSEC);
    
    // Get current time.
    gettimeofday(&tv,0);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
    
    // Add them.
    
    ts.tv_sec += secs;
    ts.tv_nsec += nsecs;
    if(ts.tv_nsec >= NANOSEC) {
      ts.tv_sec += 1;
      ts.tv_nsec -= NANOSEC;
    }
    
    int errcode;
    do {
      errcode = pthread_cond_timedwait(&cond,&umutex.mutex,&ts);
      if(errcode == ETIMEDOUT)
        break;
      // May be interrupted by EINTR... ignore and restart the wait.
      if ( errcode == 0 ) break ;
      RavlAssertMsg(errcode == EINTR,"ConditionalVariableC::Wait(), ERROR: Unexpected return code.");
    } while(errcode != 0);
    
    return (errcode != ETIMEDOUT);
  }

  //: Wait for conditional.
  // This unlocks the mutex and then waits for a signal
  // from either Signal, Broadcast or timeout.  When it get the signal
  // the mutex is re-locked and control returned to the
  // program. <p>
  // Returns false, if timeout occurs.

  bool ConditionalVariableC::WaitUntil(MutexC &umutex,const DateC &deadline)
  {
    struct timespec ts;

    ts.tv_sec = deadline.TotalSeconds();
    ts.tv_nsec = deadline.USeconds() * 1000;
    //std::cerr << "Deadline " << ts.tv_sec << " " <<ts.tv_nsec << "\n";
    int errcode;

    do {
      errcode = pthread_cond_timedwait(&cond,&umutex.mutex,&ts);
      if(errcode == ETIMEDOUT)
        break;
      // May be interrupted by EINTR... ignore and restart the wait.
      if ( errcode == 0 ) break ;
      if(errcode != EINTR) {
        if(errcode==EPERM) {
          std::cerr << "Conditional mutex not owned at time of call. \n";
        } else {
          std::cerr << "Code:" << errcode << "\n";
        }
        RavlAssertMsg(errcode == EINTR,"ConditionalVariableC::Wait(), ERROR: Unexpected return code.");
      }
    } while(errcode != 0);

    return (errcode != ETIMEDOUT);
  }

#else  
  // ----------------------------------------------------------------
  
  //! Constructor
  ConditionalVariableC::WaiterC::WaiterC()
    : m_woken(false)
  {
#if RAVL_HAVE_WIN32_THREADS
    m_sema = CreateEvent(0,0,0,0);
    if(m_sema == 0) {
      std::cerr << "Failed to create semaphore. \n";
      RavlAlwaysAssert(0); // This is really bad, stop things now.
    }
#else
    RavlAssert(0);
#endif
  }

  //! Destructor

  ConditionalVariableC::WaiterC::~WaiterC()
  {
#if RAVL_HAVE_WIN32_THREADS
    CloseHandle(m_sema);
#endif
  }

  //! Wake the waiter.

  void ConditionalVariableC::WaiterC::Wake()
  {
    m_woken = true;
    if(!SetEvent(m_sema)) {
      std::cerr << "ConditionalVariableC::Wake, Warning: Failed to wake thread. \n";
      RavlAssert(0);
    }
  }

  //! Wait for something to happen
  bool ConditionalVariableC::WaiterC::Wait(float maxWait) {
    DWORD wait = 0;
    // Make sure wait is >= 0
    if(maxWait > 0) 
      wait = Round(maxWait * 1000.0);
    DWORD rc = WaitForSingleObject(m_sema,wait);
    if(rc != WAIT_OBJECT_0 && rc != WAIT_TIMEOUT) {
      // Warn if something unexpected happened.
      std::cerr << "ConditionalVariableC::Wait(delay), Failed to wait for wake. \n";
    }
    //std::cerr << "ConditionalVariableC::Wait " << rc << "\n";
    return (rc == WAIT_OBJECT_0);
  }

  //! Wait for something to happen

  bool ConditionalVariableC::WaiterC::Wait()
  {
    DWORD rc = WaitForSingleObject(m_sema,INFINITE);
    if(rc != WAIT_OBJECT_0) {
      // Warn if something unexpected happened.
      std::cerr << "ConditionalVariableC::Wait(delay), Failed to wait for wake. \n";
    }
    return (rc == WAIT_OBJECT_0);
  }

  //: Reset condition.
  bool ConditionalVariableC::WaiterC::Reset() {
    m_woken = false;
    ResetEvent(m_sema);
    return true;
  }

  // -----------------------------------------------------------------------------------

  ConditionalVariableC::ConditionalVariableC()
    : m_waiting(false),
      m_free(false)
  {
    ONDEBUG(printf("constructor ConditionalVariableC  %p\n", this));
    //std::cerr << "Using local cond mutex. \n";
  }
  
  ConditionalVariableC::~ConditionalVariableC()
  {
    ONDEBUG(printf("destructor ConditionalVariableC %p\n", this));
    //RavlAssert(!m_access.TryLock());
    if(!m_waiting.IsEmpty()) {
      std::cerr << "ERROR: Destroying conditional mutex when there are threads waiting. \n";
    }
    m_free.Empty();
  }
  
  ConditionalVariableC::WaiterC *ConditionalVariableC::GetWaiter() {
    MutexLockC lock(m_access);
    WaiterC *waiter = 0;
    ONDEBUG(printf("%p GetWaiter  m_free: %i  m_waiting: %i\n", this, int(m_free.Size()), int(m_waiting.Size())));
    if(!m_free.IsEmpty()) {
      //FIXME: Ideally m_free would be a per thread global list.
      waiter = &m_free.PopFirst();
      // Make sure its ready to go.
      waiter->Reset();
      ONDEBUG(printf("using existing waiter %p\n", waiter));
    } else {
      waiter = new WaiterC();
      ONDEBUG(printf("created new waiter %p\n", waiter));
    }
    m_waiting.InsLast(*waiter);
    ONDEBUG(printf("%p done adding to wait list %i\n", this, int(m_waiting.Size())));
    //ONDEBUG(for(IntrDLIterC<ConditionalVariableC::WaiterC> it(m_waiting);it;it++) { printf("p : %p \n", it.operator->()); })
    return waiter;
  }

  bool ConditionalVariableC::FreeWaiter(ConditionalVariableC::WaiterC *waiter) {
    MutexLockC lock(m_access);
    ONDEBUG(printf("%p FreeWaiter %p m_free: %i  m_waiting : %i\n", this, waiter, int(m_free.Size()), int(m_waiting.Size())));
    bool got = waiter->IsWoken();
    if(!got) {
      waiter->Unlink();
    }
    // Put it on the free list.
    m_free.InsFirst(*waiter);
    ONDEBUG(printf("%p FreeWaiter done %p m_free: %i  m_waiting %i\n", this, waiter, int(m_free.Size()), int(m_waiting.Size())));
    return got;
  }

  bool ConditionalVariableC::Wait(MutexC &umutex,RealT maxTime) {
#if RAVL_HAVE_WIN32_THREADS
    RavlAssert(!umutex.TryLock());
    WaiterC *waiter = GetWaiter();
    umutex.Unlock();
    waiter->Wait(maxTime);
    umutex.Lock();
    return FreeWaiter(waiter);
#else
    RavlAlwaysAssert(0);// Not implemented.
#endif
  }
  
  bool ConditionalVariableC::WaitUntil(MutexC &umutex,const DateC &deadline)
  {
    DateC maxTime = deadline - DateC::NowUTC();
    RealT maxTimeMilliseconds = maxTime.TotalSeconds() + (maxTime.USeconds() / 1000000.0);
    return Wait(umutex,maxTimeMilliseconds);
  }
  
  //: Broadcast a signal to all waiting threads.
  // Always succeeds.
  
  void ConditionalVariableC::Broadcast() {
#if RAVL_HAVE_WIN32_THREADS
    MutexLockC lock(m_access);
    ONDEBUG(printf("Broadcast m_waiting size: %i\n", int(m_waiting.Size())));
    while(!m_waiting.IsEmpty()) {
      m_waiting.PopFirst().Wake();
    }
    return ;
#else
    RavlAssert(0); // Not implemented.
#endif
  }
  
  //: Signal one waiting thread.
  // Always succeeds.
    
  void ConditionalVariableC::Signal() {
#if RAVL_HAVE_WIN32_THREADS
    MutexLockC lock(m_access);
    ONDEBUG(printf("Signal m_waiting size: %i\n", int(m_waiting.Size())));
    if(!m_waiting.IsEmpty()) {
      m_waiting.PopFirst().Wake();
    }
#else
    RavlAssert(0); // Not implemented.
#endif
  }
  
  void ConditionalVariableC::Wait(MutexC &umutex) {
#if RAVL_HAVE_WIN32_THREADS
    RavlAssert(!umutex.TryLock());
    WaiterC *waiter = GetWaiter();
    umutex.Unlock();
    waiter->Wait();
    umutex.Lock();
    FreeWaiter(waiter);
#else
    RavlAssert(0); // Not implemented.
#endif
  }
    
#endif  
}
