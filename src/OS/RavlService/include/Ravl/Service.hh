// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlService
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.Services
#ifndef RAVL_SERVICE_HEADER
#define RAVL_SERVICE_HEADER 1

#include "Ravl/SmartLayerPtr.hh"
#include <vector>

namespace RavlN {
  class XMLFactoryContextC;

  //! Class to allow classes to run code after loaded in a preload section.

  class ServiceC
    : virtual public RavlN::RCLayerBodyC
  {
  public:
    //! Constructor
    ServiceC();

    //! XMLFactory Constructor
    ServiceC(const XMLFactoryContextC &factory);

    //! Write to an ostream
    bool Save(std::ostream &strm) const;

    //! Write to a binary stream
    // Not implemented
    bool Save(BinOStreamC &strm) const;

    //! Name of service
    const RavlN::StringC &Name() const
    { return m_name; }

    //! Set the name of the service.
    void SetName(const StringC &theName)
    { m_name = theName; }

    //! Start service.
    virtual bool Start();

    //! Shutdown service
    virtual bool Shutdown();

    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<ServiceC> RefT;

    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<ServiceC> CBRefT;

  protected:
    //! Name of service.
    RavlN::StringC m_name;

    //! Called when owner handles drop to zero.
    virtual void ZeroOwners();
  };


}

#endif
