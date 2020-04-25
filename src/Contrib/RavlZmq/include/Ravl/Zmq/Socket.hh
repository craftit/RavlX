// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQ_SOCKET_HEADER
#define RAVL_ZMQ_SOCKET_HEADER 1
//! lib=RavlZmq

#include "Ravl/Zmq/Message.hh"
#include "Ravl/DP/MemIO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/OS/SysLog.hh"
#include <zmq.h>

namespace RavlN {
  namespace ZmqN {
    class ContextC;

    enum BlockT {
      ZSB_BLOCK,
      ZSB_NOBLOCK
    };

    enum SocketTypeT {
      ZST_PAIR = ZMQ_PAIR,
      ZST_PUBLISH = ZMQ_PUB,
      ZST_SUBCRIBE = ZMQ_SUB,
      ZST_REQUEST = ZMQ_REQ,
      ZST_REPLY = ZMQ_REP,
      ZST_XREQUEST = ZMQ_XREQ,
      ZST_XREPLY = ZMQ_XREP,
      ZST_PUSH = ZMQ_PUSH,
      ZST_PULL = ZMQ_PULL,
      ZST_XPUBLISH = ZMQ_XPUB,
      ZST_XSUBCRIBE = ZMQ_XSUB,
      ZST_ROUTER = ZMQ_ROUTER,
      ZST_DEALER = ZMQ_DEALER
    };

    //! Write to text stream.
    std::ostream &operator<<(std::ostream &strm,SocketTypeT sockType);

    //! Read from text stream
    std::istream &operator>>(std::istream &strm,SocketTypeT &sockType);

    //! Convert a socket type from a name to an enum
    SocketTypeT SocketType(const std::string &name);

    //! Socket

    class SocketC
      : public RCBodyC
    {
    public:

      //! Default constructor - creates invalid handle
      SocketC();

      //! Construct a new socket.
      SocketC(ContextC &context,SocketTypeT socketType,const StringC &codec = "");

      //! Factory constructor
      SocketC(const XMLFactoryContextC &context);

      //! Destructor
      ~SocketC();

      //! Set name for socket, used in debugging
      void SetName(const RavlN::StringC &name);

      //! Access socket name
      const StringC &Name() const
      { return m_name; }

      //! Write to an ostream
      bool Save(std::ostream &strm) const;

      //! Write to a binary stream
      // Not implemented
      bool Save(BinOStreamC &strm) const;

      //! Bind to an address
      void Bind(const std::string &addr);

      //! Bind to first available port in the range.
      //! Assigned the address bound to 'addr'.
      //! Returns true if a port is successfully found and false if no free port is found.
      bool BindDynamicTCP(const std::string &devName,std::string &addr,int minPort,int maxPort);

      //! Access last bound address.
      const std::string &BoundAddress() const;

      //! Access address that can be used to connect to the last bound address.
      RavlN::StringC ConnectBoundAddress() const;

      //! Connect to an address.
      void Connect(const std::string &addr);

      //! Subscribe to a topic
      void Subscribe(const std::string &topic);

      //! Unsubscribe from a topic
      bool Unsubscribe(const std::string &topic);

      //! Set the identity of the socket.
      void SetIdentity(const std::string &identity);

      //! Set linger time for socket.
      void SetLinger(float timeSeconds);

      //! Set the high water mark.
      void SetHighWaterMark(UInt64T number);

      //! Set mandatory flag for router.
      void SetRouterMandatory(bool enable);

      //! Set send timeout.  -ve values will cause it to block forever.
      void SetSendTimeout(float seconds);

      //! Set receive timeout.  -ve values will cause it to block forever.
      void SetReceiveTimeout(float seconds);

      //! Send a message
      bool Send(const MessageC &msg,BlockT block = ZSB_BLOCK,int *errValue = 0);

      //! Send a message
      bool Send(const MessageC::RefT &msg,BlockT block = ZSB_BLOCK,int *errValue = 0);

      //! Receive a message.
      bool Recieve(MessageC::RefT &msg,BlockT block = ZSB_BLOCK);

      //! Send a message
      bool Send(const SArray1dC<char> &msg,BlockT block = ZSB_BLOCK,int *errValue = 0);

      //! Receive a message.
      //! Obsolete.
      bool Recieve(SArray1dC<char> &msg,BlockT block = ZSB_BLOCK);

      //! Receive a message.
      bool Receive(SArray1dC<char> &msg,BlockT block = ZSB_BLOCK);

      //! Send a arbitrary class
      template<typename DataT>
      bool Send(const DataT &value,BlockT block = ZSB_BLOCK) {
        SArray1dC<char> data;
        if(!MemSave(data,value,m_defaultCodec,m_verbose)) {
          RavlError("Failed to encode message using codec %s from type '%s' ",m_defaultCodec.data(),RavlN::TypeName(typeid(DataT)));
          RavlAssert(0);
          return false;
        }
        return Send(data,block);
      }

      //! Send a arbitrary class
      template<typename DataT>
      bool Recieve(DataT &value,BlockT block = ZSB_BLOCK) {
        SArray1dC<char> data;
        if(!Recieve(data,block))
          return false;
        if(!MemLoad(data,value,m_defaultCodec,m_verbose)) {
          RavlError("Failed to decode message ");
          RavlAssert(0);
          return false;
        }
        return true;
      }

      //! Access default codec for socket
      const StringC &DefaultCodec() const
      { return m_defaultCodec; }

      //! Access the raw socket.
      void *RawSocket()
      { return m_socket; }

      //! Access the socket type
      SocketTypeT LocalSocketType() const
      { return m_socketType; }

      //! Handle to socket.
      typedef SmartPtrC<SocketC> RefT;
    protected:

      RavlN::StringC m_name;
      void *m_socket;
      StringC m_defaultCodec;
      bool m_verbose;
      std::string m_boundAddress;
      SocketTypeT m_socketType;
    };
  }
}

#endif
