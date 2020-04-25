// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMPPIksemel

#include "Ravl/XMPP/IksemelConnection.hh"
#include "Ravl/RLog.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include <sys/select.h>
#include "Ravl/XMLFactoryRegister.hh"
#include <stdio.h>
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace XMPPN {

    //! Translate log

    void IksemelConnectionC::TranslateOnLog(void *handle, const char *data, size_t size, int is_incoming)
    {
      IksemelConnectionC *parent = reinterpret_cast<IksemelConnectionC *> (handle);
      RavlAssert(parent != 0);
      parent->OnLog(data, size, is_incoming);
    }

    int IksemelConnectionC::TranslateOnStream(void *handle, int type, iks *node)
    {
      IksemelConnectionC *parent = reinterpret_cast<IksemelConnectionC *> (handle);
      RavlAssert(parent != 0);
      return parent->OnStream(type, node);
    }

    //! Translate log
    int IksemelConnectionC::TranslateOnError(void *handle, ikspak *pak)
    {
      IksemelConnectionC *parent = reinterpret_cast<IksemelConnectionC *> (handle);
      RavlAssert(parent != 0);
      return parent->OnError(pak);
    }

    //! Translate result
    int IksemelConnectionC::TranslateOnResult(void *handle, ikspak *pak) {
      IksemelConnectionC *parent = reinterpret_cast<IksemelConnectionC *> (handle);
      RavlAssert(parent != 0);
      return parent->OnResult(pak);
    }

    //! Translate result
    int IksemelConnectionC::TranslateOnMessage(void *handle, ikspak *pak) {
      IksemelConnectionC *parent = reinterpret_cast<IksemelConnectionC *> (handle);
      RavlAssert(parent != 0);
      return parent->OnMessage(pak);
    }

    // --------------------------------------------------------------

    //! Constructor

    IksemelConnectionC::IksemelConnectionC()
    : m_ikParse(0),
      m_ikId(0),
      m_ikFilter(0),
      m_features(0),
      m_connectionOk(false),
      m_shutdown(false),
      m_verbose(true)
    { }

    //! Constructor

    IksemelConnectionC::IksemelConnectionC(const char *server,
                                           const char *user,
                                           const char *password,
                                           const char *resource)
    : XMPPConnectionC(server, user, password, resource), 
      m_ikParse(0),
      m_ikId(0),
      m_ikFilter(0),
      m_features(0),
      m_connectionOk(false),
      m_shutdown(false),
      m_verbose(true)
    { Init(); }

    //! Factory constructor

    IksemelConnectionC::IksemelConnectionC(const XMLFactoryContextC &factory)
    : XMPPConnectionC(factory),
      m_ikParse(0),
      m_ikId(0),
      m_ikFilter(0),
      m_features(0),
      m_connectionOk(false),
      m_shutdown(false),
      m_verbose(true)
    { Init(); }

    //! Destructor

    IksemelConnectionC::~IksemelConnectionC()
    {
      if (m_ikParse != 0) {
        iks_parser_delete(m_ikParse);
        m_ikParse = 0;
      }
      if (m_ikId != 0) {
        // FIXME:-Surely we have to free something ?
      }
      if(m_ikFilter != 0) {
        iks_filter_delete (m_ikFilter);
        m_ikFilter = 0;
      }
    }

    //! Start connection
    bool IksemelConnectionC::Init()
    {
      std::string anid = m_user + '@' + m_server + '/' + m_resource;
      return Open(anid,m_password);
    }


    //! Open connection

    bool IksemelConnectionC::Open(const std::string &id, const std::string &password)
    {
      m_connectionOk = false;
      if (!iks_has_tls()) {
        puts("Cannot make encrypted connections.");
        puts("iksemel library is not compiled with GnuTLS support.");
        return false;
      }

      rDebug("Opening stream id:%s ", id.data());
      m_ikParse = iks_stream_new(IKS_NS_CLIENT, this, (iksStreamHook *) & IksemelConnectionC::TranslateOnStream);

      // Setup logging.
      ONDEBUG(iks_set_log_hook(m_ikParse, (iksLogHook *) & IksemelConnectionC::TranslateOnLog));

      m_ikId = iks_id_new(iks_parser_stack(m_ikParse), id.data());

      if (0 == m_ikId->resource) {
        /* user gave no resource name, use the default */
        char *tmp = (char *) iks_malloc(strlen(m_ikId->user) + strlen(m_ikId->server) + 9 + 3);
        sprintf(tmp, "%s@%s/%s", m_ikId->user, m_ikId->server, "Ravl");
        m_ikId = iks_id_new(iks_parser_stack(m_ikParse), tmp);
        iks_free(tmp);
      }

      if (m_ikFilter != 0) iks_filter_delete(m_ikFilter);
      m_ikFilter = iks_filter_new();
      
      iks_filter_add_rule(m_ikFilter, &IksemelConnectionC::TranslateOnError, this,
                          IKS_RULE_TYPE, IKS_PAK_IQ,
                          IKS_RULE_SUBTYPE, IKS_TYPE_ERROR,
                          IKS_RULE_ID, "auth",
                          IKS_RULE_DONE);

      iks_filter_add_rule(m_ikFilter,  &IksemelConnectionC::TranslateOnResult, this,
                          IKS_RULE_TYPE, IKS_PAK_IQ,
                          IKS_RULE_SUBTYPE, IKS_TYPE_RESULT,
                          IKS_RULE_ID, "auth",
                          IKS_RULE_DONE);
                          
      iks_filter_add_rule(m_ikFilter, &IksemelConnectionC::TranslateOnMessage, this,
                          IKS_RULE_TYPE, IKS_PAK_MESSAGE,
                          IKS_RULE_SUBTYPE,IKS_TYPE_CHAT ,
                          IKS_RULE_DONE);
      
      

      iks_filter_add_rule(m_ikFilter, &IksemelConnectionC::TranslateOnResult, this,
                          IKS_RULE_TYPE, IKS_PAK_IQ,
                          IKS_RULE_SUBTYPE, IKS_TYPE_RESULT,
                          IKS_RULE_ID, "roster",
                          IKS_RULE_DONE);

      int ret = iks_connect_tcp(m_ikParse, m_ikId->server, IKS_JABBER_PORT);

      switch (ret)
       {
        case IKS_OK:
         break;
        case IKS_NET_NODNS:
         rError("hostname lookup failed");
         return false;
        case IKS_NET_NOCONN:
          rError("connection failed");
         return false;
        default:
         rError("io error");
         return false;
       }
      RavlN::LaunchThread(TriggerPtr(CBRefT(this),&IksemelConnectionC::RunConnection));

      return true;
    }

    //! Send a text message to someone.
    bool IksemelConnectionC::SendText(const char *to,const char *message)
    {
      if(m_ikParse == 0) {
        rError("Attempt to send message on closed connection. ");
        return false;
      }
      MutexLockC lock(m_access);
      iks *msg = iks_make_msg(IKS_TYPE_CHAT,to,message);
      iks_send(m_ikParse, msg);
      iks_delete(msg);
      return true;
    }

    //! Test if we have a connection.
    bool IksemelConnectionC::IsConnected() const
    {
      return !m_shutdown && m_connectionOk;
    }

    //: Called when owner handles drop to zero.

    void IksemelConnectionC::ZeroOwners()
    {
      m_shutdown = true;
      XMPPConnectionC::ZeroOwners();
    }

    //! Run connection

    void IksemelConnectionC::RunConnection()
    {
      rDebug("Connection thread started");

      m_connectionOk = true;

      while (!m_shutdown) {
        int fd = iks_fd (m_ikParse);
        
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(fd,&fdset);
        struct timeval timeout;
        timeout.tv_sec = 60;
        timeout.tv_usec = 0;
        int ret = select(fd+1,&fdset,0,0,&timeout);
        if(ret < 0) {
          if(errno == EINTR)
            continue;
          rError("Error waiting for file descriptor");
          break;
        }
        if(ret == 0) {
          ONDEBUG(rDebug("No file descriptor ready"));
#if 1
          //const char *msg = " ";
          MutexLockC lock(m_access);
          iks_send_raw (m_ikParse," ");
          //write(fd,msg,1);
          //iks *x = iks_make_iq (IKS_TYPE_GET,IKS_NS_VERSION);
          //iks_send(m_ikParse,x);
          //iks_delete(x);
#endif
          // Send a keep alive message?
          continue;
        }
        //rDebug("Data ready.");
        MutexLockC lock(m_access);
        ret = iks_recv(m_ikParse, 0);
        lock.Unlock();
        
        if (IKS_HOOK == ret)
          break;
        if (IKS_NET_TLSFAIL == ret) {
          rError("tls handshake failed");
          break;
        }
        if (IKS_OK != ret) {
          rError("io error");
          break;
        }
      }

      m_connectionOk = false;
    }

    //! Handle incoming messages

    int IksemelConnectionC::OnStream(int type, iks *node)
    {
      ONDEBUG(rDebug("OnStream called. Type:%d Node:%p ", type,node));
      switch (type)
       {
        case IKS_NODE_START:
         ONDEBUG(rDebug("NodeStart"));
         if (m_useTLS && !iks_is_secure(m_ikParse)) {
           iks_start_tls(m_ikParse);
           break;
         }
         if (!m_useSASL) {
           iks *x;
           char *sid = NULL;
           //if (!opt_use_plain)
           sid = iks_find_attrib(node, "id");
           x = iks_make_auth(m_ikId, m_password.data(), sid);
           iks_insert_attrib(x, "id", "auth");
           iks_send(m_ikParse, x);
           iks_delete(x);
         }
         break;
        case IKS_NODE_NORMAL:
         {
         ONDEBUG(rDebug("NodeNormal"));
          std::string nodeName = iks_name(node);
          ONDEBUG(rDebug("Got normal node '%s' ", nodeName.data()));
          if (nodeName == "stream:features") {
            m_features = iks_stream_features(node);
            if (m_useSASL) {
              if (m_useTLS && !iks_is_secure(m_ikParse)) break;
              if (m_connectionState != CS_Init) {
                iks *t;
                if (m_features & IKS_STREAM_BIND) {
                  ONDEBUG(rDebug("Sending resource bind. "));
                  t = iks_make_resource_bind(m_ikId);
                  iks_send(m_ikParse, t);
                  iks_delete(t);
                }
                if (m_features & IKS_STREAM_SESSION) {
                  ONDEBUG(rDebug("Sending make session. "));
                  t = iks_make_session();
                  iks_insert_attrib(t, "id", "auth");
                  iks_send(m_ikParse, t);
                  iks_delete(t);
                }
#if 1
                // Send presents
                iks *x = iks_make_pres (IKS_SHOW_AVAILABLE,"online");
                iks_send (m_ikParse, x);
                iks_delete (x);
#endif
                m_isReady = true;
              } else {
                if (m_features & IKS_STREAM_SASL_MD5) {
                  ONDEBUG(rDebug("Sending start MD5 "));
                  iks_start_sasl(m_ikParse, IKS_SASL_DIGEST_MD5, m_ikId->user, (char *) m_password.data());
                }
                else if (m_features & IKS_STREAM_SASL_PLAIN) {
                  ONDEBUG(rDebug("Sending start plain "));
                  iks_start_sasl(m_ikParse, IKS_SASL_PLAIN, m_ikId->user, (char *) m_password.data());
                }
              }
            }
          } else if (nodeName == "failure") {
            rError("sasl authentication failed packet recieved.");
          } else if (nodeName == "success") {
            ONDEBUG(rDebug("Got success!!"));
            if(m_connectionState != CS_Init) {
              rError("Got unexpected authentication message.");
            } else {
              m_connectionState = CS_Authenticated;
              iks_send_header(m_ikParse, m_ikId->server);
            }
          } else {
            ONDEBUG(rDebug("Filtering packet."));
            ikspak *pak;
            pak = iks_packet(node);
            iks_filter_packet(m_ikFilter, pak);
            
            //  return IKS_HOOK;
          }
         }
         break;
        case IKS_NODE_STOP:
         rDebug("Server disconnected");
         break;
        case IKS_NODE_ERROR:
          rError("Stream error");
          break;
        default:
          rDebug("Unknown packet type ");
          break;
       }

      if(node) iks_delete(node);
      return IKS_OK;
    }

    //! Log message

    void IksemelConnectionC::OnLog(const char *data, size_t size, int is_incoming)
    {
      
      std::string prefix;
      if (iks_is_secure(m_ikParse)) prefix = "Sec";

      if (is_incoming) prefix += "RECV";
      else prefix += "SEND";

      prefix += "[";
      prefix += data;
      prefix += "]";

      rDebug("%s", prefix.data());
    }

    //! Log message
    int IksemelConnectionC::OnError(ikspak *pak) {
      rError("Authentication failed.");
      return IKS_FILTER_EAT;
    }

    //! Log message
    int IksemelConnectionC::OnResult(ikspak *pak) {
      rDebug("On result called. ");
      return IKS_FILTER_EAT;
    }

    //! Log message
    int IksemelConnectionC::OnMessage(ikspak *pak) {
      const char *from = iks_find_attrib (pak->x,"from");
      const char *text = iks_find_cdata (pak->x, "body");
      if(text == 0 || from == 0) {
        if(m_verbose) {
          char * msg = iks_string(0,pak->x);
          rWarning("No message found in:  %s",msg);
          iks_free(msg);
        }
        return IKS_FILTER_EAT;
      }
      rDebug("On message called. From:'%s' Text:'%s' ",from,text);
      m_sigTextMessage(from,text);
      return IKS_FILTER_EAT;
    }

    XMLFactoryRegisterConvertC<IksemelConnectionC,XMPPConnectionC> g_xmlFactoryRegisterIksemelConnection("RavlN::XMPPN::IksemelConnectionC");

    void LinkRavlXMPPIksemelConnection()
    {
    }

  }
}
