// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetMsgCall.cc"

#include "Ravl/OS/NetMsgCall.hh"

namespace RavlN
{  

  //: Constructor.
  
  NetMsgCall0BodyC::NetMsgCall0BodyC(UIntT nid,const StringC &nname,const CallFunc0C<bool> &nsig)
    : NetMsgRegisterBodyC(nid,nname),
      sig(nsig)
  {}
  
  //: Decode and execute a packet.
  
  bool NetMsgCall0BodyC::Decode(NetEndPointC &ep,BinIStreamC &pkt) { 
    //cerr << "Decode: Call0 at " << is.Tell() << "\n";
    sig.Invoke();
    return true;
  }

}
