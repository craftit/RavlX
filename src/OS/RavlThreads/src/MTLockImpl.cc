// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/MTLockImpl.cc"

#include "Ravl/Threads/RWLock.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  void IncPThreadSysDBLock()
  {}
  
  static const int noLocks = 7;
  
  // These can't be used until we've done all constructors from file and so is safe in terms of initialisation order.
  RWLockC posixDBRWLock[noLocks];
  
  static inline void CheckLockID(int &lockId) {
    if(lockId >= noLocks || lockId < 0) {
      std::cerr << "SysDBLockImpl.cc: Illegal lock selected " << lockId << "\n";
      RavlAssert(0);
      lockId = 0;
    }
  }
  
  void DoMTSysDBReadLock(int lockId)  { 
    CheckLockID(lockId);
    posixDBRWLock[lockId].RdLock(); 
  }

  void DoMTSysDBWriteLock(int lockId) {
    CheckLockID(lockId);
    posixDBRWLock[lockId].WrLock(); 
  }

  void DoMTSysDBUnlockRd(int lockId) { 
    CheckLockID(lockId);
    posixDBRWLock[lockId].UnlockRd();
  }
  
  void DoMTSysDBUnlockWr(int lockId) { 
    CheckLockID(lockId);
    posixDBRWLock[lockId].UnlockWr(); 
  }
  
  int DoMTSysGetThreadID()  {
#if 0
    VThread *thrd = VThread::GetCurrentThread();
    if(thrd == 0)
      return 0; 
    return (int) thrd->GetID();
#else
    return -1;
#endif
  }
  
  
  // Install race resolution function into the reference counting mechanism.
  
  class PThreadInitC {
  public:
    PThreadInitC() { 
      MTGetThreadID = DoMTSysGetThreadID;
      MTReadLock = DoMTSysDBReadLock;
      MTWriteLock = DoMTSysDBWriteLock;
      MTUnlockRd = DoMTSysDBUnlockRd;
      MTUnlockWr = DoMTSysDBUnlockWr;
    }

    //Set function pointers to null in destructor to prevent the functions being
    //accessed after the handles to the mutexes have been closed
	~PThreadInitC()
	{
	   MTGetThreadID = 0;
           MTReadLock = 0;
           MTWriteLock = 0;
           MTUnlockRd = 0;
           MTUnlockWr = 0;
	}
  };
  
  PThreadInitC doVThreadInitC;
  
}
