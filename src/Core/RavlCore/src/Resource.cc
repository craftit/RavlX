// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Resource.cc"

#include "Ravl/Resource.hh"

// Declare fallback null string for DEFAULT_ROOT but this should have already
// been defaulted (to PROJECT_OUT) in defs.mk if the user has not supplied a
// definition for DEFAULT_ROOT.
#ifndef DEFAULT_ROOT
 #define DEFAULT_ROOT ""
#endif

namespace RavlN {
  static StringC resourceRoot(DEFAULT_ROOT);
  
  ResourceLookupFuncT resouceLookup = 0; 
  
  //: Replace method for doing resource lookups.
  
  bool SetResourceLookupFunc(ResourceLookupFuncT nResourceLookup) {
    resouceLookup = nResourceLookup;
    return true;
  }
  
  //: Access root of resource directory.
  
  bool SetResourceRoot(const StringC &name) { 
    resourceRoot = name; 
    return true;
  }
  
  //: Access root of resource directory.
  
  const StringC &ResourceRoot() 
  { return resourceRoot; }
  
  //: Find a resource.
  
  StringC Resource(const char *module,const char *name) {
    if(resouceLookup != 0)
      return resouceLookup(module,name);
    return resourceRoot + "/share/" + module + '/' + name;
  }
  
}
