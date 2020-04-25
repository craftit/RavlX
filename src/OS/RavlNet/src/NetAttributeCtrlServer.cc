// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetAttributeCtrlServer.cc"

#include "Ravl/OS/NetAttributeCtrlServer.hh"
#include "Ravl/OS/NetAttributeCtrl.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/HashIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  NetAttributeCtrlServerBodyC::NetAttributeCtrlServerBodyC(NetEndPointC &nep,const AttributeCtrlC &nAttrCtrl)
    : ep(nep)
  { 
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::NetAttributeCtrlServerBodyC(NetEndPointC&,AttributeCtrlC&), Called. \n");
    attrCtrls[0] = nAttrCtrl;
    RegisterHandlers(); 
  }

  //: Constructor.
  
  NetAttributeCtrlServerBodyC::NetAttributeCtrlServerBodyC(const AttributeCtrlC &attrCtrl) 
  { 
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::NetAttributeCtrlServerBodyC(const AttributeCtrlC &), Called. \n");
    attrCtrls[0] = attrCtrl; 
  }
  
  //: Destructor.
  
  NetAttributeCtrlServerBodyC::~NetAttributeCtrlServerBodyC() 
  { 
    Close(); 
    if(ep.IsValid())
      ep.Close();
  }
  
  //: Close down and release handles to any attribute controls.
  
  bool NetAttributeCtrlServerBodyC::Close() {
    // Unregister all changed signals.
    for(HashIterC<Tuple2C<IntT,StringC>,UIntT> it(sigIds);it;it++)
      attrCtrls[it.Key().Data1()].RemoveChangedSignal(it.Data());
    return true;
  }

  //: Setup connection to end point.
  
  bool NetAttributeCtrlServerBodyC::Connect(NetEndPointC &nep) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::Connect(), Called. \n");
    if(ep.IsValid()) {
      ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::Connect(), ERROR: Already connected. \n");
      return false;
    }
    ep = nep;
    RavlAssert(ep.IsValid());
    RegisterHandlers(); 
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::Connect(), Done. \n");
    return true;
  }
  
  //: Disconnect from end point.
  
  bool NetAttributeCtrlServerBodyC::Disconnect() {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::Disconnect(), Called. \n");
    // FIXME:- Should disconnect all handlers here...
    ep.Invalidate();    
    return true;
  }
  
  void NetAttributeCtrlServerBodyC::RegisterHandlers() {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::RegisterHandlers(), Called. \n");
    ep.RegisterR((UIntT) NACMsg_GetAttrStr,StringC("GetAttrString"),*this,&NetAttributeCtrlServerBodyC::HandleGetAttrString);
    ep.RegisterR((UIntT) NACMsg_GetAttrInt,StringC("GetAttrInt"),*this,&NetAttributeCtrlServerBodyC::HandleGetAttrInt);
    ep.RegisterR((UIntT) NACMsg_GetAttrReal,StringC("GetAttrReal"),*this,&NetAttributeCtrlServerBodyC::HandleGetAttrReal);
    ep.RegisterR((UIntT) NACMsg_GetAttrBool,StringC("GetAttrBool"),*this,&NetAttributeCtrlServerBodyC::HandleGetAttrBool);
    ep.RegisterR((UIntT) NACMsg_GetAttrTypes,StringC("GetAttrTypes"),*this,&NetAttributeCtrlServerBodyC::HandleGetAttrTypes);
    ep.RegisterR((UIntT) NACMsg_SetAttrStr,StringC("SetAttrString"),*this,&NetAttributeCtrlServerBodyC::HandleSetAttrString);
    ep.RegisterR((UIntT) NACMsg_SetAttrInt,StringC("SetAttrInt"),*this,&NetAttributeCtrlServerBodyC::HandleSetAttrInt);
    ep.RegisterR((UIntT) NACMsg_SetAttrReal,StringC("SetAttrReal"),*this,&NetAttributeCtrlServerBodyC::HandleSetAttrReal);
    ep.RegisterR((UIntT) NACMsg_SetAttrBool,StringC("SetAttrBool"),*this,&NetAttributeCtrlServerBodyC::HandleSetAttrBool);    
    ep.RegisterR((UIntT) NACMsg_SigRegister,StringC("SigRegister"),*this,&NetAttributeCtrlServerBodyC::HandleSigRegister);
    ep.RegisterR((UIntT) NACMsg_SigRemove,StringC("SigRemove"),*this,&NetAttributeCtrlServerBodyC::HandleSigRemove);
    
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::RegisterHandlers(), Done. \n"); 
  }

  static StringC errMsgInvalidAttrCtrl("Invalid AttributeCtrl id. ");
  static StringC errMsgGetRetFalse("Get returned false ");
  
  //: Handle get string attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleGetAttrString(UIntT &reqId,UIntT &ctrlId,StringC &name) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleGetAttrString(), Called. ReqId="<< reqId << " CtrlId=" << ctrlId << " Name='" << name << "'\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgInvalidAttrCtrl);
      return true;
    }
    // Retrieve value
    StringC value;
    if(!ctrl.GetAttr(name,value)) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgGetRetFalse);
      return true;
    }
    // Send back result
    ep.Send(NACMsg_GetAttrStr,reqId,value);
    return true;
  }
  
  //: Handle get integer attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleGetAttrInt(UIntT &reqId,UIntT &ctrlId,StringC &name) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleGetAttrInt(), Called. ReqId="<< reqId << " CtrlId=" << ctrlId << " Name='" << name << "'\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgInvalidAttrCtrl);
      return true;
    }
    // Retrieve value
    IntT value;
    if(!ctrl.GetAttr(name,value)) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgGetRetFalse);
      return true;
    }
    // Send back result
    ep.Send(NACMsg_GetAttrInt,reqId,value);
    return true;
  }
  
  //: Handle get real attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleGetAttrReal(UIntT &reqId,UIntT &ctrlId,StringC &name) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleGetAttrReal(), Called. ReqId="<< reqId << " CtrlId=" << ctrlId << " Name='" << name << "'\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgInvalidAttrCtrl);
      return true;
    }
    // Retrieve value
    RealT value;
    if(!ctrl.GetAttr(name,value)) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgGetRetFalse);
      return true;
    }
    // Send back result
    ep.Send(NACMsg_GetAttrReal,reqId,value);
    return true;
  }
  
  //: Handle get boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleGetAttrBool(UIntT &reqId,UIntT &ctrlId,StringC &name) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleGetAttrBool(), Called. ReqId="<< reqId << " CtrlId=" << ctrlId << " Name='" << name << "'\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgInvalidAttrCtrl);
      return true;
    }
    // Retrieve value
    RealT value;
    if(!ctrl.GetAttr(name,value)) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgGetRetFalse);
      return true;
    }
    // Send back result
    ep.Send(NACMsg_GetAttrReal,reqId,value);
    return true;
  }
  
  //: Handle get boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleGetAttrTypes(UIntT &reqId,UIntT &ctrlId) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleGetAttrTypes(), Called. ReqId="<< reqId << " CtrlId=" << ctrlId << " \n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgInvalidAttrCtrl);
      return true;
    }
    // Retrieve value
    DListC<AttributeTypeC> value;
    if(!ctrl.GetAttrTypes(value)) {
      ep.Send(NACMsg_GetFailed,reqId,errMsgGetRetFalse);
      return true;
    }
    // Send back result
    ep.Send(NACMsg_GetAttrTypes,reqId,value);
    return true;
  }
  
  //: Handle set string attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSetAttrString(UIntT &ctrlId,StringC &name,StringC &data) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleSetAttrString(), Called. CtrlId=" << ctrlId << " Name='" << name << "' Value=" << data << "\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlServerBodyC::HandleSetAttrString(), Invalid CtrlId. ";
      return true;
    }
    if(!ctrl.SetAttr(name,data))
      RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSetAttrString(), Failed to set '"<< name << "' to '" << data << "'";
    return true;
  }
  
  //: Handle set integer attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSetAttrInt(UIntT &ctrlId,StringC &name,IntT &data) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleSetAttrInt(), Called. CtrlId=" << ctrlId << " Name='" << name << "' Value=" << data << "\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlServerBodyC::HandleSetAttrInt(), Invalid CtrlId. \n";
      return true;
    }
    if(!ctrl.SetAttr(name,data))
      RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSetAttrInt(), Failed to set '"<< name << "' to '" << data << "'";
    return true;
  }
  
  //: Handle set real attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSetAttrReal(UIntT &ctrlId,StringC &name,RealT &data) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleSetAttrReal(), Called. CtrlId=" << ctrlId << " Name='" << name << "' Value=" << data << "\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlServerBodyC::HandleSetAttrReal(), Invalid CtrlId. \n";
      return true;
    }
    if(!ctrl.SetAttr(name,data))
      RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSetAttrReal(), Failed to set '"<< name << "' to '" << data << "'";
    return true;
  }
  
  //: Handle set boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSetAttrBool(UIntT &ctrlId,StringC &name,bool &data) {
    ONDEBUG(cerr<< "NetAttributeCtrlServerBodyC::HandleSetAttrBool(), Called. CtrlId=" << ctrlId << " Name='" << name << "' Value=" << data << "\n");
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlServerBodyC::HandleSetAttrBool(), Invalid CtrlId. \n";
      return true;
    }
    if(!ctrl.SetAttr(name,data))
      RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSetAttrBool(), Failed to set '"<< name << "' to '" << ((IntT) data) << "'";
    return true;
  }
  
  //: Handle set boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSigRegister(UIntT &ctrlId,StringC &name) {
    Tuple2C<IntT,StringC> sigId(ctrlId,name);
    if(sigIds.IsElm(sigId))
      return true; // Already sorted.
    AttributeCtrlC ctrl;
    // Lookup attribute controller
    if(!attrCtrls.Lookup(ctrlId,ctrl) || !ctrl.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetAttributeCtrlServerBodyC::HandleSigRegister(), Invalid CtrlId. \n";
      return true;
    }
    IntT id = ctrl.RegisterChangedSignal(name,TriggerR(*this,&NetAttributeCtrlServerBodyC::HandleSignal,ctrlId,name));
    if(id < 0) {
      RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSigRegister(), Failed to setup signal for attribute '" << name << "' CtrlId=" << ctrlId << " ";
      return true;
    }
    sigIds[sigId] = id;
    return true;
  }
  
  //: Handle set boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSigRemove(UIntT &ctrlId,StringC &name) {
    RavlSysLog(SYSLOG_WARNING) << "NetAttributeCtrlServerBodyC::HandleSigRemove(), Not implemented. ";
    return true;
  }
  
  //: Handle set boolean attribute.
  
  bool NetAttributeCtrlServerBodyC::HandleSignal(UIntT &ctrlId,StringC &name) {
    // Send back result
    ep.Send(NACMsg_ChangedSignal,ctrlId,name);    
    return true;
  }
  
}
