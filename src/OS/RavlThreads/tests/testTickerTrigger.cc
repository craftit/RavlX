// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/testTickerTrigger.cc"
//! docentry="Ravl.API.OS.Time"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Threads/TimedTriggerQueue.hh"
#include "Ravl/Threads/TickerTrigger.hh"
#include "Ravl/Atomic.hh"
#include "Ravl/OS/DeadLineTimer.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

ravl_atomic_t callcount;

class ExampleC
{
public: 
  bool DoIt()
    {
      ravl_atomic_inc(&callcount);
      //cerr << "Done " << dec << atomic_read(&callcount) << ". \n";
      return true;
    }  
};

int testTickerTrigger(); 
int testTimedTriggerQueueTicker();

int main()
{
  int ln;
  if((ln = testTickerTrigger()) != 0) {
    std::cerr << "Test failed on line " << ln << " \n";
    return 1;
  }
  if((ln = testTimedTriggerQueueTicker()) != 0) {
    std::cerr << "Test failed on line " << ln << " \n";
    return 1;
  }

  return 0;
}

int testTickerTrigger() {
  std::cerr << "Starting tick test. \n";
  ravl_atomic_set(&callcount,0);

  ExampleC ae;

  TickerTriggerC tt1 = TickerTrigger(0.2,ae,&ExampleC::DoIt);
  TickerTriggerC tt2 = TickerTrigger(0.2,ae,&ExampleC::DoIt);

  std::cerr << "Waiting 10 seconds. \n";

  DeadLineTimerC dlt(10.0);
  dlt.WaitForIt();
  tt1.Shutdown();
  tt2.Shutdown();
  
  int result = ravl_atomic_read(&callcount);
  int targ = 100;
  std::cerr << "Result : " << result << "  Target:" << targ << "\n";

  if(result < (targ - 6) || result > (targ + 6)) {
    std::cerr << "Result out of range. \n";
    return __LINE__;
  }
  std::cerr << "Test passed. \n";
  return 0;
}

int testTimedTriggerQueueTicker() {
  std::cerr << "Starting queued tick test. \n";
  ravl_atomic_set(&callcount,0);

  ExampleC ae;

  TimedTriggerQueueC ttq = RavlN::GlobalTriggerQueue();
  UIntT qid1 = ttq.SchedulePeriodic(Trigger(ae,&ExampleC::DoIt),0.2);
  UIntT qid2 = ttq.SchedulePeriodic(Trigger(ae,&ExampleC::DoIt),0.2);

  std::cerr << "Waiting 10 seconds. \n";

  DeadLineTimerC dlt(10.0);
  dlt.WaitForIt();

  ttq.Cancel(qid1);
  ttq.Cancel(qid2);

  int result = ravl_atomic_read(&callcount);
  int targ = 100;
  std::cerr << "Result : " << result << "  Target:" << targ << "\n";

  if(result < (targ - 6) || result > (targ + 6)) {
    std::cerr << "Result out of range. \n";
    return __LINE__;
  }
  std::cerr << "Test passed. \n";
  return 0;
}
