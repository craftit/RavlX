// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlOSIO
//! rcsid="$Id$"
//! file="Ravl/OS/IO/DynamicLink.cc"

#include "Ravl/DP/DynamicLink.hh"
#include "Ravl/HSet.hh"
#include "Ravl/String.hh"
#include "Ravl/StringList.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/Stream.hh"

#if !RAVL_COMPILER_VISUALCPP
#include <dlfcn.h>
#endif
#include <stdlib.h>

#ifndef PROJECT_OUT
#define PROJECT_OUT ""
#endif

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  static HashC<StringC,DListC<StringC> > libDepends;
  static HashC<StringC,void *> libsLoaded;
  static bool loadedDefault = false;
  
  static void * DynamicLinkLibLoad(const StringC &libName);
  
  static bool DynamicLinkLoadDepends(const char *fn) {
    ONDEBUG(cerr << "DynamicLinkLoadDepends() From '" << fn << "'\n");
    IStreamC in(fn);
    if(!in.good()) 
      return false;
    StringC lineBuff;
    int lineNo = 0;
    while(in.good()) {
      StringC target;
      lineNo++;
      lineBuff = "";
      readline(in,lineBuff);
      if(lineBuff.length() == 0)
	continue;
      if(lineBuff[0] == '#')
	continue; // Skip comment.
      StringListC sl(lineBuff);
      if(sl.IsEmpty())
	continue;
      target = sl.PopFirst();
      
      //ONDEBUG(cerr << "FileFormatDynamicBodyC::LoadFormatInfo() Targ:'" << target << "' Line '" << sl << "'\n");
      libDepends[target] = sl;
    }
    
    return true;
  }

void * DynamicLinkLoad(const StringC &libName) {
    MTWriteLockC lock(0);    
    if(!loadedDefault) {
      // Load library dependancies.
      DynamicLinkLoadDepends(PROJECT_OUT "/lib/RAVL/libdep/libDepends");
      loadedDefault = true;
    }
    return DynamicLinkLibLoad(libName);
  }
  
static void * DynamicLinkLibLoad(const StringC &libName) {

    void * handle=NULL;

    // Check if libraries have been loaded.
    if (libsLoaded.Lookup(libName,handle))
      return handle;
    
    // Load any other libraries needed for this one.
    if(libDepends.IsElm(libName)) {
      for(DLIterC<StringC> it(libDepends[libName]);it.IsElm();it.Next()) {
	if(!DynamicLinkLibLoad(it.Data()))
	  return 0;
      }
    }
    
    // Load the library itself
#if !RAVL_COMPILER_VISUALCPP
    ONDEBUG(cerr << "DynamicLinkLibLoad() Loading: '" << libName << "'\n");
    MTWriteLockC dynLinkLock(2); // Only one dynamic open at a time, so lock 'c' non-reentrant lock.
    libsLoaded.Update (libName,dlopen(libName.chars(), RTLD_GLOBAL | RTLD_LAZY));
    libsLoaded.Lookup(libName,handle);
    if (handle == NULL)
      cerr << "Error loading '" << libName << "' :" << dlerror() << "\n";
#else
    throw ExceptionC("DynamicLinkLibLoad(libName), Not implemented. ");
#endif
    return handle;
  }
}
