// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/Image.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/BinStream.hh"

namespace RavlImageN {
  
  //: Override default IO to handle byte images correctly.
  
  std::ostream &operator<<(std::ostream &s,const ImageC<ByteT> &img) {
    s << img.Rectangle() << "\n";
    for(BufferAccess2dIterC<ByteT> it(img,img.Range2());it;) {
      do {
	s << (IntT) *it << ' ';
      } while(it.Next());
      s << '\n';
    }
    return s;
  }
  
  //: Load byte image from stream 
  // Override default IO to handle byte images correctly.
  
  std::istream &operator>>(std::istream &s,ImageC<ByteT> &img) {
    IndexRange2dC rect;
    s >> rect;
    img = ImageC<ByteT>(rect);
    int x;
    for(BufferAccess2dIterC<ByteT> it(img,img.Range2());it;it++) {
      s >> x;
      *it = (ByteT) x;
    }
    return s;
  }

  /// ByteT ////////////////////////////////////////////////////////////////////////
  
  BinOStreamC &operator << (BinOStreamC &out,const ImageC<ByteT> &img) { 
    out << img.Rectangle();
    
    IntT width = img.Cols();
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      out.OBuff((const char *) &(img[atrow][offset]),width);  
    return out;
  }
  
  BinIStreamC &operator >> (BinIStreamC &in,ImageC<ByteT> &img) { 
    ImageRectangleC rect;
    in >> rect;
    img = ImageC<ByteT>(rect);
    
    IntT width = img.Cols();
    IndexC atrow = img.TRow();
    IndexC offset = img.LCol();
    IndexC brow = img.BRow();
    for(;atrow <= brow;atrow++) 
      in.IBuff((char *) &(img[atrow][offset]),width);  
    return in;
  }

}
