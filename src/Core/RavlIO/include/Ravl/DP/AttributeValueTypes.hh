// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ATTRIBUTEVALUETYPES_HEADER
#define RAVL_ATTRIBUTEVALUETYPES_HEADER 1
//! author="Charles Galambos"
//! date="23/8/2003"
//! lib=RavlIO
//! file="Ravl/Core/IO/AttributeValueTypes.hh"
//! docentry="Ravl.API.Core.Data_Processing.Attributes"

#include "Ravl/DP/AttributeType.hh"
#include "Ravl/DList.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Boolean attribute type.
  
  class AttributeTypeBoolBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeBoolBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
			   bool ndefaultValue = false);
    //: Constructor
    
    AttributeTypeBoolBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeBoolBodyC(std::istream &is);
    //: stream constructor.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 

    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.
    
    bool Default() const
    { return defaultValue; }
    //: Access defaultValue value
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
    virtual StringC DefaultString() const
    { return StringC(static_cast<IntT>(defaultValue)); }
    //: Access default values as string.
    
  protected:
    bool defaultValue;
  };
  
  //! userlevel=Normal
  //: Boolean attribute type.
  
  class AttributeTypeBoolC
    : public AttributeTypeC
  {
  public:
    AttributeTypeBoolC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeBoolC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeBoolC(std::istream &is);
    //: stream constructor.
    
    AttributeTypeBoolC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,bool ndefaultValue = false)
      : AttributeTypeC(*new AttributeTypeBoolBodyC(name,desc,nCanRead,nCanWrite,ndefaultValue))
    {}
    //: Constructor.
    
    AttributeTypeBoolC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeBoolBodyC *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
  protected:
    AttributeTypeBoolC(AttributeTypeBoolBodyC &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeBoolC(const AttributeTypeBoolBodyC *bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeBoolBodyC &Body()
    { return static_cast<AttributeTypeBoolBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
    const AttributeTypeBoolBodyC &Body() const
    { return static_cast<const AttributeTypeBoolBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
    bool Default() const
    { return Body().Default(); }
    //: DefaultValue value.
  };
  
  //:-----------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Numerical attribute value type.
  
  template<typename ValueT> 
  class AttributeTypeNumBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeNumBodyC()
    {}
    //: DefaultValue constructor.
    
    AttributeTypeNumBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
			  ValueT nmin = ValueT(),ValueT nmax = ValueT(),ValueT nstep = ValueT(),ValueT ndefaultValue = ValueT())
      : AttributeTypeBodyC(name,desc,nCanRead,nCanWrite),
	minv(nmin),maxv(nmax),step(nstep),defaultValue(ndefaultValue)
    {}
    //: Constructor.

    AttributeTypeNumBodyC(BinIStreamC &is)
      : AttributeTypeBodyC(is)
    { is >> minv >> maxv >> step >> defaultValue; }
    //: Binary stream constructor.
    
    AttributeTypeNumBodyC(std::istream &is)
      : AttributeTypeBodyC(is)
    { is >> minv >> maxv >> step >> defaultValue; }
    //: stream constructor.
    
    virtual bool Save(std::ostream & strm) const {
      if(!AttributeTypeBodyC::Save(strm))
	return false;
      strm << ' ' << minv << ' ' << maxv << ' ' << step << ' ' << defaultValue; 
      return true;
    }
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const {
      if(!AttributeTypeBodyC::Save(strm))
	return false;
      strm << minv << maxv << step << defaultValue; 
      return true;
    }
    //: Save the attribute to a stream 
    
    virtual AttributeValueTypeT ValueType() const{
      if(typeid(ValueT) == typeid(int))
        return AVT_Int;
      if(typeid(ValueT) == typeid(Int64T))
        return AVT_Int64;
      if(typeid(ValueT) == typeid(RealT))
        return AVT_Real;
      return AVT_Invalid;
    }
    //: Get hint about type of value attribute has.
    
    ValueT Min() const
    { return minv; }
    //: Minimum value
    
    ValueT Max() const
    { return maxv; }
    //: Maximum value
    
    ValueT Step() const
    { return step; }
    //: Increment steps.
    
    ValueT Default() const
    { return defaultValue; }
    //: DefaultValue value.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const
    { return ctrl.SetAttr(name,defaultValue); }
    //: Set control to default value.
    
    virtual StringC DefaultString() const
    { return StringC(defaultValue); }
    //: Access default values as string.
    
  protected:
    ValueT minv;
    ValueT maxv;
    ValueT step;
    ValueT defaultValue;
  };


  //! userlevel=Normal
  //: Numerical attribute value type.
  
  template<typename ValueT> 
  class AttributeTypeNumC
    : public AttributeTypeC
  {
  public:
    AttributeTypeNumC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeNumC(BinIStreamC &is)
      : AttributeTypeC(RAVL_VIRTUALCONSTRUCTOR(is,AttributeTypeNumBodyC<ValueT>))
    {}
    //: Binary stream constructor.
    
    AttributeTypeNumC(std::istream &is)
      : AttributeTypeC(RAVL_VIRTUALCONSTRUCTOR(is,AttributeTypeNumBodyC<ValueT>))
    {}
    //: stream constructor.
    
    AttributeTypeNumC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
		      ValueT nmin = ValueT(),ValueT nmax = ValueT(),ValueT nstep = ValueT(),ValueT ndefaultValue = ValueT())
      : AttributeTypeC(*new AttributeTypeNumBodyC<ValueT>(name,desc,nCanRead,nCanWrite,nmin,nmax,nstep,ndefaultValue))
    {}
    //: Constructor.
    
    AttributeTypeNumC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeNumBodyC<ValueT> *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
  protected:
    AttributeTypeNumC(AttributeTypeNumBodyC<ValueT> &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeNumBodyC<ValueT> &Body()
    { return static_cast<AttributeTypeNumBodyC<ValueT> &> (AttributeTypeC::Body()); }
    //: Access body.

    const AttributeTypeNumBodyC<ValueT> &Body() const
    { return static_cast<const AttributeTypeNumBodyC<ValueT> &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
    ValueT Min() const
    { return Body().Min(); }
    //: Minimum value
    
    ValueT Max() const
    { return Body().Max(); }
    //: Maximum value
    
    ValueT Step() const
    { return Body().Step(); }
    //: Increment steps.
    
    ValueT Default() const
    { return Body().Default(); }
    //: DefaultValue value.
    
  };

  //:-----------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: String attribute type.
  
  class AttributeTypeStringBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeStringBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
			     const StringC &ndefaultValue = StringC(),IntT maxLength = 65000);
    //: Constructor
    
    AttributeTypeStringBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeStringBodyC(std::istream &is);
    //: stream constructor.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.

    StringC Default() const
    { return defaultValue; }
    //: Access defaultValue value
    
    IntT MaxLength() const
    { return maxLength; }
    //: Get maximum length for the string.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
    virtual StringC DefaultString() const
    { return defaultValue; }
    //: Access default values as string.
    
  protected:
    StringC defaultValue;
    IntT maxLength;
  };
  
  //! userlevel=Normal
  //: String attribute type.
  
  class AttributeTypeStringC
    : public AttributeTypeC
  {
  public:
    AttributeTypeStringC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeStringC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
			 StringC ndefaultValue = StringC(),IntT nMaxLength = 65000)
      : AttributeTypeC(*new AttributeTypeStringBodyC(name,desc,nCanRead,nCanWrite,ndefaultValue,nMaxLength))
    {}
    //: Constructor.
    
    AttributeTypeStringC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeStringBodyC *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
    AttributeTypeStringC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeStringC(std::istream &is);
    //: Stream constructor.
    
  protected:
    AttributeTypeStringC(AttributeTypeStringBodyC &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeStringBodyC &Body()
    { return static_cast<AttributeTypeStringBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
    const AttributeTypeStringBodyC &Body() const
    { return static_cast<const AttributeTypeStringBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
    StringC Default() const
    { return Body().Default(); }
    //: Default value.
    
    IntT MaxLength() const
    { return Body().MaxLength(); }
    //: Get maximum length for the string.
    
  };

  //:-----------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Enumerated attribute type.
  
  class AttributeTypeEnumBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeEnumBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
			   const DListC<StringC> &values = DListC<StringC>(),const StringC &ndefaultValue = StringC());
    //: Constructor
    
    AttributeTypeEnumBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeEnumBodyC(std::istream &is);
    //: stream constructor.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 

    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.

    StringC Default() const
    { return defaultValue; }
    //: Access defaultValue value
    
    const DListC<StringC> &Values() const
    { return values; }
    // Possible values for enum.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
    virtual StringC DefaultString() const
    { return defaultValue; }
    //: Access default values as string.
    
  protected:
    StringC defaultValue;
    DListC<StringC> values; // Possible values for enum.
  };
  
  //! userlevel=Normal
  //: Enumerated attribute type.
  
  class AttributeTypeEnumC
    : public AttributeTypeC
  {
  public:
    AttributeTypeEnumC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeEnumC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true,
		       const DListC<StringC> &nvalues = DListC<StringC>(),const StringC &ndefaultValue = StringC())
      : AttributeTypeC(*new AttributeTypeEnumBodyC(name,desc,nCanRead,nCanWrite,nvalues,ndefaultValue))
    {}
    //: Constructor.

    AttributeTypeEnumC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeEnumBodyC *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
    AttributeTypeEnumC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeEnumC(std::istream &is);
    //: Stream constructor.
    
  protected:
    AttributeTypeEnumC(AttributeTypeEnumBodyC &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeEnumBodyC &Body()
    { return static_cast<AttributeTypeEnumBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
    const AttributeTypeEnumBodyC &Body() const
    { return static_cast<const AttributeTypeEnumBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
    StringC Default() const
    { return Body().Default(); }
    //: Default value.
    
    const DListC<StringC> &Values() const
    { return Body().Values(); }
    // Possible values for enum.
    
  };

  //:-----------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Component attribute type.
  
  class AttributeTypeComponentBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeComponentBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true);
    //: Constructor
    
    AttributeTypeComponentBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeComponentBodyC(std::istream &is);
    //: Stream constructor.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
  protected:
  };
  
  //! userlevel=Normal
  //: Miscean attribute type.
  
  class AttributeTypeComponentC
    : public AttributeTypeC
  {
  public:
    AttributeTypeComponentC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeComponentC(const StringC &name,const StringC &desc,const AttributeValueTypeT &valType,bool nCanRead = true,bool nCanWrite = true)
      : AttributeTypeC(*new AttributeTypeComponentBodyC(name,desc,nCanRead,nCanWrite))
    {}
    //: Constructor.
    
    AttributeTypeComponentC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeComponentBodyC *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
    AttributeTypeComponentC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeComponentC(std::istream &is);
    //: Stream constructor.
    
  protected:
    AttributeTypeComponentC(AttributeTypeComponentBodyC &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeComponentBodyC &Body()
    { return static_cast<AttributeTypeComponentBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
    const AttributeTypeComponentBodyC &Body() const
    { return static_cast<const AttributeTypeComponentBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
  };

  //:-----------------------------------------------------------------------------------
  
  //! userlevel=Develop
  //: Miscean attribute type.
  
  class AttributeTypeMiscBodyC
    : public AttributeTypeBodyC
  {
  public:
    AttributeTypeMiscBodyC(const StringC &name,const StringC &desc,const AttributeValueTypeT &valType,bool nCanRead = true,bool nCanWrite = true);
    //: Constructor
    
    AttributeTypeMiscBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeMiscBodyC(std::istream &is);
    //: Stream constructor.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
  protected:
    AttributeValueTypeT valType;
  };
  
  //! userlevel=Normal
  //: Miscean attribute type.
  
  class AttributeTypeMiscC
    : public AttributeTypeC
  {
  public:
    AttributeTypeMiscC()
    {}
    //: DefaultValue constructor.
    // Creates an invalid handle.
    
    AttributeTypeMiscC(const StringC &name,const StringC &desc,const AttributeValueTypeT &valType,bool nCanRead = true,bool nCanWrite = true)
      : AttributeTypeC(*new AttributeTypeMiscBodyC(name,desc,valType,nCanRead,nCanWrite))
    {}
    //: Constructor.
    
    AttributeTypeMiscC(const AttributeTypeC &base)
      : AttributeTypeC(dynamic_cast<const AttributeTypeMiscBodyC *>(BodyPtr(base)))
    {}
    //: Construct from a base handle
    // Creates an invalid handle if object is not of the correct type.
    
    AttributeTypeMiscC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeMiscC(std::istream &is);
    //: Stream constructor.
    
  protected:
    AttributeTypeMiscC(AttributeTypeMiscBodyC &bod)
      : AttributeTypeC(bod)
    {}
    //: Body constructor.
    
    AttributeTypeMiscBodyC &Body()
    { return static_cast<AttributeTypeMiscBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
    const AttributeTypeMiscBodyC &Body() const
    { return static_cast<const AttributeTypeMiscBodyC &> (AttributeTypeC::Body()); }
    //: Access body.
    
  public:
  };

}


#endif
