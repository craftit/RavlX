// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/AttributeValueTypes.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: Constructor
  
  AttributeTypeBoolBodyC::AttributeTypeBoolBodyC(const StringC &name,const StringC &desc,bool nCanRead,bool nCanWrite,bool ndefault)
    : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite),
      defaultValue(ndefault)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeBoolBodyC::AttributeTypeBoolBodyC(BinIStreamC &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue; }
  
  //: stream constructor.
  
  AttributeTypeBoolBodyC::AttributeTypeBoolBodyC(std::istream &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue; }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeBoolBodyC::Save(std::ostream & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << ' ' << defaultValue;
    return true;
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeBoolBodyC::Save(BinOStreamC & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << defaultValue;
    return true;
  }
  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeBoolBodyC::ValueType() const
  { return AVT_Bool; }
  
  //: Set control to default value.
  
  bool AttributeTypeBoolBodyC::SetToDefault(AttributeCtrlC &ctrl) const
  { return ctrl.SetAttr(name,defaultValue); }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AttributeTypeBoolBodyC,AttributeTypeBoolC,AttributeTypeC);
  
  //:----------------------------------------------------------------------------------------------------
  
  //: Constructor
  
  AttributeTypeStringBodyC::AttributeTypeStringBodyC(const StringC &name,const StringC &desc,bool nCanRead,bool nCanWrite,const StringC &ndefault,IntT nMaxLen)
    : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite),
      defaultValue(ndefault),
      maxLength(nMaxLen)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeStringBodyC::AttributeTypeStringBodyC(BinIStreamC &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue >> maxLength; }
  
  //: stream constructor.

  AttributeTypeStringBodyC::AttributeTypeStringBodyC(std::istream &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue >> maxLength; }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeStringBodyC::Save(std::ostream & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << ' ' << defaultValue << ' ' << maxLength;
    return true;
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeStringBodyC::Save(BinOStreamC & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << defaultValue << maxLength;
    return true;
  }
  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeStringBodyC::ValueType() const
  { return AVT_String; }

  //: Set control to default value.
  
  bool AttributeTypeStringBodyC::SetToDefault(AttributeCtrlC &ctrl) const
  { return ctrl.SetAttr(name,defaultValue); }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AttributeTypeStringBodyC,AttributeTypeStringC,AttributeTypeC);
  
  //:----------------------------------------------------------------------------------------------------
  
  //: Constructor
  
  AttributeTypeEnumBodyC::AttributeTypeEnumBodyC(const StringC &name,const StringC &desc,bool nCanRead,bool nCanWrite,
						 const DListC<StringC> &nvalues,const StringC &ndefaultValue)
    : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite),
      defaultValue(ndefaultValue),
      values(nvalues)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeEnumBodyC::AttributeTypeEnumBodyC(BinIStreamC &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue >> values; }
  
  //: stream constructor.
  
  AttributeTypeEnumBodyC::AttributeTypeEnumBodyC(std::istream &is)
    : AttributeTypeBodyC(is)
  { is >> defaultValue >> values; }
    
  //: Save the attribute to a stream 
  
  bool AttributeTypeEnumBodyC::Save(std::ostream & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << ' ' << defaultValue << ' ' << values;
    return true;
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeEnumBodyC::Save(BinOStreamC & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << defaultValue << values;
    return true;
  }
  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeEnumBodyC::ValueType() const
  { return AVT_Enum; }
  
  bool AttributeTypeEnumBodyC::SetToDefault(AttributeCtrlC &ctrl) const
  { return ctrl.SetAttr(name,defaultValue); }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AttributeTypeEnumBodyC,AttributeTypeEnumC,AttributeTypeC);
  
  //:----------------------------------------------------------------------------------------------------
  
  //: Constructor
  
  AttributeTypeComponentBodyC::AttributeTypeComponentBodyC(const StringC &name,const StringC &desc,bool nCanRead,bool nCanWrite)
    : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeComponentBodyC::AttributeTypeComponentBodyC(BinIStreamC &is)
    : AttributeTypeBodyC(is)
  {}
  
  //: Stream constructor.
  
  AttributeTypeComponentBodyC::AttributeTypeComponentBodyC(std::istream &is)
    : AttributeTypeBodyC(is)
  {}
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeComponentBodyC::Save(std::ostream & strm) const {
    return AttributeTypeBodyC::Save(strm);
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeComponentBodyC::Save(BinOStreamC & strm) const {
    return AttributeTypeBodyC::Save(strm);
  }
  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeComponentBodyC::ValueType() const
  { return AVT_Component; }
  
  //: Set control to default value.
  
  bool AttributeTypeComponentBodyC::SetToDefault(AttributeCtrlC &ctrl) const { 
    AttributeCtrlC subComponent;
    if(!ctrl.GetComponent(name,subComponent))
      return false; // Failed to find sub component.
    return subComponent.RestoreDefaults();
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AttributeTypeComponentBodyC,AttributeTypeComponentC,AttributeTypeC);
  
  //:----------------------------------------------------------------------------------------------------
  
  //: Constructor
  
  AttributeTypeMiscBodyC::AttributeTypeMiscBodyC(const StringC &name,const StringC &desc,const AttributeValueTypeT &nvalType,bool nCanRead,bool nCanWrite)
    : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite),
      valType(nvalType)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeMiscBodyC::AttributeTypeMiscBodyC(BinIStreamC &is) 
    : AttributeTypeBodyC(is)
  {
    int x;
    is >> x;
    valType = static_cast<AttributeValueTypeT>(x);
  }
  
  //: Stream constructor.
  
  AttributeTypeMiscBodyC::AttributeTypeMiscBodyC(std::istream &is)
    : AttributeTypeBodyC(is)
  { 
    int x;
    is >> x;
    valType = static_cast<AttributeValueTypeT>(x);
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeMiscBodyC::Save(std::ostream & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << ' ' << static_cast<IntT>(valType);
    return true;
  }
  
  //: Save the attribute to a stream 
  
  bool AttributeTypeMiscBodyC::Save(BinOStreamC & strm) const {
    if(!AttributeTypeBodyC::Save(strm))
      return false;
    strm << static_cast<IntT>(valType);
    return true;    
  }
  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeMiscBodyC::ValueType() const
  { return valType; }
  
  //: Set control to default value.
  
  bool AttributeTypeMiscBodyC::SetToDefault(AttributeCtrlC &ctrl) const
  { return true; }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AttributeTypeMiscBodyC,AttributeTypeMiscC,AttributeTypeC);
  
  //:----------------------------------------------------------------------------------------------------
  
  static VirtualConstructorInstC<AttributeTypeNumBodyC<IntT> > vc_function_AttributeTypeNumBodyC_IntT("AttributeTypeNumBodyC<IntT>" );
  static VirtualConstructorInstC<AttributeTypeNumBodyC<RealT> > vc_function_AttributeTypeNumBodyC_RealT("AttributeTypeNumBodyC<RealT>" );
  
  
}
