// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlNet
//! file="Ravl/OS/Network/NetPortFormat.cc"

#include "Ravl/OS/NetPortFormat.hh"

namespace RavlN {
  void NetPortFormat_MustLink() 
  {}
  
  static NetPortFormatC<IntT> g_netFormat_Int;
  static NetPortFormatC<ByteT> g_netFormat_Byte;
}
