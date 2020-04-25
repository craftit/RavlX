// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQN_SOCKETDISPATCHTRIGGER_HEADER
#define RAVL_ZMQN_SOCKETDISPATCHTRIGGER_HEADER 1
//! lib=RavlZmq

#include "Ravl/Zmq/SocketDispatcher.hh"
#include "Ravl/Trigger.hh"

namespace RavlN {
  namespace ZmqN {

    //! Handle events from a socket.

    class SocketDispatchTriggerC
     : public SocketDispatcherC
    {
    public:
      //! Construct from a socket.
      SocketDispatchTriggerC(const SocketC &socket,bool readReady,bool writeReady,const TriggerC &trigger);

      //! Construct from a socket.
      SocketDispatchTriggerC(int fd,bool readReady,bool writeReady,bool onError,const TriggerC &trigger);

      //! Factory constructor
      SocketDispatchTriggerC(const XMLFactoryContextC &factory);

      //! Handle event.
      virtual void Dispatch(int events);

      //! Stop handling of events.
      virtual void Stop();

      //! Is ready
      virtual bool IsActive() const;

      //! Pointer to class
      typedef SmartPtrC<SocketDispatchTriggerC> RefT;

    protected:
      TriggerC m_trigger;
    };


  }
}

#endif
