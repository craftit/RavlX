// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlThreads

#include "Ravl/AMutex.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  
  //: Stub AMutexBodyC.
  // With some basic sanity checks.
  
  class AMutexPosixBodyC 
    : public AMutexBodyC
  {
  public:        
    AMutexPosixBodyC(bool doRecursive)
      : mutex(doRecursive)
    {
      //cerr << "AMutexPosixBodyC::AMutexPosixBodyC() Called. \n";
    }
    //: Constructor.
    
    bool Lock() 
    { return mutex.Lock(); }
    //: Lock mutex.
    // Returns true if lock is obtained succesfully.
    
    bool TryLock() 
    { return mutex.TryLock(); }
    //: Try and obtain lock without blocking.
    // Return true if lock is obtained, false otherwise.
    
    bool Unlock() 
    { return mutex.Unlock(); }
    //: Lock mutex.
    // Returns true if lock is released succesfully.
    
  protected:
    MutexC mutex;
  };
  

  //========================================================================
  
  
  static AMutexBodyC *PosixLockFunc(bool recursive)
  { return new AMutexPosixBodyC(recursive); }
  
  class AMutexPThreadInitC {
  public:
    AMutexPThreadInitC() {
      RegisterAMutexFactory(&PosixLockFunc);
    }
  };
  
  static AMutexPThreadInitC aMutexInitInst;
 
  void IncPThreadAMutexInit()
  {}
 
}
