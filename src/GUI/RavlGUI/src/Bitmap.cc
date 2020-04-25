// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Bitmap.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Bitmap.hh"
#include <gdk/gdk.h>
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  //: Create a bit map of the given size.
  
  BitmapBodyC::BitmapBodyC(int nwidth,int nheight) 
    : width(nwidth),
      height(nheight),
      bmp(0)
  {
    WidgetC rootWin;
    if(!rootWin.IsValid())
      rootWin = Manager.GetRootWindow();
    rootWin.GUIShow();
    RavlAssert(rootWin.Widget() != 0);
    GdkWindow *window = rootWin.Widget()->window;
    bmp = gdk_pixmap_new(window,width,height,1);
    gdk_bitmap_ref(bmp);
  }
  
  //: Destructor.
  
  BitmapBodyC::~BitmapBodyC() {
    if(bmp!= 0)
      gdk_bitmap_unref(bmp);
  }
  
  //: Clear the bitmap.
  
  void BitmapBodyC::Clear() {
    //gdk_window_clear(bmp);
    GdkGC *agc = gdk_gc_new(bmp);
    GdkColor col;
    col.pixel = 0;
    col.red = 0;
    col.green = 0;
    col.blue = 0;
    
    gdk_gc_set_foreground(agc,&col);
    gdk_draw_rectangle(bmp,agc,1,0,0,width,height);
    gdk_gc_destroy(agc);
  }
}
