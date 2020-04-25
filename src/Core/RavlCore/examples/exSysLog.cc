// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.OS.SysLog"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/OS/Misc/exSysLog.cc"

#include "Ravl/SysLog.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  StringC name = opts.String("n","Test","Name of service. ");
  bool logStdErr = opts.Boolean("se",true,"Log to standerd error. ");
  bool logStdErrOnly = opts.Boolean("seo",false,"Log to standerd error only. ");
  bool logPid = opts.Boolean("lp",true,"Log process id. ");
  int logPriority = opts.Int("l",7,"Log priority. ");
  
  opts.Check();
  
  // Open log with appropriate paramiters
  
  SysLogOpen(name,logPid,logStdErr,logStdErrOnly);
  
  // Set the level of priority of message to allow to be logged to std error.
  
  SysLogLevelStdErr((SysLogPriorityT) logPriority);
  
  // Log a debug message.
  
  SysLog() << "Application " << name << " open. ";
  
  // Log an information message.
  
  SysLog(SYSLOG_INFO) << "Bannana's are yellow. ";
  
  return 0;
}
