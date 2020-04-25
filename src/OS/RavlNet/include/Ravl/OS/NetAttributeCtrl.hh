// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETATTRIBUTECTRL_HEADER
#define RAVL_NETATTRIBUTECTRL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlNet
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Network.NetPort"
//! file="Ravl/OS/Network/NetAttributeCtrl.hh"

#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/OS/NetRequestManager.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"

namespace RavlN {
  class NetAttributeCtrlC;
  
  enum NACMsgT {
    NACMsg_GetAttrTypes  = 30,
    NACMsg_SetAttrStr    = 31,
    NACMsg_GetAttrStr    = 32,
    NACMsg_SetAttrInt    = 33,
    NACMsg_GetAttrInt    = 34,
    NACMsg_SetAttrReal   = 35,
    NACMsg_GetAttrReal   = 36,
    NACMsg_SetAttrBool   = 37,
    NACMsg_GetAttrBool   = 38,
    NACMsg_GetFailed     = 39,
    NACMsg_SigRegister   = 40,
    NACMsg_SigRemove     = 41,
    NACMsg_ChangedSignal = 42
  };
  
  //: Messages used in NetPorts.
  
  class NetAttributeCtrlBodyC
    : public AttributeCtrlBodyC
  {
  public:
    NetAttributeCtrlBodyC();
    //: Default constructor.
    
    NetAttributeCtrlBodyC(const NetEndPointC &ep,UIntT ctrlId);
    //: Constructor.
    
    NetAttributeCtrlBodyC(UIntT ctrlId);
    //: Constructor.

    ~NetAttributeCtrlBodyC();
    //: Destructor.
    
    bool Connect(const NetEndPointC &ep);
    //: Connect to an end point.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttrList(DListC<StringC> &list) const;
    //: Get list of attributes available.
    // This method will ADD all available attribute names to 'list'.
    
    virtual bool GetAttrTypes(DListC<AttributeTypeC> &list) const;
    //: Get a list of available attribute types.
    
    virtual AttributeTypeC GetAttrType(const StringC &attrName) const;
    //: Get type of a particular attribute. 
    // Returns an invalid handle if attribute is unknown.
    
    virtual IntT RegisterChangedSignal(const StringC &attrName,const TriggerC &trig);
    //: Register a value changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    // Returns an id for the trigger, or -1 if operation fails.
    
    virtual bool RemoveChangedSignal(IntT id);
    //: Remove a changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    
    virtual bool RegisterAttribute(const AttributeTypeC &attr);
    //: Register a new attribute type.
    
    virtual bool ConnectionClosed();
    //: Handler for connection lost signal.
    
    void UpdateAttributeTypes();
    //: Request available attribute types.
  protected:
    void RegisterHandlers();
    //: Register handler functions.
    
    bool HandleGetFailed(UIntT &reqId,StringC &msg);
    //: Handle get failed
    
    bool HandleGetAttrString(UIntT &reqId,StringC &data);
    //: Handle get string attribute.
    
    bool HandleGetAttrInt(UIntT &reqId,IntT &data);
    //: Handle get integer attribute.
    
    bool HandleGetAttrReal(UIntT &reqId,RealT &data);
    //: Handle get real attribute.
    
    bool HandleGetAttrBool(UIntT &reqId,bool &data);
    //: Handle get boolean attribute.
    
    bool HandleGetAttrTypes(UIntT &reqId,DListC<AttributeTypeC> &data);
    //: Handle get boolean attribute.
    
    bool HandleSignal(UIntT &CtrlId,StringC &name);
    //: Handle changed signal
    
    NetEndPointC ep;
    UIntT ctrlId;
    NetRequestManagerC reqManager;
    SignalConnectionSetC sigConnectionSet;
    friend class NetAttributeCtrlC;
  };

  //! userlevel=Normal
  //: Networked attribute ctrl.
  
  class NetAttributeCtrlC
    : public AttributeCtrlC
  {
  public:
    NetAttributeCtrlC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle
    
    NetAttributeCtrlC(const NetEndPointC &ep,UIntT ctrlId)
      : DPEntityC(*new NetAttributeCtrlBodyC(ep,ctrlId))
    {}
    //: Constructor.
    //!param: ep - Connection to use for commincation.
    //!param: ctrlId - Id for AttributeCtrlC on server, defaults to 0.
    
    NetAttributeCtrlC(UIntT ctrlId)
      : DPEntityC(*new NetAttributeCtrlBodyC(ctrlId))
    {}
    //: Constructor.
    //!param: ctrlId - Id for AttributeCtrlC on server, defaults to 0.
    
    bool Connect(const NetEndPointC &ep)
    { return Body().Connect(ep); }
    //: Connect to an end point.
    //!param: ep - Connection to use for commincation.
    
    void UpdateAttributeTypes()
    { return Body().UpdateAttributeTypes(); }
    //: Request available attribute types.
    
    bool ConnectionClosed()
    { return Body().ConnectionClosed(); }
    //: Handler for connection lost signal.
    
  protected:
    NetAttributeCtrlBodyC &Body()
    { return dynamic_cast<NetAttributeCtrlBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const NetAttributeCtrlBodyC &Body() const
    { return dynamic_cast<const NetAttributeCtrlBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    bool HandleGetAttrString(UIntT &reqId,StringC &data)
    { return Body().HandleGetAttrString(reqId,data); }
    //: Handle get string attribute.

    bool HandleGetAttrInt(UIntT &reqId,IntT &data)
    { return Body().HandleGetAttrInt(reqId,data); }
    //: Handle get integer attribute.
    
    bool HandleGetAttrReal(UIntT &reqId,RealT &data)
    { return Body().HandleGetAttrReal(reqId,data); }
    //: Handle get real attribute.
    
    bool HandleGetAttrBool(UIntT &reqId,bool &data)
    { return Body().HandleGetAttrBool(reqId,data); }
    //: Handle get boolean attribute.
    
    bool HandleGetAttrTypes(UIntT &reqId,DListC<AttributeTypeC> &data)
    { return Body().HandleGetAttrTypes(reqId,data); }
    //: Handle get boolean attribute.
    
    friend class NetAttributeCtrlBodyC;
  };
  
}


#endif
