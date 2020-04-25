// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/testChildProc.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Exec"
//! date="24/11/1998"
//! userlevel=Develop

#include "Ravl/OS/DeadLineTimer.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;

int CountFds() 
{
  int count = 0;
  struct stat statBuf;
  for(int fd = 0;fd < 100;fd++) {
    if(fstat(fd,&statBuf) == 0) {
      ONDEBUG(cerr << "FD Open:" << fd << "\n");
      count++;
    }
  }
  return count;
}

int main(int argc,char **argv)
{
  FilenameC thisExe(argv[0]);
  
  OptionC option(argc,argv,true);
  bool testChild = option.Boolean("tc",false,"Run a test child program. (Sleeps for 1 second.)\n");  
  option.Check();

  if(testChild) {
    cerr << "StdErr: Running a test child. \n";
    cout << "StdOut: Sleeping for 1 second. \n";
    sleep(1);
    cout << "StdOut: Child done. \n";
    cerr << "StdErr: Exiting. \n";
    exit(0);
  }
  
  if(!thisExe.Exists()) {
    cerr << "ERROR: Can't find myself, for child proc check !!! \n";
    exit(1);
  }
  
  FilenameC logfile("/tmp/testChildProc");
  logfile = logfile.MkTemp();
  int beforeFds = CountFds();
  cerr << "Open fds :" << beforeFds << "\n";
  cerr << "Running child. \n";
  {
    ChildOSProcessC &aChild = *new ChildOSProcessC(thisExe + " -tc",logfile,true);
    if(!aChild.Wait(5)) {
      cerr << "Wait for child failed. \n";
      exit(1);
    }
    if(!aChild.ExitedOk()) {
      cerr << "Error running child. \n";
      exit(1);      
    }
    delete &aChild;
  }
  int afterFds = CountFds();
  cerr << "Open fds :" << afterFds << "\n";
  if(beforeFds != afterFds) {
    cerr << "ERROR: File descriptor leak detected! \n";
    return 1;
  }
  cerr << "Test passed. Cleaning up. \n";
  if(logfile.Exists())
    logfile.Remove();
  return 0;
}
