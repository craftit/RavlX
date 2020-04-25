// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ReadBack.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"

#if 1
#define RAVL_USE_GTKTHREADS  0  /* Use thread based event handling stratagy. */
#else
#define RAVL_USE_GTKTHREADS  1  /* Use thread based event handling stratagy. */
#endif

namespace RavlGUIN {
  using namespace RavlN;
  
  //: Create a lock. (May take some time.)
  
  ReadBackLockC::ReadBackLockC(bool waitForLock) 
    : lock(true),
      held(false),
      released(false)
  {
    if(Manager.IsGUIThread()) {
      held = true;
      released = true;
      return ;
    }
#if RAVL_USE_GTKTHREADS 
    Manager.ThreadEnterGUI(threadId);
#else
    Manager.Queue(Trigger(lock,&ReadBackC::Issue));
    if(waitForLock)
      Wait();
#endif
  }
  
  //: Release a lock, if held.
  
  ReadBackLockC::~ReadBackLockC() {
    Unlock();
  }
  
  //: Wait for lock to be issued.
  
  bool ReadBackLockC::Wait() {
    if(held || released)
      return held;
#if !RAVL_USE_GTKTHREADS
    lock.WaitForLock();
#endif
    Manager.ThreadEnterGUI(threadId);
    held = true;
    return true;
  }
  
  //: Undo lock.
  
  bool ReadBackLockC::Unlock() {
    if(!released) {
      Manager.ThreadLeaveGUI(threadId);
#if !RAVL_USE_GTKTHREADS 
      lock.Release();
#endif
      released = true;
      return true;
    }
    return false;
  }

}
