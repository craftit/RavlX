// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/exChild.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Exec"
//! date="24/11/1998"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/Assert.hh"
#include "Ravl/OS/Date.hh"
//#include <unistd.h>

using namespace RavlN;

int main(int nargs,char *args[])
{
  OptionC option(nargs,args,true);
  StringC cmd  = option.CharArr("c","ls","command to run");
  bool stop  = option.Boolean("s",false,"Cause segmentation fault.");
  bool hang  = option.Boolean("h",false,"Hang in an infinit loop.");
  bool waitAfterExit = option.Boolean("w",false,"Wait after exit.");
  IntT repeat = option.Int("r",1,"Number of times to repeat the command. ");
  RealT tlim = option.Real("t",-1,"Limit on waiting time. ");
  option.Check();
  //cerr << "Pid=" << getpid() << "\n";
  while(hang) ;
  if(stop)
    RavlAssert(0);
  StringC line;
  for(int i = 0;i < repeat;i++) {
    ChildOSProcessC cproc(cmd,true);
    IStreamC is = cproc.StdOut();
    if(!cproc.Wait(tlim))
      cproc.Terminate();
    is >> line;
    is.Close();
    cerr << "Done. Running:" << cproc.IsRunning() << " Ok:" << cproc.ExitedOk() << " Code:" << cproc.ExitCode() << " Ref=" << cproc.References() << "\n";
  }
  if(waitAfterExit) {
    while(1)
      Sleep(10);
  }
  return 0;
}
