// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/Mutex.cc"

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Stream.hh"

#include <string.h>

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#include <errno.h>
#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif
#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#include <conio.h>
#include "Ravl/OS/Date.hh"
#endif

namespace RavlN
{

  //: Default constructor.
  
  MutexC::MutexC() 
    : isValid(false)
#if RAVL_HAVE_WIN32_THREADS
      ,m_useCount(0),
      m_recursive(false)
#endif
  { Init(false); }
  
  //: Constructor.
  
  MutexC::MutexC(bool recursive) 
    : isValid(false)
#if RAVL_HAVE_WIN32_THREADS
      ,m_useCount(0),
      m_recursive(false)
#endif
  { Init(recursive); }
  
  //: Setup mutex.
  
  void MutexC::Init(bool recursive) {
    
    // ---------------------------------- POSIX ----------------------------------
#if RAVL_HAVE_POSIX_THREADS
#if defined(PTHREAD_MUTEX_ERRORCHECK) || defined(PTHREAD_MUTEX_ERRORCHECK_NP) || RAVL_OS_LINUX || RAVL_OS_LINUX64
#else
    ONDEBUG(std::cerr << "MutexC::Init(), Attempting to construct debugging mutex but don't know how. \n");
#endif
    
    // Build an error checking mutex.
    pthread_mutexattr_t mutAttr;
    pthread_mutexattr_init(&mutAttr);
    
    if(recursive) {  // Try and build a recursive mutex ?
#if ( RAVL_OS_LINUX || RAVL_OS_LINUX64)
      if(pthread_mutexattr_settype(&mutAttr,PTHREAD_MUTEX_RECURSIVE_NP) != 0) // Linux.
        throw ExceptionOperationFailedC("ERROR: Recursive mutex's not available. ");
#else
#ifdef PTHREAD_MUTEX_RECURSIVE
      if(pthread_mutexattr_settype(&mutAttr,PTHREAD_MUTEX_RECURSIVE) != 0) // Solaris and maybe other ?
        throw ExceptionOperationFailedC("ERROR: Recursive mutex's not available. ");
#endif
#endif
    } else {
    // Check if we want to enable debugging.
#if RAVL_CHECK
      ONDEBUG(std::cerr << "MutexC::Init(), Constructing debuging mutex. (@:" << ((void*) this) << ") \n");
    
    // Enable error checking, if available.
#if defined(PTHREAD_MUTEX_ERRORCHECK) || defined(PTHREAD_MUTEX_ERRORCHECK_NP) || RAVL_OS_LINUX || RAVL_OS_LINUX64
    // Set appropriate attribute.
#if defined(PTHREAD_MUTEX_ERRORCHECK)
      pthread_mutexattr_settype(&mutAttr,PTHREAD_MUTEX_ERRORCHECK);
#else
      pthread_mutexattr_settype(&mutAttr,PTHREAD_MUTEX_ERRORCHECK_NP);
#endif
#endif
#else
      ONDEBUG(std::cerr << "MutexC::Init(), Constructing normal mutex. (@:" << ((void*) this) << ") \n");
#endif
    }

    
    int rc;
    if((rc = pthread_mutex_init(&mutex,&mutAttr)) != 0) {
      Error("Failed to create mutex.",errno,rc); 
    } else {
      isValid = true;
    }
    
    pthread_mutexattr_destroy(&mutAttr);
#endif // RAVL_HAVE_POSIX_THREADS
    // ---------------------------------- WIN32 ----------------------------------
#if RAVL_HAVE_WIN32_THREADS
    // FIXME: Does this support recursive locks?
    m_recursive=recursive;
    RavlAssert(m_useCount == 0);
    if((mutex = CreateMutex(0,false,0)) == 0) {
      Error("Failed to create mutex.",errno,0); 
    } else
      isValid = true;
#endif
    RavlAssert(isValid);
  }
  
  void MutexC::Error(const char *msg)  {
    std::cerr << "MutexC::Error() :" << msg << " \n";
    RavlAssert(0);
  }
  
  //: Report an error, with an error number.
  
  void MutexC::Error(const char *msg,int anerrno,int rc) {
#if RAVL_COMPILER_VISUALCPPNET_2005
    char buff[1024];
    strerror_s(buff,1024,anerrno);
    std::cerr << "MutexC::Error() err=" << anerrno << " (" << buff << ") : " << msg << " \n";
    RavlAlwaysAssert(0);
#else
    std::cerr << "MutexC::Error() err=" << anerrno << " (" << strerror(anerrno) << ") : " << msg << " \n";
    std::cerr << "MutexC::Error()  rc=" << rc << " (" << strerror(rc) << ") \n";
#endif
    std::cerr << "MutexC::Error() @:" << ((void*) this) << "\n";
    RavlAssert(0);
  }
  
  //: Destructor.
  MutexC::~MutexC() { 
    ONDEBUG(std::cerr << "MutexC::~MutexC(), Destroying mutex. (@:" << ((void*) this) << ")\n");
    int maxRetry = 100;
    // We need to make sure there's no threads waiting for the lock. There shouldn't be
    // if we're freeing it, as the resource its waiting for is probably on its way out too.
    bool reportedError = false;
    while(--maxRetry > 0) {
      int ret = 0;
      if(TryLock()) { // Try get an exclusive lock.
	Unlock(); // Unlock... and destroy.
#if RAVL_HAVE_POSIX_THREADS
	if((ret = pthread_mutex_destroy(&mutex)) == 0)
	  break; // It worked ok..
        std::cerr << "Failed to destroy mutex with error: " << ret << "\n";
#endif
#if RAVL_HAVE_WIN32_THREADS
        if(CloseHandle(mutex)) 
          break;
        std::cerr << "WARNING: MutexC::~MutexC(), destroy failed. " << GetLastError() << "\n";   
#endif
      }
      if(!reportedError) {
        std::cerr << "WARNING: MutexC::~MutexC(), thread holding lock on mutex in destructor. This indicates a problem with the code. \n";
        std::cerr << "Stack dump:\n";
        DumpStack(std::cerr);
        reportedError = true;
        RavlAssert(0);
      }
#if RAVL_HAVE_WIN32_THREADS
      RavlN::Sleep(0.01);
#else
      OSYield();
#endif
    }
    if(reportedError) {
      if(maxRetry > 0)
        std::cerr << "WARNING: MutexC::~MutexC(), Thread released lock. \n";
      else
        std::cerr << "WARNING: MutexC::~MutexC(), Thread released lock. \n";
    }
    isValid = false;
    if(maxRetry <= 0)
      std::cerr << "WARNING: MutexC::~MutexC(), destroy failed. \n";
  }
  
#if !RAVL_USE_INLINEMUTEXCALLS  
  //: Lock mutex.
  
  bool MutexC::Lock(void) {
    RavlAssert(isValid);
    
#if RAVL_HAVE_POSIX_THREADS
    int rc;
    //ONDEBUG(std::cerr << "MutexC::Lock() Start @:" << ((void*) this) << " \n");
    if((rc = pthread_mutex_lock(&mutex)) == 0) {
      //ONDEBUG(std::cerr << "MutexC::Lock() Obtained @:" << ((void*) this) << " \n");
      return true;
    }
    Error("Lock failed",errno,rc);
#elif RAVL_HAVE_WIN32_THREADS
    int rc;
    if((rc = WaitForSingleObject(mutex,INFINITE)) == WAIT_OBJECT_0) {
      m_useCount++;
      if(!m_recursive && m_useCount > 1) {
        RavlAssertMsg(0,"Deadlock!");
      }
      return true;
    }
    Error("Lock failed",GetLastError(),rc);
#endif    
    RavlAssert(isValid);
    return false;
  }
    
  //: Try and lock mutex.
  
  bool MutexC::TryLock(void) {
    int rc;
    RavlAssert(isValid);
#if RAVL_HAVE_POSIX_THREADS
    //ONDEBUG(std::cerr << "MutexC:TryLock() @:" << ((void*) this) << " \n");
    if((rc = pthread_mutex_trylock(&mutex)) == 0) {
      //ONDEBUG(std::cerr << "MutexC:TryLock() Succeeded. @:" << ((void*) this) << "  \n");
      return true;
    }
    //ONDEBUG(std::cerr << "MutexC:TryLock() Failed. @:" << ((void*) this) << "  rc=" << rc << "\n");
    if(rc != EBUSY && rc != EDEADLK)
      Error("Trylock failed for unexpected reason.",errno,rc);
#endif
#if RAVL_HAVE_WIN32_THREADS
    if((rc = WaitForSingleObject(mutex,0)) == WAIT_OBJECT_0) {
      m_useCount++;
      if(!m_recursive && m_useCount > 1) {
        if(ReleaseMutex(mutex)) {
          m_useCount--;
        } else {
          Error("TryLock going very wrong!",GetLastError(),rc);
        }
        return false;
      }
      return true;
    }
    if(rc != WAIT_TIMEOUT) {
      Error("TryLock failed",GetLastError(),rc);
    }
#endif
    return false;
  }
  
  //: Unlock mutex.
  
  bool MutexC::Unlock(void) {
    //RavlAssertMsg(!TryLock(),"MutexC::Unlock() called on an unlocked mutex.");
    int rc = 0;
    RavlAssert(isValid);
#if RAVL_HAVE_POSIX_THREADS
    //ONDEBUG(std::cerr << "MutexC:Unlock() @:" << ((void*) this) << "\n");
    if((rc = pthread_mutex_unlock(&mutex)) == 0) {
      //ONDEBUG(std::cerr << "MutexC::Unlock() Released @:" << ((void*) this) << " \n");
      return true;
    }
    Error("Unlock failed.",errno,rc);
#endif
#if RAVL_HAVE_WIN32_THREADS
    RavlAssert(m_useCount > 0);
    m_useCount--;
    if(ReleaseMutex(mutex))
      return true;
    Error("Unlock failed.",GetLastError(),rc);
#endif
    return false;
  }
#endif
  
}
