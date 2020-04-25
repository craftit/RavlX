// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/Packet.cc"

#include "Ravl/OS/Packet.hh"

namespace RavlN
{  

  //: Construct a packet of 'size' bytes.
  
  NetPacketC::NetPacketC(IntT size)
    : data(size)
  {}
  
  //: Dump packet in human readable form.
  
  void NetPacketC::Dump(ostream &out) const {
    out << "NetPacketC::Dump(),  Length : " << data.Size() << " bytes. \n";
    out << hex;
    int lc = 0;
    for(UIntT i = 0; i < data.Size();i++,lc++) {
      out << ((int) data[i]) << ' ';
      if(lc > 15) {
	out << '\n';
	lc = 0;
      }
    }
    out << dec << '\n';
  }

}



