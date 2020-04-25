// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_AMUTEX_HEADER
#define RAVL_AMUTEX_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Locks"
//! rcsid="$Id$"
//! file="Ravl/Core/Base/AMutex.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Locks"
//! userlevel=Advanced
//! date="2/04/2005"

#include "Ravl/RCHandleV.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract Mutex
  // This mutex hides the thread implementation for methods in the
  // 'Core' library that required to be thread safe.  In general if its 
  // better to use the MutexC implementation in the 'OS' module.
  
  class AMutexBodyC
    : public RCBodyVC
  {
  public:
    AMutexBodyC();
    //: Default constructor.
    
    virtual bool Lock();
    //: Lock mutex.
    // Returns true if lock is obtained successfully.
    
    virtual bool TryLock();
    //: Try and obtain lock without blocking.
    // Return true if lock is obtained, false otherwise.
    
    virtual bool Unlock();
    //: Lock mutex.
    // Returns true if lock is released successfully.
    
  };
  
  //! userlevel=Develop
  //: Abstract Mutex
  // This mutex hides the thread implementation for methods in the
  // 'Core' library that required to be thread safe.  In general if its 
  // better to use the MutexC implementation in the 'OS' module.
  
  class AMutexC
    : public RCHandleVC<AMutexBodyC>
  {
  public:
    AMutexC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    AMutexC(bool recursive);
    //: Constructor.
    // If 'recursive' is false a 'fast' mutex is created otherwise
    // a recursive version is used.
    
    bool Lock()
    { return Body().Lock(); }
    //: Lock mutex.
    // Returns true if lock is obtained successfully.
    
    bool TryLock()
    { return Body().TryLock(); }
    //: Try and obtain lock without blocking.
    // Return true if lock is obtained, false otherwise.
    
    bool Unlock()
    { return Body().Unlock(); }
    //: Lock mutex.
    // Returns true if lock is released successfully.
    
  };
  
  //! userlevel=Normal
  //: Abstract Mutex Lock
  // The destructor of this object removed the lock if
  // it is held.   This ensures whenever the lock goes
  // out of scope, either by returning from the function
  // or if an exception is held, that the lock will be
  // released properly. 
  
  class AMutexLockC {
  public:
    AMutexLockC(AMutexC &theMutex)
      : mutex(theMutex),
        lockHeld(true)
    { mutex.Lock(); }
    //: Constructor

    AMutexLockC(AMutexC &theMutex,bool tryLock)
      : mutex(theMutex),
        lockHeld(true)
    { 
      if(tryLock) 
        lockHeld = mutex.TryLock(); 
      else
        mutex.Lock(); 
    }
    //: Constructor
    // If tryLock is true, only try and obtain the 
    // lock.  The success of the call can be
    // checked with 'IsLocked()'. If tryLock is 
    // false the constructor will not return 
    // until the lock is obtained.
    
    ~AMutexLockC()
    { if(lockHeld) mutex.Unlock(); }
    //: Destructor.
    
    bool Unlock() {
      RavlAssert(lockHeld);
      mutex.Unlock();
      lockHeld = false;
      return true;
    }
    //: Unlock the mutex early.
    
    bool Lock() {
      RavlAssert(!lockHeld);
      mutex.Lock();
      lockHeld = true;
      return true;
    }
    //: Relock the mutex 
    
    bool IsLocked() const
    { return lockHeld; }
    //: Is the lock currently held ?
    
  protected:
    AMutexC mutex;
    bool lockHeld;

  private:
    AMutexLockC(const AMutexLockC &x)
      : mutex(x.mutex)
    { RavlAssert(0); }
    //: Disable copy constructor.
    
    AMutexLockC &operator=(const AMutexLockC &) { 
      RavlAssert(0); 
      return *this;
    }
    //: Make sure there's no assignment.
    
  };
  
  typedef AMutexBodyC *(*AMutexLockFactoryT)(bool);
  //: Mutex constructor func.
  
  void RegisterAMutexFactory(AMutexLockFactoryT newFunc);
  //: Register constructor func for AMutexC's
}



#endif
