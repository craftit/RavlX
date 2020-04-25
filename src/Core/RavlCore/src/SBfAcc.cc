// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/SBfAcc2d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Save real array to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SizeBufferAccessC<RealT> &bf) {
    if(strm.NativeEndianTest()) {
      strm.OBuff(reinterpret_cast<const char *>(bf.DataStart()),bf.Size() * sizeof(RealT));
    } else {
      const RealT *at = bf.DataStart();
      const RealT *endOfRow = &at[bf.Size()];
      if(bf.Size() > 0) {
        for(;at != endOfRow;at++)
          strm << *at;
      }
    }
    return strm;
  }
  
  //: Load real array image from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,SizeBufferAccessC<RealT> &bf) {
    if(strm.NativeEndianTest()) {
      strm.IBuff(reinterpret_cast<char *>(bf.DataStart()),bf.Size() * sizeof(RealT));
    } else {
      RealT *at = bf.DataStart();
      RealT *endOfRow = &at[bf.Size()];
      if(bf.Size() > 0) {
        for(;at != endOfRow;at++)
          strm >> *at;
      }
    }
    return strm;
  }
  
  //: Save float array image to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SizeBufferAccessC<FloatT> &bf) {
    if(strm.NativeEndianTest()) {
      strm.OBuff(reinterpret_cast<const char *>(bf.DataStart()),bf.Size() * sizeof(FloatT));
    } else {
      const FloatT *at = bf.DataStart();
      const FloatT *endOfRow = &at[bf.Size()];
      if(bf.Size() > 0) {
        for(;at != endOfRow;at++)
          strm << *at;
      }
    }
    return strm;
  }
  
  //: Load float array image from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,SizeBufferAccessC<FloatT> &bf) {
    if(strm.NativeEndianTest()) {
      strm.IBuff(reinterpret_cast<char *>(bf.DataStart()),bf.Size() * sizeof(FloatT));
    } else {
      FloatT *at = bf.DataStart();
      FloatT *endOfRow = &at[bf.Size()];
      if(bf.Size() > 0) {
        for(;at != endOfRow;at++)
          strm >> *at;
      }
    }    
    return strm;
  }
  
  
  //: Save byte array to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SizeBufferAccessC<ByteT> &bf) {
    RavlAssert(sizeof(ByteT) == 1);
    strm.OBuff(reinterpret_cast<const char *>(bf.DataStart()),bf.Size() * sizeof(ByteT));
    return strm;    
  }
  
  //: Load byte array from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,SizeBufferAccessC<ByteT> &bf) {
    RavlAssert(sizeof(ByteT) == 1);
    strm.IBuff(reinterpret_cast<char *>(bf.DataStart()),bf.Size() * sizeof(ByteT));
    return strm;
  }
  
}
