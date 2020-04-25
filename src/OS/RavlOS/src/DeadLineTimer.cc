// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Time/DeadLineTimer.cc"

#include"Ravl/config.h"

#if RAVL_OS_SOLARIS
#define __EXTENSIONS__ 1
#include <sys/time.h>
#endif

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#if RAVL_OS_IRIX
//#ifndef _POSIX_C_SOURCE
//#define _POSIX_C_SOURCE 199309L
//#endif
#include <sys/types.h>
#include <bstring.h>
#include <sys/time.h>
#endif
#include <errno.h>
#include <stdio.h>

#include <signal.h>

#include "Ravl/OS/DeadLineTimer.hh"
#include "Ravl/Hash.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/String.hh"
#include "Ravl/Exception.hh"
#if RAVL_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <assert.h>
#include <signal.h>

#if RAVL_HAVE_SCHED_H
#include <sched.h>
#endif

#if RAVL_HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#define USE_SIGACTION 1

#define DODEBUG 0

#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

struct sigaction;

namespace RavlN {
  
  static HashC<int,bool *> alarms;
  
  static const int MaxAlarms = 64;
  
  class DeadLineAlarmsC {
  public:
    DeadLineAlarmsC()
      : active(false),
      flag(0)
      {}
    ~DeadLineAlarmsC()
      {
	// Try and stop problems when exiting.
	active = false;
      }
    
    bool active;
    volatile bool *flag;
    DateC at; // When alarm should go off.
    
  };
  
  static DeadLineAlarmsC realDeadLineAlarms[MaxAlarms];
  static int realAlarmsUsed = 0;
  static DeadLineAlarmsC virtDeadLineAlarms[MaxAlarms];
  static int virtAlarmsUsed = 0;
  
  void deadLineAlarmHandler(int sig)  {
#if !RAVL_COMPILER_VISUALCPP
    DateC now;
    DateC next;
    bool another = false;
    // Look through alarms to which have expired.
    int alarmsUsed;
    DeadLineAlarmsC *table;
    int timerType;
    
    // Virtual or real timer ??
    
    if(sig == SIGALRM) {
      alarmsUsed = realAlarmsUsed;
      table = realDeadLineAlarms;
      timerType = ITIMER_REAL;
      now.SetToNow(false);
      //ONDEBUG(cerr << "Got SIGALRM \n");
    } else {
      if(sig == SIGVTALRM) {
	alarmsUsed = virtAlarmsUsed;
	table = virtDeadLineAlarms;
	timerType = ITIMER_VIRTUAL;
	now.SetToNow(true);
	//ONDEBUG(cerr << "Got SIGVTALRM \n");
      } else {
	ONDEBUG(cerr << "Got weird signal!! \n");
	return ; // Something really weird, print a message ?
      }
    }
    
    // Check alarms...
    
    for(int i = 0;i < alarmsUsed && i < MaxAlarms;i++) {
      DeadLineAlarmsC &alrm = table[i];
      if(!alrm.active) // Only interested in active alarms.
	continue;
      if(alrm.at <= now) { // Alarm gone off ??
	*(alrm.flag) = true;
	alrm.active = false;
	continue;
      }
      // Work out when next alarm is due...
      if(!another) { // Have we found any yet ?
	next = alrm.at;
	another = true; // Got one.
	continue;
      }
      if(alrm.at < next) // Is this alarm sooner than the 'next' ?
	next = alrm.at;
    }
    if(!another)
      return ; // No more alarms pending, just exit....
    
    // Setup next alarm.
    
#if !USE_SIGACTION
    sigset(sig,deadLineAlarmHandler);
#endif
    DateC timeToGo = next - now;
    RealT rtime = timeToGo.Double();
    if(rtime < 0.001)
      rtime = 0.001; // Prevent things from getting silly.
    struct itimerval t;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_usec = (int) ((RealT) rtime * 1000000) % 1000000;
    t.it_value.tv_sec = (int) ((RealT) rtime);
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    setitimer((__itimer_which)timerType,&t,0);
#else
    setitimer(timerType,&t,0);
#endif
#endif
  };
  
//: Stop timer.
  
  void DeadLineTimerC::Stop()  {
    MTReadLockC lock;
    // Several threads can Stop timers, but not when a timer
    // is being setup.
    if((!timesUp) && (bool) (id > -1)) {
      if(virt)
	virtDeadLineAlarms[id].active = false;
      else
	realDeadLineAlarms[id].active = false;    
    }
    id = -1;
    timesUp = true;
  }
  
  
  RealT DeadLineTimerC::Remaining() const {
    if(timesUp || (bool)(id < 0))
      return 0; // Timer expired.
    DateC ret;
    MTReadLockC lock;
    if(timesUp || (bool)(id < 0)) // Double check.
      return 0; 
    if(virt)
      ret = virtDeadLineAlarms[id].at;
    else
      ret = realDeadLineAlarms[id].at;    
    lock.Unlock();
    //ONDEBUG(cerr << "Expires:" << ret.Double() << " Now:" << DateC(true).Double() << "\n");
    ret -= DateC(true);
    RealT dr = ret.Double();
    //ONDEBUG(cerr << "To go:" << dr << "\n");
    if(dr < 0) // Check it hasn't really expired.
      return 0; 
    return dr;
  }
  
  //: Reset timer with 'time' left before deadline.
  
  bool DeadLineTimerC::Reset(RealT time) {
    if(time < 0) { // Set to infinite ?
      if(!timesUp && id > -1) // Is timer running ?
	Stop();
      id = -1;
      timesUp = false;
      return true;
    }
    if(time < 0.001) {
      ONDEBUG(cerr << "DeadLineTimerC::Reset() Warning: Minimum deadline time is 1ms \n");
      return false;
    }
    DateC thisAlarm = DateC(true,virt) + time;
    return Reset(thisAlarm);
  }
  
  bool DeadLineTimerC::Reset(const DateC &theDeadLine)  {
#if !RAVL_OS_WIN32
    DateC now(true,virt);
    if(now >= theDeadLine)
      return false;
    int &alarmsUsed = (virt) ? virtAlarmsUsed : realAlarmsUsed;
    DeadLineAlarmsC *deadLineAlarms = (virt) ? virtDeadLineAlarms :realDeadLineAlarms;  
    DateC thisAlarm(theDeadLine);
    DateC nextAlarm;
    int maxUsed,minUnused;
    DateC timeToGo;
    RealT rtime = -1;
    MTWriteLockC lock;
    // Check timer is not running.
    if(!timesUp && id > -1)
      deadLineAlarms[id].active = false;
    // Find a free timer.
    bool another;
    do {
      another = false;
      nextAlarm = thisAlarm;
      maxUsed = -1;
      minUnused = -1;
      for(int i = 0;i < MaxAlarms && i < alarmsUsed;i++) {
	DeadLineAlarmsC &alrm = deadLineAlarms[i];
	if(alrm.active == false) {
	  if(minUnused < 0) {
	    minUnused = i;
	    maxUsed = i;
	  }
	  continue;
	}
	maxUsed = i;
	// While we're here we'd better check for any deadlines
	// pending else we might miss one when we reset the 
	// alarm time.
	if(now >= alrm.at) { // Set off alarm ?
	  //cerr << "Found alarm in Reset." << endl;
	  *(alrm.flag) = true;
	  alrm.active = false;
	  if(minUnused < 0) 
	    minUnused = i;
	  continue;
	}
	// We have to work out when next alarm is.
	if(nextAlarm > alrm.at) {
	  nextAlarm = alrm.at;
	  another = true;
	}
      }
      // Work out time remaining to next alarm.
      now.SetToNow(virt); 
      timeToGo = nextAlarm - now;
      rtime = timeToGo.Double();
      // If timer we we're going to wait for has expired try again,
      // provided it isn't the one we're trying to set!
    } while(rtime <= 0 && another);
    
    // Here: rtime must be positive unless,
    // the time we're trying to set has expired.
    
    alarmsUsed = maxUsed + 1;
    if(alarmsUsed == MaxAlarms && minUnused < 0) {
      cerr << "DeadLineTimerC::Reset(), ERROR: Out of alarms! \n";
      return false;
    }
    if(minUnused == -1) {
      minUnused = alarmsUsed;
      alarmsUsed++;
    }
    id = minUnused;
    //cerr << "Using Id: " << id << " alarmsUsed: " << alarmsUsed << endl;
    DeadLineAlarmsC &alrm = deadLineAlarms[id];
    if(rtime > 0) {
      timesUp = false;
      alrm.flag = &timesUp;
      alrm.active = true;
      alrm.at = thisAlarm;
    } else { 
      // Alarm expired before end of routine...
      lock.Unlock();
      //cerr << "Warning: Deadline expired before end of Reset! . \n";
      alrm.active = false;
      return false;
    }
    lock.Unlock();
    // Because different threads can have different signal vectors
    // its safer to set this every time.
    
#if USE_SIGACTION
    struct sigaction sa;
    sa.sa_flags = 0; // Default is fine.
    sa.sa_handler = deadLineAlarmHandler;
    sigprocmask(0,0,&sa.sa_mask); // Set current signal mask.
    //sigaddset(&sa.sa_mask,SIGALARM);
    if(sigaction((virt) ? SIGVTALRM : SIGALRM,&sa,0) != 0) {
      cerr << "DeadLineTimerC::Reset(), Failed to set signal. " << errno << "\n";
      return false;
    }
#else
    if(sigset((virt) ? SIGVTALRM : SIGALRM,deadLineAlarmHandler) == SIG_ERR) {
      cerr << "DeadLineTimerC::Reset(), Failed to set signal. " << errno << "\n";
      return false;
    }
#endif
    // Set up alarm
    //cerr << "Setting timer to " << rtime << "\n";
    struct itimerval t;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_usec = (int) ((RealT) (rtime - floor(rtime)) * 1000000.0) ;
    t.it_value.tv_sec = (int) ((RealT) rtime);
    if(setitimer((virt) ? ITIMER_VIRTUAL : ITIMER_REAL,&t,0) != 0) {
      std::cerr << "Failed to set the time to " << rtime << " seconds " << t.it_value.tv_sec << " " << t.it_value.tv_usec << "\n";
      perror("DeadLineTimerC::Reset(), Failed to set timer to \n");
      return false;
    }
    return true;
#else
    throw ExceptionC("DeadLineTimerC::Reset(),  Not implemented. ");
#endif
  }
  
#define USE_SIGWAIT 1
  
  //: Wait for deadline to expire.
  // May return false if no deadline pending.
  
  bool DeadLineTimerC::WaitForIt() const {
#if !RAVL_OS_WIN32 && !RAVL_OS_CYGWIN
    if(id < 0 || timesUp)
      return false; // No timer set.
    while(!timesUp) {
      ONDEBUG(cerr << "Waiting for signal. " << ((void *) this) << " \n");
      RealT rem = Remaining();
      if(rem <= 0.0000001) {// No meaning-full time left ?
	const_cast<DeadLineTimerC &>(*this).timesUp = true; // Make sure the timesup flag is set appropriatly.
	break;
      }
      if(rem <= 0.001) { // Less than 1/1000 of a second to go ?
	// Just procrastinate a little...
#if RAVL_HAVE_SCHED_YIELD
	sched_yield();
#elif RAVL_HAVE_YIELD
	yield(); 
#else
	sleep(0);
#endif
	continue;
      }
      struct timeval tv;
      tv.tv_sec = (int) rem;
      tv.tv_usec = (int) ((RealT) (rem - ((RealT)tv.tv_sec)) * 1000000);
      ONDEBUG(cerr << "Wait :" << tv.tv_sec << " : " << tv.tv_usec << "  Rem: " << rem << " \n");
      select(1,0,0,0,&tv);
      ONDEBUG(cerr << "Got signal. " << ((void *) this) << "\n");
    }
    return true;
#else
    throw ExceptionC("DeadLineTimerC::WaitForIt() not implemented on this architecture. ");
#endif
  }

  ///// DateC ////////////////////////////////////////////////////////

#if 0
  // The deadline timer was causing random segmentation faults on linux, so this has been changed to
  // an alternate implementation found in Date.cc
  
  bool DateC::Wait() const {
    DeadLineTimerC waittill(*this);
    return waittill.WaitForIt();
  }
  
  bool Sleep(RealT delay) {
    DateC now(true);
    now += delay;
    return now.Wait();
  }
#endif

}
