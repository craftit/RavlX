// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/Reactor.hh"
#include "Ravl/Zmq/SocketDispatchTrigger.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace ZmqN {

    //! Default constructor.
    ReactorC::ReactorC(ContextC &context)
     : m_zmqContext(&context),
       m_teminateCheckInterval(5.0),
       m_pollListChanged(true),
       m_verbose(false),
       m_timedQueue(false),
       m_wakeScheduled(false)
    {
      Init();
    }


    //! Factory constructor
    ReactorC::ReactorC(const XMLFactoryContextC &factory)
     : ServiceThreadC(factory),
       m_teminateCheckInterval(factory.AttributeReal("terminateCheckInterval",5.0)),
       m_pollListChanged(true),
       m_verbose(factory.AttributeBool("verbose",DODEBUG)),
       m_timedQueue(false),
       m_wakeScheduled(false)
    {
      factory.UseComponent("ZmqContext",m_zmqContext,false,typeid(RavlN::ZmqN::ContextC));
      Init();
    }

    //! Write to an ostream
    bool ReactorC::Save(std::ostream &strm) const
    {
      return ServiceThreadC::Save(strm);
    }

    //! Write to a binary stream
    // Not implemented
    bool ReactorC::Save(BinOStreamC &strm) const
    {
      RavlAssertMsg(0,"not supported");
      return false;
    }

    static unsigned g_internalConnectCount = 0;
    static RavlN::MutexC g_accessinternalConnectCount;

    //! Do some initial setup
    void ReactorC::Init()
    {
      m_wakeup = new ZmqN::SocketC(*m_zmqContext,ZST_PAIR);
      m_wakeup->SetLinger(0.0);
      RavlN::StringC sktId;

      while(true) {
        RavlN::MutexLockC lock(g_accessinternalConnectCount);
        unsigned aCount = g_internalConnectCount++;
        lock.Unlock();
        sktId.form("inproc://Reactor-%d",aCount);
        try {
          m_wakeup->Bind(sktId);
          break; //
        } catch(const RavlN::ExceptionOperationFailedC &ex) {
          continue; // Try again.
        }
        break;
      }


      ZmqN::SocketC::RefT wakeupLocal = new ZmqN::SocketC(*m_zmqContext,ZST_PAIR);
      wakeupLocal->SetLinger(0.0);
      wakeupLocal->Connect(sktId);
      CallOnRead(*wakeupLocal,RavlN::TriggerPtr(CBRefT(this),&ReactorC::WakeDone,wakeupLocal));

      m_wakeMsg = new MessageC("");
    }

    //! Sugar to make it easier to setup from a factory.
    SocketDispatcherC::RefT ReactorC::CallOnRead(const XMLFactoryContextC &factory,const std::string &name,const TriggerC &trigger,SocketC::RefT &skt)
    {
      if(!factory.UseComponent(name,skt,false,typeid(ZmqN::SocketC)))
        return 0;
      return CallOnRead(*skt,trigger);
    }

    //! Sugar to make it easier to setup from a factory.
    SocketDispatcherC::RefT ReactorC::CallOnWrite(const XMLFactoryContextC &factory,const std::string &name,const TriggerC &trigger,SocketC::RefT &skt)
    {
      if(!factory.UseComponent(name,skt,false,typeid(ZmqN::SocketC)))
        return 0;
      return CallOnWrite(*skt,trigger);
    }


    //! Add a read trigger
    SocketDispatcherC::RefT ReactorC::CallOnRead(const SocketC &socket,const TriggerC &trigger)
    {
      RavlAssert(&socket != 0);
      SocketDispatcherC::RefT ret = new SocketDispatchTriggerC(socket,true,false,trigger);
      Add(*ret);
      return ret;
    }

    //! Add a write trigger
    SocketDispatcherC::RefT ReactorC::CallOnWrite(const SocketC &socket,const TriggerC &trigger)
    {
      RavlAssert(&socket != 0);
      SocketDispatcherC::RefT ret = new SocketDispatchTriggerC(socket,false,true,trigger);
      Add(*ret);
      return ret;
    }

    //! Add a read trigger
    SocketDispatcherC::RefT ReactorC::CallOnReadOrError(int fd,const TriggerC &trigger)
    {
      RavlAssert(fd >= 0);
      SocketDispatcherC::RefT ret = new SocketDispatchTriggerC(fd,true,false,true,trigger);
      Add(*ret);
      return ret;
    }

    //! Add a read trigger
    SocketDispatcherC::RefT ReactorC::CallOnError(int fd,const TriggerC &trigger)
    {
      SocketDispatcherC::RefT ret = new SocketDispatchTriggerC(fd,false,false,true,trigger);
      Add(*ret);
      return ret;
    }


    //! Add handler to system
    bool ReactorC::Add(const SocketDispatcherC &handler)
    {
      m_sockets.push_back(&handler);
      m_pollListChanged = true;
      return true;
    }

    //! Remove handler from system
    bool ReactorC::Remove(const SocketDispatcherC &handler)
    {
      for(unsigned i = 0;i < m_sockets.size();i++) {
        if(m_sockets[i] == &handler) {
          if(i < (m_sockets.size()-1))
            m_sockets[i] = m_sockets.back();
          m_sockets.pop_back();
          m_pollListChanged = true;
          return true;
        }
      }
      RavlAssertMsg(0,"Asked to remove unknown handler.");
      return false;
    }

    //! Run reactor loop.
    bool ReactorC::Run() {
      std::vector<zmq_pollitem_t> pollArr;


      // We keep an array of socket dispatchers we're using, 'inUse'
      // so that if a socket handler delete's itself from the reactor
      // it will be kept at least until the end of the poll cycle as
      // we don't want to delete a class we're calling.

      if(m_verbose) {
        RavlDebug("Starting reactor '%s' ",Name().data());
      }
      OnStart();

      std::vector<SocketDispatcherC::RefT> inUse;
      m_pollListChanged = true; // Make sure its refreshed!

      pollArr.reserve(m_sockets.size());
      zmq_pollitem_t *first = 0;
      std::vector<SocketDispatcherC::RefT> toGo;
      toGo.reserve(4);
      while(!m_terminate) {
        if(m_pollListChanged) {
          pollArr.clear();
          inUse.clear();
          pollArr.reserve(m_sockets.size());
          inUse.reserve(m_sockets.size());
          zmq_pollitem_t item;
          for(unsigned i = 0;i < m_sockets.size();i++) {
            if(!m_sockets[i]->IsActive()) {
              // Put on list to deal with when we're finished.
              toGo.push_back(m_sockets[i]);
              continue;
            }

            // Just for paranoia zero the structure.
            memset(&item,0,sizeof(zmq_pollitem_t));
            if(m_sockets[i]->SetupPoll(item)) {
              inUse.push_back(m_sockets[i]);
              pollArr.push_back(item);
            }
          }

          if(pollArr.size() > 0)
            first = &pollArr[0];
          else
            first = 0;

          // Clear out old links.
          for(unsigned i = 0;i < toGo.size();i++) {
            toGo[i]->Clear();
            Remove(*toGo[i]);
          }
          toGo.clear();

          m_pollListChanged = false;
        }

        long timeout = -1;

        double timeToNext = m_timedQueue.ProcessStep();

        if(timeToNext < 0 || (timeToNext > m_teminateCheckInterval && m_teminateCheckInterval > 0))
          timeToNext = m_teminateCheckInterval;

        if(m_teminateCheckInterval >= 0)
          timeout = Round(timeToNext * 1000.0);

        if(m_verbose) {
          RavlDebug("Reactor '%s' polling for %u sockets. (Timeout:%u, %f seconds )",Name().data(),(unsigned) pollArr.size(),timeout,timeToNext);
        }
        // Process all pending events.
        int pollCount = 0;
        do {
          pollCount++;
          int ret = zmq_poll (first, pollArr.size(),timeout);
          if(m_verbose) {
            RavlDebug("Reactor '%s' got ready for %d sockets. (Timeout:%u, %f seconds ) ",Name().data(),ret,timeout,timeToNext);
          }
          if(ret == 0) {
            // no more events to process.
            break;
          }
          timeout = 0; // Spin around processing pending events until they're gone.
          if(ret < 0) {
            int anErrno = zmq_errno ();
            // Shutting down ?
            if(anErrno == ETERM) {
              if(m_verbose) {
                RavlDebug("Reactor '%s' context shutdown.",Name().data());
              }
              break;
            }
            if(anErrno == EINTR) {
              if(m_verbose) {
                RavlDebug("Reactor '%s' Got interrupted.",Name().data());
              }
              continue;
            }
            RavlError("Reactor '%s' poll failed : %s ",Name().data(),zmq_strerror (anErrno));
            RavlAssertMsg(0,"unexpected error");
            continue;
          }
          unsigned i = 0;
          while(i < pollArr.size() && ret > 0) {
            // Avoid repeatedly setting up try/catch as it can be expensive.
            try {
              for(;i < pollArr.size() && ret > 0;i++) {
                if(pollArr[i].revents != 0) {
                  if(!inUse[i]->CheckDispatch(pollArr[i].revents)) {
                    m_pollListChanged = true; // Refresh list.
                  }
                  ret--;
                }
              }
            } catch(std::exception &ex) {
              RavlError("Caught c++ exception %s : %s ",RavlN::TypeName(typeid(ex)),ex.what());
              RavlAssert(0);
              i++;
            } catch(RavlN::ExceptionC &ex) {
              RavlError("Caught Ravl exception %s : %s ",RavlN::TypeName(typeid(ex)),ex.what());
              ex.Dump(std::cerr);
              RavlAssert(0);
              i++;
            } catch(...) {
              // FIXME: Be more informative!
              RavlError("Caught unknown exception dispatching message. ");
              RavlAssert(0);
              i++; // Skip it an go to next.
            }
          }
          RavlAssertMsg(ret == 0,"Poll event count doesn't match events found!");
        } while(!m_pollListChanged && !m_terminate && pollCount < 100) ;
      }

      for(unsigned i = 0;i < m_sockets.size();i++)
        m_sockets[i]->Clear();
      m_sockets.clear();
      OnFinish();

      if(m_verbose) {
        RavlDebug("Shutdown of reactor '%s' complete.",Name().data());
      }
      return true;
    }

    //! Shutdown service
    bool ReactorC::Shutdown()
    {
      ServiceThreadC::Shutdown();
      RavlN::MutexLockC lock(m_accessWakeup);
      RavlAssert(m_wakeup.IsValid());
      if(!m_wakeScheduled && m_wakeup.IsValid()) {
        m_wakeScheduled = true;
        m_wakeup->Send(*m_wakeMsg,ZSB_NOBLOCK);
      }
      lock.Unlock();
      return true;
    }

    //! Called by the main loop when its first run.
    bool ReactorC::OnStart()
    {

      return true;
    }

    //! Called when loop is exiting.
    bool ReactorC::OnFinish()
    {

      return true;
    }

    //! Schedule event for running on the reactor thread
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.
    UIntT ReactorC::Schedule(const TriggerC &se)
    {
      return SendWakeForTimeQueue(m_timedQueue.Schedule(0,se));
    }

    //! Schedule event for running on the reactor thread when its idle
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it. The returned ID never has the value 0, so it may be used to flag not set.
    UIntT ReactorC::ScheduleIdle(const TriggerC &se)
    {
      return SendWakeForTimeQueue(m_timedQueue.ScheduleIdle(se));
    }

    //! Schedule event for running after time 't' (in seconds).
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.
    UIntT ReactorC::Schedule(RealT t,const TriggerC &se,float period)
    {
      return SendWakeForTimeQueue(m_timedQueue.Schedule(t,se,period));
    }

    //! Schedule event for running.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.
    UIntT ReactorC::Schedule(const DateC &at,const TriggerC &se,float period)
    {
      return SendWakeForTimeQueue(m_timedQueue.Schedule(at,se,period));
    }

    //! Schedule event for running periodically.
    // Thread safe.
    // Returns an ID for the event, which can
    // be used for cancelling it.
    UIntT ReactorC::SchedulePeriodic(const TriggerC &se,float period)
    {
      return SendWakeForTimeQueue(m_timedQueue.SchedulePeriodic(se,period));
    }

    //! Called when wake up has been done
    bool ReactorC::WakeDone(ZmqN::SocketC::RefT &skt)
    {
      RavlN::MutexLockC lock(m_accessWakeup);
      m_wakeScheduled = false;
      MessageC::RefT msg;
      skt->Recieve(msg);
      return true;
    }

    //! Send wake up event for timed event queue.
    UIntT ReactorC::SendWakeForTimeQueue(UIntT eventId)
    {
      RavlN::MutexLockC lock(m_accessWakeup);
      if(!m_wakeScheduled) {
        m_wakeScheduled = true;
        m_wakeup->Send(*m_wakeMsg,ZSB_NOBLOCK);
      }
      return eventId;
    }

    //! Change period of event.
    // Returns true if event is found and has been updated.
    // This will take effect after the event is next run.
    bool ReactorC::ChangePeriod(UIntT eventId, float period)
    {
      return m_timedQueue.ChangePeriod(eventId,period);
    }

    //! Cancel pending event.
    // Will return TRUE if event in cancelled before
    // it was run.
    bool ReactorC::Cancel(UIntT eventID)
    {
      return m_timedQueue.Cancel(eventID);
    }

    //! Called when owner handles drop to zero.
    void ReactorC::ZeroOwners() {
      ServiceThreadC::ZeroOwners();
    }

    ServiceThreadC::RefT Reactor2ServiceThread(const ReactorC::RefT &reactor)
    { return reactor.BodyPtr(); }

    DP_REGISTER_CONVERSION(Reactor2ServiceThread,1.0);

    static XMLFactoryRegisterConvertC<ReactorC,ServiceThreadC> g_regiserBreeder("RavlN::ZmqN::ReactorC");

  }
}
