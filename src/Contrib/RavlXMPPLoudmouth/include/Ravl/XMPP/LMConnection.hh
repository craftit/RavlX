// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMPP_CONNECTION_HEADER
#define RAVL_XMPP_CONNECTION_HEADER 1
//! lib=RavlXMPPLoudmouth

#include "loudmouth/loudmouth.h"
#include "Ravl/XMLFactory.hh"
#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/XMPP/XMPPConnection.hh"
#include <string>


namespace RavlN { namespace XMPPN {

  class LMConnectionC
    : public XMPPConnectionC
  {
  public:
    //! Constructor
    LMConnectionC(const char *server,const char *user,const char *password,const char *resource="default");

    //! Factory constructor
    LMConnectionC(const XMLFactoryContextC &factory);

    //! Destructor
    ~LMConnectionC();

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
    bool Open();
    
    //! Test if we have a connection.
    bool IsConnected() const;

    //! Is connection ready for use ?
    bool IsReady() const
    { return m_isReady; }

    //! Send a text message to someone.
    bool SendText(const char *to,const char *message);

    //! Access text received signal.
    Signal2C<std::string,std::string> &SigTextMessage()
    { return m_sigTextMessage; }

    //! Handle to class.
    typedef RavlN::SmartOwnerPtrC<LMConnectionC> RefT;

    //! Callback handle to class.
    typedef RavlN::SmartCallbackPtrC<LMConnectionC> CBRefT;

    //! Handle incoming messages.
    //! Internal method, don't call directly.
    virtual LmHandlerResult MessageHandler(LmMessageHandler *handler,
                                           LmConnection     *connection,
                                           LmMessage        *message);

    //! Called when connection open complete.
    virtual void CBConnectionOpen(LmConnection *connection,gboolean success);

    //! Called when connection open complete.
    virtual void CBConnectionAuth(LmConnection *connection,gboolean success);
  protected:



    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.

    //! Start the server
    int Init(bool inOwnThread);

    // GLib main event loop if needed.
    int MainLoop();

    bool m_asyncOpen;

    GMainContext *m_context;
    GMainLoop *m_mainLoop;
    LmConnection *m_conn;
    LmMessageHandler  *m_defaultHandler;
    SemaphoreC m_mainLoopStarted;

  };
}}



#endif
