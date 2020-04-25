// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_WIDGETSTYLE_HEADER
#define RAVLGUI_WIDGETSTYLE_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/WidgetStyle.hh"
//! lib=RavlGUI
//! author="James Smith"
//! date="28/10/2002"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Styles"
//! example="exStyle.cc"

#include "Ravl/GUI/GTKTypes.hh"
#include "Ravl/String.hh"
#include "Ravl/Image/ByteRGBValue.hh"

#include <gtk/gtkenums.h>

// Namespace fix for Visual C++
#if RAVL_COMPILER_VISUALCPP6
#ifndef RavlGUIN
#define RavlGUIN RavlN
#endif
#endif

namespace RavlGUIN {
  class PixmapC;
  class WidgetC;
  using namespace RavlN;
  using RavlImageN::ByteRGBValueC;
  
  //! userlevel=Normal
  enum WidgetColourTypeT {
    WIDGET_COLOUR_FG,
    WIDGET_COLOUR_BG,
    WIDGET_COLOUR_LIGHT,
    WIDGET_COLOUR_DARK,
    WIDGET_COLOUR_MID,
    WIDGET_COLOUR_TEXT,
    WIDGET_COLOUR_BASE,
    WIDGET_COLOUR_BLACK,
    WIDGET_COLOUR_WHITE
  };
  //: Specifies which colour to set
  
  //! userlevel=Develop
  //: Widget style

  class WidgetStyleBodyC
    : public RCBodyVC 
  {
  public:
    WidgetStyleBodyC();
    //: Default constructor.
    
    virtual ~WidgetStyleBodyC();
    //: Destructor.
    
    GtkStyle *Style() { return style; }
    //: Access gtk style.
    
    void LoadDefault();
    //: Loads the default style

    void SetBackground(const PixmapC& pixmap);
    //: Set the background of the widget

    void SetBackground(const PixmapC& pixmap, GtkStateType& state);
    //: Set the background of the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.
    
    void SetColour(WidgetColourTypeT& type, ByteRGBValueC& col);
    //: Set a colour for the widget
    
    void SetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state);
    //: Set a colour for the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    void SetFont(StringC& strFontDesc);
    //: Set a font for the widget to use
    // strFontDesc is an XLFD string, such as "-*-helvetica-bold-r-normal--*-120-*-*-*-*-iso8859-1".
    // XLFD strings are described <a href="http://library.gnome.org/devel/gdk/stable/gdk-Fonts.html">here</a>
    
  protected:

    bool GUILoadDefault();
    //: Loads the default style

    bool GUISetBackground(PixmapC& pixmap, GtkStateType state);
    // Sets the background to a pixmap
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetBackground(PixmapC& pixmap);
    //: Sets the background of the widget

    GdkColor GUIRGB2Colour(ByteRGBValueC& rgb);
    // Loads a colour from a ByteRGBValueC

    bool GUISetColour(WidgetColourTypeT type, GdkColor color, GtkStateType state);
    // Sets a colour 
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col);
    //: Set a colour for the widget
    
    bool GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state);
    //: Set a colour for the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetFont(StringC& strFontDesc);
    //: Set a font for the widget to use
    // strFontDesc is an XLFD string, such as "-*-helvetica-bold-r-normal--*-120-*-*-*-*-iso8859-1".
    // XLFD strings are described <a href="http://library.gnome.org/devel/gdk/stable/gdk-Fonts.html">here</a>

    GtkStyle *style;
    //: The GTK style itself
    
    friend class WidgetStyleC;

  };
  
  //! userlevel=Normal
  //: Widget style
  
  class WidgetStyleC
    : public RCHandleC<WidgetStyleBodyC>
  {
    
  public:
    WidgetStyleC()
      : RCHandleC<WidgetStyleBodyC>(*new WidgetStyleBodyC())
    {}
    //: Constructor.
    // Creates default style
    // FIXME:- This is the opposite behavour to normal.
    
    WidgetStyleC(bool)
    {}
    //: Constructor.
    // Creates an invalid handle.
    // FIXME:- This is the opposite behavour to normal.
    
  protected:
    
    WidgetStyleC(WidgetStyleBodyC &bod) 
      : RCHandleC<WidgetStyleBodyC>(bod)
    {}
    //: Body Constructor.
    
    WidgetStyleBodyC &Body() 
    { return RCHandleC<WidgetStyleBodyC>::Body(); }
    //: Access body.
    
    const WidgetStyleBodyC &Body() const
    { return RCHandleC<WidgetStyleBodyC>::Body(); }
    //: Access body.  
  
  public:

    GtkStyle *Style() 
    { return Body().Style(); }
    //: Access gtk style.
    
    void LoadDefault() 
    { Body().LoadDefault(); }
    //: Loads the default style

    void SetBackground(const PixmapC& pixmap)
    { Body().SetBackground(pixmap); }
    //: Set the background of the widget

    void SetBackground(const PixmapC& pixmap, GtkStateType& state)
    { Body().SetBackground(pixmap,state); }
    //: Set the background of the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.
    
    void SetColour(WidgetColourTypeT& type, ByteRGBValueC& col)
    { Body().SetColour(type,col); }
    //: Set a colour for the widget
    
    void SetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state)
    { Body().SetColour(type,col,state); }
    //: Set a colour for the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    void SetFont(StringC& strFontDesc)
    { Body().SetFont(strFontDesc); }
    //: Set a font for the widget to use
    // strFontDesc is an XLFD string, such as "-*-helvetica-bold-r-normal--*-120-*-*-*-*-iso8859-1".
    // XLFD strings are described <a href="http://library.gnome.org/devel/gdk/stable/gdk-Fonts.html">here</a>

  protected:

    bool GUILoadDefault()
    { return Body().GUILoadDefault(); }
    //: Loads the default style

    bool GUISetBackground(PixmapC& pixmap)
    { return Body().GUISetBackground(pixmap); }
    //: Sets the background of the widget

    bool GUISetBackground(PixmapC& pixmap, GtkStateType& state) 
    { return Body().GUISetBackground(pixmap,state); }
    //: Sets the background of the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col)
    { return Body().GUISetColour(type,col); }
    //: Set a colour for the widget
    
    bool GUISetColour(WidgetColourTypeT& type, ByteRGBValueC& col, GtkStateType& state)
    { return Body().GUISetColour(type,col,state); }
    //: Set a colour for the widget
    // Values for state are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSTATETYPE">here</A>.

    bool GUISetFont(StringC& strFontDesc)
    { return Body().GUISetFont(strFontDesc); }
    //: Set a font for the widget to use
    // GTK 2.x: strFontDesc is a Pango font description string, such as "Arial 12".
    // Pango strings are described <a href="http://library.gnome.org/devel/pango/stable/pango-Fonts.html#pango-font-description-from-string">here</a><br>
    // GTK 1.x: strFontDesc is an XLFD string, such as "-*-helvetica-bold-r-normal--*-120-*-*-*-*-iso8859-1".
    // XLFD strings are described <a href="http://library.gnome.org/devel/gdk/stable/gdk-Fonts.html">here</a>

   friend class WidgetStyleBodyC;

  };

}
#endif
