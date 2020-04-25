// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="24/9/2003"
//! file="Ravl/GUI/GTK/Pixbuf.cc"

#include "Ravl/GUI/Pixbuf.hh"

#include <gdk/gdk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  static void DeleteImageRef(guchar *pixels, gpointer data)
  {
    ONDEBUG(std::cerr << "DeleteImageRef(), Called. \n");
    
    delete ((ImageC<ByteRGBValueC> *) data);
  }

  static void DeleteImageRGBARef(guchar *pixels, gpointer data)
  {
    ONDEBUG(std::cerr << "DeleteImageRef(), Called. \n");
    
    delete ((ImageC<ByteRGBAValueC> *) data);
  }
  
  //: Copy constructor

  PixbufC::PixbufC(const PixbufC &copy) :
    pixbuf(copy.pixbuf)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(copy)\n");
    
    if (pixbuf != 0)
      gdk_pixbuf_ref(pixbuf);
  }
  
  //: Handle raw pixbuf.
  
  PixbufC::PixbufC(GdkPixbuf *rawPixBuf) :
    pixbuf(rawPixBuf)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(GdkPixbuf*)\n");
    
    if (pixbuf != 0)
      gdk_pixbuf_ref(pixbuf);
  }
  
  //: Create from an image.
  
  PixbufC::PixbufC(const ImageC<ByteRGBValueC> &img) 
    : pixbuf(0)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(const ImageC<ByteRGBValueC>&)\n");
    
    if (img.Frame().Area() > 0)
    {
      void *imgHandle = new ImageC<ByteRGBValueC>(img); // Create a refrence to the image.
      pixbuf = gdk_pixbuf_new_from_data ((guchar *) &(img[img.Frame().Origin()]),
                                         GDK_COLORSPACE_RGB,false,8,img.Cols(),img.Rows(),
					 img.Stride() * sizeof(ByteRGBValueC),
					 &DeleteImageRef,
					 imgHandle
					 );
    }
  }
  
  //: Create from an image.
  
  PixbufC::PixbufC(const ImageC<ByteRGBAValueC> &img) 
    : pixbuf(0)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(const ImageC<ByteRGBAValueC>&) Stride=" << img.Stride() << "\n");
    if (img.Frame().Area() > 0) {
      void *imgHandle = new ImageC<ByteRGBAValueC>(img); // Create a refrence to the image.
      pixbuf = gdk_pixbuf_new_from_data ((guchar *) &(img[img.Frame().Origin()]),
					 GDK_COLORSPACE_RGB,true,8,img.Cols(),img.Rows(),
					 img.Stride() * sizeof(ByteRGBAValueC),
					 &DeleteImageRGBARef,
					 imgHandle
					 );
    }
  }

  //: Create from XPM data.
  
  PixbufC::PixbufC(const char **data) 
    : pixbuf(0)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(const char**)\n");
    
    pixbuf = gdk_pixbuf_new_from_xpm_data(data);
  }
  
  //: Create from filename.
  
  PixbufC::PixbufC(const StringC &name) :
    pixbuf(0)
  {
    ONDEBUG(std::cerr << "PixbufC::PixbufC(const StringC&)\n");
    
    pixbuf = gdk_pixbuf_new_from_file(name, NULL);
  }

  //: Create a new pixbuf.
  
  PixbufC::PixbufC(IntT rows,IntT cols,bool withAlpha,IntT bitsPerPixel)
    : pixbuf(0)
  {
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,withAlpha,bitsPerPixel,cols,rows);
  }

  //: Destructor.
  
  PixbufC::~PixbufC()
  {
    ONDEBUG(std::cerr << "PixbufC::~PixbufC\n");
    
    if (pixbuf != 0)
      gdk_pixbuf_unref(pixbuf);
    pixbuf = 0;
  }

  
  
  PixbufC& PixbufC::operator= (const PixbufC &other)
  {
    ONDEBUG(std::cerr << "PixbufC::operator=\n");
    
    GdkPixbuf *tempPixbuf = pixbuf;
    pixbuf = other.pixbuf;
    if (pixbuf != 0)
      gdk_pixbuf_ref(pixbuf);
    
    if (tempPixbuf != 0)
      gdk_pixbuf_unref(tempPixbuf);

    return *this;
  }



  const IntT PixbufC::Rows() const
  {
    return (pixbuf != 0 ? gdk_pixbuf_get_height(pixbuf) : 0);
  }
  
  
  
  const IntT PixbufC::Cols() const
  {
    return (pixbuf != 0 ? gdk_pixbuf_get_width(pixbuf) : 0);
  }
  
}
