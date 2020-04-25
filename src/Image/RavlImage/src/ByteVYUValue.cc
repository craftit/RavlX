// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ByteVYUValue.cc"

#include "Ravl/Image/ByteVYUValue.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/BinStream.hh"

namespace RavlImageN {
  using namespace RavlN;

  ////////////////////////////////////////////////////////////
  // Accelerated IO routines...
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<ByteVYUValueC> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<ByteVYUValueC> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<ByteVYUValueC>(rect);
    
    IntT width = img.Cols() * 3;
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  

    return in;
  }

}
