// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! author="Charles Galambos"
//! file="Ravl/OS/Network/NetAttributeCtrl.cc"

#include "Ravl/OS/NetAttributeCtrl.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Default constructor.
  
  NetAttributeCtrlBodyC::NetAttributeCtrlBodyC()
    : ctrlId(0),
      reqManager(false)
  {}

  //: Constructor.
  
  NetAttributeCtrlBodyC::NetAttributeCtrlBodyC(const NetEndPointC &nep,UIntT nCtrlId) 
    : ctrlId(nCtrlId),
      reqManager(false)
  { Connect(nep); }
  
  //: Constructor.
  
  NetAttributeCtrlBodyC::NetAttributeCtrlBodyC(UIntT nCtrlId)
    : ctrlId(nCtrlId),
      reqManager(false)
  {}

  //: Destructor.
  
  NetAttributeCtrlBodyC::~NetAttributeCtrlBodyC() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::~NetAttributeCtrlBodyC(), Called.");
    sigConnectionSet.DisconnectAll(true);
  }
  
  //: Connect to an end point.
  
  bool NetAttributeCtrlBodyC::Connect(const NetEndPointC &nep) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::Connect(const NetEndPointC &), Called.");
    if(ep.IsValid())
      return false;
    ep = nep;
    
    // Register method to call on connection closed.
    sigConnectionSet += ConnectRef(ep.SigConnectionBroken(),*this,&NetAttributeCtrlBodyC::ConnectionClosed);
    
    RegisterHandlers();
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::Connect(const NetEndPointC &), Done.");
    return true;
  }
  
  //: Register handler functions.
  
  void NetAttributeCtrlBodyC::RegisterHandlers() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::RegisterHandlers(), Called.");
    ep.RegisterR((UIntT) NACMsg_GetAttrStr,StringC("GetAttrString"),*this,&NetAttributeCtrlBodyC::HandleGetAttrString);
    ep.RegisterR((UIntT) NACMsg_GetAttrInt,StringC("GetAttrInt"),*this,&NetAttributeCtrlBodyC::HandleGetAttrInt);
    ep.RegisterR((UIntT) NACMsg_GetAttrReal,StringC("GetAttrReal"),*this,&NetAttributeCtrlBodyC::HandleGetAttrReal);
    ep.RegisterR((UIntT) NACMsg_GetAttrBool,StringC("GetAttrBool"),*this,&NetAttributeCtrlBodyC::HandleGetAttrBool);
    ep.RegisterR((UIntT) NACMsg_GetAttrTypes,StringC("GetAttrTypes"),*this,&NetAttributeCtrlBodyC::HandleGetAttrTypes);
    ep.RegisterR((UIntT) NACMsg_GetFailed,StringC("GetFailed"),*this,&NetAttributeCtrlBodyC::HandleGetFailed);
    ep.RegisterR((UIntT) NACMsg_ChangedSignal,StringC("ChangedSignal"),*this,&NetAttributeCtrlBodyC::HandleSignal);
  }

  //: Handler for connection lost signal.
  
  bool NetAttributeCtrlBodyC::ConnectionClosed() {
    ep.Close();
    reqManager.ConnectionClosed();
    return true;
  }
  
  //: Request available attribute types.
  
  void NetAttributeCtrlBodyC::UpdateAttributeTypes() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::UpdateAttributeTypes(), Called.");
    UIntT id;
    reqManager.CreateReq(id); // Create request.
    DListC<AttributeTypeC> attrs;
    ep.Send(NACMsg_GetAttrTypes,ctrlId,id); // Send message.
    if(!reqManager.WaitForReq(id,attrs)) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlBodyC::UpdateAttributeTypes(), Failed. ";
      return ;
    }
    for(DLIterC<AttributeTypeC> it(attrs);it;it++)
      AttributeCtrlBodyC::RegisterAttribute(*it);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetAttributeCtrlBodyC::UpdateAttributeTypes(), Done.");
  }
  
  //: Get a attribute.
  
  bool NetAttributeCtrlBodyC::GetAttr(const StringC &attrName,StringC &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanRead())
      return false;
    UIntT id;
    reqManager.CreateReq(id); // Create request.
    ep.Send(NACMsg_GetAttrStr,id,ctrlId,attrName); // Send message.
    return reqManager.WaitForReq(id,attrValue); // Wait for reply.
  }
  
  //: Set a attribute.
  
  bool NetAttributeCtrlBodyC::SetAttr(const StringC &attrName,const StringC &attrValue){
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanWrite())
      return false;
    ep.Send(NACMsg_SetAttrStr,ctrlId,attrName,attrValue);
    return true;
  }
  
  //: Get a attribute.
  
  bool NetAttributeCtrlBodyC::GetAttr(const StringC &attrName,IntT &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanRead())
      return false;
    UIntT id;
    reqManager.CreateReq(id); // Create request.
    ep.Send(NACMsg_GetAttrInt,id,ctrlId,attrName); // Send message.
    return reqManager.WaitForReq(id,attrValue); // Wait for reply.
  }
  
  //: Set a attribute.
  
  bool NetAttributeCtrlBodyC::SetAttr(const StringC &attrName,const IntT &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanWrite())
      return false;
    ep.Send(NACMsg_SetAttrInt,ctrlId,attrName,attrValue);
    return true;
  }
  
  //: Get a attribute.
  
  bool NetAttributeCtrlBodyC::GetAttr(const StringC &attrName,RealT &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanRead())
      return false;
    UIntT id;
    reqManager.CreateReq(id); // Create request.
    ep.Send(NACMsg_GetAttrReal,id,ctrlId,attrName); // Send message.
    return reqManager.WaitForReq(id,attrValue); // Wait for reply.
  }
  
  //: Set a attribute.
  
  bool NetAttributeCtrlBodyC::SetAttr(const StringC &attrName,const RealT &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanWrite())
      return false;
    ep.Send(NACMsg_SetAttrReal,ctrlId,attrName,attrValue);
    return true;
  }
  
  //: Get a attribute.
  
  bool NetAttributeCtrlBodyC::GetAttr(const StringC &attrName,bool &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanRead())
      return false;
    UIntT id;
    reqManager.CreateReq(id); // Create request.
    ep.Send(NACMsg_GetAttrBool,id,ctrlId,attrName); // Send message.
    return reqManager.WaitForReq(id,attrValue); // Wait for reply.
  }
  
  //: Set a attribute.
  
  bool NetAttributeCtrlBodyC::SetAttr(const StringC &attrName,const bool &attrValue) {
    AttributeTypeC at = AttributeCtrlBodyC::GetAttrType(attrName);
    if(!at.IsValid() || !at.CanWrite())
      return false;
    ep.Send(NACMsg_SetAttrBool,ctrlId,attrName,attrValue);
    return true;
  }
  
  //: Get list of attributes available.
  
  bool NetAttributeCtrlBodyC::GetAttrList(DListC<StringC> &list) const {
    return AttributeCtrlBodyC::GetAttrList(list);
  }
  
  //: Get a list of available attribute types.
  
  bool NetAttributeCtrlBodyC::GetAttrTypes(DListC<AttributeTypeC> &list) const {
    return AttributeCtrlBodyC::GetAttrTypes(list);
  }
  
  //: Get type of a particular attribute. 
  
  AttributeTypeC NetAttributeCtrlBodyC::GetAttrType(const StringC &attrName) const {
    return AttributeCtrlBodyC::GetAttrType(attrName);
  }
  
  //: Register a value changed signal.
  
  IntT NetAttributeCtrlBodyC::RegisterChangedSignal(const StringC &attrName,const TriggerC &trig) {
    ep.Send(NACMsg_SigRegister,ctrlId,attrName); // Request signals.
    return AttributeCtrlBodyC::RegisterChangedSignal(attrName,trig);
  }
  
  //: Remove a changed signal.
  
  bool NetAttributeCtrlBodyC::RemoveChangedSignal(IntT id) {
    // FIXME:- Should check if we still want changed signals ?
    return AttributeCtrlBodyC::RemoveChangedSignal(id);
  }
  
  //: Register a new attribute type.
  
  bool NetAttributeCtrlBodyC::RegisterAttribute(const AttributeTypeC &attr) {
    RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlBodyC::RegisterAttribute(), Attempt to register attribute on network port.";
    return AttributeCtrlBodyC::RegisterAttribute(attr);
  }
  
  //: Handle get failed
  
  bool NetAttributeCtrlBodyC::HandleGetFailed(UIntT &reqId,StringC &msg) {
    RCWrapAbstractC invalid;
    RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlBodyC::HandleGetFailed(), Msg=" << msg;
    reqManager.DeliverReq(reqId,invalid);
    return true;
  }
  
  //: Handle get string attribute.
  
  bool NetAttributeCtrlBodyC::HandleGetAttrString(UIntT &reqId,StringC &data) {
    reqManager.DeliverReq(reqId,data);
    return true;
  }
  
  //: Handle get integer attribute.
  
  bool NetAttributeCtrlBodyC::HandleGetAttrInt(UIntT &reqId,IntT &data) {
    reqManager.DeliverReq(reqId,data);
    return true;
  }
  
  //: Handle get real attribute.
  
  bool NetAttributeCtrlBodyC::HandleGetAttrReal(UIntT &reqId,RealT &data) {
    reqManager.DeliverReq(reqId,data);
    return true;
  }
  
  //: Handle get boolean attribute.
  
  bool NetAttributeCtrlBodyC::HandleGetAttrBool(UIntT &reqId,bool &data) {
    reqManager.DeliverReq(reqId,data);
    return true;
  }
  
  //: Handle get boolean attribute.
  
  bool NetAttributeCtrlBodyC::HandleGetAttrTypes(UIntT &reqId,DListC<AttributeTypeC> &data) {
    reqManager.DeliverReq(reqId,data);
    return true;    
  }

  //: Handle changed signal
  
  bool NetAttributeCtrlBodyC::HandleSignal(UIntT &sigCtrlId,StringC &name) {
    if(ctrlId != sigCtrlId)
      return true; // Not for me! 
    SignalChange(name);
    return true;
  }
  
}
