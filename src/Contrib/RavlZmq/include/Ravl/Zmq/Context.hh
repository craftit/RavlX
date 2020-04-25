// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQN_CONTEXT_HEADER
#define RAVL_ZMQN_CONTEXT_HEADER
//! lib=RavlZmq

#include "Ravl/SmartPtr.hh"

namespace RavlN {
  class XMLFactoryContextC;

  namespace ZmqN {

    //! Context for connection
    // The context is thread safe and may be
    // shared between as many threads in the program
    // as you wish.

    class ContextC :
       public RCBodyC
    {
    public:
      //! Constructor.
      ContextC(int noThreads);

      //! Constructor.
      ContextC(const XMLFactoryContextC &context);

      //! Write to an ostream
      bool Save(std::ostream &strm) const;

      //! Write to a binary stream
      // Not implemented
      bool Save(BinOStreamC &strm) const;

      //! Destructor.
      ~ContextC();

      //! Access raw context
      void *RawContext()
      { return m_zmqContext; }

      //! Handle to context
      typedef SmartPtrC<ContextC> RefT;
    protected:
      void *m_zmqContext;
    };

  }
}

#endif
