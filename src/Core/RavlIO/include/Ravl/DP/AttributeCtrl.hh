// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ATTRIBUTECTRL_HEADER
#define RAVL_ATTRIBUTECTRL_HEADER 1
//! author="Charles Galambos"
//! date="23/8/2003"
//! lib=RavlIO
//! file="Ravl/Core/IO/AttributeCtrl.hh"
//! docentry="Ravl.API.Core.Data_Processing.Attributes"

#include "Ravl/DP/Entity.hh"
#include "Ravl/String.hh"

namespace RavlN {
  class AttributeTypeC;
  class AttributeCtrlC;
  class TriggerC;
  template<class DataT> class DListC;
  class AttributeCtrlInternalC;
  class AttributeSetBodyC;
  class XMLIStreamC;
  class XMLOStreamC;
  
  //! userlevel=Develop
  //: Interface for reading and writing attribute values.
  
  class AttributeCtrlBodyC 
    : virtual public DPEntityBodyC 
  {
  public:
    AttributeCtrlBodyC();
    //: Default constructor.
    
    AttributeCtrlBodyC(std::istream &in);
    //: Stream constructor.
    
    AttributeCtrlBodyC(const AttributeCtrlBodyC &bod);
    //: Copy constructor.
    
    virtual ~AttributeCtrlBodyC();
    //: Destructor.
    
    AttributeCtrlBodyC &operator=(const AttributeCtrlBodyC &);
    //: Assignment operator.
    
    virtual AttributeCtrlC ParentCtrl() const;
    //: Get Parent attribute control.

    StringC GetAttr(const StringC &attrName);
    //: Get named attribute.
    
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

    virtual bool GetAttr(const StringC &attrName,Int64T &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const Int64T &attrValue);
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
    
    virtual bool GetComponent(const StringC &attrName,AttributeCtrlC &attr);
    //: Get a sub component.
    // Returns false if the attribute name is unknown.
    
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
    
    virtual bool MapBackChangedSignal(const StringC &name);
    //: Map attribute changed signal to parent even if its an attribute at this level.
    
    virtual bool RegisterAttribute(const AttributeTypeC &attr);
    //: Register a new attribute type.
    
    bool SaveAttributes(XMLOStreamC &strm,bool useEnclosingTag = true) const;
    //: Save setup to XML stream.
    
    bool LoadAttributes(XMLIStreamC &strm,bool expectEnclosingTag = true);
    //: Load setup from XML stream.
    
    bool RestoreDefaults();
    //: Set all attributes to default values.
    
    AttributeCtrlC AttributeCtrl();
    //: Access this access control as a handle object.
    
  protected:
    virtual bool SignalChange(const StringC &attrName);
    //: Signal that an attribute has changed.
    
    bool ReparentAttributeCtrl(const AttributeCtrlC &newParent);
    //: Let the attribute ctrl know its parent has changed.
    
    bool SignalAttributeRefresh();
    //: Signal refresh for all registered attributes.
    
  private:
    AttributeCtrlInternalC *attrInfo;
    
    friend class AttributeSetBodyC;
  };
  
  //! userlevel=Normal
  //: Interface for reading and writing attribute values.
  
  class AttributeCtrlC 
    : virtual public DPEntityC 
  {
  public:   
    AttributeCtrlC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    AttributeCtrlC(std::istream &strm)
      : DPEntityC(strm)
    {}
    //: Stream constructor.
    
    AttributeCtrlC(const DPEntityC &base)
      : DPEntityC(dynamic_cast<const AttributeCtrlBodyC *>(BodyPtr(base)))
    {}
    //: Create from base handle.
    // Handle is invalid if object is not derived from AttributeCtrlC.
    
  protected:
    AttributeCtrlC(AttributeCtrlBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    AttributeCtrlC(const AttributeCtrlBodyC *bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    inline AttributeCtrlBodyC &Body() 
    { return dynamic_cast<AttributeCtrlBodyC &> (DPEntityC::Body()); }
    //: Access body.
    
    inline const AttributeCtrlBodyC &Body() const
    { return dynamic_cast<const AttributeCtrlBodyC &> (DPEntityC::Body()); }
    //: Access body.
    
  public:
    StringC GetAttr(const StringC &attrName);
    //: Get a attribute.
    // Return the value of an attribute or an empty string if its unkown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool GetAttr(const StringC &attrName,StringC &attrValue)
    { return Body().GetAttr(attrName,attrValue); }    
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool SetAttr(const StringC &attrName,const StringC &attrValue)
    { return Body().SetAttr(attrName,attrValue); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool SetAttr(const StringC &attrName,const char * value)
    { return Body().SetAttr(attrName,StringC(value)); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool GetAttr(const StringC &attrName,IntT &attrValue)
    { return Body().GetAttr(attrName,attrValue); }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

    inline bool SetAttr(const StringC &attrName,const IntT &attrValue)
    { return Body().SetAttr(attrName,attrValue); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

    inline bool GetAttr(const StringC &attrName,Int64T &attrValue)
    { return Body().GetAttr(attrName,attrValue); }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

    inline bool SetAttr(const StringC &attrName,const Int64T &attrValue)
    { return Body().SetAttr(attrName,attrValue); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.

    inline bool GetAttr(const StringC &attrName,RealT &attrValue)
    { return Body().GetAttr(attrName,attrValue); }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool SetAttr(const StringC &attrName,const RealT &attrValue)
    { return Body().SetAttr(attrName,attrValue); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool GetAttr(const StringC &attrName,bool &attrValue)
    { return Body().GetAttr(attrName,attrValue); }
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool SetAttr(const StringC &attrName,const bool &attrValue)
    { return Body().SetAttr(attrName,attrValue); }
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    inline bool GetComponent(const StringC &attrName,AttributeCtrlC &attr)
    { return Body().GetComponent(attrName,attr); }
    //: Get a sub component.
    // Returns false if the attribute name is unknown.
    
    inline bool GetAttrList(DListC<StringC> &list) const
    { return Body().GetAttrList(list); }
    //: Get list of attributes available.
    
    inline bool GetAttrTypes(DListC<AttributeTypeC> &list) const
    { return Body().GetAttrTypes(list); }
    //: Get a list of available attribute types.
    
    AttributeTypeC GetAttrType(const StringC &attrName) const;
    //: Get type of a particular attribute. 
    // Returns an invalid handle if attribute is unknown.
    
    IntT RegisterChangedSignal(const StringC &attrName,const TriggerC &trig)
    { return Body().RegisterChangedSignal(attrName,trig); }
    //: Register a value changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    // Returns an id for the trigger, or -1 if operation fails.
    
    bool RemoveChangedSignal(IntT id)
    { return Body().RemoveChangedSignal(id); }
    //: Remove a changed signal.
    // Note: This method may not be implemented for all AttributeCtrl's.
    
    bool MapBackChangedSignal(const StringC &name)
    { return Body().MapBackChangedSignal(name); }
    //: Map attribute changed signal to parent even if its an attribute at this level.
    
    bool RegisterAttribute(const AttributeTypeC &attr) 
    { return Body().RegisterAttribute(attr); }
    //: Register a new attribute type.
    
    bool SaveAttributes(XMLOStreamC &strm,bool useEnclosingTag = true) const
    { return Body().SaveAttributes(strm,useEnclosingTag); }
    //: Save setup to XML stream.
    
    bool LoadAttributes(XMLIStreamC &strm,bool expectEnclosingTag = true)
    { return Body().LoadAttributes(strm,expectEnclosingTag); }
    //: Load setup from XML stream.
    
    bool RestoreDefaults()
    { return Body().RestoreDefaults(); }
    //: Set all attributes to default values.
    
    friend class AttributeCtrlBodyC;
  };
}


#endif
