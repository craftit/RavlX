// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/Port.cc"

#include "Ravl/DP/Port.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/TypeName.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  ////////////////////////////////////////////////////////
    static AttributeTypeStringC idAttribute("id","Port identifier");

  //: Default constructor.
  
  DPPortBodyC::DPPortBodyC()
  {
#if RAVL_CPUTYPE_64
    portId = (StringC("Port-") + StringC((UInt64T) this >> 2)); 
#else
    portId = (StringC("Port-") + StringC((UIntT) this >> 2)); 
#endif
    RegisterID() ; 
}
  
  //: Stream constructor.

  DPPortBodyC::DPPortBodyC(std::istream &in) 
    : DPEntityBodyC(in)
  {
    in >> portId;
    RegisterID() ;
  }

  //: Destructor.
  
  DPPortBodyC::~DPPortBodyC() 
  {}
  
  //: Is this port connected to another ?
  // If not returns invalid handle.
  
  DPPortC DPPortBodyC::ConnectedTo() const { 
    return DPPortC(); 
  }
  
  //: Register ID
  // registers the id with the attriubte ctrl mechanism 
  void DPPortBodyC::RegisterID(void) 
{  RegisterAttribute ( idAttribute ) ;  }


  //: Does port work asynchronously ?
  
  bool DPPortBodyC::IsAsync() const  { 
    return false; 
  }

  //: Save to std::ostream.
  
  bool DPPortBodyC::Save(std::ostream &out) const { 
    if(!DPEntityBodyC::Save(out))
      return false;
    out << portId << ' ';
    return true;
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  
  bool DPPortBodyC::GetAttr(const StringC &attrName,StringC &attrValue) { 
    if(attrName == "id") {
      attrValue = portId;
      return true;
    }
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  
  bool DPPortBodyC::SetAttr(const StringC &attrName,const StringC &attrValue) { 
    if(attrName == "id") {
      portId = attrValue;
      return true;
    }
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::GetAttr(const StringC &attrName,IntT &attrValue) {
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::SetAttr(const StringC &attrName,const IntT &attrValue) {
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::GetAttr(const StringC &attrName,RealT &attrValue) {
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::SetAttr(const StringC &attrName,const RealT &attrValue) {
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }

  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::GetAttr(const StringC &attrName,bool &attrValue) {
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DPPortBodyC::SetAttr(const StringC &attrName,const bool &attrValue) {
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }

  //: Get list of attributes available.
  
  bool DPPortBodyC::GetAttrList(DListC<StringC> &list) const {
    return AttributeCtrlBodyC::GetAttrList(list);
  }

  //: Get Parent attribute control.
  
  AttributeCtrlC DPPortBodyC::ParentCtrl() const 
  { return ConnectedTo(); }
  
  
  //: Get a list of available attribute types.
  
  bool DPPortBodyC::GetAttrTypes(DListC<AttributeTypeC> &list) const {
    return AttributeCtrlBodyC::GetAttrTypes(list);
  }
  
  //: Get type of a particular attribute. 
  // Returns an invalid handle if attribute is unknown.
  AttributeTypeC DPPortBodyC::GetAttrType(const StringC &attrName) const {
    if(attrName == "id")
      return idAttribute;
    return AttributeCtrlBodyC::GetAttrType(attrName);
  }

  /////////////////////////////////////////////////////////
  
  // Input type.
  
  const std::type_info &DPIPortBaseBodyC::InputType() const  {
    RavlAssert(0);
    return typeid(void);
  }
  
  //: Is some data ready ?
  // true = yes.
  
  bool DPIPortBaseBodyC::IsGetReady() const  { 
    return !IsGetEOS(); 
  }
  
  //: Is some data ready ?
  // true = yes.
  
  bool DPIPortBaseBodyC::IsGetEOS() const  { 
    return false; 
  }

  //: Discard the next input datum.
  
  bool DPIPortBaseBodyC::Discard()  { 
    return false; 
  }

  /////////////////////////////////////////////////////////
  
  //: Is port ready for data ?
  
  bool DPOPortBaseBodyC::IsPutReady() const  { 
    return true; 
  }

  //: Put End Of Stream marker.
  
  void DPOPortBaseBodyC::PutEOS()
  {}
  
  // Output type.
  
  const std::type_info &DPOPortBaseBodyC::OutputType() const {
    RavlAssert(0);
    return typeid(void);
  }
}

