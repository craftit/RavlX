// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Cursor.cc"

#include <gdk/gdktypes.h>
#include <gdk/gdk.h>
#include "Ravl/GUI/Cursor.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Hash.hh"

#define GDKCURSOR(x) Tuple2C<const char *,GdkCursorType>(# x ,x)

namespace RavlGUIN {

  static
  Tuple2C<const char *,GdkCursorType> cursorNames[] = 
  {
    GDKCURSOR(GDK_X_CURSOR),
    GDKCURSOR(GDK_ARROW),
    GDKCURSOR(GDK_BASED_ARROW_DOWN),
    GDKCURSOR(GDK_BASED_ARROW_UP),
    GDKCURSOR(GDK_BOAT),
    GDKCURSOR(GDK_BOGOSITY),
    GDKCURSOR(GDK_BOTTOM_LEFT_CORNER),
    GDKCURSOR(GDK_BOTTOM_RIGHT_CORNER),
    GDKCURSOR(GDK_BOTTOM_SIDE),
    GDKCURSOR(GDK_BOTTOM_TEE),
    GDKCURSOR(GDK_BOX_SPIRAL),
    GDKCURSOR(GDK_CENTER_PTR),
    GDKCURSOR(GDK_CIRCLE),
    GDKCURSOR(GDK_CLOCK),
    GDKCURSOR(GDK_COFFEE_MUG),
    GDKCURSOR(GDK_CROSS),
    GDKCURSOR(GDK_CROSS_REVERSE),
    GDKCURSOR(GDK_CROSSHAIR),
    GDKCURSOR(GDK_DIAMOND_CROSS),
    GDKCURSOR(GDK_DOT),
    GDKCURSOR(GDK_DOTBOX),
    GDKCURSOR(GDK_DOUBLE_ARROW),
    GDKCURSOR(GDK_DRAFT_LARGE),
    GDKCURSOR(GDK_DRAFT_SMALL),
    GDKCURSOR(GDK_DRAPED_BOX),
    GDKCURSOR(GDK_EXCHANGE),
    GDKCURSOR(GDK_FLEUR),
    GDKCURSOR(GDK_GOBBLER),
    GDKCURSOR(GDK_GUMBY),
    GDKCURSOR(GDK_HAND1),
    GDKCURSOR(GDK_HAND2),
    GDKCURSOR(GDK_HEART),
    GDKCURSOR(GDK_ICON),
    GDKCURSOR(GDK_IRON_CROSS),
    GDKCURSOR(GDK_LEFT_PTR),
    GDKCURSOR(GDK_LEFT_SIDE),
    GDKCURSOR(GDK_LEFT_TEE),
    GDKCURSOR(GDK_LEFTBUTTON),
    GDKCURSOR(GDK_LL_ANGLE),
    GDKCURSOR(GDK_LR_ANGLE),
    GDKCURSOR(GDK_MAN),
    GDKCURSOR(GDK_MIDDLEBUTTON),
    GDKCURSOR(GDK_MOUSE),
    GDKCURSOR(GDK_PENCIL),
    GDKCURSOR(GDK_PIRATE),
    GDKCURSOR(GDK_PLUS),
    GDKCURSOR(GDK_QUESTION_ARROW),
    GDKCURSOR(GDK_RIGHT_PTR),
    GDKCURSOR(GDK_RIGHT_SIDE),
    GDKCURSOR(GDK_RIGHT_TEE),
    GDKCURSOR(GDK_RIGHTBUTTON),
    GDKCURSOR(GDK_RTL_LOGO),
    GDKCURSOR(GDK_SAILBOAT),
    GDKCURSOR(GDK_SB_DOWN_ARROW),
    GDKCURSOR(GDK_SB_H_DOUBLE_ARROW),
    GDKCURSOR(GDK_SB_LEFT_ARROW),
    GDKCURSOR(GDK_SB_RIGHT_ARROW),
    GDKCURSOR(GDK_SB_UP_ARROW),
    GDKCURSOR(GDK_SB_V_DOUBLE_ARROW),
    GDKCURSOR(GDK_SHUTTLE),
    GDKCURSOR(GDK_SIZING),
    GDKCURSOR(GDK_SPIDER),
    GDKCURSOR(GDK_SPRAYCAN),
    GDKCURSOR(GDK_STAR),
    GDKCURSOR(GDK_TARGET),
    GDKCURSOR(GDK_TCROSS),
    GDKCURSOR(GDK_TOP_LEFT_ARROW),
    GDKCURSOR(GDK_TOP_LEFT_CORNER),
    GDKCURSOR(GDK_TOP_RIGHT_CORNER),
    GDKCURSOR(GDK_TOP_SIDE),
    GDKCURSOR(GDK_TOP_TEE),
    GDKCURSOR(GDK_TREK),
    GDKCURSOR(GDK_UL_ANGLE),
    GDKCURSOR(GDK_UMBRELLA),
    GDKCURSOR(GDK_UR_ANGLE),
    GDKCURSOR(GDK_WATCH),
    GDKCURSOR(GDK_XTERM),
    GDKCURSOR(GDK_X_CURSOR),
  };
  
  static HashC<const char *,GdkCursorType> cursorTable(cursorNames);
  
  const static gchar g_emptyCursor[] = 
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  const static gchar g_emptyCursorMask[] = 
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  //: Constructor.
  
  CursorBodyC::CursorBodyC(const char *name)
    : cursor(0),
      cname(name)
  {}
  
  //: Get the cursor.

  GdkCursor *CursorBodyC::Cursor() {
    if(cursor != 0)
      return cursor;
    if(cursorTable.IsElm(cname)) {
      GdkCursorType ct = cursorTable[cname];
      cursor = gdk_cursor_new(ct);
      return cursor;
    }
    else
    {
      if (StringC(cname) == "NONE")
      {
        GdkColor fg = { 0, 65535, 65535, 65535 }; /* White. */
        GdkColor bg = { 0,     0,     0,     0 }; /* Black. */
        GdkPixmap *source = gdk_bitmap_create_from_data(NULL, g_emptyCursor, 16, 16);
        GdkPixmap *mask = gdk_bitmap_create_from_data(NULL, g_emptyCursorMask, 16, 16);
        cursor = gdk_cursor_new_from_pixmap(source, mask, &fg, &bg, 8, 8);
        gdk_pixmap_unref(source);
        gdk_pixmap_unref(mask);
      }
      else
        std::cerr << "CursorBodyC::Cursor(), Load pixmap cursor not implemented for '" << cname << "'\n";
    }
    return 0;
  }
  
  //: Set cursor in window to this one.
  
  void CursorBodyC::SetWindow(GdkWindow *win) {
    if(win == 0 || Cursor() == 0)
      return ;
    gdk_window_set_cursor(win,cursor);
  }
  
  CursorBodyC::~CursorBodyC() {
    if(cursor != 0) {
      gdk_cursor_destroy(cursor);
      cursor = 0;
    }
  }

}
