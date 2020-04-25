// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// $Id$
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ByteYUVValue.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  static TypeNameC type2(typeid(ByteYUVValueC),"ByteYUVValueC");
  
  ////////////////////////////////////////////////////////////
  // Accelerated IO routines...
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<ByteYUVValueC> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<ByteYUVValueC> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<ByteYUVValueC>(rect);
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  

    return in;
  }
  
  BinOStreamC &operator<<(BinOStreamC &out,const ByteYUVValueC &img) {
    out << img.Y() << img.U() << img.V();
    return out;
  }
  //: Save pixel to binary stream
  
  BinIStreamC &operator>>(BinIStreamC &in,ByteYUVValueC &img) {
    in >> img.Y() >> img.U() >> img.V();
    return in;
  }
  //: Load pixel from binary stream
  
}
