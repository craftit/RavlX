// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLOSPROCESS_HEADER
#define RAVLOSPROCESS_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Exec/OSProcess.hh"
//! lib=RavlOS
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Exec"
//! date="07/01/99"

#include "Ravl/RefCounter.hh"
#include "Ravl/String.hh"
#include "Ravl/RCHandleV.hh"
//#include "Ravl/UserInfo.hh"

namespace RavlN {

  class UserInfoC;
  class OSProcessC;
  
  //////////////////////////
  //! userlevel=Develop
  //: OSProcess body.
  
  class OSProcessBodyC 
    : public RCBodyVC
  {
  public:
    OSProcessBodyC(int npid = -1)
      : pid(npid)
      {}
    //: Default constructor.
    
    bool Signal(IntT signo);
    //: Send a signal to the process.
    
    bool Kill(bool hard = false);
    //: Kill the child process.
    // If hard is false, a SIGQUIT is sent.(This can be intercepted by the
    // process)  Otherwise a SIGKILL is sent.
    
    bool SigStop();
    //: Stop this process.  (As Ctrl-z)
    
    bool SigContinue();
    //: Continue this process. 
    // Restart the this process after a stop.
    
    inline int Pid() const { return pid; }
    //: Get process id.
    
    UserInfoC Owner();
    //: Find the owner of the process.
    
    UIntT Hash() const { return pid; }
    //: Hash on pid.
    
    UIntT MemorySize();
    //: Get the amount of memory this process is using. 
    // Returns 0 if failed.   1 if special
    // Will return ((UIntT) -1) if process size is bigger than 4Gb.
    
  protected:
    StringC ProcFile();
    //: Generate filename for entry in /proc/
    
    int pid;
  };
  
  //////////////////////////
  //! userlevel=Normal
  //: OSProcess handle.
  
  class OSProcessC 
    : public RCHandleC<OSProcessBodyC>
  {
  public:
    OSProcessC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    inline OSProcessC(int npid)
      : RCHandleC<OSProcessBodyC>(*new OSProcessBodyC(npid))
      {}
    //: Default constructor.
    
    inline bool Signal(IntT signo)
      { return Body().Signal(signo); }
    //: Send a signal to the process.
    
    inline bool Kill(bool hard = false)
      { return Body().Kill(hard); }
    //: Kill the child process.
    // The process should die sometime after this call though it may not
    // be immediatly.
    
    inline int Pid() const
      { return Body().Pid(); }
    //: Get process id.
    
    inline UIntT Hash() const
      { return Body().Hash(); }
    //: Hash on pid.
    
    inline bool operator==(const OSProcessC &oth) const
      { return Body().Pid() == oth.Pid(); }
    //: Compair two processes.
    
    static int WaitForChild(int &status);
    //: Wait for change in state of a child process.
    // returns PID of child that's changed state.
    // -1 = Call interupted.
    //  0 = No children. <p>
    
  protected:
    OSProcessC(OSProcessBodyC &bod)
      : RCHandleC<OSProcessBodyC>(bod)
      {}
    //: Body constructor.
    
    OSProcessBodyC &Body() { return RCHandleC<OSProcessBodyC>::Body(); }
    //: Access body.
    
    const OSProcessBodyC &Body() const { return RCHandleC<OSProcessBodyC>::Body(); }
    //: Access body.
    
  public:
    
    //  UserInfoC Owner()
    //    { return Body().Owner(); }
    //: Find the owner of the process.
    
    UIntT MemorySize()
      { return Body().MemorySize(); }
    //: Get the amount of memory this process is using. 
    // Returns 0 on failure. 1 if special
    // Will return ((UIntT) -1) if process size is bigger than 4Gb.
    
    bool SigStop()
      { return Body().SigStop(); }
    //: Stop this process.  (Like Ctrl-z)
    
    bool SigContinue()
      { return Body().SigContinue(); }
    //: Start this process. 
    // Restart the this process after a stop.
    
  };
  
}
#endif
