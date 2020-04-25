// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlThreads
//! file="Ravl/OS/Threads/Tools/TimedTriggerQueue.cc"

#include "Ravl/config.h"

#ifdef __sgi__
#undef _POSIX_C_SOURCE
#include <standards.h>
#endif

#include "Ravl/Threads/TimedTriggerQueue.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Exception.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/TypeName.hh"

#define DODEBUG 0

#if DODEBUG 
#include "Ravl/String.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

// Disabling catching of exceptions can be useful
// if you need to establish what's throwing them
#define CATCHEXCEPTIONS 0

namespace RavlN
{

  //: Default constructor
  
  TimedTriggerQueueBodyC::TimedTriggerQueueBodyC(bool launchProcessThread)
    : m_started(true),
      eventCount(0),
      m_idlePlace(0),
      done(false),
      semaSched(0)
  {
    if(launchProcessThread)
      LaunchThread(TimedTriggerQueueC(*this,RCLH_CALLBACK),&TimedTriggerQueueC::Process);
  }
  
  //: Destructor.
  
  TimedTriggerQueueBodyC::~TimedTriggerQueueBodyC()
  {}
  
  //: Called when owning handles drops to zero.
  
  void TimedTriggerQueueBodyC::ZeroOwners() {
    // Start shutdown
    done = true;
    // Pass back to parent on principle.
    RCLayerBodyC::ZeroOwners();
  }
  
  //: Shutdown even queue.
  
  bool TimedTriggerQueueBodyC::Shutdown() {
    done = true;
    return true;
  }
  
  //! Start service.
  bool TimedTriggerQueueBodyC::Start() {
    MutexLockC holdLock(access);
    if(m_started) return true;
    m_started = true;
    holdLock.Unlock();

    LaunchThread(TimedTriggerQueueC(*this,RCLH_CALLBACK),&TimedTriggerQueueC::Process);
    return true;
  }

  //: Schedule event for running after time 't' (in seconds).
  
  UIntT TimedTriggerQueueBodyC::Schedule(RealT t,const TriggerC &se,float period) {
    DateC at = DateC::NowUTC(); // Set to now.
    at += t; // Add delay...
    return Schedule(at,se,period);
  }

  //: Schedule event for running periodically.
  UIntT TimedTriggerQueueBodyC::SchedulePeriodic(const TriggerC &se,float period) {
    if(period == 0)
      return ScheduleIdle(se);
    return Schedule(period,se,period);
  }

  UIntT TimedTriggerQueueBodyC::AllocateId()
  {
    do {
      eventCount++;
      if(eventCount == 0)
        eventCount++;
    } while(events.IsElm(eventCount));
    UIntT nEvent = eventCount;
    return nEvent;
  }
  //: Allocate id,
  // Mutex access must be held on call.

  //: Schedule event for running periodically when otherwise idle.
  UIntT TimedTriggerQueueBodyC::ScheduleIdle(const TriggerC &se)
  {
    if(!se.IsValid() || done)
      return 0;
    MutexLockC holdLock(access);
    UIntT nEvent = AllocateId();
    Tuple2C<TriggerC,float> &entry = events[nEvent];
    entry.Data1() = se;
    entry.Data2() = 0;
    m_onIdle.push_back(nEvent);
    holdLock.Unlock();
    semaSched.Post();
    return nEvent;
  }

  //: Schedule event for running.
  // Thread safe.
  
  UIntT TimedTriggerQueueBodyC::Schedule(const DateC &at,const TriggerC &se,float period) {
    if(!se.IsValid() || done)
      return 0;
    MutexLockC holdLock(access);
    UIntT nEvent = AllocateId();
    schedule.Insert(at,nEvent);
    Tuple2C<TriggerC,float> &entry = events[nEvent];
    entry.Data1() = se;
    entry.Data2() = period;
    
    holdLock.Unlock();
    ONDEBUG(std::cerr << "TimedTriggerQueueBodyC::Schedule() Event " << nEvent << " at " << at.Text() << " \n");
    semaSched.Post();
    return nEvent;
  }

  //: Process pending calls, return time to next event.
  // This will always return a positive time unless there are no events pending,
  // when it returns -1 .

  double TimedTriggerQueueBodyC::ProcessStep()
  {
    DateC toGo(-1.0);
    double retTime = -1;
    MutexLockC holdLock(access);
    bool doneWork = false;
    // Are any events scheduled ??
    while(!done) {
      if(!schedule.IsElm()) {
        ONDEBUG(std::cerr << "Waiting for event to be scheduled. Size:" << schedule.Size() << "\n");
        holdLock.Unlock();
        return -1.0;
      }
      DateC nextTime = schedule.TopKey();
      DateC now = DateC::NowUTC();
      toGo = nextTime - now;
      ONDEBUG(std::cerr << "Next scheduled event in " << toGo.Double() << " seconds\n");
      retTime = toGo.Double();
#if 1
      if(doneWork) {
        if(retTime < 0)
          retTime = 0;
        break;
      }
#endif
      UIntT eventNo = 0;
      bool onIdle = false;
      if(retTime > 0.00001) {
        // Tackle any idle work ?
        if(m_onIdle.size() <= 0)
          break;
        m_idlePlace++;
        if(m_idlePlace >= (int) m_onIdle.size())
          m_idlePlace = 0;
        onIdle = true;
        eventNo = m_onIdle[m_idlePlace];
      } else {
        eventNo = schedule.GetTop();
      }
      // Might as well do it now...
      ONDEBUG(std::cerr << "Executing event  " << eventNo << "\n");
      Tuple2C<TriggerC,float> &entry = events[eventNo];
      TriggerC ne = entry.Data1();
      // Is function non-periodic or cancelled?
      if(entry.Data2() < 0 || !ne.IsValid()) {
        events.Del(eventNo); // Remove from queue.
        if(onIdle) {
          RavlAssert(m_idlePlace < (int)m_onIdle.size());
          m_onIdle.erase(m_onIdle.begin() + m_idlePlace);
          if(m_idlePlace > 0) {
            m_idlePlace--;
          } else {
            m_idlePlace = m_onIdle.size()-1;
          }
        }
      } else {
        // Reschedule periodic functions.
        if(entry.Data2() > 0) {
          DateC at = now + entry.Data2();
          schedule.Insert(at,eventNo);
        }
      }
      holdLock.Unlock(); // Unlock before invoke the event, in case it wants to add another.
      if(ne.IsValid()) { // Check if event has been cancelled.
#if CATCHEXCEPTIONS
        try {
#endif
          doneWork = true;
          ne.Invoke();
          ne.Invalidate(); // Make sure handle is invalidated without lock held.
#if CATCHEXCEPTIONS
        } catch(ExceptionC &ex) {
          RavlError("Caught exception in timed trigger event thread. Type:%s Message:'%s'",RavlN::TypeName(typeid(ex)),ex.Text());
          // Dump a stack.
          ex.Dump(std::cerr);
          // If in check or debug stop.
          RavlAssertMsg(0,"Aborting due to exception in timed trigger event thread. ");
          // Otherwise ignore and struggle on.
        } catch(std::exception &ex) {
          RavlError("Caught std exception in timed trigger event thread. Type:%s What:%s ",RavlN::TypeName(typeid(ex)),ex.what());
          // If in check or debug stop.
          RavlAssertMsg(0,"Aborting due to exception in timed trigger event thread. ");
          // Otherwise ignore and struggle on.
        } catch(...) {
          RavlError("Caught unknown exception in timed trigger event thread.");
          // If in check or debug stop.
#ifndef NDEBUG
          throw; // Throw it on, as its the only way to report what it is.
          //RavlAssertMsg(0,"Aborting due to exception in timed trigger event thread. ");
#endif
          // Otherwise ignore and struggle on.
        }
#endif
      }
      ONDEBUG(else std::cerr << "Event cancelled. \n");
      holdLock.Lock();
    }
    if(!m_onIdle.empty())
      retTime = 0;
    holdLock.Unlock();
    return retTime;
  }

  //: Process event queue.
  
  bool TimedTriggerQueueBodyC::Process() {
    ONDEBUG(std::cerr << "TimedTriggerQueueBodyC::Process(), Called. \n");
    while(!done) {
      double nextEvent = ProcessStep();
      if(nextEvent < 0) {
        semaSched.Wait();
      } else {
        if(nextEvent > 0)
          semaSched.Wait(nextEvent);
      }
      ONDEBUG(std::cerr << "Time to check things out.\n");
    }
    // FIXME:- Dump all pending events.
    return true;
   }
  
  //: Cancel pending event.
  // Will return TRUE if event in cancelled before
  // it was run.
  
  bool TimedTriggerQueueBodyC::Cancel(UIntT eventID)
  {
    MutexLockC holdLock(access);
    Tuple2C<TriggerC,float> *eventEntry = events.Lookup(eventID);
    if(eventEntry == 0)
      return false;
    if(eventEntry->Data2() <= 0) {
      for(auto at = m_onIdle.begin();at != m_onIdle.end();++at) {
        if(*at == eventID) {
          m_onIdle.erase(at);
          break;
        }
      }
    }
    eventEntry->Data1().Invalidate(); // Cancel event.

    return true;
  }

  //: Change period
  bool TimedTriggerQueueBodyC::ChangePeriod(UIntT eventId, float period)
  {
    MutexLockC holdLock(access);
    Tuple2C<TriggerC,float> *eventEntry = events.Lookup(eventId);
    if(eventEntry == 0)
      return false;
    if(eventEntry->Data2() <= 0) {
      if(period <= 0)
        return true; // Don't need to do anything.
      for(auto at = m_onIdle.begin();at != m_onIdle.end();++at) {
        if(*at == eventId) {
          m_onIdle.erase(at);
          break;
        }
      }
    }
    // Need to schedule ?
    if(eventEntry->Data2() == 0 && period > 0)
    {
      RavlN::DateC at = RavlN::DateC::NowUTC() + period;
      schedule.Insert(at,eventId);
    }

    eventEntry->Data2() = period;
    if(period <= 0)
      m_onIdle.push_back(eventId);
    return true;
  }

  //! Access a global trigger queue.
  // As one thread is sharing all the work,
  // long (>0.1s) tasks be spawned on a separate thread.
  TimedTriggerQueueC GlobalTriggerQueue() {
    static TimedTriggerQueueC triggerQueue(true);
    return triggerQueue;
  }


}
