// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003,  OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_PIXBUF_HEADER
#define RAVLGUI_PIXBUF_HEADER 1
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! date="24/9/2003"
//! file="Ravl/GUI/GTK/Pixbuf.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/GUI/Widget.hh"

extern "C" {
  typedef struct _GdkPixbuf GdkPixbuf;
}

namespace RavlGUIN {
  using namespace RavlImageN;
  using namespace RavlN;
  
  //: Image buffer
  // GTK-2.x and above only.
  
  class PixbufC {
  public:
    PixbufC()
      : pixbuf(0)
    {}
    //: Default constructor.
    // Creates an invalid pix buf.
    
    PixbufC(const PixbufC &copy);
    //: Copy constructor.
    
    PixbufC(GdkPixbuf *rawPixBuf);
    //: Handle raw pixbuf.
    
    PixbufC(const ImageC<ByteRGBValueC> &img);
    //: Create from an image.
    
    PixbufC(const ImageC<ByteRGBAValueC> &img);
    //: Create from an image.
    
    explicit PixbufC(const char **data);
    //: Create from XPM data.
    
    explicit PixbufC(const StringC &name);
    //: Create from filename
    
    PixbufC(IntT rows,IntT cols,bool withAlpha,IntT bitsPerPixel = 8);
    //: Create a new pixbuf.
    
    ~PixbufC();
    //: Destructor.
    
    PixbufC& operator= (const PixbufC &other);
    //: Assignment operator.
    
    GdkPixbuf *Pixbuf()
    { return pixbuf; }
    //: Access raw pixbuf
    
    const GdkPixbuf *Pixbuf() const
    { return pixbuf; }
    //: Access raw pixbuf
    
    const bool IsValid() const
    { return (pixbuf != 0); }
    //: Test if this is a valid pixbuf
    
    const IntT Rows() const;
    //: Number of rows in pixbuf
    
    const IntT Cols() const;
    //: Number of cols in pixbuf
    
  protected:
    GdkPixbuf *pixbuf;
  };
  
}
#endif
