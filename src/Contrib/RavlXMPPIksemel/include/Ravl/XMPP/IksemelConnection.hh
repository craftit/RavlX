// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/* 
 * File:   IksemelConnection.hh
 * Author: charlesgalambos
 *
 * Created on November 14, 2010, 12:42 PM
 */
//! lib=RavlXMPPIksemel
#ifndef RAVL_XMPP_IKSEMEL_HH
#define	RAVL_XMPP_IKSEMEL_HH

#include "Ravl/XMPP/XMPPConnection.hh"
#include "Ravl/Threads/Mutex.hh"
#include "iksemel.h"

namespace RavlN { namespace XMPPN {

  class IksemelConnectionC
   : public XMPPConnectionC
  {
  public:
    //! Default constructor
    IksemelConnectionC();
    
    //! Constructor
    IksemelConnectionC(const char *server,const char *user,const char *password,const char *resource="default");

    //! Factory constructor
    IksemelConnectionC(const XMLFactoryContextC &factory);

    //! Destructor
    ~IksemelConnectionC();

    //! Open connection
    virtual bool Open(const std::string &id,const std::string &password);

    //! Test if we have a connection.
    virtual bool IsConnected() const;

    //! Send a text message to someone.
    virtual bool SendText(const char *to,const char *message);


    //! Handle to class.
    typedef RavlN::SmartOwnerPtrC<IksemelConnectionC> RefT;

    //! Callback handle to class.
    typedef RavlN::SmartCallbackPtrC<IksemelConnectionC> CBRefT;

  protected:
    //! Start connection
    bool Init();

    //! Run connection
    virtual void RunConnection();

    //! Called when owner handles drop to zero.
    virtual void ZeroOwners();

    //! Translate call from C to C++
    static int TranslateOnStream(void *handle, int type, iks *node);

    //! Translate log
    static void TranslateOnLog(void *handle, const char *data, size_t size, int is_incoming);

    //! Translate error
    static int TranslateOnError(void *handle, ikspak *pak);

    //! Translate result
    static int TranslateOnResult(void *handle, ikspak *pak);

        //! Translate result
    static int TranslateOnMessage(void *handle, ikspak *pak);

    //! Log message
    void OnLog( const char *data, size_t size, int is_incoming);

    //! Handle incoming messages
    int OnStream(int type, iks *node);

    //! error message
    int OnError(ikspak *pak);

    //! On result message
    int OnResult(ikspak *pak);

    //! chat message
    int OnMessage(ikspak *pak);
    
    RavlN::MutexC m_access;
    iksparser *m_ikParse;
    iksid *m_ikId;
    iksfilter *m_ikFilter;
    int m_features;
    bool m_connectionOk;
    volatile bool m_shutdown;
    bool m_verbose;
  };


}}

#endif	/* IksemelConnection_HH */

