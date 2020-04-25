// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/OS/XMLFactory/ConfigParameter.cc"
//! lib=RavlXMLFactory

#include "Ravl/ConfigParameter.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{

  ConfigParameterStringC::ConfigParameterStringC(const XMLFactoryContextC &factory)
  : ConfigParameterC<std::string>(factory)
  {}

  void ConfigParameterStringC::ZeroOwners()
  { ConfigParameterC<std::string>::ZeroOwners(); }

  bool SetupConfigParameter(const XMLFactoryContextC &factory,const std::string &name,const std::string &value,bool acceptEmptyString) {
    // Setup server address if required.
    if(!factory.HasChild(name)) {
      ONDEBUG(RavlDebug("Parameter %s not found. ",name.data()));
      return false;
    }
    RavlN::ConfigParameterStringC::RefT paramStr;
    if(!factory.UseChildComponent(name,paramStr)) {
      RavlError("Failed to setup '%s'.",name.data());
      return false;
    }
    if(!acceptEmptyString && value.empty()) {
      RavlError("No value given for '%s' ",name.data());
      return false;
    }
    ONDEBUG(RavlDebug("Setting parameter '%s' to '%s' . ",name.data(),value.data()));
    paramStr->SetValue(value.data());
    return true;
  }

  static RavlN::XMLFactoryRegisterC<ConfigParameterStringC> g_FactoryRegisterString("RavlN::ConfigParameterStringC");

  void LinkConfigParameter() {}
  
}
