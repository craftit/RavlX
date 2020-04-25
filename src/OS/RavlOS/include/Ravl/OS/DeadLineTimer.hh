// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DEADLINETIMER_HEADER
#define RAVL_DEADLINETIMER_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.OS.Time"
//! example="exDeadLine.cc"
//! file="Ravl/OS/Time/DeadLineTimer.hh"
//! lib=RavlOS
//! author="Charles Galambos"

#include "Ravl/Types.hh"

namespace RavlN {
  
  class DateC;
  
  //! userlevel=Normal
  //: Dead line timer.
  // When the timer is started the timesUp flag is set to
  // false, when the time expires the flag is set to true.
  // Note the timer flag is always true if the timer is not
  // running. <p>
  // You should not expect a resolution in time better than 
  // 1ms.  The actual resolution depends on the system the
  // code is run on and may be worse than this.
  // This is a SMALL object. <p>
  
  class DeadLineTimerC  {
  public:
    DeadLineTimerC(bool useVirt = false)
      : virt(useVirt),
	timesUp(true),
	id(-1)
    {}
    //: Default constructor.
  
    ~DeadLineTimerC()
    { Stop(); }
    //: Destructor.
    
    DeadLineTimerC(const DateC &timetoGo,bool useVirt = false)
      : virt(useVirt),
	timesUp(true),
	id(-1)
    { Reset(timetoGo); }
    //: Constructor for an absolute time.
    
    DeadLineTimerC(RealT timetoGo,bool useVirt = false)
      : virt(useVirt),
	timesUp(true),
	id(-1)
    { Reset(timetoGo); }
    //:  Constructor for a relative (to now) time.
    // Deadline will expire in 'timetoGo' seconds.
    // 
    void Stop();
    //: Stop timer.
    // This will set the timesUp flag, and free 
    // the deadline timer.
    
    bool Reset(RealT time); 
    //: Reset timer with 'time' left before deadline.
    // Will return false if deadline setup failed, or if
    // the time as expired before the setup is completed.
    // Any time smaller than zero is treated as infinite.
    // NB. There is currently a maximum number of 64 
    // deadline timers that can be active at any time. 
    // <p>
    // This isn't a trivial routine, it involves several
    // system calles, and alot of mucking about.  Avoid
    // calling it excessively.
    
    bool Reset(const DateC &time); 
    //: Set the deadline for 'time'
    // Will return false if deadline setup failed, or if
    // the time as expired before the setup is completed.
    // NB. There is currently a maximum number of 64 
    // deadline timers that can be active at any time. 
    // <p>
    // This isn't a trivial routine, it involves several
    // system calls, and much mucking about.  Avoid
    // calling it excessively.
    
    inline volatile const bool &IsTimeUp() const { return timesUp; }
    //: Has dead line been reached ?
    
    inline bool IsRunning() const { return !timesUp; }
    //: Is timer still running ?
    
    inline bool IsVirtual() const { return virt; }
    //: Is virtual timer.
    
    RealT Remaining() const;
    //: Get an estimate of the time remaining on deadline.
    // If dead line has expired 0 is returned.
    
    bool WaitForIt() const;
    //: Wait for deadline to expire.
    // May return false if no deadline pending, which 
    // could happen if the timer expires before the call. 
    
  private:
    bool virt;    // Virtual timer ?
    volatile bool timesUp;
    IntT id;
  };
  
}

#endif
