// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMPP

#include "Ravl/XMPP/XMPPConnection.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace XMPPN {

    //! Default constructor

    XMPPConnectionC::XMPPConnectionC()
    : m_dumpRaw(true),
      m_sigTextMessage("", ""),
      m_isReady(false),
      m_connectionState(CS_Init),
      m_useTLS(false),
      m_useSASL(true)
    {}


    //! Constructor

    XMPPConnectionC::XMPPConnectionC(const char *server,
                                 const char *user,
                                 const char *password,
                                 const char *resource)
    : m_server(server),
      m_user(user),
      m_password(password),
      m_resource(resource),
      m_dumpRaw(true),
      m_sigTextMessage("", ""),
      m_isReady(false),
      m_connectionState(CS_Init),
      m_useTLS(false),
      m_useSASL(true)
    {
    }

    //! Factory constructor

    XMPPConnectionC::XMPPConnectionC(const XMLFactoryContextC &factory)
    : m_server(factory.AttributeString("server", "jabber.org").data()),
      m_user(factory.AttributeString("user", "auser").data()),
      m_password(factory.AttributeString("password", "apassword").data()),
      m_resource(factory.AttributeString("resource", "default").data()),
      m_dumpRaw(factory.AttributeBool("dumpRaw", false)),
      m_sigTextMessage("", ""),
      m_isReady(false),
      m_connectionState(CS_Init),
      m_useTLS(false),
      m_useSASL(true)
    {
    }


    //! Destructor

    XMPPConnectionC::~XMPPConnectionC()
    {
    }


    //: Called when owner handles drop to zero.

    void XMPPConnectionC::ZeroOwners()
    {

    }

    //! Start the server

    int XMPPConnectionC::Init()
    {
      return true;
    }

    //! Start opening

    bool XMPPConnectionC::Open()
    {
      return true;
    }

    //! Test if we have a connection.

    bool XMPPConnectionC::IsConnected() const
    {
      RavlAssertMsg(0,"abstract method called.");
      return false;
    }

    //! Send a text message to someone.

    bool XMPPConnectionC::SendText(const char *to, const char *message)
    {
      RavlAssertMsg(0,"abstract method called.");
      return false;
    }


  }
}
