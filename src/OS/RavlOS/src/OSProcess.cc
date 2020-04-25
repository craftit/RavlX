// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/OSProcess.cc"

#include "Ravl/OS/OSProcess.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Stream.hh"
//#include "Ravl/OS/UserInfo.hh"
#include "Ravl/config.h"

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 3
#endif

#include <sys/types.h>
#if !RAVL_COMPILER_VISUALCPP
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#if RAVL_OS_SOLARIS
#define WNOWAIT         0200    /* non destructive form of wait */
#endif

#ifdef __linux__
extern "C" {
  int kill(pid_t pid, int sig);
};
#endif

namespace RavlN {

#if 0
  //////////////////////////////////
  //: Find the owner of the process.
  
  UserInfoC OSProcessBodyC::Owner()
    { return ProcFile().OwnerInfo(); }
#endif

  
  //////////////////////////////////
  //: Generate filename for entry in /proc/
  
  StringC OSProcessBodyC::ProcFile()
    { return StringC("/proc/") + StringC(pid); }

  //////////////////////////////////
  //: Send a signal to the process.
  
  bool OSProcessBodyC::Signal(IntT signo) {
#if !RAVL_COMPILER_VISUALCPP
    if(kill(pid,signo) < 0)
      return false;
    return true;
#else
    throw ExceptionC("OSProcessBodyC::Signal(), Not implemented. ");
#endif
  }
  
  ///////////////////////////
  //: Kill the child process.
  
  bool OSProcessBodyC::Kill(bool hard){ 
#if !RAVL_COMPILER_VISUALCPP
    if(hard) {
      if(!Signal(SIGKILL))
	return false;
    } else {
      if(!Signal(SIGINT))
	return false;
    }
    // Do anything ??
    return true;
#else
    throw ExceptionC("OSProcessBodyC::Kill(), Not implemented. ");
#endif
  }

  //////////////////////////////////
  //: Wait for change in state of a child process.
  // returns PID of child that's changed state.
  // -1 = Call interupted.
  //  0 = No children.
  
  int OSProcessC::WaitForChild(int &status) { 
#if !RAVL_COMPILER_VISUALCPP
    return waitpid(0,&status,0); 
#else
    throw ExceptionC("OSProcessC::WaitForChild(), Not implemented. ");
#endif
  }
}
