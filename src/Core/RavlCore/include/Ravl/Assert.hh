// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ASSERT_HEADER
#define RAVL_ASSERT_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Error Handling"
//! userlevel=Normal
//! file="Ravl/Core/Base/Assert.hh"
//! lib=RavlCore
//! author="Charles Galambos"

#include <stdarg.h>
#include "Ravl/config.h"
#include <iostream>

namespace RavlN {  
  class StringC; // Forward declare StringC.
  
  bool AssertThrowException();
  //: Test if an exception will be thrown on an assertion failure.
  
  bool SetAssertThrowException(bool val);
  //: Throw an exception on an assertion failure.
  // Note this may failed if exceptions are not supported,
  // in which case it will return false.

  void SetAssertDisplayStackTrace(bool val);
  //: Enable/Disable display of stack traces on assertions.
  //: this is on by default.

  void AssertFailed(const char *file,int lineNo);
  //: Called if assertion failed.
  // This will either cause the program to trap to the debugger
  // or throw an 'ExceptionAssertionFailedC'

  void AssertFailed(const char *file,int lineNo,const char *msg);
  //: Called if assertion failed, with message.
  // This will either cause the program to trap to the debugger
  // or throw an 'ExceptionAssertionFailedC'
  
  void IssueError(const char *file,int lineNo,const char *msg ...);
  //: Call when program encounters an error.
  // This will either cause the program to trap to the debugger
  // or throw an 'ExceptionAssertionFailedC'

  void IssueWarning(const char *file,int lineNo,const char *msg ...);
  //: Call when program encounters an a unexpected occurrence.
  // this prints the message and continues.
  
  void IssueError(const char *file,int lineNo,const StringC &str);
  //: Call when program encounters an error.
  // This will either cause the program to trap to the debugger
  // or throw an 'ExceptionAssertionFailedC'
  
  void IssueWarning(const char *file,int lineNo,const StringC &str);
  //: Call when program encounters an a unexpected occurrence.
  // this prints the message and continues.

  bool DumpStack(std::ostream &strm);
  //: Dump the current stack to the given stream.

}

#if RAVL_CHECK
#define RavlAssert(x) { if(!(x)) RavlN::AssertFailed(__FILE__,__LINE__); }
#define RavlAssertMsg(x,m) { if(!(x)) RavlN::AssertFailed(__FILE__,__LINE__,m); }
#else
#define RavlAssert(x) {}
#define RavlAssertMsg(x,m) {}
#endif
// If we want to check an assertion, even in optimised code.
#define RavlAlwaysAssert(x) { if(!(x)) RavlN::AssertFailed(__FILE__,__LINE__); }
#define RavlAlwaysAssertMsg(x,m) { if(!(x)) RavlN::AssertFailed(__FILE__,__LINE__,m); }

#define RavlIssueError(x) RavlN::IssueError(__FILE__,__LINE__,x);
#define RavlIssueWarning(x) RavlN::IssueWarning(__FILE__,__LINE__,x);

#endif
