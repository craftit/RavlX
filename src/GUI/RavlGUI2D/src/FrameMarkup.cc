// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: FrameMarkup.cc 4259 2004-09-28 08:53:43Z charles $"
//! lib=RavlGUI2D
//! file="Magellan/Core/FrameMarkup.cc"

#include "Ravl/GUI/FrameMarkup.hh"
#include "Ravl/GUI/MarkupImageRGB.hh"

namespace RavlGUIN {
  
  //: find background image
  
  ImageC<ByteRGBValueC> FrameMarkupC::Image() const {
    for(DLIterC<MarkupInfoC> it(markup);it;it++) {
      MarkupImageRGBC mi(*it);
      if(mi.IsValid())
	return mi.Image();
    }
    ImageC<ByteRGBValueC> img(3,3);
    img.Fill(ByteRGBValueC(0,0,0));
    return img;
  }
  
  
  //:--------------------------------------------------------------------------
  
  //: Stream operator.
  std::ostream &operator<<(std::ostream &out,const FrameMarkupC &obj) {
    IntT version = 0;
    out << version << ' '  << ' ' << obj.Markup();
    return out;
  }
  
  //: Binary stream operator.
  BinOStreamC &operator<<(BinOStreamC &out,const FrameMarkupC &obj) {
    IntT version = 0;
    out << version << obj.Markup();
    return out;
  }
  
  //: Stream operator.
  std::istream &operator>>(std::istream &in,FrameMarkupC &obj) {
    IntT version;
    in >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("FrameMarkupC(std::istream &), Unrecognised version number in stream.");
    in >> obj.Markup();
    return in;
  }
  
  //: Binary stream operator.
  BinIStreamC &operator>>(BinIStreamC &in,FrameMarkupC &obj) {
    IntT version;
    in >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("FrameMarkupC(std::istream &), Unrecognised version number in stream.");
    in >> obj.Markup();
    return in;
  }

}
