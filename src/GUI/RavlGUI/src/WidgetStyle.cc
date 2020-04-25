// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/WidgetStyle.cc"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/WidgetStyle.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Pixmap.hh"
#include <gtk/gtk.h>
#include <gdk/gdktypes.h>
#include <gdk/gdk.h>
#include <pango/pango.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  using namespace RavlImageN;
  using namespace RavlN;
  
  WidgetStyleBodyC::WidgetStyleBodyC() {
    // Load default style
    LoadDefault();
  }

  WidgetStyleBodyC::~WidgetStyleBodyC() {
    // Unref background pixmaps
    for (int i=0; i<5; i++) {
      if (style->bg_pixmap[i]) {
	gdk_pixmap_unref(style->bg_pixmap[i]);
	style->bg_pixmap[i] = NULL;
      }
    }
  }

  void WidgetStyleBodyC::LoadDefault() {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUILoadDefault));
  }

  bool WidgetStyleBodyC::GUILoadDefault() {
    // Copy default style
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    style = gtk_style_copy(gtk_widget_get_default_style());
    // Done
    return style != 0;
  }

  bool WidgetStyleBodyC::GUISetBackground(PixmapC& pixmap, GtkStateType state) {
    // Create pixmap if not already done
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if (!pixmap.Widget())
      pixmap.Create();
    // Check required data
    if (style == NULL) return false;
    // Incrememt refcount
    if (pixmap.Pixmap() != NULL) {
      gdk_pixmap_ref(pixmap.Pixmap());
    }
    // Set pointer
    style->bg_pixmap[state] = pixmap.Pixmap();
    // Done
    return true;
  }

  bool WidgetStyleBodyC::GUISetBackground(PixmapC& pixmap) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    bool bSuccess = true;
    bSuccess &= GUISetBackground(pixmap,GTK_STATE_NORMAL);
    bSuccess &= GUISetBackground(pixmap,GTK_STATE_ACTIVE);
    bSuccess &= GUISetBackground(pixmap,GTK_STATE_PRELIGHT);
    bSuccess &= GUISetBackground(pixmap,GTK_STATE_SELECTED);
    bSuccess &= GUISetBackground(pixmap,GTK_STATE_INSENSITIVE);
    return bSuccess;
  }

  //: Set the background of the window.
  
  void WidgetStyleBodyC::SetBackground(const PixmapC& pixmap, GtkStateType& state) {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUISetBackground,pixmap,state));
  }
  
  void WidgetStyleBodyC::SetBackground(const PixmapC& pixmap) {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUISetBackground,pixmap));
  }

  GdkColor WidgetStyleBodyC::GUIRGB2Colour(ByteRGBValueC& rgb) {
    // Get system colourmap
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GdkColormap *cmap = gdk_colormap_get_system();
    // Create colour
    GdkColor col;
    col.pixel = 0;
    col.red   = rgb.Red()   * 256;
    col.green = rgb.Green() * 256;
    col.blue  = rgb.Blue()  * 256;
    // Allocate our new colour in the colour map
    if (!gdk_colormap_alloc_color(cmap,&col,TRUE,TRUE)) {
      std::cerr << "Could not allocate colour!" << std::endl;
    }
    // Done
    return col;
  }

  bool WidgetStyleBodyC::GUISetColour(WidgetColourTypeT type, GdkColor color, GtkStateType state) {
    // Check required data
    if (style==NULL) return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Set colour
    switch (type) {
    case WIDGET_COLOUR_FG:
      style->fg[state] = color;
      break;
    case WIDGET_COLOUR_BG:
      style->bg[state] = color;
      break;
    case WIDGET_COLOUR_LIGHT:
      style->light[state] = color;
      break;
    case WIDGET_COLOUR_DARK:
      style->dark[state] = color;
      break;
    case WIDGET_COLOUR_MID:
      style->mid[state] = color;
      break;
    case WIDGET_COLOUR_TEXT:
      style->text[state] = color;
      break;
    case WIDGET_COLOUR_BASE:
      style->base[state] = color;
      break;
    case WIDGET_COLOUR_BLACK:
      style->black = color;
      break;
    case WIDGET_COLOUR_WHITE:
      style->white = color;
      break;
    default:
      break;
    }
    return true;
  }

  bool WidgetStyleBodyC::GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state) {
    return GUISetColour(type,GUIRGB2Colour(col),state);
  }

  bool WidgetStyleBodyC::GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GdkColor gdkcol = GUIRGB2Colour(col);
    bool bSuccess = true;
    bSuccess &= GUISetColour(type,gdkcol,GTK_STATE_NORMAL);
    bSuccess &= GUISetColour(type,gdkcol,GTK_STATE_ACTIVE);
    bSuccess &= GUISetColour(type,gdkcol,GTK_STATE_PRELIGHT);
    bSuccess &= GUISetColour(type,gdkcol,GTK_STATE_SELECTED);
    bSuccess &= GUISetColour(type,gdkcol,GTK_STATE_INSENSITIVE);
    return bSuccess;
  }

  void WidgetStyleBodyC::SetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state) {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUISetColour,type,col,state));
  }

  void WidgetStyleBodyC::SetColour(WidgetColourTypeT& type, ByteRGBValueC& col) {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUISetColour,type,col));
  }

  void WidgetStyleBodyC::SetFont(StringC& strFontDesc) {
    Manager.Queue(Trigger(WidgetStyleC(*this),&WidgetStyleC::GUISetFont,strFontDesc));
  }

  bool WidgetStyleBodyC::GUISetFont(StringC& strFontDesc) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    PangoFontDescription *font_desc = pango_font_description_from_string(strFontDesc);
    if (font_desc)
      style->font_desc = font_desc;

    return true;
  }


}

