// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////////
// Self check code.
//! rcsid="$Id$"
//! file="Ravl/OS/Validate/Extra.cc"

#include "Ravl/RefCounter.hh"
#include "Ravl/HandleRefCounter.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/DeadLineTimer.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/StringList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Exception.hh"
#include "Ravl/Stream.hh"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>

#define DODEBUG 0

#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;

extern StringC procname;

int CheckTypes();
int CheckBool();
int CheckDList();
int CheckString();
int CheckRefCount();
int CheckDeadLine();
int CheckFiles();
int CheckChildProc();

// Do Self check.
// This is designed to ensure this program will
// run correctly. It is not a full check of basic
// Ravl functionality.

int SelfCheck() 
{
  // Intercept signals ??
  int lineNo;
  std::cout << "VALIDATE: Starting self test\n" << flush;
  if((lineNo = CheckTypes()) != 0) {
    std::cerr << "ERROR: Basic type check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckBool()) != 0) {
    std::cerr << "ERROR: Boolean check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckDList()) != 0) {
    std::cerr << "ERROR: DList check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckRefCount()) != 0) {
    std::cerr << "ERROR: RefCounter check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckString()) != 0) {
    std::cerr << "ERROR: String check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckDeadLine()) != 0) {
    std::cerr << "ERROR: Deadline check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckFiles()) != 0) {
    std::cerr << "ERROR: File handling check failed. Line:" << lineNo << "\n";
    return 0;
  }
  if((lineNo = CheckChildProc()) != 0) {
    std::cerr << "ERROR: Child proc check failed. Line:" << lineNo << "\n";
    return 0;
  }
  std::cout << "VALIDATE: Self test PASSED \n" << flush;
  return 1;
}

// Check some basic assumtions made in Ravl.

int CheckTypes()
{
  if(sizeof(char) != 1) // If char's arn't one byte I don't have a clue.
    return __LINE__;
  if(sizeof(int) < 4) // Ints must be at least 32 bit.
    return __LINE__;
  if(sizeof(float) < 4) // Lots of things won't work with less.
    return __LINE__;
  if(sizeof(short) < 2) // Shorts must be at least 16 bit.
    return __LINE__;
  return 0;
}

// Basic boolean functions work correctly.

int CheckBool()
{
  bool cbool(true);
  if(!cbool)
    return __LINE__;
  cbool = false;
  if(cbool)
    return __LINE__;
  if(cbool && true)
    return __LINE__;
  cbool |= true;
  if(!(cbool && true))
    return __LINE__;
  if(cbool && false)
    return __LINE__;
  if(!cbool)
    return __LINE__;
  return 0;
}

class CheckRC 
  : public HandleRefCounterC
{
public:
  CheckRC()
  {}
  
  CheckRC(const CheckRC &oth)
    : HandleRefCounterC(oth)
  {}
  
  ~CheckRC()
  {}
  
};

class RCCheckC
  : public RCBodyVC
{
public:
  RCCheckC()
    {}
};

// Reference std::couting.

int CheckRefCount()
{  
  // Basic stuff.
  CheckRC r;
  if(r.References() != 1)
    return __LINE__;
  CheckRC *r2 = new CheckRC(r);
  if(r.References() != 2)
    return __LINE__;
  delete r2;
  if(r.References() != 1)
    return __LINE__;
  
  // More advanced.
  RCAbstractC a;
  if(a.IsValid())
    return __LINE__;
  a = RCAbstractC(*new RCCheckC());
  if(!a.IsValid())
    return __LINE__;
  if(a.References() != 1)
    return __LINE__;
  RCAbstractC b(a),c;
  if(a.References() != 2)
    return __LINE__;
  if(b.References() != 2)
    return __LINE__;
  if(a != b)
    return __LINE__;
  if(!(a == b))
    return __LINE__;
  c = a;
  if(c.References() != 3)
    return __LINE__;
  a = RCAbstractC();
  if(a.IsValid())
    return __LINE__;
  c = a;
  if(c.IsValid())
    return __LINE__;
  if(!b.IsValid())
    return __LINE__;
  if(a == b)
    return __LINE__;
  if(!(a != b))
    return __LINE__;
  if(b.References() != 1)
    return __LINE__;
  b = a;
  if(a.IsValid())
    return __LINE__;
  if(b.IsValid())
    return __LINE__;
  return 0;
}

// Check DLists.

int CheckDList()
{
  DListC<int> il;
  if(!il.IsEmpty())
    return __LINE__;
  il.InsLast(2);
  if(il.Size() != 1)
    return __LINE__;
  il.InsLast(3);
  if(il.Size() != 2)
    return __LINE__;
  il.InsFirst(1);
  if(il.Size() != 3)
    return __LINE__;
  // Check iterator.
  int i = 1;
  for(DLIterC<int> it(il);it.IsElm();it.Next(),i++) {
    if(i > 3)
      return __LINE__;
    if(it.Data() != i)
      return __LINE__;
  }
  if(i != 4)
    return __LINE__;
  return 0;
}

// Check basic string stuff.  

int CheckString()
{
  StringC a("two");
  StringC x("hello");
  if(x != "hello")
    return __LINE__;
  x = "whatsup";
  if(x == "hello")
    return __LINE__;
  if(x != "whatsup")
    return __LINE__;
  StringC r = a + ' ' + x;
  if(r != "two whatsup")
    return __LINE__;
  if(a != "two")
    return __LINE__; 
  if(x != "whatsup")
    return __LINE__;
  if(strcmp(x.chars(),"whatsup") != 0)
    return __LINE__;
  if(x.length() != 7)
    return __LINE__;
  if(a.length() != 3)
    return __LINE__;
  if(StringC(123) != "123")
    return __LINE__;
  
  // Quick check of string lists.
  
  StringListC lst("1 23  4 ");
  if(lst.Size() != 3)
    return __LINE__;
  if(lst.First() != "1")
    return __LINE__;
  if(lst.Last() != "4")
    return __LINE__;
  int i= 0;
  for(DLIterC<StringC> it(lst);it.IsElm();it.Next(),i++) 
    {
    switch(i)
      {
      case 0:
	if(it.Data() != "1")
	  return __LINE__;
	break;
      case 1:
	if(it.Data() != "23")
	  return __LINE__;
	break;
      case 2:
	if(it.Data() != "4")
	  return __LINE__;
	break;
      default:
	return __LINE__;
      }
    }
  
  return 0;
}

// Check dead line code.

int CheckDeadLine()
{
  int i = 0,k = 0;
  double v1 = 1;
  DeadLineTimerC t1(0.5);
  DeadLineTimerC t2(0.1);  
  while(t1.IsRunning() && t2.IsRunning() && i < 50000000) {
    i++;
    k += getpid(); // Waste some time.
    v1 *= k * 1.2345; // And some more.
  }
  if(v1 == k && i == k) // Use the variables.
    return __LINE__; 
  if(!t1.IsRunning())
    return __LINE__;
  if(t2.IsRunning())
    return __LINE__;
  
  return 0;
}

int CheckFiles()
{
  FilenameC myself(procname);
  if(!myself.Exists()) {
    std::cerr << "I can't find myself! '" << procname << "'\n";
    return __LINE__;
  }
#if 0
  if(!myself.IsExecutable()) {
    std::cerr << "I'm not self executable! \n";
    return __LINE__;
  }
#endif
  FilenameC tmpFile("/tmp/validFile");
  tmpFile = tmpFile.MkTemp(4,100);
  if(tmpFile.Exists() || tmpFile.IsEmpty()) {
    std::cerr << "Failed to choose temp file. \n";
    return __LINE__;
  }
  {
    OStreamC *os = new OStreamC(tmpFile);
    (*os) << "hello\n" << flush;
    delete os;
  }
  if(!tmpFile.Exists()) {
    std::cerr << "Failed to create temp file. \n";
    return __LINE__;
  }
  if(!tmpFile.Remove()) {
    std::cerr << "Failed to remove temp file. \n";
    return __LINE__;
  }
  if(tmpFile.Exists()) {
    std::cerr << "Failed to delete temp file. \n";
    return __LINE__;
  }
  return 0;
}

// Check child proc code.

int CheckChildProc()
{
  ONDEBUG(std::cerr << "Starting child prog test.... \n");
  StringC pn(procname);
  ONDEBUG(std::cerr << "Using procname='" << procname << "' \n");
  //cerr << "Validate:" << pn << std::endl;
  if(pn.IsEmpty()) {
    std::cerr << "Can't find path to validation program. \n";
    return __LINE__;
  }
#if 1
  // Check return true.
  ChildOSProcessC xp3(pn + " -t");
  if(!xp3.Wait(1)) {
    std::cerr << "ERROR: Wait failed 2 \n";
    return __LINE__;
  }
  if(!xp3.ExitedOk()) {
    std::cerr << "ERROR: Child exited with error\n";
    return __LINE__;
  }
  if(xp3.ExitCode() != 0) {
    std::cerr << "Expected exitcode == 0 : " << xp3.ExitCode() << std::endl;
    return __LINE__;
  }
  
  // Check return false.
  ChildOSProcessC xp4(pn + " -f");
  if(!xp4.Wait(1)) {
    std::cerr << "ERROR: Wait failed 2 \n";
    return __LINE__;
  }
  if(!xp4.ExitedOk()) {
    std::cerr << "ERROR: Child exited with error\n";
    return __LINE__;
  }
  if(xp4.ExitCode() == 0) {
    std::cerr << "Expected exitcode != 0 : " << xp4.ExitCode() << std::endl;
    return __LINE__;
  }
  
  // Check segmentation faults.
  ONDEBUG(std::cerr << "Starting seg fault test... \n");
  FilenameC tmpDir("/tmp");
  if(!tmpDir.Exists()) {
    std::cerr << "Can't find temp directory. \n";
    return __LINE__;
  }
  FilenameC tmpFile("/tmp/validate");
  ONDEBUG(std::cerr << "Calling mktemp \n");
  tmpFile = tmpFile.MkTemp(4,100);
  if(tmpFile.Exists() || tmpFile.IsEmpty()) {
    std::cerr << "Failed to choose temp file. \n";
    return __LINE__;
  }
  StringC segcmd = pn + " -s";
  ONDEBUG(std::cerr << "Executing '" << segcmd << "'\n");
#if 0
  ChildOSProcessC xp2(segcmd,tmpFile,true);
  if(!tmpFile.Exists()) {
    std::cerr << "Failed to create temp file. \n";
    return __LINE__;
  }
#else  
  ChildOSProcessC xp2(segcmd);
#endif
  ONDEBUG(std::cerr << "Waiting for exit... \n");
  xp2.Wait(1.5);
  if(xp2.IsRunning()) {
    std::cerr << "ERROR: Child running, during test.\n";
    return __LINE__;
  }
  tmpFile.Remove();
  if(xp2.ExitedOk()) {
    std::cerr << "ERROR: Child exited ok, during failure test!\n";
    return __LINE__;
  }
#endif
  // Check for hangs.
  StringC cmd = pn + " -hang";
  ONDEBUG(std::cerr << "Starting hang test... '" << cmd << "'\n");
  ChildOSProcessC xp1(cmd);
  if(!xp1.IsRunning()) {
    std::cerr << "Run failed.\n";
    return __LINE__;
  }
  if(xp1.Wait(0.5)) {
    std::cerr << "Wait failed.\n";
    return __LINE__;
  }
  if(!xp1.IsRunning()) {
    std::cerr << "ERROR: Child not running.!\n";
    return __LINE__;
  }
  if(!xp1.Terminate()) {
    std::cerr << "ERROR: Termination failed.!\n";
    return __LINE__;
  }
  ONDEBUG(std::cerr << "hang test done. \n");
  return 0;
}
