// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUIGnome

#include "Ravl/GUI/Gnome.hh"

#if !RAVL_OS_WIN32
#include <libgnome/libgnome.h>
#endif


// Declare fallback null strings use by for GNOME_PROGRAM_STANDARD_PROPERTIES
// These should all be already defined by defs.mk assuming PROJECT_OUT is set
// or $prefix, $datadir, etc. are set or PREFIX et al were user defined.
#ifndef PREFIX
#define PREFIX ""
#endif

#ifndef DATADIR
#define DATADIR ""
#endif

#ifndef SYSCONFDIR
#define SYSCONFDIR ""
#endif

#ifndef LIBDIR
#define LIBDIR ""
#endif

namespace RavlGUIN {
  
  //: Initialise gnome library.
  
  bool GnomeInit(const StringC &appName,const StringC &appVersion,int &nargs,char *args[]) {
#if !RAVL_OS_WIN32    
    gnome_program_init (appName,appVersion,
                        LIBGNOME_MODULE,
                        nargs, args,
                        GNOME_PROGRAM_STANDARD_PROPERTIES
                        ,NULL);
#endif
    return true;
  }
  
}
