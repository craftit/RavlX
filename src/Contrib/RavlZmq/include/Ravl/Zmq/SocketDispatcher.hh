// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQN_SOCKETDISPATCHER_HEADER
#define RAVL_ZMQN_SOCKETDISPATCHER_HEADER 1
//! lib=RavlZmq

#include "Ravl/Zmq/Socket.hh"
#include "Ravl/Collection.hh"
#include <zmq.h>

namespace RavlN {
  namespace ZmqN {

    //! Handle events from a socket.

    class SocketDispatcherC
     : public RCBodyVC
    {
    public:
      //! Construct from a socket.
      SocketDispatcherC(const SocketC &socket,bool readReady,bool writeReady);

      //! Construct from a file descriptor
      SocketDispatcherC(int fd,bool readReady,bool writeReady,bool onError);

      //! Factory constructor
      SocketDispatcherC(const XMLFactoryContextC &factory);

      //! Handle event.
      bool CheckDispatch(int events);

      //! Handle event.
      virtual void Dispatch(int events);

      //! Stop handling of events.
      virtual void Stop();

      //! Setup poll item,
      // Return false if socket should be ignored.
      virtual bool SetupPoll(zmq_pollitem_t &pollItem);

      //! Is ready
      virtual bool IsActive() const;

      //! Clear handles
      virtual bool Clear();

      //! Pointer to class
      typedef SmartPtrC<SocketDispatcherC> RefT;

    protected:
      SocketC::RefT m_socket;
      int m_fd;
      bool m_onReadReady;
      bool m_onWriteReady;
      bool m_onError;
      bool m_isStopped;
    };

    //! Set of dispatchers to disconnect when finished with.

    class SocketDispatchSetC
     : public RavlN::CollectionC<SocketDispatcherC::RefT>
    {
    public:
      //! Default constructor
      SocketDispatchSetC();

      //! Destructors
      ~SocketDispatchSetC();

      //! Disconnect all dispatchers.
      void DisconnectAll();

      //! Add dispatcher
      bool operator+=(const SocketDispatcherC::RefT &dispatcher)
      {
        if(dispatcher.IsValid()) {
          (*this).Append(dispatcher);
          return true;
        }
        return false;
      }

    protected:

    };
  }
}

#endif
