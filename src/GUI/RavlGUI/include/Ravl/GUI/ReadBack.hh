// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_READBACK_HEADER
#define RAVLGUI_READBACK_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="28/01/2003"
//! rcsid="$Id$"
//! lib=RavlGUI
//! userlevel=Advanced
//! docentry="Ravl.API.Graphics.GTK.Internal"
//! file="Ravl/GUI/GTK/ReadBack.hh"

#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/RefCounter.hh"

namespace RavlGUIN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Read back delay.
  
  class ReadBackBodyC 
    : public RCBodyC
  {
  public:
    ReadBackBodyC()
      : init(0),
	done(0)
    {}
    //: Constructor.
    
    ~ReadBackBodyC()
    { done.Post(); }
    //: Destructor.
    
    void WaitForLock()
    { init.Wait(); }
    //: Wait for permission to use GUI structures.
    
    void Release()
    { done.Post(); }
    //: Release lock on GUI structures.
    
    bool Issue() {
      init.Post();
      done.Wait();
      return true;
    }
    //: Issue permission to use GUI structures.
  protected:
    SemaphoreC init;
    SemaphoreC done;
  };
  
  //! userlevel=Advanced
  //: Read back delay.
  
  class ReadBackC 
    : public RCHandleC<ReadBackBodyC>
  {
  public:
    ReadBackC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ReadBackC(bool)
      : RCHandleC<ReadBackBodyC>(*new ReadBackBodyC())
    {}
    //: Constructor.
    
    void WaitForLock()
    { Body().WaitForLock(); }
    //: Wait for permission to use GUI structures.
    
    void Release()
    { Body().Release(); }
    //: Release lock on GUI structures.
    
    bool Issue() 
    { return Body().Issue(); }
    //: Issue permission to use GUI structures.
    
  };
  
  //! userlevel=Normal
  //: Obtain and hold a readback lock.
  // This class allows the GUI thread to be locked so data
  // can be read back from the interface safely.  Note: This lock
  // may take a little time to aquire, and will suspend all GUI operations
  // while its held.  Its best to use this sparingly, and should be held
  // for the minimum possible time.
  
  class ReadBackLockC {
  public:
    ReadBackLockC(bool waitForLock = true);
    //: Create a lock. (May take some time.)
    // If waitForLock is false, 'Wait()' must be called before
    // doing any operation.  Note, this constructor queue's the wait
    // operation and will freeze the GUI until Unlock() is called.  The
    // call to Wait() only ensures the lock has been obtained.
    
    ~ReadBackLockC();
    //: Release a lock, if held.
    
    bool Wait();
    //: Wait for lock to be issued.
    // Only needed if 'waitForLock' in the constructor is false.
    // Returns true if the lock is currenlty held.
    
    bool Unlock();
    //: Undo lock.
    // Returns true if the lock was actually released with this call.
    
  protected:
    ReadBackC lock; // Read back lock info.
    bool held;     // Is lock currently held ?
    bool released; // Has lock been released ?
    SizeT threadId;
  };
  
}

#endif
