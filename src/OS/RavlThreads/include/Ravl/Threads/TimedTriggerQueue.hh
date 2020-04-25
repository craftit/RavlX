// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TIMEDTRIGGERQUEUE_HEADER
#define RAVL_TIMEDTRIGGERQUEUE_HEADER 1
/////////////////////////////////////////////////////////
//! file="Ravl/OS/Threads/Tools/TimedTriggerQueue.hh"
//! lib=RavlThreads
//! docentry="Ravl.API.OS.Time"
//! author="Charles Galambos"
//! date="23/09/1999"

#include "Ravl/OS/Date.hh"
#include "Ravl/PriQueue.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCLayer.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/Calls.hh"
#include "Ravl/SmartLayerPtr.hh"
#include <vector>

// The new timed trigger code doesn't rely the un*x select system call,
// but gives less accurate timing.  

namespace RavlN
{
  class TimedTriggerQueueC;
  
  //! userlevel=Develop
  //: Timed event queue body.
  // This is a queue of routines to be triggered at some point in the future.
  // See the handle class for more details.
  
  class TimedTriggerQueueBodyC 
    : public RavlN::RCLayerBodyC
  {
  public:
    TimedTriggerQueueBodyC(bool launchProcessThread = true);
    //: Constructor
    // If launchProcessThread is false the user must either call 'Start()' on their own thread
    // or use the 'ProcessStep()' method to call run scheduled events.

    virtual ~TimedTriggerQueueBodyC();
    //: Destructor.
    // This will not return until shutdown is complete.
    
    UIntT Schedule(RealT t,const TriggerC &se,float period = -1);
    //: Schedule event for running after time 't' (in seconds).
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.
    
    UIntT Schedule(const DateC &startAt,const TriggerC &se,float period = -1);
    //: Schedule event for running.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.

    UIntT SchedulePeriodic(const TriggerC &se,float period);
    //: Schedule event for running periodically.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.

    UIntT ScheduleIdle(const TriggerC &se);
    //: Schedule event for running periodically when otherwise idle.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.

    bool Cancel(UIntT eventID);
    //: Cancel pending event.
    // Will return TRUE if event in cancelled before
    // it was run.

    bool ChangePeriod(UIntT eventId, float period);
    //: Change period of event.
    // If 0 period is given the event will be run on idle.
    // Returns true if event is found and has been updated.
    // This will take effect after the event is next run.

    virtual bool Shutdown();
    //: Shutdown even queue.
    
    virtual bool Start();
    //: Start service.

    double ProcessStep();
    //: Process pending calls, return time to next event.
    // This can be used to embed a scheduler in another main loop. It is up to the
    // user of the function to ensure the delay is cut short and its recalled
    // if any new work is scheduled. Start should not be called
    // This will always return a positive time unless there are no events pending,
    // when it returns -1.

    typedef RavlN::SmartOwnerPtrC<TimedTriggerQueueBodyC> RefT;
    //: Owner reference counted ptr to class

    typedef RavlN::SmartCallbackPtrC<TimedTriggerQueueBodyC> CBRefT;
    //: Callback reference counter ptr to class

  protected:
    UIntT AllocateId();
    //: Allocate id,
    // Mutex access must be held on call.

    bool Process();
    //: Process event queue.

    virtual void ZeroOwners();
    //: Called when owning handles drops to zero.
    
    MutexC access;
    bool m_started;
    UIntT eventCount;
    PriQueueC<DateC,UIntT> schedule;
    HashC<UIntT,Tuple2C<TriggerC,float> > events;
    int m_idlePlace;
    std::vector<UIntT> m_onIdle;
    bool done;
    // Queue fd's
    SemaphoreC semaSched;
    
    friend class TimedTriggerQueueC;
  };
  
  //! userlevel=Normal
  //: Timed event queue.
  // This is a queue of routines to be triggered at some point in the future.
  // This class creates a new thread which triggers a list of routines
  // in the requested sequence.  A single thread is used to call all the
  // routines, so if any lengthy processing is required it is better to
  // spawn a new thread to complete it.   
  
  class TimedTriggerQueueC 
    : public RCLayerC<TimedTriggerQueueBodyC>
  {
  public:
    TimedTriggerQueueC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    TimedTriggerQueueC(bool launchProcessThread)
      : RCLayerC<TimedTriggerQueueBodyC>(*new TimedTriggerQueueBodyC(launchProcessThread))
    {}
    //: Constructor.
    // If launchProcessThread is false the user must either call 'Start()' on their own thread
    // or use the 'ProcessStep()' method to call run scheduled events.
    
  protected:
    TimedTriggerQueueC(TimedTriggerQueueBodyC &bod,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<TimedTriggerQueueBodyC>(bod,handleType)
    {}
    //: Body constructor.
    
    TimedTriggerQueueBodyC &Body() 
    { return RCLayerC<TimedTriggerQueueBodyC>::Body(); }
    //: Access body.

    const TimedTriggerQueueBodyC &Body() const
    { return RCLayerC<TimedTriggerQueueBodyC>::Body(); }
    //: Access body.
    
    bool Process()
    { return Body().Process(); }
    //: Used to start internal thread.
  public:
    UIntT Schedule(RealT t,const TriggerC &se,float period = -1)
    { return Body().Schedule(t,se,period); }
    //: Schedule event for running after time 't' (in seconds).
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.  The returned ID never has the value 0, so it may be used to flag not set.
    
    UIntT Schedule(const DateC &at,const TriggerC &se,float period = -1)
    { return Body().Schedule(at,se,period); }
    //: Schedule event for running.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.  The returned ID never has the value 0, so it may be used to flag not set.
    
    UIntT SchedulePeriodic(const TriggerC &se,float period)
    { return Body().SchedulePeriodic(se,period); }
    //: Schedule event for running periodically.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.  The returned ID never has the value 0, so it may be used to flag not set.

    UIntT ScheduleIdle(const TriggerC &se)
    { return Body().ScheduleIdle(se); }
    //: Schedule event for running periodically when otherwise idle.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.

    bool Cancel(UIntT eventID)
    { return Body().Cancel(eventID); }
    //: Cancel pending event.
    // Will return TRUE if event in cancelled before
    // it was run.
    
    void Shutdown()
    { Body().Shutdown(); }
    //: Shutdown even queue.
    
    double ProcessStep()
    { return Body().ProcessStep(); }
    //: Process pending calls, return time to next event.
    // This can be used to embed a scheduler in another main loop. It is up to the
    // user of the function to ensure the delay is cut short and its recalled
    // if any new work is scheduled. Start should not be called
    // This will always return a positive time unless there are no events pending,
    // when it returns -1.

    bool ChangePeriod(UIntT eventId, float period)
    { return Body().ChangePeriod(eventId,period); }
    //: Change period of event.
    // Returns true if event is found and has been updated.
    // This will take effect after the event is next run.
    // If a 0 period is given the event will be run on idle.

    friend class TimedTriggerQueueBodyC;
  };

  //! Access a global trigger queue.
  // As one thread is sharing all the work,
  // long (>0.1s) tasks be spawned on a separate thread.
  TimedTriggerQueueC GlobalTriggerQueue();
  
}

#endif
