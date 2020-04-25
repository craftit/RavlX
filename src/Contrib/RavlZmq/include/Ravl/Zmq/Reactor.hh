// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQN_REACTOR_HEADER
#define RAVL_ZMQN_REACTOR_HEADER
//! lib=RavlZmq

#include "Ravl/Zmq/Socket.hh"
#include "Ravl/Zmq/SocketDispatcher.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/String.hh"
#include "Ravl/Trigger.hh"
#include "Ravl/ServiceThread.hh"
#include "Ravl/Threads/TimedTriggerQueue.hh"
#include <vector>
#include <string>

namespace RavlN {
  namespace ZmqN {

    //! Reactor.

    class ReactorC
      : public ServiceThreadC
    {
    public:
      //! Default constructor.
      ReactorC(ContextC &context);

      //! Factory constructor
      ReactorC(const XMLFactoryContextC &factory);

      //! Write to an ostream
      bool Save(std::ostream &strm) const;

      //! Write to a binary stream
      // Not implemented
      bool Save(BinOStreamC &strm) const;

      //! Sugar to make it easier to setup from a factory.
      SocketDispatcherC::RefT CallOnRead(const XMLFactoryContextC &factory,const std::string &name,const TriggerC &trigger,SocketC::RefT &skt);

      //! Sugar to make it easier to setup from a factory.
      SocketDispatcherC::RefT CallOnWrite(const XMLFactoryContextC &factory,const std::string &name,const TriggerC &trigger,SocketC::RefT &skt);

      //! Add a read trigger
      SocketDispatcherC::RefT CallOnRead(const SocketC &socket,const TriggerC &trigger);

      //! Add a write trigger
      SocketDispatcherC::RefT CallOnWrite(const SocketC &socket,const TriggerC &trigger);

      //! Add a read trigger
      SocketDispatcherC::RefT CallOnReadOrError(int fd,const TriggerC &trigger);

      //! Add a read trigger
      SocketDispatcherC::RefT CallOnError(int fd,const TriggerC &trigger);

      //! Add handler to system
      bool Add(const SocketDispatcherC &handler);

      //! Remove handler from system
      bool Remove(const SocketDispatcherC &handler);

      //! Schedule event for running on the reactor thread
      // Thread safe.
      // Returns an ID for the event, which can
      // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
      UIntT Schedule(const TriggerC &se);

      //! Schedule event for running on the reactor thread when its idle
      // Thread safe.
      // Returns an ID for the event, which can
      // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
      UIntT ScheduleIdle(const TriggerC &se);

      //! Schedule event for running after time 't' (in seconds).
      // Thread safe.
      // Returns an ID for the event, which can
      // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
      UIntT Schedule(RealT t,const TriggerC &se,float period = -1);

      //! Schedule event for running.
      // Thread safe.
      // Returns an ID for the event, which can
      // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
      UIntT Schedule(const DateC &at,const TriggerC &se,float period = -1);

      //! Schedule event for running periodically.
      // Thread safe.
      // Returns an ID for the event, which can
      // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
      UIntT SchedulePeriodic(const TriggerC &se,float period);

      //! Change period of event.
      // Returns true if event is found and has been updated.
      // A period of zero or less will schedule event on idle.
      // This will take effect after the event is next run.
      bool ChangePeriod(UIntT eventId, float period);

      //! Cancel pending event.
      // Will return TRUE if event in cancelled before
      // it was run.
      bool Cancel(UIntT eventID);

      //! Run reactor loop.
      virtual bool Run();

      //! Shutdown service
      virtual bool Shutdown();

      //! Set verbose flag.
      void SetVerbose(bool verboseMode)
      { m_verbose = verboseMode; }

      //! Owner reference counted ptr to class
      typedef RavlN::SmartOwnerPtrC<ReactorC> RefT;

      //! Callback reference counter ptr to class
      typedef RavlN::SmartCallbackPtrC<ReactorC> CBRefT;

    protected:
      //! Do some initial setup
      void Init();

      //! Called by the main loop when its first run.
      virtual bool OnStart();

      //! Called when loop is exiting.
      virtual bool OnFinish();

      //! Called when wake up has been done
      bool WakeDone(ZmqN::SocketC::RefT &wakeupLocal);

      //! Send wake up event for timed event queue.
      UIntT SendWakeForTimeQueue(UIntT eventId);

      RavlN::ZmqN::ContextC::RefT m_zmqContext;

      std::vector<SocketDispatcherC::RefT> m_sockets;
      float m_teminateCheckInterval;
      bool m_pollListChanged;
      bool m_verbose;

      TimedTriggerQueueC m_timedQueue;
      MutexC m_accessWakeup;
      ZmqN::SocketC::RefT m_wakeup;
      MessageC::RefT m_wakeMsg;
      bool m_wakeScheduled;

      //! Called when owner handles drop to zero.
      virtual void ZeroOwners();

    };
  }
}

#endif
