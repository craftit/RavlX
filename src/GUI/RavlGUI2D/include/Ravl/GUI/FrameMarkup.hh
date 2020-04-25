// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_FRAMEMARKUP_HEADER
#define RAVLGUI_FRAMEMARKUP_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id: FrameMarkup.hh 4259 2004-09-28 08:53:43Z charles $"
//! lib=RavlGUI2D
//! file="Magellan/Core/FrameMarkup.hh"
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/DList.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/GUI/MarkupInfo.hh"

namespace RavlGUIN {
  using namespace RavlImageN;
  
  //! userlevel=Normal
  //: Info about a frame.
  
  class FrameMarkupC {
  public:
    FrameMarkupC()
    {}
    //: Default constructor.
    
    FrameMarkupC(const DListC<MarkupInfoC> &_markup)
      : markup(_markup)
    {}
    //: Constructor.
    
    const DListC<MarkupInfoC> &Markup() const
    { return markup; }
    //: Markup for the image.
    
    DListC<MarkupInfoC> &Markup()
    { return markup; }
    //: Markup for the image.
    
    void SetFrameNumber(Int64T curFrameNo)
    { frameNo = curFrameNo; }
    //: Set the frame number.
    
    Int64T FrameNumber() const
    { return frameNo; }
    //: Frame number in the sequence.
    
    ImageC<ByteRGBValueC> Image() const;
    //: find background image
    
  protected:
    DListC<MarkupInfoC> markup;
    Int64T frameNo;
  };
  
  std::ostream &operator<<(std::ostream &out,const FrameMarkupC &obj);
  //: Stream operator.
  
  BinOStreamC &operator<<(BinOStreamC &out,const FrameMarkupC &obj);
  //: Binary stream operator.
  
  std::istream &operator>>(std::istream &in,FrameMarkupC &obj);
  //: Stream operator.
  
  BinIStreamC &operator>>(BinIStreamC &in,FrameMarkupC &obj);
  //: Binary stream operator.
  
}


#endif
