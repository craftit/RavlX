// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/SocketDispatchTrigger.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace ZmqN {

    //! Construct from a socket.
    SocketDispatchTriggerC::SocketDispatchTriggerC(const SocketC &socket,bool readReady,bool writeReady,const TriggerC &trigger)
     : SocketDispatcherC(socket,readReady,writeReady),
       m_trigger(trigger)
    {
    }

    //! Construct from a socket.
    SocketDispatchTriggerC::SocketDispatchTriggerC(int fd,bool readReady,bool writeReady,bool onError,const TriggerC &trigger)
     : SocketDispatcherC(fd,readReady,writeReady,onError),
       m_trigger(trigger)
    {

    }

    //! Factory constructor
    SocketDispatchTriggerC::SocketDispatchTriggerC(const XMLFactoryContextC &factory)
     : SocketDispatcherC(factory)
    {
    }

    //! Handle event.
    void SocketDispatchTriggerC::Dispatch(int events)
    {
      if(!m_trigger.IsValid()) {
        ONDEBUG(RavlDebug("No trigger!"));
        return ;
      }
      m_trigger();
    }

    //! Stop handling of events.
    void SocketDispatchTriggerC::Stop() {
      m_trigger.Invalidate();
      SocketDispatcherC::Stop();
    }

    //! Is ready
    bool SocketDispatchTriggerC::IsActive() const
    {
      return m_trigger.IsValid();
    }


  }
}
