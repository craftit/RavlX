// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlService
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.Services

#include "Ravl/Service.hh"
#include "Ravl/XMLFactoryAutoStart.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/SysLog.hh"

namespace RavlN {

  ServiceC::ServiceC()
  {}

  //! XMLFactory Constructor
  ServiceC::ServiceC(const XMLFactoryContextC &factory)
   : m_name(factory.Path().data())
  {}

  //! Write to an ostream
  bool ServiceC::Save(std::ostream &strm) const
  {
    strm << "Service:" << m_name;
    return true;
  }

  //! Write to a binary stream
  // Not implemented
  bool ServiceC::Save(BinOStreamC &strm) const
  {
    RavlAssertMsg(0,"not supported");
    return false;
  }

  //! Start service.
  bool ServiceC::Start() {
    return true;
  }

  //! Shutdown service
  bool ServiceC::Shutdown() {
    return true;
  }

  void ServiceC::ZeroOwners() {
    RavlN::RCLayerBodyC::ZeroOwners();
  }
  //: Called when owner handles drop to zero.


  void LinkService()
  {}

}
