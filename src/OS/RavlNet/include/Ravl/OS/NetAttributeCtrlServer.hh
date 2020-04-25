// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETATTRIBUTECTRLSERVER_HEADER
#define RAVL_NETATTRIBUTECTRLSERVER_HEADER 1
//! rcsid="$Id$"
//! lib=RavlNet
//! docentry="Ravl.API.OS.Network.NetPort"
//! author="Charles Galambos"
//! file="Ravl/OS/Network/NetAttributeCtrlServer.hh"

#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Net attribute server.
  
  class NetAttributeCtrlServerBodyC
    : public RCBodyVC
  {
  public:
    NetAttributeCtrlServerBodyC(NetEndPointC &nep,const AttributeCtrlC &attrCtrl);
    //: Constructor.

    NetAttributeCtrlServerBodyC(const AttributeCtrlC &attrCtrl);
    //: Constructor.
    
    ~NetAttributeCtrlServerBodyC();
    //: Destructor.
    
    virtual bool Connect(NetEndPointC &nep);
    //: Setup connection to end point.
    
    virtual bool Disconnect();
    //: Disconnect from end point.
    
    bool Close();
    //: Close down and release handles to any attribute controls.
    
  protected:
    void RegisterHandlers();
    //: Register message handlers.
    
    bool HandleGetAttrString(UIntT &reqId,UIntT &ctrlId,StringC &name);
    //: Handle get string attribute.
    
    bool HandleGetAttrInt(UIntT &reqId,UIntT &ctrlId,StringC &name);
    //: Handle get integer attribute.
    
    bool HandleGetAttrReal(UIntT &reqId,UIntT &ctrlId,StringC &name);
    //: Handle get real attribute.
    
    bool HandleGetAttrBool(UIntT &reqId,UIntT &ctrlId,StringC &name);
    //: Handle get boolean attribute.
    
    bool HandleGetAttrTypes(UIntT &reqId,UIntT &ctrlId);
    //: Handle get boolean attribute.
    
    bool HandleSetAttrString(UIntT &ctrlId,StringC &name,StringC &data);
    //: Handle set string attribute.
    
    bool HandleSetAttrInt(UIntT &ctrlId,StringC &name,IntT &data);
    //: Handle set integer attribute.
    
    bool HandleSetAttrReal(UIntT &ctrlId,StringC &name,RealT &data);
    //: Handle set real attribute.
    
    bool HandleSetAttrBool(UIntT &ctrlId,StringC &name,bool &data);
    //: Handle set boolean attribute.
    
    bool HandleSigRegister(UIntT &ctrlId,StringC &name);
    //: Handle set boolean attribute.
    
    bool HandleSigRemove(UIntT &ctrlId,StringC &name);
    //: Handle set boolean attribute.
    
    bool HandleSignal(UIntT &ctrlId,StringC &name);
    //: Handle set boolean attribute.
    
    NetEndPointC ep;
    HashC<UIntT,AttributeCtrlC> attrCtrls;
    HashC<Tuple2C<IntT,StringC>,UIntT> sigIds;
  };
  
  //! userlevel=Normal
  //: Net attribute server.
  
  class NetAttributeCtrlServerC
    : public RCHandleC<NetAttributeCtrlServerBodyC>
  {
  public:
    NetAttributeCtrlServerC()
    {}
    //: Default constructor.
    
    bool Connect(NetEndPointC &nep)
    { return Body().Connect(nep); }
    //: Setup connection to end point.
    
    bool Disconnect()
    { return Body().Disconnect(); }
    //: Disconnect from end point.
    
  protected:
    NetAttributeCtrlServerC(NetAttributeCtrlServerBodyC &bod)
      : RCHandleC<NetAttributeCtrlServerBodyC>(bod)
    {}
    //: Body constructor
    
    NetAttributeCtrlServerBodyC &Body()
    { return RCHandleC<NetAttributeCtrlServerBodyC>::Body(); }
    //: Access body.
    
    const NetAttributeCtrlServerBodyC &Body() const
    { return RCHandleC<NetAttributeCtrlServerBodyC>::Body(); }
    //: Access body.
    
  };
}

#endif
