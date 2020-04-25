// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THREADSTHREAD_HEADER
#define RAVL_THREADSTHREAD_HEADER 1
//////////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/Thread.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="02/07/1999"
//! example=exThread.cc

#include "Ravl/config.h"
#if (!defined(__sgi__) && !RAVL_OS_FREEBSD)
#define _GNU_SOURCE 1
#define _POSIX_SOURCE 1
#endif

//#if defined(__sol2__)
#if RAVL_HAVE_SIGNAL_H
#include <sys/signal.h>
#endif

#if RAVL_HAVE_POSIX_THREADS
#include <pthread.h>
#endif

#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#endif

#if RAVL_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "Ravl/RCHandleV.hh"

namespace RavlN
{
  //! userlevel=Normal
  
  void OSYield();
  //: Yield control of processor
  // call if you wish a brief delay in execution.  
  // Particularly useful if you are forced to poll for an event.
   
  //! userlevel=Normal
  SizeT CurrentThreadID();
  //: Get ID of current running thread.
  
  SizeT ThisThreadID();
  //: Get ID for this thread.
  // OBSOLETE, use CurrentThreadID().

  enum ThreadSchedulingPolicyT {
    ThreadSchedulingPolicy_OTHER,
    ThreadSchedulingPolicy_BATCH,
    ThreadSchedulingPolicy_IDLE,
    ThreadSchedulingPolicy_FIFO,
    ThreadSchedulingPolicy_RR
  };

  bool SetCurrentSchedulingPolicy(ThreadSchedulingPolicyT policy,UIntT priority);
  //: Set current threads shedulign policy and priority

  bool SetCurrentThreadPriority(UIntT priority);
  //: Set the priority of the process
  // THREAD SAFE.
  //!param: priority - 0 to 32767, Higher means more runtime will be given to the thread.
  //!return: true if priority change succeeded, false otherwise.
  
  int CurrentThreadPriority();
  //: Get the priority of the process
  // THREAD SAFE.
  //!return: priority - 0 to 32767, Higher means more runtime will be given to the thread.
  
  extern void cancellationHandler(void *data);
  //! userlevel=Develop
  //: Called when a thread is cancelled.

#if RAVL_HAVE_POSIX_THREADS    
  void *StartThread(void *Data);
#endif
#if RAVL_HAVE_WIN32_THREADS
  DWORD WINAPI StartThread(LPVOID data);
#endif
  
  //! userlevel=Develop
  //: Thread body.
  
  class ThreadBodyC 
    : public RCBodyVC
  {
  public:
    ThreadBodyC(SizeT initStackSize = 0);
    //: Default constructor.

    ~ThreadBodyC();
    //: Destructor.
    
    bool Execute();
    //: Start thread running
    // use this command to start thread running after it has been created.  This function can only be called 
    // once.
    
    virtual void Terminate();
    //: Terminate thread.
    // The default version of this function is very dangerous. Stopping a thread whilst its
    // running is likely to cause memory leaks, and deadlocks. <br>
    // It is much better to have a thread check a flag (see ThreadEventC) 
    // and exit normally. <br>
    // If your going to use this method you should ensure that the thread
    // is not using reference counting, and has no resource locks at the
    // time this method is called. <br>
    // THREAD SAFE.
    
    bool SetPriority(int pri);
    //: Set the priority of the process
    //!param: pri - 0 to 32767, Higher faster.
    //!return: true if priority change succeeded, false otherwise.
    // THREAD SAFE.
    
    int Priority() const;
    //: Get the priority of the thread
    //!return: 0 to 32767, Higher means more runtime will be given to the thread.
    // THREAD SAFE.
    
    SizeT ID() const;
    //: Get a unique ID for this thread.
    // NB. An id may no be assigned to the thread until
    // after Execute() has been called.
    // THREAD SAFE.
    
    void SetStackSize(SizeT nStackSize)
    { stackSize = nStackSize; }
    //: Set the stack size in bytes to use for the thread. 
    // Note this will only have an effect before 'Execute()' is called.
    
  protected:
    virtual int Start(); 
    //: Called when thread started.  
    // This function should be overloaded
    // is a derived class.  It is called with the new thread after 
    // Execute() is called. 
    
    virtual int End();
    //: Called when thread terminates normally.  
    // Overloading this method
    // is optional.  There is no need to call this function 
    // directly. 

  protected:
    volatile bool terminatePending; // True if Terminate function has been called.
    
  private:
    void Startup();
    //: Start user code.
    
    void Cancel();
    //: Cancel thread.

#if RAVL_HAVE_POSIX_THREADS
    pthread_t threadID;
#endif
#if RAVL_HAVE_WIN32_THREADS
    HANDLE threadID;
#endif
    
    bool live; // Set to true after thread is created.
    
    SizeT stackSize; // Size of stack to create. 0=Use system default
    
#if RAVL_HAVE_POSIX_THREADS    
    friend void *StartThread(void *Data);
#endif
#if RAVL_HAVE_WIN32_THREADS
    friend DWORD WINAPI StartThread(LPVOID data);
#endif
    friend void cancellationHandler(void *data);
  };
  
  //! userlevel=Normal
  //: Handle to a thread.
  // In general it is not necessary to use this class directly;
  // it is better to use the LaunchThread(...) functions to start
  // a thread on a method. <br>
  // Note: The thread itself holds a reference count on the object.
  // This reference count is held until the thread terminates. 
  
  class ThreadC 
    : public RCHandleC<ThreadBodyC>
  {
  public:
    ThreadC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    ThreadC(ThreadBodyC &bod)
      : RCHandleC<ThreadBodyC>(bod)
    {}
    //: Body constructor.
    
    ThreadBodyC &Body() 
    { return RCHandleC<ThreadBodyC>::Body(); }
    //: Access body.
    
    const ThreadBodyC &Body() const
    { return RCHandleC<ThreadBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Execute()
    { return Body().Execute(); }
    //: Start thread running.
    // use this command to start thread running
    // after it has been created.  This function can only be called 
    // once.
    
    void Terminate()
    { Body().Terminate(); }
    //: Terminate thread.
    // The default versin of this function is very dangerous. Stopping a thread whilst its
    // running is likely to cause memory leaks, and deadlocks. <br>
    // It is much better to have a thread check a flag (see ThreadEventC) 
    // and exit normally. <br>
    // If your going to use this method you should ensure that the thread
    // is not using reference counting, and has no resource locks at the
    // time this method is called. <br>
    // THREAD SAFE.
    
    bool SetPriority(int pri)
    { return Body().SetPriority(pri); }
    //: Set the priority of the process
    //!param: pri - 0 to 32767, Higher means more runtime will be given to the thread.
    //!return: true if priority change succeeded.
    // THREAD SAFE.
    
    int Priority() const
    { return Body().Priority(); }
    //: Get the priority of the thread
    //!return: 0 to 32767, Higher means more runtime will be given to the thread.
    // THEAD SAFE.
    
    inline SizeT ID() const
    { return Body().ID(); }
    //: Get a unique ID for this thread.
    // NB. An id may no be assigned to the thread until
    // after Execute() has been called.
    // THEAD SAFE.
    
    void SetStackSize(SizeT nStackSize)
    { Body().SetStackSize(nStackSize); }
    //: Set the stack size in bytes to use for the thread. 
    // Note this will only have an effect before 'Execute()' is called.
    
  };

}



#endif
