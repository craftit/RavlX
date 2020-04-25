// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Posix/ConditionalMutex.cc"

#include "Ravl/config.h"
#if RAVL_OS_SOLARIS
#define __EXTENSIONS__ 1
#endif

#include "Ravl/Threads/ConditionalMutex.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Math.hh"
#include "Ravl/OS/Date.hh"

#if defined(VISUAL_CPP)
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <stdio.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{


  ConditionalMutexC::ConditionalMutexC()
  {
  }

  //: Destructor
  
  ConditionalMutexC::~ConditionalMutexC() { 
  }
  
  //: Broadcast a signal to all waiting threads.
  // Always succeeds.

  void ConditionalMutexC::Broadcast()
  {
    m_condVar.Broadcast();
  }

  //: Signal one waiting thread.
  // Always succeeds.  The particular thread selected is arbitrary.

  void ConditionalMutexC::Signal()
  {
    m_condVar.Signal();
  }

  void ConditionalMutexC::Wait()
  {
    m_condVar.Wait(*this);
  }

  bool ConditionalMutexC::Wait(RealT maxTime)
  {
    return m_condVar.Wait(*this,maxTime);
  }

  bool ConditionalMutexC::WaitUntil(const DateC &deadline)
  {
    return m_condVar.WaitUntil(*this,deadline);
  }

}
