// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlService
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.Services

#include "Ravl/ServiceThread.hh"
#include "Ravl/XMLFactoryAutoStart.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/CallMethodPtrs.hh"

namespace RavlN {

  ServiceThreadC::ServiceThreadC()
    : m_started(false),
      m_terminate(false)
  {}

  //! XMLFactory Constructor
  ServiceThreadC::ServiceThreadC(const XMLFactoryContextC &factory)
   : ServiceC(factory),
     m_started(false),
     m_terminate(false)
  {}

  //! Write to an ostream
  bool ServiceThreadC::Save(std::ostream &strm) const
  {
    return ServiceC::Save(strm);
  }

  //! Write to a binary stream
  // Not implemented
  bool ServiceThreadC::Save(BinOStreamC &strm) const
  {
    RavlAssertMsg(0,"not supported");
    return false;
  }


  //! Start service.
  bool ServiceThreadC::Start() {
    RavlN::MutexLockC lock(m_accessStarted);
    if(m_started)
      return true;
    m_started = true;
    lock.Unlock();
    LaunchThread(TriggerPtr(CBRefT(this),&ServiceThreadC::Run));
    return true;
  }

  bool ServiceThreadC::StartOwner() {
    RavlN::MutexLockC lock(m_accessStarted);
    if(m_started)
      return true;
    m_started = true;
    lock.Unlock();
    LaunchThread(TriggerPtr(RefT(this),&ServiceThreadC::Run));
    return true;
  }

  //! Shutdown service
  bool ServiceThreadC::Shutdown() {
    m_terminate = true;
    return false;
  }

  //! Code called on the thread.
  bool ServiceThreadC::Run()
  {
    RavlAssertMsg(0,"abstract method called.");
    return true;
  }

  void ServiceThreadC::ZeroOwners() {
    Shutdown();
    RavlN::RCLayerBodyC::ZeroOwners();
  }
  //: Called when owner handles drop to zero.

  ServiceC::RefT ServiceThread2Service(const ServiceThreadC::RefT &reactor)
  { return reactor.BodyPtr(); }

  DP_REGISTER_CONVERSION(ServiceThread2Service,1.0);

  void LinkServiceThread()
  {}


}
