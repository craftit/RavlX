// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MTLOCK_HEADER
#define RAVL_MTLOCK_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Base/MTLocks.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Locks"
//! userlevel=Advanced
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/config.h"
#include "Ravl/Types.hh"

// These functions are designed to provide just enough functionality
// to ensure that the core library is thread safe.  Full threading
// functionality will be provided elsewhere.
//
// Id 0 is a general system lock.
// Id 1 is used for locking the random number generator.
// Id 2 is used for non-reentrant 'C' library calls.
// Id 3 is reserved for use in RavlLogic & RavlIO.
// Id 4 is used for generating uuid's
// Id 5 is used for type conversion
// Id 6 is used for file formats

//: Ravl namespace.

namespace RavlN {
  
  typedef void (*MTLockFuncT)(int LockId);
  typedef int (*MTThreadIDFuncT)();
  
  extern MTLockFuncT MTReadLock;
  extern MTLockFuncT MTWriteLock;
  extern MTLockFuncT MTUnlockRd;
  extern MTLockFuncT MTUnlockWr;
  extern MTThreadIDFuncT MTGetThreadID;
    
  int MTThreadID();
  //! userlevel=Advanced
  //: Get current thread id.
  
  //:-
  
  //: Scoped lock class system for reading.
  // This class provides exceptions safe system DB locking.
  // To lock a resoruce create an instance of this class, to unlock
  // a resource use Unlock() or let the class go out of scope. Note:
  // this class should not be used as a lock itself, and should not be
  // used from more than one thread.
  // NB. The locking mechanism is not guaranteed to
  // be recursive, so you can't call ReadLock twice then Unlock twice.
  // You also can't go directly between a read lock to a write lock
  // just by calling MTWriteLock(), you have to Unlock the ReadLock
  // first.
  
  class MTReadLockC {
  public:
    MTReadLockC(int nlockId = 0)
      : lockId(nlockId),
        held(false)
    { Lock(); }
    //: Construct lock.
    
    ~MTReadLockC(){  
      if(held) {
        // There is a danger during program shutdown
        // that MTUnlockRd maybe reset to zero.
        MTLockFuncT unlockRd = MTUnlockRd;
        if(unlockRd != 0) unlockRd(lockId);
      }
    }
    //: Construct lock.
    
    void Unlock() {
      if(MTUnlockRd == 0)
	return ;
      if(!held)
	return;
      MTUnlockRd(lockId);
      held = false;
    }
    //: Unlock.
    
    void Lock() {
      if(MTReadLock == 0)
	return ;
      if(held)
	return;
      MTReadLock(lockId); 
      held = true;
    }
    //: Relock for reading.
    
  protected:
    int lockId;
    bool held; // True if lock is held.
  };
  
  //: Lock system for writing.
  // This class provides exceptions safe system DB locking.
  // To lock a resoruce create an instance of this class, to unlock
  // a resource use Unlock() or let the class go out of scope. Note:
  // this class should not be used as a lock itself, and should not be
  // used from more than one thread.
  // NB. The locking mechanism is NOT garuanteed to
  // be recursive, so you can't call ReadLock twice then Unlock twice.
  // You also can't go directly between a read lock to a write lock
  // just by calling MTWriteLock(), you have to Unlock the ReadLock
  // first.
  
  class MTWriteLockC {
  public:
    MTWriteLockC(int nlockId = 0)
      : lockId(nlockId),
      held(false)
      {  Lock(); }
    //: Construct lock.
    
    ~MTWriteLockC() {  
      if(held)
	MTUnlockWr(lockId);
    }
    //: Construct lock.
    
    void Unlock() {
      if(MTUnlockWr == 0)
	return ;
      if(!held)
	return;
      MTUnlockWr(lockId);
      held = false;
    }
    //: Unlock.
    
    void Lock() {
      if(MTWriteLock == 0)
	return ;
      if(held)
	return;
      MTWriteLock(lockId);
      held = true;
    }
    //: Relock for writing.
    
  protected:
    int lockId;
    bool held; // True if lock is held.
  };
  
}

#endif
