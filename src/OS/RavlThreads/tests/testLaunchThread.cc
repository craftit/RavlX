// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/testLaunchThread.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Threads"
//! userlevel=Develop

#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Threads/MessageQueue.hh"
#include "Ravl/Stream.hh"

#include <unistd.h>

using namespace RavlN;

int count1 = 0;
int count2 = 0;

class ExampleC
{
public:
  bool DoIt()
    {
      std::cerr << "Done it. \n";
      count1++;
      return true;
    }
  
  bool DoItArg(int &x)
    {
      std::cerr << "Done it:" << x << ". \n";
      count2 += x;
      return true;
    }
};

int TestLaunchThreads();
int TestMessageQueue();

int main()
{
  int ln;
  if((ln = TestLaunchThreads()) != 0) {
    std::cerr << "Test failed line " << ln << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

bool TestProg() {
  return true;
}

int TestLaunchThreads() {
  ExampleC ae;
  
  LaunchThread(32000,Trigger(&TestProg));
  LaunchThread(ae,&ExampleC::DoIt);
  LaunchThread(ae,&ExampleC::DoItArg,2);
  sleep(1);
  if(count1 != 1 || count2 != 2) {
    std::cerr << "Error launching threads. \n";
    return __LINE__;
  }
  return 0;
}

int TestMessageQueue() {
  MessageQueueC<int> q(4);
  // Do some basic tests.
  if(q.IsElm()) return __LINE__;
  q.Put(1);
  if(!q.IsElm()) return __LINE__;
  if(q.Get() != 1) return __LINE__;
  if(q.IsElm()) return __LINE__;
  
  return 0;
}
