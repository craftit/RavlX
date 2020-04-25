// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETMSGOPORTDATA_HEADER
#define RAVL_NETMSGOPORTDATA_HEADER 1
//! rcsid="$Id$"
//! lib=RavlNet

#include "Ravl/OS/NetMessage.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/TypeInfo.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Abstract data decoder for the NetOPortServer.
  
  class NetMsgOPortDataBodyC
    : public NetMsgRegisterBodyC 
  {
  public:
    NetMsgOPortDataBodyC(UIntT nid,const StringC &nname,const DPOPortBaseC &portBase,const DPSeekCtrlC &seekCtrl, bool ndecodeArray);
    //: Constructor.
    
    virtual bool Decode(NetEndPointC &ep,BinIStreamC &pkt);
    //: Decode and execute a packet.
    
  protected:
    Int64T at;
    DPOPortBaseC oportBase;
    DPSeekCtrlC seekCtrl;
    DPTypeInfoC dataType;
    bool decodeArray;
  };
  

  //! userlevel=Develop
  //: Abstract data decoder for the NetOPortServer.
  
  class NetMsgOPortDataC
    : public NetMsgRegisterC 
  {
  public:
    NetMsgOPortDataC(UIntT nid,const StringC &nname,const DPOPortBaseC &portBase,const DPSeekCtrlC &seekCtrl, bool ndecodeArray = false)
      : NetMsgRegisterC(*new NetMsgOPortDataBodyC(nid,nname,portBase,seekCtrl,ndecodeArray))
    {}
    //: Constructor.
    
  };

}

#endif
