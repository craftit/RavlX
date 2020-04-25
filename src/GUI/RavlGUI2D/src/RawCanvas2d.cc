// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI2D
//! file="Ravl/GUI/2D/RawCanvas2d.cc"

#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include <gtk/gtk.h>
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

// Some raw canvas ops that use images....

namespace RavlGUIN {
  
  //: Draw an image into the canvas with its origin offset by 'offset'.
  // Note: You have to include the RavlGUI2d library to use this function.
  template<>
  void RawCanvasBodyC::GUIDrawImage(const ImageC<ByteRGBValueC> &img,const Index2dC &offset,bool ignoreImageOrigin) {
    //cerr << "RawCanvasBodyC::GUIDrawImage(), Called.  Img=" << img.Frame() << " Offset=" << offset << "\n";
    if(img.IsEmpty())
      return;
    Index2dC at = offset;
    if(!ignoreImageOrigin)
      at += img.Frame().Origin();
    ImageC<ByteRGBValueC> anImg = img;
    if(!img.IsBlock())
      anImg = img.Copy();
    gdk_draw_rgb_image(DrawArea(),
                       widget->style->black_gc,
                       at[1].V(),at[0].V(),
                       anImg.Cols(),anImg.Rows(),
                       GDK_RGB_DITHER_NORMAL,
                       (unsigned char *) const_cast<ImageC<ByteRGBValueC> &>(anImg).Row(anImg.TRow()),
                       anImg.Stride() * sizeof(ByteRGBValueC));
  }

  //: Draw an image into the canvas with its origin offset by 'offset'.
  // Note: You have to include the RavlGUI2d library to use this function.
  template<>
  void RawCanvasBodyC::GUIDrawImage(const ImageC<ByteT> &img,const Index2dC &offset,bool ignoreImageOrigin) {
    if(img.IsEmpty())
      return;
    Index2dC at = offset;
    if(!ignoreImageOrigin)
      at += img.Frame().Origin();
    ImageC<ByteT> anImg = img;
    if(!img.IsBlock())
      anImg = img.Copy();
    gdk_draw_gray_image(DrawArea(),
                        widget->style->black_gc,
                        at[1].V(),at[0].V(),
                        anImg.Cols(),anImg.Rows(),
                        GDK_RGB_DITHER_NORMAL,
                        const_cast<ImageC<ByteT> &>(anImg).Row(anImg.TRow()),
                        anImg.Stride());
  }

  
  //: Draw an image into the canvas with its origin offset by 'offset'.
  // Note: You have to include the RavlGUI2d library to use this function.
  
  void RawCanvasBodyC::GUIDrawImage(const PixbufC &pixbuf,const Index2dC &offset) {
    if(!pixbuf.IsValid())
      return;
    Index2dC at = offset;
    gdk_draw_pixbuf(DrawArea(),
                    widget->style->black_gc,
                    const_cast<PixbufC &>(pixbuf).Pixbuf(),
                    0,0,
                    at[1].V(),at[0].V(),
                    pixbuf.Cols(),pixbuf.Rows(),
                    GDK_RGB_DITHER_NORMAL,
                    0,0);
  }
  
  
}
