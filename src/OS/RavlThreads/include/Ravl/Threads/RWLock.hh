// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PTHREADSRWLOCK_HEADER
#define RAVL_PTHREADSRWLOCK_HEADER 1
//////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Threads"
//! file="Ravl/OS/Threads/Posix/RWLock.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! date="02/07/1999"

#include "Ravl/config.h"
#include "Ravl/Threads/ConditionalVariable.hh"

#if RAVL_HAVE_POSIX_THREADS        
#include <pthread.h>
#endif

#if RAVL_OS_SOLARIS
#include <sys/signal.h>
#endif

#if RAVL_OS_UNIX
#include <errno.h>
#endif

#if !RAVL_HAVE_POSIX_THREADS_RWLOCK
#include <assert.h>
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#endif

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"

namespace RavlN {

  enum RWLockKindT { RWLOCK_PREFER_READERS,
                     RWLOCK_PREFER_WRITERS };
  //: Lock type.
  
  enum RWLockModeT { RWLOCK_READONLY, 
		     RWLOCK_WRITE,
		     RWLOCK_TRY_READONLY, 
		     RWLOCK_TRY_WRITE,
                     RWLOCK_NOLOCK
  };
  //: Lock type.
  
#if RAVL_HAVE_POSIX_THREADS_RWLOCK
  //! userlevel=Normal
  //: Read/Write lock.
  // A small object.
  
  class RWLockC {
  public:
    RWLockC(RWLockKindT kind = RWLOCK_PREFER_READERS);
    //: Constructor.
    // If preferWriters is true, the lock will
    // give writers priority where this feature is supported.

    RWLockC(const RWLockC &);
    //: Copy constructor.
    // This just creates another lock.
    
    ~RWLockC(); 
    //: Destructor.
    
  protected:
    void Error(const char *msg,int ret);
    //: Print an error.
    
  public:

    bool RdLock(float timeout);
    //: Aquire a read lock with timeout.
    // Returns true if lock aquired, false if timeout.
    // Negative timeout's will cause the wait to last forever
    
    bool RdLock(void);
    //: Get a read lock.
    
    bool TryRdLock(void) 
    { return (pthread_rwlock_tryrdlock(&id) == 0); }
    //: Try and get a read lock.

    bool WrLock(float timeout);
    //: Aquire a write lock with timeout
    // Returns true if lock aquired, false if timeout.
    // Negative timeout's will cause the wait to last forever

    bool WrLock(void);
    //: Get a write lock.
    
    bool TryWrLock(void) {
      RavlAssert(isValid);      
      return (pthread_rwlock_trywrlock(&id) == 0); 
    }
    //: Try and get a write lock.
    
    bool UnlockWr(void) { 
      RavlAssert(isValid);      
      return (pthread_rwlock_unlock(&id) == 0); 
    }
    //: Unlock write lock.
    
    bool UnlockRd(void) { 
      RavlAssert(isValid);      
      return (pthread_rwlock_unlock(&id) == 0); 
    }
    //: Unlock read lock.
    
    bool AreWritersPrefered() const
    { return m_preferWriter; }
    //: Test if the lock prefer's writers.
    
  private:
    
    pthread_rwlock_t id;
    bool isValid; // Flag a valid lock. Used for debugging.
    bool m_preferWriter; // Do we prefer writers over readers?
  };

#else

  //: Read Write Lock.
  // There may be a better way, any suggestions ??
  // Gives priority to write requests.
  
  class RWLockC {
  public:
    RWLockC(RWLockKindT kind = RWLOCK_PREFER_READERS);
    // Constructor.

    RWLockC(const RWLockC &);
    //: Copy constructor.
    // This just creates another lock.    
    
    bool RdLock(void);  
    // Get a read lock.

    bool RdLock(float timeout);
    // Get a read lock.

    bool TryRdLock(void);  
    // Try and get a read lock.
    
    bool WrLock(void);  
    // Get a write lock.

    bool WrLock(float timeout);
    //: Acquire a write lock with timeout
    // Returns true if lock acquired, false if timeout.
    // Negative timeout's will cause the wait to last forever
    
    bool TryWrLock(void);
    // Try and get a write lock.
    
    inline bool UnlockRd(void) 
    { return Unlock(); }
    // Unlock a read lock.
    
    inline bool UnlockWr(void) 
    { return Unlock(); }
    // Unlock a write lock.

    bool AreWritersPrefered() const
    { return m_preferWriter; }
    //: Test if the lock prefer's writers.

  protected:
    bool Unlock(void);
    // Unlock.
    
    void Error(const char *msg, int ret );
    //: Print an error.
    
  private:    
    MutexC m_accM; // Access control.
    int m_rdCount; // Count of readers with locks. -1=Writing
    int m_wrWait;  // Count of writers waiting.
    int m_rdWait;  // Count of readers waiting.
    ConditionalVariableC m_writeQueue; // Writers queue.
    ConditionalVariableC m_readQueue;  // Readers queue.
    bool m_preferWriter; // Do we prefer writers over readers?
  };
  
#endif

  std::ostream &operator<<(std::ostream &strm,const RWLockC &vertex);
  //: Text stream output.
  // Not implemented
  
  std::istream &operator>>(std::istream &strm,RWLockC &vertex);
  //: Text stream input.
  // Not implemented
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RWLockC &vertex);
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,RWLockC &vertex);
  //: Binary stream input.
  // Not implemented

  //! userlevel=Normal
  //: Exception-safe "handle" class for <a href="RavlN.RWLockC.html">RWLockC</a>.
  // <p>SMALL OBJECT</p>
  // <p>The destructor of this object removes the lock if it is held.  This
  // ensures that, whenever this object goes out of
  // scope, e.g. by returning from the function or if an exception is held,
  // the <code>RWLockC</code> lock will be released properly. </p>  
  // <p>It is the users responsibility to ensure that the 
  // RWLockC remains valid for the lifetime of any RWLockHoldC 
  // instance. </p>
  
  class RWLockHoldC {
  public:    

    RWLockHoldC(const RWLockC &m,RWLockModeT lockMode)
      : rwlock(const_cast<RWLockC &>(m)),
	rLocked(false),
	wLocked(false)
    { 
      switch(lockMode) {
      case RWLOCK_READONLY:     rLocked = rwlock.RdLock();    break;
      case RWLOCK_WRITE:        wLocked = rwlock.WrLock();    break;
      case RWLOCK_TRY_READONLY: rLocked = rwlock.TryRdLock(); break;
      case RWLOCK_TRY_WRITE:    wLocked = rwlock.TryWrLock(); break;
      case RWLOCK_NOLOCK:       break;
      }
    }
    //: Create a lock on a rwlock.
    // The type of lock is determined by <code>lockMode</code>.

    RWLockHoldC(const RWLockC &m,bool readOnly = true,bool tryOnly = false)
      : rwlock(const_cast<RWLockC &>(m)),
	rLocked(false),
	wLocked(false)
    { 
      if(!tryOnly) {
	if(readOnly) {
	  rwlock.RdLock();
	  rLocked = true;
	} else {
	  rwlock.WrLock();
	  wLocked = true;
	}
      } else {
	if(readOnly) {
	  if(rwlock.TryRdLock())
	    rLocked = true;
	} else {
	  if(rwlock.TryWrLock())
	    wLocked = true;
	}
      }
    }
    //: Create a lock on a rwlock.
    //!deprecated: This method is obsolete and may be remove in future versions;
    //!deprecated: use constructor with <code>RWLockModeT</code> parameter instead. <br>
    // This may not seem like a good idea,
    // but it allows otherwise constant functions to
    // lock out other accesses to data without undue
    // faffing about.
    
    ~RWLockHoldC()
    { Unlock(); }
    //: Destructor
    
    void Unlock() {
      RavlAssert(!(wLocked && rLocked)); // Not both at once!
      if(wLocked)
	rwlock.UnlockWr();
      if(rLocked)
	rwlock.UnlockRd();
      rLocked = false;
      wLocked = false;
    }
    //: Unlock the rwlock.
    
    void LockRd() {
      RavlAssertMsg(!(wLocked || rLocked),"RWLockHoldC::LockRd(), ERROR: lock already exists."); // Must be no locks.
      rwlock.RdLock();
      rLocked = true;
    }
    //: relock for read
    
    void LockWr() {
      RavlAssertMsg(!(wLocked || rLocked),"RWLockHoldC::LockWr(), ERROR: lock already exists."); // Must be no locks.
      rwlock.WrLock();
      wLocked = true;
    }
    //: relock for write

    bool LockRd(float timeout) {
      RavlAssertMsg(!(wLocked || rLocked),"RWLockHoldC::LockRd(), ERROR: lock already exists."); // Must be no locks.
      if(!rwlock.RdLock(timeout))
        return false;
      rLocked = true;
      return true;
    }
    //: relock for read
    // Negative timeout's will cause the wait to last forever

    bool LockWr(float timeout) {
      RavlAssertMsg(!(wLocked || rLocked),"RWLockHoldC::LockWr(), ERROR: lock already exists."); // Must be no locks.
      if(!rwlock.WrLock(timeout))
        return false;
      wLocked = true;
      return true;
    }
    //: relock for write
    // Negative timeout's will cause the wait to last forever

    bool IsReadLocked() const
    { return rLocked || wLocked; }
    //: Test if safe for reading.
    // This will return true if either a write or read lock is inplace.
    
    bool IsWriteLocked() const
    { return  wLocked; }
    //: Test if safe for writing.
    
  protected:
    RWLockC &rwlock;
    bool rLocked;
    bool wLocked;

  private:
    RWLockHoldC(const RWLockHoldC &o)
      : rwlock(o.rwlock),
        rLocked(false),
        wLocked(false)
    { RavlAssert(0); }
    //: Disable copy constructor.
  };

}
#endif
