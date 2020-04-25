// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETMESSAGE_HEADER
#define RAVL_NETMESSAGE_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Network/NetMessage.hh"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network"
//! date="14/11/2000"

#include "Ravl/RefCounter.hh"
#include "Ravl/OS/Packet.hh"
#include "Ravl/Hash.hh"

namespace RavlN
{  
  class NetEndPointC;
  
  ///// NetMessage ////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Net message body.
  
  class NetMsgRegisterBodyC 
    : public RCBodyVC
  {
  public:
    NetMsgRegisterBodyC(UIntT nid,const StringC &nname)
      : id(nid),
        name(nname)
    {}
    //: Constructor.
    
    virtual bool Decode(NetEndPointC &ep,BinIStreamC &pkt);
    //: Decode and execute a packet.
    
    UIntT Id() const
    { return id; }
    //: Access id.
    
    const StringC &Name() const
    { return name; }
    //: Access type name.
    
    bool Encode(BinOStreamC &os) {
      os << id;
      return true;
    }
    //: Encode stream.
  protected:
    UIntT id;
    StringC name;
  };

  //! userlevel=Advanced
  //: Net message handle.
  
  class NetMsgRegisterC 
    : public RCHandleC<NetMsgRegisterBodyC>
  {
  public:
    NetMsgRegisterC()
    {}
    //: Default constructor.
    
  protected:
    NetMsgRegisterC(NetMsgRegisterBodyC &bod)
      : RCHandleC<NetMsgRegisterBodyC>(bod)
    {}
    //: Body constructor.
    
    NetMsgRegisterC(const NetMsgRegisterBodyC *bod)
      : RCHandleC<NetMsgRegisterBodyC>(bod)
    {}
    //: Body constructor.
    
  public:    
    inline bool Decode(NetEndPointC &ep,BinIStreamC &str)
    { return Body().Decode(ep,str); }
    //: Decode a packet.
    
    UIntT Id() const
    { return Body().Id(); }
    //: Access id.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Access type name.
    
    static HashC<UIntT,NetMsgRegisterC> &MsgTypes() { 
      static HashC<UIntT,NetMsgRegisterC> reg;
      return reg;
    }
    //: Access message register.
    
    static bool Register(const NetMsgRegisterC &msg) { 
      HashC<UIntT,NetMsgRegisterC> &reg = MsgTypes();
      if(reg.IsElm(msg.Id())) {
	cerr << "NetMsgRegisterC::Register(), WARNING id conflict. \n";
	return false;
      }
      reg[msg.Id()] = msg;
      return true;
    }
    
  };

  
}


#endif
