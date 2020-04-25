// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/testTimedTriggerQueue.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Time"
//! userlevel=Develop

#include "Ravl/Threads/TimedTriggerQueue.hh"
#include "Ravl/Random.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/Calls.hh"
#include "Ravl/SysLog.hh"
#include <math.h>

using namespace RavlN;

const int noTests = 10;
const RealT g_timePeriod = 2.0;
const RealT maxTimerError = 0.04;

DateC when[noTests];

bool MarkTime(int &i) {
  when[i] = DateC(true); // Mark now.
  RavlN::Sleep(0.05);
  //  std::cerr << " i:" << i << "\n";
  return true;
}


int testTiming();
int testPeriodChange();

int main()
{
  //RAVL_RUN_TEST(testTiming());
  RAVL_RUN_TEST(testPeriodChange());
  return 0;
}


int testTiming()
{
  std::cerr << "Running test... \n";
  TimedTriggerQueueC eventQueue(true);
  Sleep(0.01);  // Give event queue time to setup properly...
  DateC requestedTime[noTests];
  int i;
  for(i = 0;i < noTests;i++) {
#if 0
    // Test handling of no delay
    if(i % 2 == 0) {
      requestedTime[i] = DateC::NowUTC();
      eventQueue.Schedule(-1.0,Trigger(MarkTime,i));
    }
    else
#endif
    {
      requestedTime[i] = DateC::NowUTC() + Random1()*g_timePeriod;
      eventQueue.Schedule(requestedTime[i],Trigger(&MarkTime,i));
    }
  }
  Sleep(g_timePeriod + maxTimerError*10.0);  // Wait for all events to take place.
  for(i = 0;i < noTests;i++) {
    RealT diff = (requestedTime[i] - when[i]).Double();
    std::cerr << "Timing error:" << diff << " " << requestedTime[i].ODBC(false,true) << " " << when[i].ODBC(false,true) << " \n";
    if(fabs(diff) > maxTimerError) {
      std::cerr << "ERROR: Timing out of spec \n";
      return 1;
    }
  }
  std::cerr << "TEST PASSED.\n";
  return 0; 
}

bool ChangeTime(int *theEventId,TimedTriggerQueueC *queue) {
  static int state = 0;
  static DateC lastCall = RavlN::DateC::NowUTC();
  DateC now = RavlN::DateC::NowUTC();
  RavlDebug(" %d at %f ",state,(now - lastCall).Double());
  lastCall = now;
  switch(state) {
    case 0:
      queue->ChangePeriod(*theEventId,1.0);
      break;
    case 1:
      queue->ChangePeriod(*theEventId,2.0);
      break;
    case 2:
      break;
    case 3:
      queue->ChangePeriod(*theEventId,0.0);
      break;
    case 4:
      break;
    case 5:
      queue->ChangePeriod(*theEventId,1.0);
      break;
    case 6:
      break;
    case 7:
      break;
    default:
      *theEventId = 0;
      break;
  }
  state++;
  return true;
}

int testPeriodChange()
{
  TimedTriggerQueueC eventQueue(true);
  int theEventId = 0;
  theEventId = eventQueue.SchedulePeriodic(Trigger(&ChangeTime,&theEventId,&eventQueue),0.1);
  DateC start = DateC::NowUTC();
  while(theEventId != 0) {
    Sleep(0.2);
    if((DateC::NowUTC() - start).Double() > 10) {
      std::cerr << "Test failed.\n";
      return 1; //
    }
  }
  return 0;
}
