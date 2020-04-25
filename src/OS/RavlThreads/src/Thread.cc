// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/Thread.cc"

#include "Ravl/config.h"

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <errno.h>

#if RAVL_HAVE_THREAD_H
#include <thread.h>
#endif
#if RAVL_HAVE_SCHED_H
#include <sched.h>
#endif

#if RAVL_HAVE_POSIX_THREADS
#include <pthread.h>
#endif

#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#endif

#include "Ravl/Threads/Thread.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  extern void IncPThreadSysDBLock();
  extern void IncPThreadAMutexInit();
  
  void DummyIncFunc() { 
    IncPThreadAMutexInit();
    IncPThreadSysDBLock(); 
  }
  
  void OSYield() {
#if RAVL_HAVE_THR_YIELD
    thr_yield();
#elif RAVL_HAVE_SCHED_YIELD
    sched_yield();
#elif RAVL_HAVE_YIELD
    yield();
#elif RAVL_HAVE_WIN32_THREADS
    ::Sleep(0);
#else
    sleep(0);
#endif
  }

  // Function to force inclusion of RWLock code
  // for system DB.
  
#if RAVL_HAVE_POSIX_THREADS    
  extern void *StartThread(void *Data);
#endif
#if RAVL_HAVE_WIN32_THREADS
  extern DWORD WINAPI StartThread(LPVOID data);
#endif
  
  //: Get ID of current thread.

  SizeT CurrentThreadID() {
#if RAVL_HAVE_POSIX_THREADS
    return (size_t) pthread_self();
#endif
#if RAVL_HAVE_WIN32_THREADS
    return (UInt64T) GetCurrentThread();
#endif
  }


  //: Set current threads shedulign policy and priority
  
  bool SetCurrentSchedulingPolicy(ThreadSchedulingPolicyT newPolicy,UIntT priority) {
#if RAVL_HAVE_POSIX_THREADS
    pthread_t threadID = pthread_self();
    int policy;
    struct sched_param sparam;
    pthread_getschedparam(threadID,&policy,&sparam);
    sparam.sched_priority = priority;

    switch(newPolicy)
    {

      case ThreadSchedulingPolicy_BATCH:
#ifdef SCHED_BATCH
        policy = SCHED_BATCH; break;
#else
        policy = SCHED_OTHER; break;
#endif
      case ThreadSchedulingPolicy_IDLE:
#ifdef SCHED_IDLE
        policy = SCHED_IDLE; break;
#else
        policy = SCHED_OTHER; break;
#endif
      case ThreadSchedulingPolicy_OTHER: policy = SCHED_OTHER; break;
      case ThreadSchedulingPolicy_FIFO: policy = SCHED_FIFO; break;
      case ThreadSchedulingPolicy_RR: policy = SCHED_RR; break;
    }

    if(pthread_setschedparam(threadID,policy,&sparam) == 0)
      return true; 
    //cerr << "SetCurrentThreadPriority(). Error:" << errno << "\n";
    return false;
#else
    return false;
#endif
  }



  // Set the priority of the process
  // THEAD SAFE.
  //!param: priority - 0 to 32767, Higher means more runtime will be given to the thread.
  //!return: true if priority change succeeded, false otherwise.
  
  bool SetCurrentThreadPriority(UIntT priority) {
#if RAVL_HAVE_POSIX_THREADS    
    pthread_t threadID = pthread_self();
    int policy;
    struct sched_param sparam;
    pthread_getschedparam(threadID,&policy,&sparam);
    sparam.sched_priority = priority;
    if(pthread_setschedparam(threadID,policy,&sparam) == 0)
      return true; // I hope, but don't know.
    //cerr << "SetCurrentThreadPriority(). Error:" << errno << "\n";
    return false;
#endif
#if RAVL_HAVE_WIN32_THREADS
    SetThreadPriority(GetCurrentThread(),priority);
    return true;
#endif
  }

  //: Get the priority of the process
  // THEAD SAFE.
  //!return: priority - 0 to 32767, Higher means more runtime will be given to the thread.
  
  int CurrentThreadPriority() {
#if RAVL_HAVE_POSIX_THREADS    
    pthread_t threadID = pthread_self();
    int policy;
    struct sched_param sparam;
    pthread_getschedparam(threadID,&policy,&sparam);
    return sparam.sched_priority;
#endif
#if RAVL_HAVE_WIN32_THREADS
    RavlAssertMsg(0,"CurrentThreadPriority(), Not implemented on WIN32. ");
    return 0;
#endif
  }

  //: Default constructor.
  
  ThreadBodyC::ThreadBodyC(SizeT initStackSize)
    : terminatePending(false),
      threadID(0),
      live(false),
      stackSize(initStackSize)
  {}
  
  //: Destructor.
  
  ThreadBodyC::~ThreadBodyC()
  { 
    if(live) {
      RavlAssertMsg(0,"ERROR: Destructor called on live thread."); // This is bad!  Core dump so we can trace where this happened.
      std::cerr << "WARNING: Destructor called on live thread. \n";
      
#if RAVL_HAVE_POSIX_THREADS
#if RAVL_OS_CYGWIN
      pthread_exit((void *)threadID);
#else
      pthread_cancel(threadID);
#endif
#endif
#if RAVL_HAVE_WIN32_THREADS
      TerminateThread(threadID,-1);
#endif
    }
#if RAVL_HAVE_POSIX_THREADS
    pthread_t tmpThreadID = threadID;
    threadID = 0; // Forget the ID, no other function will work now.
    if (tmpThreadID != 0)
      pthread_detach(tmpThreadID);
#endif
#if RAVL_HAVE_WIN32_THREADS
    HANDLE tmpThreadID = threadID;
    threadID = 0; // Forget the ID, no other function will work now.
    CloseHandle(threadID);
#endif
    RavlAssert(!live);
  }

  //: Called when thread started.  
  // This function should be overloaded
  // is a derived class.  It is called with the new thread after 
  // Execute() is called. 
  
  int ThreadBodyC::Start() {
    std::cerr << "WARNING: Abstract method called, ThreadBodyC::Start(). \n";
    return 0;
  }
  
  //: Called when thread terminates normally.  
  // Overloading this method
  // is optional.  There is no need to call this function 
  // directly. 
  
  int ThreadBodyC::End() {
    return 0;
  }
  
  //: Cancel thread.
  // NOT A USER FUNTION, DO NOT CALL DIRECTLY.
  
  void ThreadBodyC::Cancel() { 
    RavlAssert(live); // This should be a live thread.
    terminatePending = true;
    try {
      End();
    } catch(ExceptionC &e) {
      std::cerr << "RAVL Exception :'" << e.what() << "'\n";
      std::cerr << "While terminating thread. \n";
    } catch(exception &e) {
      std::cerr << "C++ Exception :'" << e.what() << "'\n";
      std::cerr << "While terminating thread. \n";
    } catch(...) {
      std::cerr << "Unknown exception caught. \n";
      std::cerr << "While terminating thread. \n";
    }
    // Lastly remove reference to class from thread.
    // and delete if needed.
    if(live) { // This should only be called once, on a live thread.
      live = false;
      if(DecRefCounter())
        delete this;
    }
  }
  
  // Thread cancellation function.
  
  void cancellationHandler(void *data) {
    ((ThreadBodyC *) data)->Cancel();
  }
  
  //: Terminate thread.
  // Call to stop the thread running.
  // THEAD SAFE.
  
  void ThreadBodyC::Terminate() {
    terminatePending = true;
#if RAVL_HAVE_WIN32_THREADS
    if(!TerminateThread(threadID,-1)) 
      std::cerr << "ThreadBodyC::Terminate(), Error killing thread. Error:" << GetLastError() << "\n";
#endif
#if RAVL_HAVE_POSIX_THREADS
    int ret = 0;
#if defined(__cygwin_OLD__)
    if((ret = pthread_exit((void *)threadID)) != 0)
      std::cerr << "ThreadBodyC::Terminate(), Error killing thread. " << ret << "\n";
#else
    if((ret = pthread_cancel(threadID)) != 0)
      std::cerr << "ThreadBodyC::Terminate(), Error killing thread. " << ret << "\n";
#endif
#endif
  }
  
  // Start thread running, use this command to start thread running
  // after it has been created.  This function can only be called 
  // once.
  
  bool ThreadBodyC::Execute() {
    if(live)
      return false;
    if(threadID != 0) { // Restarting thread ??
#if RAVL_HAVE_WIN32_THREADS
      CloseHandle(threadID);
#endif
#if RAVL_HAVE_POSIX_THREADS
      pthread_detach(threadID);
#endif
    }
    terminatePending = false;
    live = true;
    RavlAssert(References() > 0); // There should be at least one reference here.
    // Add reference to new thread which will be released by cleanup function.
    // This will be held while the thread is running and will
    // be removed by the thread cancellation function.
    IncRefCounter();
#if RAVL_HAVE_POSIX_THREADS 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
#if 0
    // The following code is used to dump the default stacksize.
    SizeT val;
    pthread_attr_getstacksize(&attr,&val);
    std::cerr << "StackSize=" << val << "\n";
#endif
#if 0
    // Check the guard size used to detect overflows.
    size_t guardSize = 0;
    if(pthread_attr_getguardsize (&attr,&guardSize) != 0){
      std::cerr << "Failed to obtain guard size. \n";
    }
    std::cerr << "GuardSize=" << guardSize << "\n";
#endif

    if(stackSize > 0) {
      if(pthread_attr_setstacksize(&attr,stackSize) != 0) {
        std::cerr << "ThreadBodyC::Execute, Failed to set stack size to " << stackSize << "\n";
      }
    }
    
    if(pthread_create(&threadID,&attr,StartThread,(void *) this) != 0) 
#endif
#if RAVL_HAVE_WIN32_THREADS
    DWORD winThreadId;
    if((threadID = CreateThread(0,0,StartThread,(void *) this,0,&winThreadId)) == 0) 
#endif
      {
	cerr << "ThreadBodyC::Execute(), ERROR: Failed to execute thread. " << errno << " \n";
	live = false; // Its definitly not live!
	DecRefCounter(); // This can't be the last reference.
	RavlAssert(References() > 0);
#if RAVL_HAVE_POSIX_THREADS     
        pthread_attr_destroy(&attr);
#endif
	return false;
      }    
#if RAVL_HAVE_POSIX_THREADS     
    pthread_attr_destroy(&attr);
#endif
    return true;
  }
  
  bool ThreadBodyC::SetPriority(int Pri) {
#if RAVL_HAVE_POSIX_THREADS    
    int policy;
    struct sched_param sparam;
    pthread_getschedparam(threadID,&policy,&sparam);
    sparam.sched_priority = Pri;
    if(pthread_setschedparam(threadID,policy,&sparam) == 0)
      return true; // I hope, but don't know.
    return false;
#endif
#if RAVL_HAVE_WIN32_THREADS
    SetThreadPriority(threadID,Pri);
    return true;
#endif
  }
  
  //: Get the priority of the thread
  //!return: 0 to 32767, Higher means more runtime will be given to the thread.
  // THEAD SAFE.
  
  int ThreadBodyC::Priority() const {
#if RAVL_HAVE_POSIX_THREADS    
    int policy;
    struct sched_param sparam;
    pthread_getschedparam(threadID,&policy,&sparam);
    return sparam.sched_priority;
#endif
#if RAVL_HAVE_WIN32_THREADS
    RavlAssertMsg(0,"ThreadBodyC::Priority(), Not implemented on WIN32. ");
    return 0;
#endif
  }
  
  
  // Set the priority of the process
  // 0 to 32767, Higher faster.
  // THEAD SAFE.

  void ThreadBodyC::Startup() {
#if 0
    try {
#endif
      Start();
#if 0
    } catch(ExceptionC &e) {
      std::cerr << "RAVL Exception :'" << e.what() << "'\n";
      std::cerr << "Terminating thread. \n";
    } catch(exception &e) {
      std::cerr << "C++ Exception :'" << e.what() << "'\n";
      std::cerr << "Terminating thread. \n";
    } catch(...) {
      std::cerr << "Unknown exception caught. \n";
      std::cerr << "Terminating thread. \n";
    }
#endif
  }

  ///////////////////////
  // Start thread running.
  
#if RAVL_HAVE_POSIX_THREADS    
  void *StartThread(void *data) {
    //cerr << "Started1\n" << flush <<endl;
    pthread_cleanup_push(cancellationHandler,data);
    ((ThreadBodyC *) data)->Startup();
    pthread_cleanup_pop(1); // Execute cleanup function.
    return NULL;
  }
#endif
#if RAVL_HAVE_WIN32_THREADS
  DWORD WINAPI StartThread(LPVOID data) {
    //cerr << "Started1\n" << flush <<endl;
    ((ThreadBodyC *) data)->Startup();
    ((ThreadBodyC *) data)->Cancel();
    ExitThread(0);
    return 0;
  }
#endif

  //: Get a unique ID for this thread.
  // NB. An id may no be assigned to the thread until
  // after Execute() has been called.
  // THREAD SAFE.

  SizeT ThreadBodyC::ID() const {
#if RAVL_HAVE_WIN32_THREADS
    return (UInt64T) threadID;
#else
    return (size_t) threadID;
#endif
  }
   

  //: Get ID for this thread.
  
  SizeT ThisThreadID() {
#if RAVL_HAVE_POSIX_THREADS
    return (size_t) pthread_self();
#endif
#if RAVL_HAVE_WIN32_THREADS
    return (SizeT) GetCurrentThreadId();
#endif
  }
  
};
