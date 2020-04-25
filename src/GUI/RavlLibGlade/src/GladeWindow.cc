// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/GladeWindow.cc"

#include "Ravl/GUI/GladeWindow.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/DP/TypeConverter.hh"

#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor
  
  GladeWindowBodyC::GladeWindowBodyC(const GladeXMLC &gladeXml,
                                     const StringC &widgetName,
                                     bool aCustomWidget,
                                     const StringC prefix) 
    : GladeWidgetBodyC(gladeXml,widgetName,aCustomWidget,prefix),
      m_interceptDeleteEvent(false)
  {

  }
  
  //: Constructor
  
  GladeWindowBodyC::GladeWindowBodyC(const StringC &widgetName,bool aCustomWidget) 
    : GladeWidgetBodyC(widgetName,aCustomWidget),
      m_interceptDeleteEvent(false)
  {

  }
  
  //: Factory constructor

  GladeWindowBodyC::GladeWindowBodyC(const XMLFactoryContextC &factory)
   : GladeWidgetBodyC(factory),
     m_interceptDeleteEvent(factory.AttributeBool("interceptDeleteEvent",false))
  {

  }

  //: Raise window to top
  
  bool GladeWindowBodyC::Raise() {
    Manager.Queue(Trigger(GladeWindowC(*this),&GladeWindowC::GUIRaise));
    return true;
  }
  
  //: Raise window to top
  
  bool GladeWindowBodyC::GUIRaise() {
    if (widget!=0)
      gdk_window_raise(widget->window);
    return true;
  }

  
  //: Show window and raise it to top.
  bool GladeWindowBodyC::ShowAndRaise() {
    Manager.Queue(Trigger(GladeWindowC(*this), &GladeWindowC::GUIShowAndRaise));
    return true;
  }


  bool GladeWindowBodyC::GUIShowAndRaise()
  {
    if (widget != 0)
    {
      GUIShow();
      GUIRaise();
    }
    return true;
  }
    //: Show window and raise it to top -GUI thread.

  bool GladeWindowBodyC::SetTitle(const StringC &title)
  {
    Manager.Queue(Trigger(GladeWindowC(*this), &GladeWindowC::GUISetTitle, title));
    return true;
  }
  
  bool GladeWindowBodyC::GUISetTitle(const StringC &title)
  {
    if (widget != NULL)
      gtk_window_set_title(GTK_WINDOW(widget), title.chars());
    m_theTitle = title;
    return true;
  }
  
  bool GladeWindowBodyC::GUISetIcon(const PixbufC &pix)
  {
    RavlAssert(Manager.IsGUIThread());
    
    if (!pix.IsValid())
      return false;
    
    // Store the icon (in case we're not 'shown' yet)
    m_icon = PixbufC(pix);
    
    if (widget == NULL)
      return false;

    gtk_window_set_icon(GTK_WINDOW(widget), m_icon.Pixbuf());
    
    return true;
  }

  //: Common object creation
  bool GladeWindowBodyC::CommonCreate(GtkWidget *newWidget) {
    if(!GladeWidgetBodyC::CommonCreate(newWidget))
      return false;
    if(m_icon.IsValid())
      gtk_window_set_icon(GTK_WINDOW(widget), m_icon.Pixbuf());
    if(!m_theTitle.IsEmpty())
      gtk_window_set_title(GTK_WINDOW(widget), m_theTitle.chars());
    if(m_interceptDeleteEvent)
      InterceptDeleteEvent();
    return true;
  }
  
  //: Called when gtk widget is destroyed.
  
  void GladeWindowBodyC::WidgetDestroy() {
    GladeWidgetBodyC::WidgetDestroy();
  }
  
  // --------------------------------------
  
  class GladeWindowWrapperBodyC
    : public WindowBodyC
  {
  public:
    // Constructor.
    GladeWindowWrapperBodyC(GladeWindowBodyC *gladeWin)
      : m_gladeWin(*gladeWin)
    {
      widget = m_gladeWin.Widget();    
    }
    
    //: Create window
    bool Create() {
      widget = m_gladeWin.Widget();
      return true;
    }
    
  protected:
    GladeWindowC m_gladeWin;
  };

  class GladeWindowWrapperC
    : public WindowC
  {
  public:
    //: Constructor
    GladeWindowWrapperC()
    {}
    
    //: Constructor
    GladeWindowWrapperC(GladeWindowBodyC *gladeWin)
      : WindowC(*new GladeWindowWrapperBodyC(gladeWin))
    {}
    
  };
  
  //: Get a handle to a basic window.
  
  WindowC GladeWindowBodyC::Window() {
    return GladeWindowWrapperC(this);
  }

  static GladeWindowC ConvertGladeWindowBodyPtrToHandle(const GladeWindowBodyC::RefT &ref)
  { return GladeWindowC(ref.BodyPtr()); }

  DP_REGISTER_CONVERSION(ConvertGladeWindowBodyPtrToHandle,1.0);


  void LinkGladeWindow()
  {}

  static XMLFactoryRegisterHandleConvertC<GladeWindowC,GladeWidgetC> g_registerXMLFactoryGladeWindow("RavlGUIN::GladeWindowC");

}
