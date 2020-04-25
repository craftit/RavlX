// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTHREADS_CONDITIONALMUTEX_HEADER
#define RAVLTHREADS_CONDITIONALMUTEX_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Posix/ConditionalMutex.hh"
//! lib=RavlThreads
//! userlevel=Normal
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="02/07/1999"

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/ConditionalVariable.hh"
#include "Ravl/Threads/ConditionalVariable.hh"

namespace RavlN
{
  
  class DateC;

  //! userlevel=Normal
  //: Sleeping until signal arrives.
  //
  // <p>This class implements a "condition variable".  
  // It causes a thread to sleep until signalled from another thread.  </p>
  //
  // <p>ConditionalMutexC wraps the pthreads condition variable and
  // its associated mutex
  // into a single object.  See man pages on e.g. pthread_cond_init for
  // a full description.  See also SemaphoreC for an example of its
  // use.</p>
  //
  // <p>In this class, Wait() will only wake up once after each Broadcast(), which is why it does not need resetting after a Broadcast() (in contrast to <a href="RavlN.ThreadEventC.html">ThreadEventC</a>).  If this "edge-triggered" behaviour is not what is wanted, or if this class is used only for its ability to wake up other sleeping threads, <a href="RavlN.ThreadEventC.html">ThreadEventC</a> may be a better choice.</p>
  
  class ConditionalMutexC
   : public MutexC
  {
  public:
    ConditionalMutexC();
    //: Constructor.

    ~ConditionalMutexC();
    //: Destructor

    void Broadcast();
    //: Broadcast a signal to all waiting threads.
    // Always succeeds.
    
    void Signal();
    //: Signal one waiting thread.
    // Always succeeds.  The particular thread selected is arbitrary.
    
    void Wait();
    //: Wait for conditional.
    // <p>This unlocks the mutex and then waits for a signal
    // from either Signal or Broadcast.  When it gets the signal
    // the mutex is re-locked and control returned to the
    // program. </p>
    // <p>Always succeeds.</p>
    
    bool Wait(RealT maxTime);
    //: Wait for conditional.
    // This unlocks the mutex and then waits for a signal
    // from either Signal, Broadcast or timeout.  When it get the signal
    // the mutex is re-locked and control returned to the
    // program. <p>
    // Returns false, if timeout occurs.

    bool WaitUntil(const DateC &deadline);
    //: Wait for conditional.
    // This unlocks the mutex and then waits for a signal
    // from either Signal, Broadcast or timeout.  When it get the signal
    // the mutex is re-locked and control returned to the
    // program. <p>
    // Returns false, if timeout occurs.

  protected:
    void Error(const char *msg);
    //: Report an error.

  private:
    ConditionalMutexC(const ConditionalMutexC &)
    {}
    //: This is just a bad idea.

    ConditionalVariableC m_condVar;
  };
}

#endif
