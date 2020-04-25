// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Pixmap.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Pixmap.hh"
#include <gtk/gtk.h>
  
#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  PixmapBodyC::PixmapBodyC(const char * const *ndata,const WidgetC &nrootWin)
    : rootWin(nrootWin),
      pixmap(0),
      mask(0),
      data(ndata)
  {} 
  
  //: Constructor.
  
  PixmapBodyC::PixmapBodyC(const StringC &nfilename,const WidgetC &rwin)
    : rootWin(rwin),
      pixmap(0),
      mask(0),
      filename(nfilename),
      data(0)
  {
    ONDEBUG(std::cerr << "PixmapBodyC::PixmapBodyC() fn:'" << filename << "'\n");
  }
  
  //: Constructor.

  PixmapBodyC::PixmapBodyC(const ImageC<ByteRGBValueC>& im,const WidgetC &rwin) 
    : rootWin(rwin),
      pixmap(0),
      mask(0),
      data(0),
      image(im)
  {
    ONDEBUG(std::cerr << "PixmapBodyC::PixmapBodyC() image constructor\n");
  }


  //: Constructor.
  
  PixmapBodyC::PixmapBodyC(int nwidth,int nheight,int ndepth) 
    : pixmap(0),
      mask(0),
      data(0),
      width(nwidth),
      height(nheight),
      depth(ndepth)
  {}
  
  //: Create the widget.
  
  bool PixmapBodyC::CommonCreate(GtkWidget *_widget)
  {
    if(widget != 0)
      return true; // Shown already.
    
    if(!rootWin.IsValid())
      rootWin = Manager.GetRootWindow();
    rootWin.GUIShow();
    RavlAssert(rootWin.Widget() != 0);
    GdkWindow *window = rootWin.Widget()->window;
    GtkStyle *style = gtk_widget_get_style(rootWin.Widget());  
    
    /* now for the pixmap from gdk */
    
    if(data != 0) {
      pixmap = gdk_pixmap_create_from_xpm_d(window,  &mask,
					    &style->bg[GTK_STATE_NORMAL],
					    (gchar **)data );
    } else if (!filename.IsEmpty()) {
      pixmap = gdk_pixmap_create_from_xpm(window,  &mask,
					  &style->bg[GTK_STATE_NORMAL],
					  filename );
    } else if (!image.IsEmpty()) {
      // Create new pixmap
      pixmap = gdk_pixmap_new(rootWin.Widget()->window,
			      image.Cols(),
			      image.Rows(),
			      -1);
      // Load image into pixmap
      if (pixmap && style->black_gc) {
        gdk_draw_rgb_image(pixmap,
			               style->black_gc,
			               0,0,
			               image.Cols(),image.Rows(),
			               GDK_RGB_DITHER_NORMAL,
			               (unsigned char *) image.Row(image.TRow()),
			               image.Cols() * 3);
      }
    }
    else {
      pixmap = gdk_pixmap_new(window,width,height,depth);
    }

    if(pixmap == 0) {
      // As a last resort... 
      std::cerr << "PixmapBodyC::Create(), No pixmap specified. \n";
      pixmap = gdk_pixmap_create_from_xpm_d( window,  &mask,
					     &style->bg[GTK_STATE_NORMAL],
					     (char **) xpmData_OpenFile);
      
    }
    
    /* a pixmap widget to contain the pixmap */
    if (_widget == NULL)
    {
      widget = gtk_pixmap_new(pixmap, mask);
    }
    else
    {
      widget = _widget;
      gtk_pixmap_set(GTK_PIXMAP(widget), pixmap, mask);
    }

    rootWin.Invalidate(); // Finished with it, so let it go.
    return true;
  }
  
  //: Get size of pixmap.
  // Will return false if unknown. 
  
  bool PixmapBodyC::GUIGetSize(int &width,int &height) {
    if(pixmap == 0)
      return false;
    gdk_window_get_size(pixmap,&width,&height);
    return true;
  }


  
  bool PixmapBodyC::GUISetPixmap(PixmapC &pix)
  {
    if (pixmap == 0)
      return false;
    
    GdkPixmap *val = NULL, *mask = NULL;
    gtk_pixmap_get(GTK_PIXMAP(pix.Widget()), &val, &mask);
    
    if (val != NULL && mask != NULL)
    {
      gtk_pixmap_set(GTK_PIXMAP(widget), val, mask);
    }
    
    return true;
  }


  
  bool PixmapBodyC::SetPixmap(PixmapC &pix)
  {
    Manager.Queue(Trigger(PixmapC(*this), &PixmapC::GUISetPixmap, pix));
    return true;
  }
  
  
  
  /* XPM data of Open-File icon */
  const char * xpmData_OpenFile[] = {
    "16 16 3 1",
    "       c None",
    ".      c #000000000000",
    "X      c #FFFFFFFFFFFF",
    "                ",
    "   ......       ",
    "   .XXX.X.      ",
    "   .XXX.XX.     ",
    "   .XXX.XXX.    ",
    "   .XXX.....    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .XXXXXXX.    ",
    "   .........    ",
    "                ",
    "                "};
}
