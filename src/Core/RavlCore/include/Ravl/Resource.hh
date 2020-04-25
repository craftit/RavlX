// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RESOURCE_HEADER
#define RAVL_RESOURCE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Resource"
//! userlevel=Normal
//! file="Ravl/Core/Base/Resource.hh"

#include "Ravl/String.hh"

namespace RavlN {
  
  //!userlevel=Normal
  
  bool SetResourceRoot(const StringC &Name);
  //: Set root of resource directory.
  
  const StringC &ResourceRoot();
  //: Access root of resource directory.
  // See <a href="../Tree/Ravl.API.Core.Resource.html">above</a> for the default value.
  
  StringC Resource(const char *Module,const char *Name);
  //: Get location of resource.
  // The default location is composed of <code>ResourceRoot() + "/share/" + module + '/' + Name</code>.  Use <a href="RavlN.SetResourceLookupFuncObResourceLookupFuncTCb.html"><code>SetResourceLookupFunc()</code></a> for a different method.<br>
  // Leading '/'s in <code>Module</code> or <code>Name</code> are ignored.
  
  //!userlevel=Advanced
  
  typedef StringC (*ResourceLookupFuncT)(const char *Module,const char *Name);
  
  bool SetResourceLookupFunc(ResourceLookupFuncT resourceLookup);
  //: Replace method for doing resource lookups.
  
}



#endif
