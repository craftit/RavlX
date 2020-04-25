// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Window.cc"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/CallMethods.hh"

#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN {
  
  static int rootWinCount = 0;
  // when invoked (via signal delete_event), terminates the application.
  

  //: Constructor.
  
  WindowBodyC::WindowBodyC(int nsx,int nsy,const char *ntitle,
                           GtkWindowType nWinType,
                           int nborder,
                           bool nrootWin,
                           bool nconnectDeleteEvent)
    : sx(nsx), sy(nsy),
      border(nborder),
      title(ntitle),
      rootWin(nrootWin),
      closeDown(false),
      cursorChange(false),
      userresizable(true),
      m_bDecorated(true),
      winType(nWinType),
      isFullscreen(false),
      connectDeleteEvent(nconnectDeleteEvent)
  {
    if(rootWin)
      Manager.GetRootWindow() = WindowC(*this);
  }
  
  //: Destructor.
  
  WindowBodyC::~WindowBodyC() {
    ONDEBUG(std::cerr << "WindowBodyC::~WindowBodyC() Started." << ((void *) this) << " \n");
    Destroy();
    ONDEBUG(std::cerr << "WindowBodyC::~WindowBodyC() Done. " << ((void *) this) << " \n");
  }
  
  //: Create the widget.
  
  bool WindowBodyC::Create() {
    ONDEBUG(std::cerr << "WindowBodyC::Create(), Title=" << title << " Root=" << rootWin << " Type=" << ((int) winType) << " Border=" << border << "\n");
    if(widget == 0) {
      widget = gtk_window_new (winType);  
      if(rootWin && winType == GTK_WINDOW_TOPLEVEL) {
        rootWinCount++;
      }
      if (connectDeleteEvent) {
        ConnectRef(Signal("delete_event"),*this,&WindowBodyC::Close);
      }
      if(title != 0)
        gtk_window_set_title (GTK_WINDOW (widget),title.chars());
      if(border != 0)
        gtk_container_set_border_width( GTK_CONTAINER (widget), border );    
    }
    if(child.IsValid()) {
      if(child.Create()) {
        gtk_container_add (GTK_CONTAINER (widget), child.Widget());
        gtk_widget_show(child.Widget());
      }
    }
    ConnectSignals();
    // Set resizableness
    if (!userresizable) GUIUserResizable(userresizable);
    // Set transience
    if (m_wParent.IsValid()) {
      GUIMakeTransient(m_wParent,m_wPosition);
      m_wParent.Invalidate();
    }
    // Set decorations
    if (!m_bDecorated) GUISetDecorated(m_bDecorated);
    // Done
    return true;
  }
  
  void WindowBodyC::Destroy() {
    if(cursor.IsValid()) 
      cursor.Invalidate();
    
    OneChildBodyC::Destroy();
#if 0
    // Pre-empt destructor...
    // This ensures a expected behaviour even
    // if there are still handles to the window
    // somewhere in the program.
    if(!rootWin) {
      if(widget != 0) {
        gtk_widget_hide (widget);
        gtk_widget_destroy(widget);
        widget = 0;
      }
      if(widgetId != 0)
        Manager.Deregister(*this); 
    }
#endif
  }

  //: Called when the underlying widget it destroyed.
  
  void WindowBodyC::WidgetDestroy() {
    // Just call parent
    WidgetBodyC::WidgetDestroy();
  }
  
  
  //: Close window.
  // Thread safe.
  
  bool WindowBodyC::Close() {
    ONDEBUG(std::cerr << "WindowBodyC::Close() called" << endl);
    Manager.QueueOnGUI(Trigger(WindowC(*this),&WindowC::GUICloseDown));
    ONDEBUG(std::cerr << "WindowBodyC::Close() done" << endl);
    return true;
  }
  
  //: Set cursor.
  
  void WindowBodyC::SetCursor(CursorC &cur) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUIUpdateCursor,cur));
  }
  
  //: Update cursor.

  
  bool WindowBodyC::GUIUpdateCursor(CursorC &newCursor) 
  {
    RavlAssert(Manager.IsGUIThread());
    if(widget != 0 && newCursor.IsValid())
      newCursor.SetWindow(widget->window);
    cursor = newCursor;
    return true;
  }

  //: Set the title of the window.

  bool WindowBodyC::GUISetTitle(StringC &str) {
    RavlAssert(Manager.IsGUIThread());
    title = str.Copy();
    if(widget == 0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_window_set_title (GTK_WINDOW (widget),title.chars());
    return true;
  }

  
  
  bool WindowBodyC::GUISetIcon(const PixbufC &pix)
  {
    RavlAssert(Manager.IsGUIThread());

    if (!pix.IsValid())
      return false;
    
    // Store the icon (in case we're not 'shown' yet)
    icon = PixbufC(pix);

    if (widget == NULL)
      return false;

    gtk_window_set_icon(GTK_WINDOW(widget), icon.Pixbuf());
    
    return true;
  }
  

  //: Show widget to the world.
  // Call only from GUI thread.
  
  bool WindowBodyC::GUIShow() {
    RavlAssert(Manager.IsGUIThread());
    if (!WidgetBodyC::GUIShow())
      return false;
    if (cursor.IsValid() && widget != 0)
      cursor.SetWindow(widget->window);
    if (icon.IsValid())
      GUISetIcon(icon);
    return true;
  }
  
  //: Set the title of the window.
  
  void WindowBodyC::SetTitle(const StringC &str) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUISetTitle,str));
  }
  
  //: Set the title of the window.

  //: Close down window.
  
  bool WindowBodyC::GUICloseDown() {
    RavlAssert(Manager.IsGUIThread());
    ONDEBUG(std::cerr << "WindowBodyC::GuiCloseDown() called" << endl);
    if(closeDown)
      return true;
    closeDown = true;
    if(rootWin) {
      if (--rootWinCount == 0) {
        ONDEBUG(std::cerr << "WindowBodyC::GuiCloseDown() calling Manager.Quit()" << endl);
        Manager.Quit(); 
        return true;
      }
    }
    GUIHide();
    Destroy();
    ONDEBUG(std::cerr << "WindowBodyC::GuiCloseDown() done" << endl);
    return true;
  }

  void WindowBodyC::SetPositioning(GtkWindowPosition& pos) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUISetPositioning,pos));
  }
  
  bool WindowBodyC::GUISetPositioning(GtkWindowPosition& pos) {
    RavlAssert(Manager.IsGUIThread());
    if (widget==0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_window_set_position(GTK_WINDOW(widget),pos);
    return true;
  }
  
  void WindowBodyC::UserResizable(bool& resizable) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUIUserResizable,resizable));
  }

  bool WindowBodyC::GUIUserResizable(bool& resizable) {
    RavlAssert(Manager.IsGUIThread());
    if (widget!=0) {    
      RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
      if (resizable) {
        gtk_window_set_policy(GTK_WINDOW(widget), false, true, false);
      }
      else {
        gtk_window_set_policy(GTK_WINDOW(widget), false, false, true);
      }
    }
    else
      userresizable = resizable;
    return true;
  }

  void WindowBodyC::Raise() {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUIRaise));
  }
  
  bool WindowBodyC::GUIRaise() {
    RavlAssert(Manager.IsGUIThread());
    if (widget==0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_window_raise(widget->window);
    return true;
  }

  void WindowBodyC::Lower() {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUILower));
  }
  
  bool WindowBodyC::GUILower() {
    RavlAssert(Manager.IsGUIThread());
    if (widget==0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_window_lower(widget->window);
    return true;
  }

  void WindowBodyC::Maximise(bool& maximise) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUIMaximise,maximise));
  }

  bool WindowBodyC::GUIMaximise(bool& maximise) {
    RavlAssert(Manager.IsGUIThread());
    if (widget==0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if (maximise) {
      gtk_window_maximize(GTK_WINDOW(widget));
    }
    else {
      gtk_window_unmaximize(GTK_WINDOW(widget));
    }
    return true;
  }

  void WindowBodyC::SetModal(bool& modal) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUISetModal,modal));
  }

  bool WindowBodyC::GUISetModal(bool& modal) {
    RavlAssert(Manager.IsGUIThread());
    if (widget==0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_window_set_modal(GTK_WINDOW(widget),modal);
    return true;
  }

  void WindowBodyC::MakeTransient(WindowC& parent, GtkWindowPosition &position) 
  {
     Manager.QueueOnGUI(Trigger(WindowC(*this),&WindowC::GUIMakeTransient,parent,position));
  }

  void WindowBodyC::MakeTransient(WindowC& parent) 
  {
    Manager.QueueOnGUI(Trigger(WindowC(*this),&WindowC::GUIMakeTransient,parent));
  }

  bool WindowBodyC::GUIMakeTransient(OneChildC& parent) {
    RavlAssert(Manager.IsGUIThread());
    GtkWindowPosition position = GTK_WIN_POS_CENTER_ON_PARENT;
    GUIMakeTransient(parent,position);
    return true;
  }

  bool WindowBodyC::GUIMakeTransient(OneChildC& parent, GtkWindowPosition &position) {
    RavlAssert(Manager.IsGUIThread());
    if (widget!=0 && parent.Widget()!=0) {
      gtk_window_set_transient_for(GTK_WINDOW(widget),GTK_WINDOW(parent.Widget()));
      gtk_window_set_position(GTK_WINDOW(widget), position);
    } else {
      m_wParent = parent;
      m_wPosition = position;
    }
    return true;
  }

  void WindowBodyC::SetDecorated(bool& decorated) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUISetDecorated,decorated));
  }

  bool WindowBodyC::GUISetDecorated(bool& decorated) {
    RavlAssert(Manager.IsGUIThread());
    if (widget!=0 && widget->window!=0) {
      RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
      GdkWMDecoration dec = decorated ? GdkWMDecoration(127) : GdkWMDecoration(0);
      gdk_window_set_decorations(widget->window,dec);
      //gtk_window_set_decorated(widget->window,decorated);
    }
    else 
      m_bDecorated = decorated;
    return true;
  }

  void WindowBodyC::SetFullScreen(bool &fullscreen) {
    Manager.Queue(Trigger(WindowC(*this),&WindowC::GUISetFullScreen,fullscreen));
  }

  bool WindowBodyC::GUISetFullScreen(bool &fullscreen) {
    RavlAssert(Manager.IsGUIThread());
    if (widget != NULL && isFullscreen != fullscreen) {
      RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
      if (fullscreen) {
        gtk_window_fullscreen(GTK_WINDOW (widget));
        isFullscreen = true;
      } else {
        gtk_window_unfullscreen(GTK_WINDOW (widget));
        isFullscreen = false;
      }
    }
    return true;
  }

  bool WindowBodyC::IsFullScreen() {
    return isFullscreen;
  }
   
  bool WindowC::GUISetDefaultIcon(const PixbufC &pix)
  {
    RavlAssert(Manager.IsGUIThread());

    if (!pix.IsValid())
      return false;

    PixbufC tempPix(pix);
    gtk_window_set_default_icon(tempPix.Pixbuf());

    return true;
  }

  //////////////////////////////////////////////
  //: Constructor.
  
  WindowC::WindowC(int sx,int sy,const char *ntitle,GtkWindowType nWinType,int nborder,bool nrootWin,bool nconnectDeleteEvent)
    : OneChildC(*new WindowBodyC(sx,sy,ntitle,nWinType,nborder,nrootWin,nconnectDeleteEvent))
  {}

}
