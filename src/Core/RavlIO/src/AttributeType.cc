// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/AttributeType.cc"

#include "Ravl/DP/AttributeType.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: Constructor.
  
  AttributeTypeBodyC::AttributeTypeBodyC()
    : canRead(true),
      canWrite(true)
  {}
  
  //: Constructor.
  
  AttributeTypeBodyC::AttributeTypeBodyC(const StringC &nname,const StringC &ndesc,bool nCanRead,bool nCanWrite)
    : name(nname),
      description(ndesc),
      canRead(nCanRead),
      canWrite(nCanWrite)
  {}
  
  //: Binary stream constructor.
  
  AttributeTypeBodyC::AttributeTypeBodyC(BinIStreamC &is)
    : RCBodyVC(is)
  {
    IntT version;
    is >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("AttributeTypeBodyC(BinIStreamC &), Unrecognised version number in stream.");
    is >> name >> description >> canRead >> canWrite;
  }
  
  //: stream constructor.
  
  AttributeTypeBodyC::AttributeTypeBodyC(std::istream &is) 
    : RCBodyVC(is)
  {
    IntT version;
    is >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("AttributeTypeBodyC(std::istream &), Unrecognised version number in stream.");
    is >> name >> description >> canRead >> canWrite;
  }

  
  //: Save the attribute to a stream 
  
  bool AttributeTypeBodyC::Save(BinOStreamC & strm) const { 
    if(!RCBodyVC::Save(strm))
      return false;
    IntT version = 0;
    strm << version << name << description << canRead << canWrite ; 
    return true;
  }

  //: Save attribute to stream 
  
  bool AttributeTypeBodyC::Save(std::ostream & strm) const {
    if(!RCBodyVC::Save(strm))
      return false;
    IntT version = 0;
    strm << ' ' << version << "\t" << name << "\t" << description << "\t" << canRead << "\t" << canWrite ; 
    return true ; 
  }

  
  //: Get hint about type of value attribute has.
  
  AttributeValueTypeT AttributeTypeBodyC::ValueType() const
  { return AVT_Invalid; }
  
  //: Set control to default value.
  
  bool AttributeTypeBodyC::SetToDefault(AttributeCtrlC &ctrl) const {
    RavlAssertMsg(0,"AttributeTypeBodyC::SetToDefault(), ERROR: Abstract method called. ");
    return false;
  }
  
  ////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  AttributeTypeC::AttributeTypeC(std::istream &strm) 
    : RCHandleVC<AttributeTypeBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,AttributeTypeBodyC))
  {}
    
  //: Load from binary stream.
  
  AttributeTypeC::AttributeTypeC(BinIStreamC &strm) 
    : RCHandleVC<AttributeTypeBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,AttributeTypeBodyC))    
  {}
  
  ////////////////////////////////////////////////////////
  
  //: Write attribute type to binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const AttributeTypeC &attrType) {
    RavlAssert(attrType.IsValid());
    attrType.Save(strm);
    return strm;
  }
  
  //: Read attribute type from binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,AttributeTypeC &attrType) {
    attrType = AttributeTypeC(strm);
    return strm;
  }
  
  //: Write attribute type to stream.
  
  std::ostream &operator<<(std::ostream &strm,const AttributeTypeC &attrType) {
    RavlAssert(attrType.IsValid());
    attrType.Save(strm);
    return strm;
  }
  
  //: Read attribute type from stream.
  
  std::istream &operator>>(std::istream &strm,AttributeTypeC &attrType){
    attrType = AttributeTypeC(strm);
    return strm;
  }
  
}


