// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCRWLOCK_HEADER
#define RAVL_RCRWLOCK_HEADER 1
//////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Threads"
//! file="Ravl/OS/Threads/Posix/RCRWLock.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! date="10/06/2006"

#include "Ravl/Threads/RWLock.hh"
#include "Ravl/RCWrap.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Reference counted RWLockC
  
  class RCRWLockC
    : public RCWrapC<RWLockC>
  {
  public:
    RCRWLockC()
    {}
    //: Constructor.
    // Creates an invalid handle.
    
    RCRWLockC(bool);
    //: Construct a valid rwlock.
    
    bool RdLock(void) 
    { return Data().RdLock(); }
    //: Get a read lock.
    
    bool TryRdLock(void) 
    { return Data().TryRdLock(); }
    //: Try and get a read lock.
    
    bool WrLock(void) 
    { return Data().WrLock();}
    //: Get a write lock.
    
    bool TryWrLock(void) 
    { return Data().TryWrLock(); }
    //: Try and get a write lock.
    
    bool UnlockWr(void) 
    { return Data().UnlockWr(); }
    //: Unlock write lock.
    
    bool UnlockRd(void)
    { return Data().UnlockRd(); }
    //: Unlock read lock.

    
  };
}


#endif
