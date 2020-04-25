// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DYNAMICLINK_HEADER
#define RAVL_DYNAMICLINK_HEADER 1
//! rcsid="$Id$"
//! lib=RavlOSIO
//! userlevel=Normal
//! docentry="Ravl.API.OS.Dynamic Linking"
//! author="Charles Galambos"
//! example=exDynamicLink.cc
//! date="04/07/2000"
//! file="Ravl/OS/IO/DynamicLink.hh"

#include "Ravl/String.hh"

namespace RavlN {

void * DynamicLinkLoad(const StringC &libName);
  //: Load a library.
  // Returns pointer to loaded library
}


#endif
