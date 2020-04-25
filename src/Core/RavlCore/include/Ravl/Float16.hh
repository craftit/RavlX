#ifndef RAVL_FLOAT16_HEADER
#define RAVL_FLOAT16_HEADER 1
// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Normal
//! file="Ravl/Core/Base/Float16.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="18/09/2008"

#include "Ravl/Types.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: 16 bit floating point number.
  // This has 5 exponent bits and 10 mantissa bits.
  // This is primarily intended to provide compact storage for
  // large number's of floating point values that need realtively little
  // precision.
  // This class follows the IEEE 754 standard, see http://en.wikipedia.org/wiki/IEEE_754 for 
  // details.
  
  // Notes:
  // <code>
  // float 16, 1 sign bit,5 exponent bits,10 mantissa bits
  //                  FEDCBA9876543210
  //                  seeeeemmmmmmmmmm
  //
  // float 32, 1 sign bit,8 exponent bits,23 mantissa bits
  //  FEDCBA9876543210FEDCBA9876543210
  //  seeeeeeeemmmmmmmmmmmmmmmmmmmmmmm
  // </code>
  
  class Float16C {
  public:    
    Float16C()
    {}
    //: Default constructor.
    // Value is undefined
    
    Float16C(Int16T val,bool build) 
      : m_data(val)
    {}
    //: Build a value from a short in.
    
    Float16C(Int8T value)
    { SetInt8(value); }
    //: Construct from a signed byte
    
    Float16C(UInt8T value)
    { SetUInt8(value); }
    //: Construct from a unsigned byte
    
    Float16C(unsigned int value)
    { SetUInt(value); }
    //: Construct from an unsigned int
    
    Float16C(int value)
    { SetInt(value); }
    //: Construct from an int
    
    Float16C(float value)
    { SetFloat(value); }
    //: Construct from a float
    
    bool IsPositive() const
    { return (m_data & 0x8000) == 0; }
    //: Test if the value is positive
    
    bool IsNegative() const
    { return (m_data & 0x8000) != 0; }
    //: Test if the value is positive
    
    bool IsDenormal() const
    { return (m_data & 0x7C00) == 0x0000; }
    //: Test if number is denormal.
    
    bool IsInf() const
    { return (m_data & 0x7fff) == 0x7C00 ; }
    //: Is infinity ?
    
    bool IsNan() const
    { return ((m_data & 0x7C00) == 0x7C00) && ((m_data & 0x03ff) != 0); }
    //: Is a nan ?
    
    float Float() const;
    //: convert to a 32 bit float.
    
    operator float() const
    { return Float(); }
    // Convert to a floating point number.
    
    const Int16T &V() const
    { return m_data; }
    //: Access raw data.
    
    Int16T &V()
    { return m_data; }
    //: Access raw data.
    
    const Float16C & operator=(Int8T value)
    { SetInt8(value); return *this; }
    //: Assign to Int8T
    
    const Float16C & operator=(UInt8T value)
    { SetUInt8(value); return *this; }
    //: Assign to Int8T
    
    const Float16C & operator=(float value)
    { SetFloat(value); return *this; }
    //: Assign to float

    const Float16C & operator=(int value)
    { SetInt(value); return *this; }
    //: Assign to int
    
    const Float16C & operator=(unsigned int value)
    { SetUInt(value); return *this; }
    //: Assign to unsigned 
    
    bool operator==(const Float16C &v) const
    { return V() == v.V() ; }
    //: Equality.
    
    bool operator!=(const Float16C &v) const
    { return V() != v.V() ; }
    //: Inequality.
    
    bool operator>(const Float16C &v) const
    { return V() > V(); }
    //: Greater than
    
    bool operator<(const Float16C &v) const
    { return V() < V(); }
    //: Less than
    
    bool operator>=(const Float16C &v) const
    { return V() >= V(); }
    //: Greater than, Equal too
    
    bool operator<=(const Float16C &v) const
    { return V() <= V(); }
    //: Less than, Equal too
    
    SizeT StdHash() const
    { return V(); }
    //: Hash value.
    
  protected:
    void SetInt8(Int8T val);
    //: Set to byte value.
    
    void SetUInt8(UInt8T val);
    //: Set to byte value.
    
    void SetInt(int val);
    //: Set to integer value.
    
    void SetUInt(unsigned int val);
    //: Set to unsigned integer value.
    
    void SetFloat(float val);
    //: Set to float value.
    
    Int16T m_data;
  };
  
  inline bool IsPInf(const Float16C &v)
  { return v.IsPositive() && v.IsInf(); }
  //: Is positive infinity ?
  
  inline bool IsNInf(const Float16C &v)
  { return v.IsNegative() && v.IsInf(); }
  //: Is negative infinity ?
  
  inline bool IsNan(const Float16C &v) 
  { return v.IsNan(); }
  //: Is a nan ?
  
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const Float16C &value) { 
    strm << value.V(); 
    return strm;
  }
  //: Write to binary stream.
  
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,Float16C &value) { 
    strm >> value.V(); 
    return strm;
  }
  //: Read from binary stream.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const Float16C &value)
  { strm << value.Float(); return strm; }
  //: Write to text stream.
  
  inline
  std::istream &operator>>(std::istream &strm,Float16C &value) { 
    float rval;
    strm >> rval;
    value = rval;
    return strm; 
  }
  //: Read from text stream.
  
}


#endif
