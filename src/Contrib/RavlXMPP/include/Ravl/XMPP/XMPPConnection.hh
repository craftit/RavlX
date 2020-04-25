// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMPP_XMPPCONNECTION_HEADER
#define RAVL_XMPP_XMPPCONNECTION_HEADER 1
//! lib=RavlXMPP

#include "Ravl/XMLFactory.hh"
#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Semaphore.hh"


namespace RavlN { namespace XMPPN {

  class XMPPConnectionC
    : public RavlN::RCLayerBodyC
  {
  public:
    //! Default constructor
    XMPPConnectionC();

    //! Constructor
    XMPPConnectionC(const char *server,const char *user,const char *password,const char *resource="default");

    //! Factory constructor
    XMPPConnectionC(const XMLFactoryContextC &factory);

    //! Destructor
    ~XMPPConnectionC();

    //! Access current server
    const std::string &Server() const
    { return m_server; }

    //! Access current user
    const std::string &User() const
    { return m_user; }

    //! Access current password
    const std::string &Password() const
    { return m_password; }

    //! Access current resource
    const std::string &Resource() const
    { return m_resource; }

    //! Start opening
    virtual bool Open();

    //! Test if we have a connection.
    virtual bool IsConnected() const;

    //! Is connection ready for use ?
    bool IsReady() const
    { return m_isReady; }

    //! Send a text message to someone.
    virtual bool SendText(const char *to,const char *message);

    //! Access text received signal.
    // from,text
    Signal2C<std::string,std::string> &SigTextMessage()
    { return m_sigTextMessage; }

    //! Handle to class.
    typedef RavlN::SmartOwnerPtrC<XMPPConnectionC> RefT;

    //! Callback handle to class.
    typedef RavlN::SmartCallbackPtrC<XMPPConnectionC> CBRefT;

  protected:

    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    //! Start the server
    int Init();

    std::string m_server;
    std::string m_user;
    std::string m_password;
    std::string m_resource;

    bool m_dumpRaw;
    Signal2C<std::string,std::string> m_sigTextMessage;  //!< from,text

    
    bool m_isReady;

    enum ConnectionStateT {
      CS_Init,
      CS_Open,
      CS_Authenticated,
      CS_Closed
    } m_connectionState;

    bool m_useTLS;
    bool m_useSASL;
  };

}}

#endif
