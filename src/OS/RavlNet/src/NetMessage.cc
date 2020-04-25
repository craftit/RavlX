// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetMessage.cc"

#include "Ravl/OS/NetMessage.hh"
#include "Ravl/Stream.hh"

namespace RavlN
{
  //: Decode and execute a packet.
  
  bool NetMsgRegisterBodyC::Decode(NetEndPointC &ep,BinIStreamC &pkt) {
    cerr << "NetMsgRegisterBodyC::Decode(NetEndPointC &,BinIStreamC &), Abstract function called \n";
    return false;
  }

}
