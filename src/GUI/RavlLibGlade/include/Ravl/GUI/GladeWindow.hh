// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_GLADEWINDOW_HEADER
#define RAVLGUI_GLADEWINDOW_HEADER 1
//! docentry="Ravl.API.Graphics.GTK.LibGlade"
//! rcsid="$Id$"
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/GladeWindow.hh"

#include "Ravl/GUI/GladeWidget.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/GUI/Window.hh"

namespace RavlGUIN {
  class GladeWindowWrapperBodyC;
  
  //! userlevel=Develop
  //: Glade widget
  
  class GladeWindowBodyC
    : public GladeWidgetBodyC
  {
  public:
    GladeWindowBodyC(const GladeXMLC &gladeXml,
                     const StringC &widgetName,
                     bool customWidget = true,
                     const StringC prefix = "");
    //: Constructor
    
    GladeWindowBodyC(const StringC &widgetName,bool customWidget = true);
    //: Constructor
    // NOTE: If using this constructor a Glade XML object must be set with SetXML()
    
    GladeWindowBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor.

    bool Raise();
    //: Raise window to top
    
    bool GUIRaise();
    //: Raise window to top

    bool ShowAndRaise();
    //: Show window and raise it to top.

    bool GUIShowAndRaise();
    //: Show window and raise it to top -GUI thread.

    bool SetTitle(const StringC &title);
    //: Set the window title
    
    bool GUISetTitle(const StringC &title);
    //: Set the window title
    
    bool GUISetIcon(const PixbufC &pix);
    //: Set an icon for the window
    
    WindowC Window();
    //: Get a handle to a basic window.

    typedef RavlN::SmartPtrC<GladeWindowBodyC> RefT;
    //: Handle to glade widget

  protected:
    virtual bool CommonCreate(GtkWidget *newWidget = NULL);
    //: Common object creation
    
    virtual void WidgetDestroy();
    //: Called when gtk widget is destroyed.
    
    PixbufC m_icon;
    bool m_interceptDeleteEvent;
    StringC m_theTitle;
  };
  
  //! userlevel=Normal
  //: Glade widget
  
  class GladeWindowC
    : public GladeWidgetC
  {
  public:
    GladeWindowC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    GladeWindowC(const GladeXMLC &gladeXml,
                 const StringC &widgetName,
                 bool customWidget = true,
                 const StringC prefix = "")
      : GladeWidgetC(*new GladeWindowBodyC(gladeXml,widgetName,customWidget,prefix))
    {}
    //: Constructor
    
    GladeWindowC(const StringC &widgetName,bool customWidget = true)
      : GladeWidgetC(*new GladeWindowBodyC(widgetName,customWidget))
    {}
    //: Constructor
    // NOTE: If using this constructor a Glade XML object must be set with SetXML()

    GladeWindowC(const XMLFactoryContextC &factory)
     : GladeWidgetC(*new GladeWindowBodyC(factory))
    {}
    //: Factory constructor.

    GladeWindowC(const GladeWindowBodyC *body)
      : GladeWidgetC(body)
    {}
    //: Body ptr constructor

  protected:
    GladeWindowC(GladeWindowBodyC &body)
      : GladeWidgetC(body)
    {}
    //: Body constructor
    
    GladeWindowBodyC &Body()
    { return static_cast<GladeWindowBodyC &>(WidgetC::Body()); }
    //: Access widget body.
    
    const GladeWindowBodyC &Body() const
    { return static_cast<const GladeWindowBodyC &>(WidgetC::Body()); }
    //: Access widget body.
    
  public:
    bool Raise()
    { return Body().Raise(); }
    //: Raise window to top
    
    bool GUIRaise()
    { return Body().GUIRaise(); }
    //: Raise window to top
    
    bool ShowAndRaise()
    { return Body().ShowAndRaise(); } 
    //: Show window and raise it to top. 
    
    bool GUIShowAndRaise()
    { return Body().GUIShowAndRaise(); }
    //: Show window and raise it to top -GUI thread.


    bool SetTitle(const StringC &title)
    { return Body().SetTitle(title); }
    //: Set the window title
    
    bool GUISetTitle(const StringC &title)
    { return Body().GUISetTitle(title); }
    //: Set the window title
    
    bool GUISetIcon(const PixbufC &pix)
    { return Body().GUISetIcon(pix); }
    //: Set an icon for the window
    
    WindowC Window()
    { return Body().Window(); }
    //: Get a handle to a basic window.
    
    friend class GladeWindowBodyC;
    friend class GladeWindowWrapperBodyC;
  };

}


#endif
