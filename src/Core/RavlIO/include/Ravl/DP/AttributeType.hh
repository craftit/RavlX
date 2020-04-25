// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ATTRIBUTETYPE_HEADER
#define RAVL_ATTRIBUTETYPE_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="23/8/2003"
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/AttributeType.hh"
//! docentry="Ravl.API.Core.Data_Processing.Attributes"

#include "Ravl/String.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/DP/AttributeCtrl.hh"

namespace RavlN {

  enum AttributeValueTypeT
    { AVT_Bool,           // Boolean
      AVT_Int,            // Signed integer
      AVT_Int64,          // Signed 64-bit integer.
      AVT_Real,           // Double precision floating point.
      AVT_String,         // Text string
      AVT_Enum,           // Enumerated type.
      AVT_Component,      // Sub component 
      AVT_Abstract,       // Not implemented.
      AVT_ByteRGBImage,   // Not implemented.
      AVT_None,           // Flag.
      AVT_Invalid         // Flag.
    };
  
  //! userlevel=Develop
  //: Attribute type information.
  
  class AttributeTypeBodyC 
    : public RCBodyVC
  {
  public:
    AttributeTypeBodyC();
    //: Constructor.
    
    AttributeTypeBodyC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeBodyC(std::istream &is);
    //: stream constructor.
    
    AttributeTypeBodyC(const StringC &name,const StringC &desc,bool nCanRead = true,bool nCanWrite = true);
    //: Constructor.
    
    virtual bool SetToDefault(AttributeCtrlC &ctrl) const;
    //: Set control to default value.
    
    virtual AttributeValueTypeT ValueType() const;
    //: Get hint about type of value attribute has.
    
    virtual bool Save(std::ostream & strm) const ; 
    //: Save the attribute to a stream 
    
    virtual bool Save(BinOStreamC & strm) const ; 
    //: Save the attribute to a stream 
    
    const StringC &Name() const
    { return name; }
    //: Access name of attribute.
    
    const StringC &Description() const
    { return description; }
    //: Access description of attribute.
    
    bool CanRead() const
    { return canRead; }
    //: Can you read the value of this attribute ?
    
    bool CanWrite() const
    { return canWrite; }
    //: Can you write the value of this attribute.

    bool CanRead(bool can ) { 
      bool ret = canRead ; 
      canRead = can ;
      return ret ; 
    }
    //: Set value of canRead - returns old value  
    
    bool CanWrite (bool can) { 
      bool ret = canWrite ; 
      canWrite = can ; 
      return ret ; 
    }
    //: Set value of canWrite - returns old value 
    
    virtual StringC DefaultString() const
    { return StringC(""); }
    //: Access default values as string.
    
  protected:
    StringC name;  // Name of attribute
    StringC description; // Optional discription
    bool canRead;  // Can be read.
    bool canWrite; // Can be written to
    

    friend std::ostream & operator << (std::ostream & strm, const AttributeTypeBodyC & obj) ; 
    //: output operator 

  };
  
  //! userlevel=Normal
  //: Attribute type information.
  
  class AttributeTypeC 
    : public RCHandleVC<AttributeTypeBodyC>
  {
  public:
    AttributeTypeC()
    {}
    //: Default constructor
    // creates an invalid handle.
    
    AttributeTypeC(BinIStreamC &is);
    //: Binary stream constructor.
    
    AttributeTypeC(std::istream &is);
    //: stream constructor.
    
  protected:
    AttributeTypeC(AttributeTypeBodyC &bod)
      : RCHandleVC<AttributeTypeBodyC>(bod)
    {}
    //: Body constructor.
    
    AttributeTypeC(const AttributeTypeBodyC *bod)
      : RCHandleVC<AttributeTypeBodyC>(bod)
    {}
    //: Body constructor.
    
    AttributeTypeBodyC &Body()
    { return RCHandleC<AttributeTypeBodyC>::Body(); }
    //: Access body.

    const AttributeTypeBodyC &Body() const
    { return RCHandleC<AttributeTypeBodyC>::Body(); }
    //: Access body.
    
  public:
    AttributeValueTypeT ValueType() const
    { return Body().ValueType(); }
    //: Get hint about type of value attribute has.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Access name of attribute const .
    
    const StringC &Description() const
    { return Body().Description(); }
    //: Access description of attribute.
    
    bool CanRead() const
    { return Body().CanRead(); }
    //: Can you read the value of this attribute const 
    
    bool CanWrite() const
    { return Body().CanWrite(); }
    //: Can you write the value of this attribute const
    
    bool CanRead(bool canRead) 
    { return Body().CanRead( canRead ) ; } 
    //: Set value of CanRead 

    bool CanWrite (bool canWrite ) 
    { return Body().CanWrite( canWrite ) ; } 
    //: Set value of CanWrite 

    bool SetToDefault(AttributeCtrlC &ctrl) const
    { return Body().SetToDefault(ctrl); }
    //: Set control to default value.
    
    StringC DefaultString() const
    { return Body().DefaultString(); }
    //: Access default values as string.
    
  };
  
  BinOStreamC &operator<<(BinOStreamC &strm,const AttributeTypeC &attrType);
  //: Write attribute type to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,AttributeTypeC &attrType);
  //: Read attribute type from binary stream.
  
  std::ostream &operator<<(std::ostream &strm,const AttributeTypeC &attrType);
  //: Write attribute type to stream.
  
  std::istream &operator>>(std::istream &strm,AttributeTypeC &attrType);
  //: Read attribute type from stream.
  
}

#endif
