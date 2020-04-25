// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/RBfAcc2d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Save byte image to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RangeBufferAccess2dC<RealT> &rb) {
    if(strm.NativeEndianTest()) {
      IntT width = rb.Frame().Cols();
      IndexC atrow = rb.Frame().TRow();
      IndexC offset = rb.Frame().LCol();
      IndexC brow = rb.Frame().BRow();
      for(;atrow <= brow;atrow++)
        strm.OBuff(reinterpret_cast<const char *>(&(rb[atrow][offset])),width * sizeof(RealT));  
    } else {
      // Write element by element.
      for(BufferAccess2dIterC<RealT> it(rb,rb.Range2());it;it++)
        strm << *it;
    }
    
    return strm;
  }
  
  //: Load byte image from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,RangeBufferAccess2dC<RealT> &rb) {
    if(strm.NativeEndianTest()) {
      IntT width = rb.Frame().Cols();
      IndexC atrow = rb.Frame().TRow();
      IndexC offset = rb.Frame().LCol();
      IndexC brow = rb.Frame().BRow();
      for(;atrow <= brow;atrow++)
        strm.IBuff(reinterpret_cast<char *>(&(rb[atrow][offset])),width * sizeof(RealT));  
    } else {
      // Read element by element.      
      for(BufferAccess2dIterC<RealT> it(rb,rb.Range2());it;it++)
        strm >> *it;
    }
    return strm;
  }
  
  //: Save byte image to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RangeBufferAccess2dC<FloatT> &rb) {
    if(strm.NativeEndianTest()) {
      IntT width = rb.Frame().Cols();
      IndexC atrow = rb.Frame().TRow();
      IndexC offset = rb.Frame().LCol();
      IndexC brow = rb.Frame().BRow();
      for(;atrow <= brow;atrow++)
        strm.OBuff(reinterpret_cast<const char *>(&(rb[atrow][offset])),width * sizeof(FloatT));  
    } else {
      // Write element by element.
      for(BufferAccess2dIterC<FloatT> it(rb,rb.Range2());it;it++)
        strm << *it;
    }

    return strm;
  }
  
  //: Load byte image from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,RangeBufferAccess2dC<FloatT> &rb) {
    if(strm.NativeEndianTest()) {
      IntT width = rb.Frame().Cols();
      IndexC atrow = rb.Frame().TRow();
      IndexC offset = rb.Frame().LCol();
      IndexC brow = rb.Frame().BRow();
      for(;atrow <= brow;atrow++)
        strm.IBuff(reinterpret_cast<char *>(&(rb[atrow][offset])),width * sizeof(FloatT));  
    } else {
      // Read element by element.      
      for(BufferAccess2dIterC<FloatT> it(rb,rb.Range2());it;it++)
        strm >> *it;
    }
    return strm;
  }

  //: Save byte image to binary stream 
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RangeBufferAccess2dC<ByteT> &rb) {
    IntT width = rb.Frame().Cols();
    IndexC atrow = rb.Frame().TRow();
    IndexC offset = rb.Frame().LCol();
    IndexC brow = rb.Frame().BRow();
    for(;atrow <= brow;atrow++)
      strm.OBuff(reinterpret_cast<const char *>(&(rb[atrow][offset])),width * sizeof(ByteT));  
    return strm;
  }
  
  //: Load byte image from binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &strm,RangeBufferAccess2dC<ByteT> &rb) {
    IntT width = rb.Frame().Cols();
    IndexC atrow = rb.Frame().TRow();
    IndexC offset = rb.Frame().LCol();
    IndexC brow = rb.Frame().BRow();
    for(;atrow <= brow;atrow++)
      strm.IBuff(reinterpret_cast<char *>(&(rb[atrow][offset])),width * sizeof(ByteT));  
    return strm;
  }

}
