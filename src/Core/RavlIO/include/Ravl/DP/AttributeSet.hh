// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ATTRIBUTESET_HEADER
#define RAVL_ATTRIBUTESET_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data_Processing.Attributes"
//! file="Ravl/Core/IO/AttributeSet.hh"

#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Set of attribute values.
  
  class AttributeSetBodyC
    : public AttributeCtrlBodyC 
  {
  public:
    AttributeSetBodyC(bool enforceSchema = false);
    //: Default constructor
    // Creates an empty attribute set.
    
    AttributeSetBodyC(AttributeCtrlC &attr,bool copySettings,bool enforceSchema = false);
    //: Copy schema from 'attr' and optionally current settings.
    // If current settings aren't copied then the values will be 
    // set to their defaults.
    
    bool SetSchema(const DListC<AttributeTypeC> &attrTypes);
    //: Set attribute schema to use.
    
    bool CopyFrom(AttributeCtrlC &ctrl);
    //: Copy settings from 'ctrl'.
    
    bool CopyTo(AttributeCtrlC &ctrl) const;
    //: Copy settings to 'ctrl'.
    
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

  protected:
    bool UpdateValue(const StringC &attrName,const StringC &newValue);
    //: Update value in table.
    // Issue changed signal, if value has been changed.
    
    HashC<StringC,StringC> values;
    bool enforceSchema;
  };
  
  
  //! userlevel=Normal
  //: Set of attribute values.
  
  class AttributeSetC
    : public AttributeCtrlC 
  {
  public:
    AttributeSetC()
      : DPEntityC(true)
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    AttributeSetC(bool enforceSchema)
      : DPEntityC(*new AttributeSetBodyC(enforceSchema))
    {}
    //: Create an empty attribute set.
    // If enforceSchema is false, new attribute types will be automaticly created 
    // for unknow attribute types.
    
    AttributeSetC(AttributeCtrlC &attr,bool copySettings,bool enforceSchema = false)
      : DPEntityC(*new AttributeSetBodyC(attr,copySettings,enforceSchema))
    {}
    //: Copy schema from 'attr' and optionally current settings.
    // If current settings aren't copied then the values will be 
    // set to their defaults.
    
  protected:
    AttributeSetC(AttributeSetBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    AttributeSetBodyC &Body()
    { return dynamic_cast<AttributeSetBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const AttributeSetBodyC &Body() const
    { return dynamic_cast<const AttributeSetBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool SetSchema(const DListC<AttributeTypeC> &attrTypes)
    { return Body().SetSchema(attrTypes); }
    //: Set attribute schema to use.
    
    bool CopyFrom(AttributeCtrlC &ctrl)
    { return Body().CopyFrom(ctrl); }
    //: Copy settings from 'ctrl'.
    
    bool CopyTo(AttributeCtrlC &ctrl)
    { return Body().CopyTo(ctrl); }
    //: Copy settings to 'ctrl'.
    
  };
}


#endif
