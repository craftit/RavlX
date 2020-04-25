// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_WINDOW_HEADER
#define RAVLGUI_WINDOW_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Window.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/OneChild.hh"
#include "Ravl/GUI/Cursor.hh"
#include "Ravl/GUI/Pixbuf.hh"

#include <gtk/gtkenums.h>

#define GTK_WINDOW_DIALOG GTK_WINDOW_TOPLEVEL

namespace RavlGUIN {
  
  class CursorC;
  class WindowC;
  class GladeWindowC;
  
  //! userlevel=Develop
  //: Window body.
  
  class WindowBodyC 
    : public OneChildBodyC
  {
  public:
    WindowBodyC(int sx = 1,int sy = 1,const char *title = 0,GtkWindowType winType = GTK_WINDOW_TOPLEVEL,
                int nborder = 0,bool rootWin = true,bool connectDeleteEvent = true);
    //: Constructor.
    // Values for winType are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowType">here</A>.
    
    virtual ~WindowBodyC();
    //: Destructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Close();
    //: Close window.
    // Thread safe.
    // If you override this, you MUST call WindowBodyC::Close() at the end of your Close() function
    
    void SetCursor(CursorC &cur);
    //: Set cursor.
    
    void SetTitle(const StringC &str);
    //: Set the title of the window.
  
    void SetPositioning(GtkWindowPosition& pos);
    //: Influences positioning of the window.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition">here</A>.
    
    void UserResizable(bool& resizable);
    //: Allow the user to resize the window?
    // Default is true. If set to false, window will shrink to the minimum size dictated by it's children.

    void SetDecorated(bool& decorated);
    //: Does the window have decorations?
    // Default value is true. If false, the window manager shouldn't decorate the window
    // This may not have any effect at all, depending on the window manager. I wouldn't rely on it.

    void Raise();
    //: Raises the window

    void Lower();
    //: Lowers the window

    void Maximise(bool& maximise);
    //: Maximise the window - requires GTK >= 2
    // maximise=true  : maximise
    // maximise=false : unmaximise

    void SetModal(bool& modal);
    //: Makes the window modal

    void MakeTransient(WindowC& parent, GtkWindowPosition &position);
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times

    void MakeTransient(WindowC& parent);
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.  Also the window will be centred on the parent.

    void SetFullScreen(bool &fullscreen);
    //: Set the window as fullscreen (assuming the window manager supports it)

    bool IsFullScreen();
    //: Find out whether or not the window is in "full screen" mode
    
    bool GUISetTitle(StringC &str);
    //: Set the title of the window.
    
    bool GUISetIcon(const PixbufC &pix);
    //: Set an icon for the window
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    // Used to avoid problems with circluar references.
    // This should undo all references to RAVL and GUI
    // objects which are not in the destroy tree. 
    // The function is called by the root window in its
    // destructor.
    
    virtual void WidgetDestroy();
    //: Called when the underlying widget it destroyed.
    
    bool GUISetPositioning(GtkWindowPosition& pos);
    //: Influences positioning of the window.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition">here</A>.

    
    bool GUICloseDown();
    //: Close down window.
    
    bool GUIUpdateCursor(CursorC &newCursor);
    //: Update cursor.
  
    bool GUIUserResizable(bool& resizable);
    //: Allow the user to resize the window?
    // Default is true. If set to false, window will shrink to the minimum size dictated by it's children.

    bool GUISetDecorated(bool& decorated);
    //: Does the window have decorations?
    // Default value is true. If false, the window manager shouldn't decorate the window
    // This may not have any effect at all, depending on the window manager. I wouldn't rely on it.

    bool GUIRaise();
    //: Raises the window

    bool GUILower();
    //: Lowers the window

    bool GUIMaximise(bool& maximise);
    //: Maximise the window - requires GTK >= 2
    // maximise=true  : maximise
    // maximise=false : unmaximise

    bool GUISetModal(bool& modal);
    //: Makes the window modal

    bool GUIMakeTransient(OneChildC& parent, GtkWindowPosition &position);
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.

    bool GUIMakeTransient(OneChildC& parent);
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.  Also the window will be centred on the parent.
    
    virtual bool GUIShow();
    //: Show window on display.
    
    bool GUISetFullScreen(bool &fullscreen);
    //: Set the window as fullscreen (assuming the window manager supports it)

    int sx,sy;
    int border;
    StringC title;
    bool rootWin;
    bool closeDown; // Indicates close down is in progress or pending.
    CursorC cursor; // Invalid == default.
    bool cursorChange;
    bool userresizable; // Used only to cache UserResizable calls - irrelevant after Create()
    bool m_bDecorated; // Used only to cache SetDecorated calls - irrelevant after Create()
    GtkWindowType winType;
    OneChildC m_wParent; // Used only to cache MakeTransient calls - irrelevant after Create()
    GtkWindowPosition m_wPosition;
    bool isFullscreen;
    bool connectDeleteEvent;
    PixbufC icon;
    
    friend class WindowC;
  };
  
  //! userlevel=Normal
  //: Window handle.
  // Note: A window can only have ONE child widget, if you want more than
  // one thing in the window you have to add a layout widget first,
  // and add things to that.
  
  class WindowC 
    : public OneChildC
  {
  public:
    WindowC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    WindowC(int col,int row = 1,const char *ntitle = 0,
            GtkWindowType winType = GTK_WINDOW_TOPLEVEL,
            int borderWidth = 0,
            bool rootWin = true,
            bool connectDeleteEvent = true
            );
    //: Constructor.
    //!param: col, row - nominal window size
    //!param: winType - Values for winType are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowType">here</A>.
    //!param: borderWidth - border width
    //!param: rootWin - if true, closing this window will terminate window manager
    //!param: connectDeleteEvent - if true, window is closed on receipt of signal "delete_event"

  protected:
    WindowC(WindowBodyC &bod)
      : OneChildC(bod)
    {}
    //: Body constructor.
    
    WindowBodyC &Body() 
    { return static_cast<WindowBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
    const WindowBodyC  &Body() const
    { return static_cast<const WindowBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
    bool GUICloseDown()
    { return Body().GUICloseDown(); }
    //: Close down window.
    
    bool GUIUpdateCursor(CursorC &newCursor)
    { return Body().GUIUpdateCursor(newCursor); }
    //: Update cursor.
    
    bool GUISetPositioning(GtkWindowPosition& pos)
    { return Body().GUISetPositioning(pos); }
    //: Influences positioning of the window.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition">here</A>.

    bool GUIUserResizable(bool& resizable) 
    { return Body().GUIUserResizable(resizable); }
    //: Allow the user to resize the window?
    // Default is true. If set to false, window will shrink to the minimum size dictated by it's children.

    bool GUISetDecorated(bool& decorated)
    { return Body().GUISetDecorated(decorated); }
    //: Does the window have decorations?
    // Default value is true. If false, the window manager shouldn't decorate the window
    // This may not have any effect at all, depending on the window manager. I wouldn't rely on it.

    bool GUIMaximise(bool& maximise)
    { return Body().GUIMaximise(maximise); }
    //: Maximise the window - requires GTK >= 2
    // maximise=true  : maximise
    // maximise=false : unmaximise

    bool GUISetModal(bool& modal) 
    { return Body().GUISetModal(modal); }
    //: Makes the window modal
    
    bool GUISetFullScreen(bool &fullscreen) {
      return Body().GUISetFullScreen(fullscreen);
    }
    //: Set the window as fullscreen (assuming the window manager supports it)

 public:
    
    void UserResizable(bool& resizable) 
    { Body().UserResizable(resizable); }
    //: Allow the user to resize the window?
    // Default is true. If set to false, window will shrink to the minimum size dictated by it's children.

    void Raise()
    { Body().Raise(); }
    //: Raises the window
    
    void Lower() 
    { Body().Lower(); }
    //: Lowers the window
    
     bool GUIRaise()
    { return Body().GUIRaise(); }
    //: Raises the window

    bool GUILower() 
    { return Body().GUILower(); }
    //: Lowers the window

    void Maximise(bool& maximise)
    { Body().Maximise(maximise); }
    //: Maximise the window - requires GTK >= 2
    // maximise=true  : maximise
    // maximise=false : unmaximise

    void SetModal(bool& modal)
    { Body().SetModal(modal); }
    //: Makes the window modal

    void MakeTransient(WindowC& parent, GtkWindowPosition &position)
    { Body().MakeTransient(parent,position); }
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.

    void MakeTransient(WindowC& parent) {
      Body().MakeTransient(parent);
    }
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.  Also the window will be centred on the parent.

    void SetDecorated(bool& decorated)
    { Body().SetDecorated(decorated); }
    //: Does the window have decorations?
    // Default value is true. If false, the window manager shouldn't decorate the window
    // This may not have any effect at all, depending on the window manager. I wouldn't rely on it.

    void SetTitle(const StringC &str)
    { Body().SetTitle(str); }
    //: Set the title of the window.
    
    bool GUISetTitle(StringC &str)
    { return Body().GUISetTitle(str); }
    //: Set the title of the window.
    // GUI thread only.
    
    bool GUISetIcon(const PixbufC &pix)
    { return Body().GUISetIcon(pix); }
    //: Set an icon for the window
    
    void SetPositioning(GtkWindowPosition& pos)
    { Body().SetPositioning(pos); }
    //: Influences positioning of the window.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition">here</A>.

    bool Close()
    { return Body().Close(); }
    //: Close window.
    // If window is root window, window manager is terminated.<br>
    // Thread safe.
    
    void SetCursor(CursorC &cur)
    { Body().SetCursor(cur); }
    //: Set cursor.
    
    bool GUIMakeTransient(WindowC& parent,GtkWindowPosition &position)
    { return Body().GUIMakeTransient(parent,position); }
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.
    
    bool GUIMakeTransient(WindowC& parent)
    { return Body().GUIMakeTransient(parent); }
    //: Makes this window transient for the parent
    // This means it will stay on top of it at all times.  Also the window will be centred on the parent.
    
    void SetFullScreen(bool &fullscreen) 
    { Body().SetFullScreen(fullscreen); }
    //: Set the window as fullscreen (assuming the window manager supports it)
    
    bool IsFullScreen() 
    { return Body().IsFullScreen(); }
    //: Find out whether or not the window is in "full screen" mode
    
    static bool GUISetDefaultIcon(const PixbufC &pix);
    //: Set a default icon for all windows.

    friend class WindowBodyC;
    friend class GladeWindowC;
  };

}

#endif
