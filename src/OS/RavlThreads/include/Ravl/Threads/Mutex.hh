// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PTHREADSMUTEX_HEADER
#define RAVL_PTHREADSMUTEX_HEADER 1
//////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Threads"
//! file="Ravl/OS/Threads/Posix/Mutex.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! date="02/07/1999"

#include "Ravl/config.h"

#if RAVL_OS_SOLARIS
#include <sys/signal.h>
#endif
#if !RAVL_OS_OSF
#include <signal.h>
#endif

#if RAVL_HAVE_POSIX_THREADS
#include <pthread.h>
#include <errno.h>
#endif

#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#endif

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"

#define RAVL_USE_INLINEMUTEXCALLS 0
// Enable/Disable the inlining of mutex calls.

namespace RavlN
{
  class ConditionalVariableC;
  //! userlevel=Normal
  //: Mutual Exclusion lock.
  // SMALL OBJECT<br>
  // Note in general it is bad practice to Lock and Unlock the MutexC object directly 
  // as if the code locking the mutex throws an exception the mutex maybe left
  // in an in appropriate state.
  // The <a href="RavlN.MutexLockC.html">MutexLockC</a> class allows exception safe locking of the mutex.
  
  class MutexC {
  public:
    MutexC();
    //: Default constructor.
    
    MutexC(bool recursive);
    //: Constructor.
    // Recursive mutex's allow the same thread to recusively enter the locked region, however it
    // is slower than an normal lock.
    // If this type of mutex is unavailable an ExceptionOperationFailedC will be thrown.
    // Currently recursive mutex's are known to be available under Linux, Solaris and IRIX.
    
    ~MutexC();
    //: Destructor.
    
#if RAVL_USE_INLINEMUTEXCALLS
    inline bool Lock(void) {
      int rc;
      if((rc = pthread_mutex_lock(&mutex)) == 0)
	return true;
      Error("Lock failed",errno,rc);
      return false;
    }
    //: Lock mutex.
    
    inline bool TryLock(void) {
      int rc;
      if((rc = pthread_mutex_trylock(&mutex)) == 0)
	return true;
      if(errno != EPERM && errno != EBUSY && errno != EINTR && 
	 rc != EBUSY && errno != EAGAIN && rc !=  EDEADLK)
	Error("Trylock failed for unexpected reason.",errno,rc);
      return false;
    }
    //: Try and lock mutex.
    
    inline bool Unlock(void) {
      RavlAssertMsg(!TryLock(),"MutexC::Unlock() called on an unlocked mutex.");
      int rc;
      if((rc = pthread_mutex_unlock(&mutex)) == 0)
	return true;
      Error("Unlock failed.",errno,rc);
      return false;
    }
    //: Unlock mutex.
#else
    bool Lock(void);
    //: Lock mutex.
    
    bool TryLock(void);
    //: Try and lock mutex.
    
    bool Unlock(void);
    //: Unlock mutex.
#endif    
  protected:
#if RAVL_HAVE_POSIX_THREADS
    pthread_mutex_t mutex;
#endif
#if RAVL_HAVE_WIN32_THREADS
    HANDLE mutex;
    int m_useCount;
    bool m_recursive;
#endif
    bool isValid; // Used for debugging.
    
    void Error(const char *msg);  
    //: Report an error.
    
    void Error(const char *msg,int anerrno,int rc);  
    //: Report an error, with an error number.
    
  private:
    void Init(bool recursive);
    //: Setup mutex.
    
    MutexC &operator=(const MutexC &) { 
      RavlAssert(0); 
      return *this;
    }
    //: Make sure there's no assignment.
    
    MutexC(const MutexC &)
      : isValid(false)
    { RavlAssert(0); }
    //: Make sure there's no attempt to use the copy constructor.

    friend class ConditionalVariableC;
  };
  
  //! userlevel=Normal
  //: Exception safe MutexC locking class.
  // SMALL OBJECT 
  // <p>The destructor of this object removes the lock if
  // it is held.   This ensures that, whenever the lock goes
  // out of scope, either by returning from the function
  // or if an exception is held, the lock will be
  // released properly. </p>  
  // <p>It is the user's responsibility
  // to ensure that the MutexC remains valid for the 
  // lifetime of any MutexLockC instance.</p> 
  
  class MutexLockC {
  public:
    MutexLockC(MutexC &m)
    : mutex(m),
      locked(true)
    { mutex.Lock(); }
    //: Create a lock on a mutex.
    
    MutexLockC(const MutexC &m)
      : mutex(const_cast<MutexC &>(m)),
	locked(true)
    { mutex.Lock(); }
    //: Create a lock on a mutex.
    // This may not seem like a good idea,
    // but it allows otherwise constant functions to
    // lock out other accesses to data without undue
    // faffing.
    
    MutexLockC(const MutexC &m,bool tryLock)
      : mutex(const_cast<MutexC &>(m)),
	locked(false)
    { 
      if(tryLock)
	locked = mutex.TryLock(); 
      else {
	mutex.Lock();
	locked = true;
      }
    }
    //: Try and create a lock on a mutex.
    // This may not seem like a good idea,
    // but it allows otherwise constant functions to
    // lock out other accesses to data without undue
    // faffing.
    
    ~MutexLockC() { 
      if(locked)
	mutex.Unlock(); 
    }
    //: Create a lock on a mutex.
    
    void Unlock() {
      RavlAssert(locked);
      mutex.Unlock();
      locked = false;
    }
    //: Unlock the mutex.
    
    void Lock() {
      RavlAssert(!locked);
      mutex.Lock();
      locked = true;
    }
    //: re Lock the mutex.
    
    bool IsLocked() const
    { return locked; }
    //: Is it locked ?
    
    MutexC &Mutex()
    { return mutex; }
    //: Access mutex we're locking.
    
  protected:
    MutexC &mutex;
    bool locked;
    
  private:
    MutexLockC(const MutexLockC &x)
      : mutex(x.mutex),
        locked(false)
    { RavlAssert(0); }
    //: Dissable copy constructor.
    
    MutexLockC &operator=(const MutexLockC &) { 
      RavlAssert(0); 
      return *this;
    }
    //: Make sure there's no assignment.
  };
  
}


#endif
