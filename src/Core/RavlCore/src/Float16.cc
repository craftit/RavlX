// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/Float16.hh"
#include "Ravl/Math.hh"
#include <iostream>

#if !RAVL_HAVE_IEEE_DOUBLE
#warning "Machine doesn't have IEEE double values, float16 may not work as expected.";
#endif

namespace RavlN {
  

  //: Set to byte value.
  void Float16C::SetInt8(Int8T value) 
  {
    m_data = 0;
    if(value == 0) {
      return;
    }
    if(value < 0) {
      m_data = 0x8000;
      value *= -1;
    }
    int hb = ILog2(value);
    //std::cerr << "Val=" << ((int) value) << " Bit=" << hb << "\n";
    m_data |= (value << (10 - hb)) & 0x03ff;
    m_data |= (((hb + 15) & 0x1f) << 10);
  }
  
  //: Set to byte value.
  void Float16C::SetUInt8(UInt8T value) 
  {
    if(value == 0) {
      m_data = 0;
      return;
    }
    int hb = ILog2(value);
    //std::cerr << "Val=" << ((int) value) << " Bit=" << hb << "\n";
    m_data = (value << (10 - hb)) & 0x03ff;
    m_data |= (((hb + 15) & 0x1f) << 10);
  }
  
  
  //: Set to float value.
  
  void Float16C::SetFloat(float value) {
    union{ float _f; Int32T _u; } val;
    val._f = value;
    // Copy the sign bit.
    m_data = (val._u >> 16) & 0x8000;
    
    // Sort out exponent
    int exp = ((val._u >> 23)&0xff) - 127;
    if(exp < -14) {
      if(exp > -24) {
	int shift = (13+14-exp);
	int frac = ((val._u & 0x007fffff) | (0x0080000)) >> shift  ;
	//std::cerr << "Exp= " << exp << " Shift=" << shift << " frac=" << (frac & 0x03ff) << "\n";
	m_data |= frac & 0x03ff;
      }      
      return ;
    }
    if(exp > 15) {
      // Is value a nan ?
      if(exp == 128 && ((val._u & 0x007fffff) != 0)) {
        // Nan
        m_data = 0x7fff; 
      } else {
        // Number to big, set to infinity.
        m_data |= 0x7c00;
      }
      return ;
    }
    m_data |= ((exp+15) & 0x3f) << 10;
    
    // Copy fraction.
    m_data |= ((val._u) >> 13) & 0x03ff;    
  }
  
  //: Set to unsigned integer value.
  
  void Float16C::SetUInt(unsigned int value) {
    if(value & 0x80000000) { // Large value ?
      m_data = 0x7C00; // Set to positive infinity.
      return ;
    }
    SetInt((int) value);    
  }
  
  //: Set to integer value.
  
  void Float16C::SetInt(int value) {
    // Take care of sign bit.
    m_data = 0;
    if(value == 0)
      return;
    if(value < 0) {
      value *= -1;
      m_data |= 0x8000;
    }
    int hb = ILog2(value);
    int shift = 10 - hb;
    //std::cerr << "Val=" << ((int) value) << " Bit=" << hb << " Shift=" << shift <<" (" << hex << ((hb + 15) & 0x1f) << dec << ")\n";
    if(shift < -6) { // To big for type ?
      m_data |= 0x7c00; // Infinity.
      return ;
    }
    if(shift >= 0) {
      m_data |= (value << shift) & 0x03ff;
    } else {
      m_data |= (value >> (-shift)) & 0x03ff;
    }
    m_data |= (((hb + 15) & 0x1f) << 10);
    
  }
  
  //: convert to a 32 bit float.
  
  float Float16C::Float() const {
    union{ float _f; Int32T _u; } val;
    // Put in sign bit.
    val._u = (m_data & 0x8000) << 16;
    int exp = (m_data & 0x7C00);
    if(exp == 0) { // Is this a denormal ?
      int frac = (m_data & 0x03ff);
      if(frac == 0) 
	return val._f;
      int hb = ILog2(frac);
      int shift = 13 + hb;
      val._u |= (frac & 0x7c00) << shift;
      val._u |= ((127-37) + shift) << 23;
      
    } else {
      if(exp == 0x7C00) { // Is infinity / nan ?
        if((val._u & 0x03ff) != 0) { 
          // A Nan.
          val._f = RavlConstN::nanFloat;
        } else {
          // We have an infinity.
          val._u |= 0x7f800000;
        }
      } else {
        val._u |= (Int32T)(m_data & 0x03ff) << 13;
        val._u |= ((Int32T)(exp + ((127-15) << 10))) << 13;
      }
    }
    return val._f;
  }

}
