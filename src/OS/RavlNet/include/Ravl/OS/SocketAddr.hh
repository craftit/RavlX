// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SOCKETADDR_HEADER
#define RAVL_SOCKETADDR_HEADER 1
//! lib=RavlNet

#include "Ravl/String.hh"
// Some forward declarations to avoid including extra header
// files.

struct sockaddr_in;
struct sockaddr;

namespace RavlN {
  
  bool GetHostByName(const char *name,struct sockaddr_in &sin);
  //: Attempt to get info about named host.
  // returns true on success.

  bool GetHostByAddr(struct sockaddr &sin,int sinLen,StringC &name);
  //: Attempt to find hostname by the address.
  // returns true on success and assignes the hostname to name.
  
}

#endif

