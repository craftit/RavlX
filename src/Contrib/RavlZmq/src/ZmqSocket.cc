// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/Socket.hh"
#include "Ravl/Zmq/Context.hh"
#include "Ravl/Zmq/MsgBuffer.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/Exception.hh"
#include "Ravl/XMLFactoryRegister.hh"

#if RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_MACOSX
#include <unistd.h>
#endif
#include <string.h>

#if RAVL_OS_MACOSX
// FIXME: There is probably a define I can use to get this from the header, but I haven't worked out which one yet...
extern "C" {
  int  gethostname(char *, size_t);
}
#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace ZmqN {

    static const struct {
      const char *m_name;
      SocketTypeT m_type;
    } g_socketTypeNames[] =
    {
        {"PAIR",ZST_PAIR    },
        {"PUB" ,ZST_PUBLISH   },
        {"SUB" ,ZST_SUBCRIBE  },
        {"REQ" ,ZST_REQUEST   },
        {"REP" ,ZST_REPLY     },
        {"XREQ",ZST_XREQUEST  },
        {"XREP",ZST_XREPLY    },
        {"PULL",ZST_PULL      },
        {"PUSH",ZST_PUSH      },
        {"XPUB",ZST_XPUBLISH  },
        {"XSUB",ZST_XSUBCRIBE },
        {"ROUTER",ZST_ROUTER  },
        {"DEALER",ZST_DEALER },
        {0,ZST_PAIR }
    };

    //! Write to text stream.
    std::ostream &operator<<(std::ostream &strm,SocketTypeT sockType)
    {
      for(unsigned i = 0;g_socketTypeNames[i].m_name != 0;i++) {
        if(sockType == g_socketTypeNames[i].m_type) {
          strm << g_socketTypeNames[i].m_name;
          return strm;
        }
      }
      RavlError("Unknown socket type '%u' ",(unsigned) sockType);
      throw ExceptionC("Unknown socket type");
      return strm;
    }

    //! Convert a socket type from a name to an enum
    SocketTypeT SocketType(const std::string &name) {
      int i = 0;
      for(;g_socketTypeNames[i].m_name != 0;i++) {
        if(name == g_socketTypeNames[i].m_name) {
          return g_socketTypeNames[i].m_type;
        }
      }
      RavlError("Unknown socket type '%s' ",name.c_str());
      throw ExceptionC("Unknown socket type");
    }

    //! Read from text stream
    std::istream &operator>>(std::istream &strm,SocketTypeT &sockType)
    {
      std::string socketTypeName;
      strm >> socketTypeName;
      sockType = SocketType(socketTypeName);
      return strm;
    }


    //! Default constructor - creates invalid handle
    SocketC::SocketC()
     : m_socket(0),
       m_verbose(false),
       m_socketType(ZST_PAIR)
    {};

    //! Construct a new socket.
    SocketC::SocketC(ContextC &context,SocketTypeT socketType,const StringC &codec)
     : m_socket(0),
       m_defaultCodec(codec),
       m_verbose(false),
       m_socketType(socketType)
    {
      m_socket = zmq_socket(context.RawContext(),(int) socketType);
      if(m_socket == 0) {
        RavlError("Failed to create socket: %s ",zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("Failed to create socket. ");
      }
      SetLinger(2.0);
    }


    //! Factory constructor
    SocketC::SocketC(const XMLFactoryContextC &context)
     : m_name(context.Path()),
       m_socket(0),
       m_defaultCodec(context.AttributeString("defaultCodec","")),
       m_verbose(context.AttributeBool("verbose",false))
    {
      ContextC::RefT ctxt;
      if(!context.UseComponent("ZmqContext",ctxt,false,typeid(ZmqN::ContextC))) {
        RavlError("No context for socket at %s ",context.Path().c_str());
        throw ExceptionOperationFailedC("No context. ");
      }
      m_socketType = SocketType(context.AttributeString("socketType",""));
      m_socket = zmq_socket(ctxt->RawContext(),(int) m_socketType);
      if(m_socket == 0) {
        RavlError("Failed to create socket '%s' in %s ",zmq_strerror (zmq_errno ()),context.Path().c_str());
        throw ExceptionOperationFailedC("Failed to create socket. ");
      }
      // Setup a default linger period.
      SetLinger(context.AttributeReal("linger",2.0));

      // Bind if required.
      StringC defaultBind = context.AttributeString("bind","");
      if(!defaultBind.IsEmpty()) {
        Bind(defaultBind);
      }


      // Connect if required.
      StringC defaultConnect = context.AttributeString("connect","");
      if(!defaultConnect.IsEmpty()) {
        Connect(defaultConnect);
      }

      // Setup properties
      {
        XMLFactoryContextC childContext;
        if(context.ChildContext("Properties",childContext))
        for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
          if(it->Name() == "Subscribe") {
            StringC sub = it->AttributeString("value","");
            if(m_verbose)
              RavlDebug("Subscribing to '%s' ",sub.c_str());
            Subscribe(sub);
            continue;
          }
          if(it->Name() == "Bind") {
            StringC addr = it->AttributeString("value","");
            if(m_verbose)
              RavlDebug("Binding to '%s' ",addr.c_str());
            Bind(addr.c_str());
            continue;
          }
          if(it->Name() == "AutoBind") {
            int minPort = it->AttributeUInt("minPort",0);
            int maxPort = it->AttributeUInt("maxPort",0);
            bool mustBind = it->AttributeBool("mustBind",false);
            std::string autoBindDev = it->AttributeString("value","");

            if(minPort == 0) {
              RavlError("minPort not specified in %s ",context.Path().c_str());
              throw ExceptionOperationFailedC("minPort not specified ");
            }
            if(maxPort == 0) {
              RavlError("maxPort not specified in %s ",context.Path().c_str());
              throw ExceptionOperationFailedC("maxPort not specified ");
            }
            std::string bindAddr;
            if(!BindDynamicTCP(autoBindDev,bindAddr,minPort,maxPort)) {
              RavlError("Failed to dynamically bind port.");
              // FIXME:- Should this be fatal ?
              if(mustBind)
                throw ExceptionOperationFailedC("Failed to find free port. ");
            }
            if(m_verbose)
              RavlDebug("Auto bound to '%s' ",bindAddr.c_str());
            continue;
          }
          if(it->Name() == "Connect") {
            StringC addr = it->AttributeString("value","");
            Connect(addr.c_str());
            if(m_verbose)
              RavlDebug("Connecting to '%s' ",addr.c_str());
            continue;
          }
          if(it->Name() == "Identity") {
            StringC addr = it->AttributeString("value","");
            SetIdentity(addr.c_str());
            if(m_verbose)
              RavlDebug("Setting identity to '%s' ",addr.c_str());
            continue;
          }
          if(it->Name() == "Linger") {
            SetLinger(it->AttributeReal("value",-1.0));
            continue;
          }
          if(it->Name() == "HighWaterMark") {
            SetHighWaterMark(it->AttributeUInt("value",0));
            continue;
          }
          RavlError("Unknown socket property '%s' at %s ",it->Name().c_str(),childContext.Path().c_str());
          throw ExceptionOperationFailedC("Unknown property. ");
        }
      }
    }

    //! Destructor
    SocketC::~SocketC()
    {
      if(m_socket != 0)
        zmq_close (m_socket);
      ONDEBUG(RavlDebug("Socket %s destructing.",Name().c_str()));
    }

    //! Write to an ostream
    bool SocketC::Save(std::ostream &strm) const
    {
      RavlAssertMsg(0,"not implemented");
      return false;
    }

    //! Write to a binary stream
    // Not implemented
    bool SocketC::Save(BinOStreamC &strm) const
    {
      RavlAssertMsg(0,"not supported");
      return false;
    }

    //! Set name for socket, used in debugging
    void SocketC::SetName(const RavlN::StringC &name)
    { m_name = name; }

    //! Access last bound address.
    const std::string &SocketC::BoundAddress() const
    {
      return m_boundAddress;
    }

    //! Access address that can be used to connect to the last bound address.
    RavlN::StringC SocketC::ConnectBoundAddress() const
    {
      StringC boundAddress = BoundAddress();
      char hostnameBuffer[257];
      StringC theHost;
      if(gethostname(hostnameBuffer,255) != 0) {
        RavlError("Failed to get hostname, using 'localhost' ");
        strcpy(hostnameBuffer,"localhost");
        theHost = StringC(hostnameBuffer);
      } else {
        StringC ahost(hostnameBuffer);
        // Stick with fully qualified name.
#if 0
        if(ahost.contains('.'))
          theHost = ahost.before('.');
        else
          theHost = ahost;
#else
        // Use the full host name
        theHost = ahost;
#endif
      }

      // FIXME:- Should check for real local interface names ?
      static const char *g_substNames[] = {
          "*",
          "eth0",
          "eth1",
          "eth2",
          "eth3",
          "eth4",
          "eth5",
          0
      };
      for(unsigned i = 0;g_substNames[i] != 0;i++) {
        if(boundAddress.contains(g_substNames[i]) == 1) {
          boundAddress.gsub(g_substNames[i],theHost);
          break;
        }
      }
      return boundAddress;
    }


    //! Bind to an address
    void SocketC::Bind(const std::string &addr)
    {
      if(m_name.IsEmpty())
        m_name = StringC("Bind:") + addr.c_str();
      RavlAssert(m_socket != 0);
      int ret;
      if((ret = zmq_bind (m_socket, addr.c_str())) != 0) {
        RavlError("Failed to bind to %s : %s ",addr.c_str(),zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("bind failed. ");
      }
      m_boundAddress = addr;
    }

    //! Bind to first available port in the range.
    bool SocketC::BindDynamicTCP(const std::string &devName,std::string &addr,int minPort,int maxPort)
    {
      StringC rootName = "tcp://" + devName + ":";
      RavlAssert(minPort > 0);
      RavlAssert(maxPort >= minPort);

      int ret;
      int numPorts = (maxPort - minPort)+1;
      // Start at a random port in the range.
      int at = Floor(Random1() * numPorts) + minPort;
      for(int i = 0;i < numPorts;i++,at++) {
        if(at > maxPort)
          at = minPort;
        StringC newAddr = rootName + StringC(at);
        if(m_verbose) {
          RavlInfo("Trying to connect to '%s'", newAddr.data());
        }
        ret = zmq_bind (m_socket, newAddr.c_str());
        if(ret != 0 && errno == EADDRINUSE) {
          if(m_verbose) {
            RavlDebug("Address '%s' in use, trying the next.", newAddr.data());
          }
          continue;
        } else {
          addr = newAddr.c_str();
          m_boundAddress = newAddr.c_str();
          if(m_verbose) {
            RavlInfo("Socket '%s' bound to address '%s' ",m_name.c_str(),newAddr.c_str());
          }
          return true;
        }
        // If we get here we got an error that was not an EADDRINUSE!
        RavlError("Failed to bind to %s : %s ",newAddr.c_str(),zmq_strerror (errno));
        break;
      }
      RavlInfo("Failed to dynamically bind TCP!");
      return false;
    }

    //! Connect to an address.
    void SocketC::Connect(const std::string &addr)
    {
      if(m_name.IsEmpty())
        m_name = StringC("Connect:") +addr.c_str();
      RavlAssert(m_socket != 0);
      int ret;
      if((ret = zmq_connect(m_socket, addr.c_str())) != 0) {
        RavlError("Failed to connect to %s (Type:%s): %s ",
            addr.c_str(),
            RavlN::StringOf(m_socketType).c_str(),
            zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("connect failed. ");
      }
    }

    //! Set linger time for socket.
    void SocketC::SetLinger(float timeSeconds)
    {
      RavlAssert(m_socket != 0);
      int lingerTime = 0;
      if(timeSeconds >= 0) {
        lingerTime = Round(timeSeconds) * 1000.0;
      } else {
        lingerTime = -1;
      }
      int ret;
      if((ret = zmq_setsockopt (m_socket,ZMQ_LINGER,&lingerTime,sizeof(lingerTime))) != 0) {
        RavlError("Failed to set linger to %f : %s ",timeSeconds,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("connect failed. ");
      }
    }

    //! Set the high water mark.
    void SocketC::SetHighWaterMark(UInt64T queueLimit) {
      RavlAssert(m_socket != 0);
      int ret;
#ifdef ZMQ_HWM
      if((ret = zmq_setsockopt (m_socket,ZMQ_HWM,&queueLimit,sizeof(queueLimit))) != 0) {
        RavlError("Failed to set high water mark to %u : %s ",(UIntT) queueLimit,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("connect failed. ");
      }
#else
      if((ret = zmq_setsockopt (m_socket,ZMQ_SNDHWM,&queueLimit,sizeof(queueLimit))) != 0) {
        RavlError("Failed to set high water mark to %u : %s ",(UIntT) queueLimit,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("connect failed. ");
      }
      if((ret = zmq_setsockopt (m_socket,ZMQ_RCVHWM,&queueLimit,sizeof(queueLimit))) != 0) {
        RavlError("Failed to set high water mark to %u : %s ",(UIntT) queueLimit,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("request failed. ");
      }
#endif
    }

    //! Set mandatory flag for router.
    void SocketC::SetRouterMandatory(bool enable)
    {
#ifdef ZMQ_ROUTER_MANDATORY
      int ret = 0;
      int value = enable;
      if((ret = zmq_setsockopt (m_socket,ZMQ_ROUTER_MANDATORY,&value,sizeof(value))) != 0) {
        RavlError("Failed to set mandatory on router to %d, ret=%d ",value,ret);
        throw ExceptionOperationFailedC("connect failed. ");
      }
#else
      RavlError("Failed to set mandatory on router, option not supported ");
      throw ExceptionOperationFailedC("request failed. ");
#endif
    }


    //! Subscribe to a topic
    void SocketC::Subscribe(const std::string &topic)
    {
      RavlAssert(m_socket != 0);
      RavlDebug("Subscribing '%s' to '%s' ",m_name.c_str(),topic.c_str());
      int ret = zmq_setsockopt (m_socket,ZMQ_SUBSCRIBE,topic.c_str(),topic.size());
      if(ret != 0) {
        RavlError("Failed to subscribe to %s : %s ",topic.c_str(),zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("Subscribe failed. ");
      }
    }

    //! Unsubscribe from a topic
    bool SocketC::Unsubscribe(const std::string &topic)
    {
      RavlAssert(m_socket != 0);
      RavlDebug("Unsubscribing '%s' from %s ",m_name.c_str(),topic.c_str());
      int ret = zmq_setsockopt (m_socket,ZMQ_UNSUBSCRIBE,topic.c_str(),topic.size());
      if(ret != 0) {
        RavlError("Failed to unsubscribe to %s : %s ",topic.c_str(),zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("Unsubscribe failed. ");
      }
      return true;
    }

    //! Set the identity of the socket.
    void SocketC::SetIdentity(const std::string &identity) {
      RavlAssert(m_socket != 0);
      RavlDebug("Setting identity to %s ",identity.c_str());
      int ret = zmq_setsockopt (m_socket,ZMQ_IDENTITY,identity.c_str(),identity.size());
      if(ret != 0) {
        RavlError("Failed to set identity to %s : %s ",identity.c_str(),zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("SetIdentity failed. ");
      }
    }

    //! Set send timeout.  -ve values will cause it to block forever.
    void SocketC::SetSendTimeout(float seconds)
    {
      int timeOut = seconds * 1000.0;
      int ret = zmq_setsockopt (m_socket,ZMQ_SNDTIMEO,&timeOut,sizeof(timeOut));
      if(ret != 0) {
        RavlError("Failed to set send timeout to %f : %s ",seconds,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("SetSendTimeout failed. ");
      }
    }

    //! Set receive timeout.  -ve values will cause it to block forever.
    void SocketC::SetReceiveTimeout(float seconds)
    {
      int timeOut = seconds * 1000.0;
      int ret = zmq_setsockopt (m_socket,ZMQ_RCVTIMEO,&timeOut,sizeof(timeOut));
      if(ret != 0) {
        RavlError("Failed to set receive timeout to %f : %s ",seconds,zmq_strerror (zmq_errno ()));
        throw ExceptionOperationFailedC("SetReceiveTimeout failed. ");
      }
    }


    //! Send a message
    bool SocketC::Send(const SArray1dC<char> &msg,BlockT block,int *errValue) {
      zmq_msg_t zmsg;
      if(m_verbose) {
        StringC tmp(msg.ReferenceElm(),msg.Size(),msg.Size());
        RavlDebug("Send %s:'%s'",m_name.c_str(),tmp.c_str());
      }
      ArrayToMessage(zmsg,msg);
      int ret;
      int flags = 0;
#if ZMQ_VERSION_MAJOR >= 3
      if(block == ZSB_NOBLOCK)
        flags = ZMQ_DONTWAIT;
      if((ret = zmq_sendmsg (m_socket, &zmsg, flags)) < 0)
#else
      if(block == ZSB_NOBLOCK)
        flags = ZMQ_NOBLOCK;
      if((ret = zmq_send (m_socket, &zmsg, flags)) != 0)
#endif
      {
        int anErrno = zmq_errno();
        if(errValue != 0)
          *errValue = anErrno;
        if(block == ZSB_NOBLOCK && (anErrno == EAGAIN || anErrno == EINTR))
          return false;
        RavlError("Send failed : %s   flags=%x errno=%d ",zmq_strerror (anErrno),flags,anErrno);
        zmq_msg_close(&zmsg);
#if 0
        throw ExceptionOperationFailedC("Send failed. ");
#else
        return false;
#endif
      }
      zmq_msg_close(&zmsg);
      return true;
    }

    //! Receive a message.
    bool SocketC::Recieve(SArray1dC<char> &msg,BlockT block)
    {
      return Receive(msg,block);
    }

    //! Receive a message.
    bool SocketC::Receive(SArray1dC<char> &msg,BlockT block)
    {
      int64_t more = 0;
      int ret;
      int flags = 0;
      static size_t more_size = sizeof (more);
      MsgBufferC msgBuffer(0);

#if ZMQ_VERSION_MAJOR >= 3
      if((ret = zmq_recvmsg (m_socket, msgBuffer.Msg(), flags)) < 0)
#else
      if((ret = zmq_recv (m_socket, msgBuffer.Msg(), flags)) != 0)
#endif
      {
        int anErrno = zmq_errno();
        RavlError("Recv failed : %s ",zmq_strerror (anErrno));
        throw ExceptionOperationFailedC("Recv failed. ");
      }
      msgBuffer.Build();
      msg = SArray1dC<char>(msgBuffer,msgBuffer.Size());

      if((ret = zmq_getsockopt (m_socket, ZMQ_RCVMORE, &more, &more_size)) != 0) {
        int anErrno = zmq_errno();
        RavlError("RCVMORE failed : %s ",zmq_strerror (anErrno));
        throw ExceptionOperationFailedC("Recv failed. ");
      }
      while(more)  {
        zmq_msg_t zmsg;
        zmq_msg_init(&zmsg);
        RavlWarning("Discarding message part.");
#if ZMQ_VERSION_MAJOR >= 3
        if((ret = zmq_recvmsg (m_socket, msgBuffer.Msg(), flags)) < 0)
#else
        if((ret = zmq_recv (m_socket, msgBuffer.Msg(), flags)) != 0)
#endif
        {
          int anErrno = zmq_errno();
          RavlError("RCVMORE failed : %s ",zmq_strerror (anErrno));
          throw ExceptionOperationFailedC("Recv failed. ");
        }
        if((ret = zmq_msg_close(&zmsg)) != 0) {
          int anErrno = zmq_errno();
          RavlWarning("close failed : %s ",zmq_strerror (anErrno));
        }

        if((ret = zmq_getsockopt (m_socket, ZMQ_RCVMORE, &more, &more_size)) != 0) {
          int anErrno = zmq_errno();
          RavlError("RCVMORE failed : %s ",zmq_strerror (anErrno));
          throw ExceptionOperationFailedC("Recv failed. ");
        }
      }
      if(m_verbose) {
        StringC tmp(msg.ReferenceElm(),msg.Size(),msg.Size());
        RavlDebug("Received %s:'%s'",m_name.c_str(),tmp.c_str());
      }
      return true;
    }

    //! Send a message
    bool SocketC::Send(const MessageC::RefT &msg,BlockT block,int *errValue)
    {
      RavlAssert(msg.IsValid());
      if(!msg.IsValid())
        return true;
      return Send(*msg,block,errValue);
    }

    //! Send a message
    bool SocketC::Send(const MessageC &msg,BlockT block,int *errValue)
    {
      RavlAssert(m_socket != 0);
      size_t elems = msg.Parts().size();
      RavlAssert(elems > 0);
      size_t lastElem = elems -1;
      int ret;
      //RavlDebug("Sending %zu parts.",elems);
      for(size_t i = 0;i < elems;i++) {
        zmq_msg_t zmsg;
        ArrayToMessage(zmsg,msg.Parts()[i]);

#ifdef ZMQ_DONTWAIT
       int flags = block == ZSB_BLOCK ? 0 : ZMQ_DONTWAIT;
#else
       int flags = block == ZSB_BLOCK ? 0 : ZMQ_NOBLOCK;
#endif

        if(i < lastElem) {
          flags |= ZMQ_SNDMORE;
        }
#if ZMQ_VERSION_MAJOR > 2
        if((ret = zmq_sendmsg (m_socket, &zmsg, flags)) < 0)
#else
        if((ret = zmq_send (m_socket, &zmsg, flags)) != 0)
#endif
        {
          int anErrno = zmq_errno();
          if(errValue != 0)
            *errValue = anErrno;
          zmq_msg_close(&zmsg);
          if(i == 0 && block == ZSB_NOBLOCK && (anErrno == EAGAIN || anErrno == EINTR))
            return false;
          if(anErrno == EHOSTUNREACH)
            return false;
          RavlError("Send failed : %s   flags=%x errno=%d in %s ",zmq_strerror (anErrno),flags,anErrno,Name().c_str());
          return false;
        }
        if((ret = zmq_msg_close(&zmsg)) != 0) {
          int anErrno = zmq_errno();
          RavlWarning("Failed to close message: %s ",zmq_strerror (anErrno));
        }
      }
      return true;
    }


    //! Receive a message.
    bool SocketC::Recieve(MessageC::RefT &msg,BlockT block)
    {
      RavlAssert(m_socket != 0);
      msg = new MessageC();
      int64_t more = 0;
      int ret;
      static size_t more_size = sizeof (more);
      do {
        MsgBufferC msgBuffer(0);
        int flags = 0;

#if ZMQ_VERSION_MAJOR >= 3
        if((ret = zmq_recvmsg (m_socket, msgBuffer.Msg(), flags)) < 0)
#else
        if((ret = zmq_recv (m_socket, msgBuffer.Msg(), flags)) != 0)
#endif
        {
          int anErrno = zmq_errno();
          RavlError("Recv failed : %s ",zmq_strerror (anErrno));
          throw ExceptionOperationFailedC("Recv failed. ");
        }
        msgBuffer.Build();
        msg->Parts().push_back(SArray1dC<char>(msgBuffer,msgBuffer.Size()));

        if((ret = zmq_getsockopt (m_socket, ZMQ_RCVMORE, &more, &more_size)) != 0) {
          int anErrno = zmq_errno();
          RavlError("RCVMORE failed : %s ",zmq_strerror (anErrno));
          throw ExceptionOperationFailedC("Recv failed. ");
        }
	//RavlDebug("More: %d Size:%zu ",more,msgBuffer.Size().V());
      } while(more) ;
      return true;
    }

    void LinkSocket()
    {}

    static XMLFactoryRegisterC<SocketC> g_regiserSocket("RavlN::ZmqN::SocketC");

}}
