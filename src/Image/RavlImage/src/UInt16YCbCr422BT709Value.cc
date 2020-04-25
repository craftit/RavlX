// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: ByteYUV422Value.cc 3700 2003-11-19 17:44:12Z craftit $"
//! lib=RavlImage
//! file="Ravl/Image/Base/ByteYCbCr422Value.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/UInt16YCbCr422BT709Value.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<UInt16YCbCr422BT709ValueC> &img)
  { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 2;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++)
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<UInt16YCbCr422BT709ValueC> &img)
  { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<UInt16YCbCr422BT709ValueC>(rect);
    
    IntT width = img.Cols() * 2;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++)
      in.IBuff((char *) &(img[atrow][offset]),width);
    return in;
  }
}
