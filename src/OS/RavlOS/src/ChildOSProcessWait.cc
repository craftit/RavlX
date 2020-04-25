// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/ChildOSProcessWait.cc"

#include"Ravl/config.h"

#include <sys/types.h>
#if !RAVL_COMPILER_VISUALCPP
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/OS/DeadLineTimer.hh"

namespace RavlN {
  // This code is in a seperate file because is uses
  // the deadline class. This avoids pulling in dependancies
  // if they are not needed.
  
  //: Wait for program to exit.
  // maxTime in the maximum time in seconds,
  // if it is negative it won't time out.
  
  bool ChildOSProcessBodyC::Wait(double maxTime)  {
#if !RAVL_COMPILER_VISUALCPP
    if(maxTime < 0)
      return Wait();
    int stat;
    DeadLineTimerC tlim(maxTime);
    bool ok = false;
    do {
      // The deadline timer should cause waitpid to exit with an EINTR 
      // every time the alarm goes off. So if we get an invalid exit
      // we should check the errno to find out why, check the time
      // limit and restart if necassary.
      if(waitpid(pid,&stat,0) >= 0) {
	ok = true;
	break;
      }
      if(errno != EINTR) {
	perror("ChildProcessBodyC::Wait(), Failed. ");
	return false;
      }
    } while(tlim.IsRunning()) ;
    if(!ok)
      return false; // Time limit exceeded.
    running = false;
    if(WIFEXITED(stat)) { // Exited normaly ?
      exitcode = WEXITSTATUS(stat);
      exitok = true;
      return true;
    }
    if(WIFSIGNALED(stat)) { // Exited on signal ?
      exitcode = WTERMSIG(stat);
      exitok = false;
      return true;
    }
    cerr << "ERROR: Unexplained exit. \n";
    return true;
#else
    throw ExceptionC("ChildProcessBodyC::Wait() Not Implemented. ");
#endif
  }

  //: Terminate a process politely.
  
  bool ChildOSProcessBodyC::Terminate(double maxTime) {
    if(!IsRunning())
      return true;
    Kill(false); // Send sigquit.
    if(Wait(maxTime))
      return true;
    Kill(true); // Send sigterm.
    if(!Wait(maxTime)) {
      cerr << "WARNING: ChildProcessBodyC::Terminate(), Hard kill of : " << Pid() << " failed. \n";
      return false;
    }
    return true;
  }
  
}
