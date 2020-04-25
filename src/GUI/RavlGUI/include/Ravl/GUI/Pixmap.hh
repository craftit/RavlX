// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_PIXMAP_HEADER
#define RAVLGUI_PIXMAP_HEADER 1
////////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Pixmap.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! rcsid="$Id$"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"

struct _GdkVisual;
typedef struct _GdkVisual        GdkVisual;

namespace RavlGUIN {
  
  using namespace RavlImageN;

  class PixmapC;
  
  //! userlevel=Develop
  //: Pixmap body
  
  class PixmapBodyC 
    : public WidgetBodyC
  {
  public:
    PixmapBodyC(const char * const *data,const WidgetC &rwin = WidgetC());
    //: Constructor.
    
    PixmapBodyC(const StringC &filename,const WidgetC &rwin = WidgetC());
    //: Constructor.
    // Currently this will ONLY load xpm's
    
    PixmapBodyC(const ImageC<ByteRGBValueC>& im,const WidgetC &rwin = WidgetC());
    //: Constructor.
    // This will load a pixmap from an RGB image
    
    PixmapBodyC(int width,int height,int depth);
    //: Constructor.
    
    virtual ~PixmapBodyC() { }
    //: Have virtual methods - need virtual destructor

    virtual bool Create()
    { return CommonCreate(); }
    //: Create the widget.

    virtual bool Create(GtkWidget *_widget)
    { return CommonCreate(_widget); }
    //: Create with a widget supplied from elsewhere.
    
    GdkPixmap * &Pixmap() 
    { return pixmap; }
    //: Access pixmap.
    
    GdkBitmap * &Mask()
    { return mask; }
    //: Access pixmap mask.
    
    bool GUIGetSize(int &width,int &height);
    //: Get size of pixmap.
    // Will return false and leave 'width' and 'height' unchanged
    // if unknown (The pixmap is unallocated. )
    // Call only with the GUI thread.
    
    bool GUISetPixmap(PixmapC &pix);
    //: Copy a pixmap into the current pixmap
    
    bool SetPixmap(PixmapC &pix);
    //: Copy a pixmap into the current pixmap
    
  protected:
    bool CommonCreate(GtkWidget *_widget = NULL);
    //: Common widget creation function
    
    WidgetC rootWin;  // Handle to root win, invalidated after Create.
    GdkPixmap *pixmap;  
    GdkBitmap *mask;
    StringC filename;
    const char * const *data;
    int width,height,depth;
    ImageC<ByteRGBValueC> image;
  };
  
  
  //! userlevel=Normal
  //: Pixmap 
  // Note: Its an unfortunate fact that the root application window
  // MUST be instanciated before the pixmap is created.
  
  class PixmapC 
    : public WidgetC
  {
  public:
    PixmapC(const WidgetC &rootWin,const char * const *data)
      : WidgetC(*new PixmapBodyC(data,rootWin))
    {}
    //: Constructor
    
    PixmapC(const WidgetC &rootWin,const StringC &filename)
      : WidgetC(*new PixmapBodyC(filename,rootWin))
    {}
    //: Constructor
    
    PixmapC(const WidgetC &rootWin, const ImageC<ByteRGBValueC>& im) 
      : WidgetC(*new PixmapBodyC(im,rootWin))
    {}
    //: Constructor.

    explicit PixmapC(const char *const *data)
      : WidgetC(*new PixmapBodyC(data))
    {}
    //: Constructor
    
    explicit PixmapC(const StringC &filename)
      : WidgetC(*new PixmapBodyC(filename))
    {}
    //: Constructor
    
    explicit PixmapC(const ImageC<ByteRGBValueC>& im)
      : WidgetC(*new PixmapBodyC(im))
    {}
    //: Constructor.

    explicit PixmapC(int width,int height,int depth)
      : WidgetC(*new PixmapBodyC(width,height,depth))
    {}
    //: Constructor.
    
    PixmapC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    PixmapC(PixmapBodyC &body) : 
      WidgetC(body)
    {}
    //: Body constructor.
    
    PixmapC(const PixmapBodyC *body) :
      WidgetC(body)
    {}
    //: Body constructor.
    
    PixmapBodyC &Body()
    { return static_cast<PixmapBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const PixmapBodyC &Body() const
    { return static_cast<const PixmapBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    GdkPixmap* &Pixmap()
    { return Body().Pixmap(); }
    //: Access pixmap mask.
    
    GdkBitmap* &Mask()
    { return Body().Mask(); }
    //: Access pixmap mask.
    
    bool GUIGetSize(int &width,int &height)
    { return Body().GUIGetSize(width,height); }
    //: Get size of pixmap.
    // Will return false and leave 'width' and 'height' unchanged
    // if unknown (The pixmap is unallocated. )
    // Call only with the GUI thread.

    bool GUISetPixmap(PixmapC &pix)
    { return Body().GUISetPixmap(pix); }
    //: Copy a pixmap into the current pixmap
    
    bool SetPixmap(PixmapC &pix)
    { return Body().SetPixmap(pix); }
    //: Copy a pixmap into the current pixmap

    friend class PixmapBodyC;
  };
  
  extern const char * xpmData_OpenFile[];

};

#endif
