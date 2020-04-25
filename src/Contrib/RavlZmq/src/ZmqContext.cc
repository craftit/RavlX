// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/Context.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <zmq.h>

namespace RavlN { namespace ZmqN {

    //! Constructor.
    ContextC::ContextC(int numThreads)
    {
#if ZMQ_VERSION_MAJOR > 2
      m_zmqContext = zmq_ctx_new();

      if(numThreads > 1 ) {
        zmq_ctx_set (m_zmqContext, ZMQ_IO_THREADS, numThreads);
      }
#else
      m_zmqContext = zmq_init (numThreads);
#endif
    }

      //! Constructor.
    ContextC::ContextC(const XMLFactoryContextC &context)
    {
      int numThreads = context.AttributeInt("threads",1);
      int numSockets = context.AttributeInt("maxSockets",-1);

#if ZMQ_VERSION_MAJOR > 2
      m_zmqContext = zmq_ctx_new();

      if(numThreads > 1 ) {
        zmq_ctx_set (m_zmqContext, ZMQ_IO_THREADS, numThreads);
      }
      if(numSockets >= 0 ) {
        zmq_ctx_set (m_zmqContext, ZMQ_MAX_SOCKETS,numSockets);
      }
#else
      m_zmqContext = zmq_init (numThreads);

      if ((numSockets >= 0 ) && (numSockets != 512 )) {
        RavlDebug("maxSockets unsupported in this version of 0MQ");
        // Default value in 2.x usually 512
      }
#endif
    }

    //! Destructor.
    ContextC::~ContextC()
    {
      if(m_zmqContext != 0)
#if ZMQ_VERSION_MAJOR > 2
        zmq_ctx_destroy(m_zmqContext);
#else
        zmq_term(m_zmqContext);
#endif
      m_zmqContext = 0;
    }

    //! Write to an ostream
    bool ContextC::Save(std::ostream &strm) const {
      return true;
    }

    //! Write to a binary stream
    // Not implemented
    bool ContextC::Save(BinOStreamC &strm) const {
      RavlAssertMsg(0,"not supported");
      return false;
    }

    void LinkContext()
    {}

    XMLFactoryRegisterC<ContextC> g_registerZmqContext("RavlN::ZmqN::ContextC");
}}

