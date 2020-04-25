// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_FRAME_HEADER
#define RAVLGUI_FRAME_HEADER 1
//////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Frame.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="17/06/1999"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! example=exFrame.cc

#include "Ravl/GUI/OneChild.hh"
#include "Ravl/String.hh"

namespace RavlGUIN {
  
  class FrameC;
  
  //! userlevel=Develop
  //: Put a frame around some other widgets.
  
  class FrameBodyC
    : public OneChildBodyC
  {
  public:
    FrameBodyC();
    //: Default constructor.
    
    FrameBodyC(const WidgetC &widge,int nborder,const StringC &ntitle = "");
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    void SetShadowType(GtkShadowType shadow);
    //: Set frame type.
    // Values for shadow are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSHADOWTYPE">here</A>.
    
    void SetLabel(const StringC &name);
    //: Set label

    bool GUISetLabel(const StringC &name);
    //: Set label
    // Call with GUI thread only!
    
  protected:
    bool GUISetShadow(GtkShadowType &nshadow);
    //: Setup shadow.
    // Values for shadow are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSHADOWTYPE">here</A>.
    
    StringC title;
    int border;
    GtkShadowType shadow;
    GtkResizeMode resizeMode;
    
    friend class FrameC;
  };
  
  //! userlevel=Normal
  //: Put a frame around some other widgets.
  
  class FrameC
    : public OneChildC
  {
  public:
    FrameC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FrameC(const WidgetC &widge,const StringC &title,int border = 2)
      : OneChildC(*new FrameBodyC(widge,border,title))
    {}
    //: Constructor.
    
    FrameC(const WidgetC &widge,int border)
      : OneChildC(*new FrameBodyC(widge,border))
    {}
    //: Constructor.

    FrameC(bool)
      : OneChildC(*new FrameBodyC())
    {}
    //: Constructor.
    
  protected:
    FrameC(FrameBodyC &body)
      : OneChildC(body)
    {}
    //: Body constructor.
    
    explicit FrameC(const FrameBodyC *body)
      : OneChildC(body)
    {}
    //: Body constructor.
    
    FrameBodyC &Body() 
    { return static_cast<FrameBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const FrameBodyC &Body() const
    { return static_cast<const FrameBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool GUISetShadow(GtkShadowType &shadow)
    { return Body().GUISetShadow(shadow); }
    //: Setup shadow.
    // Values for shadow are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSHADOWTYPE">here</A>.
    
    
  public:
    explicit FrameC(const WidgetC &widge)
      : OneChildC(dynamic_cast<const FrameBodyC *>(BodyPtr(widge)))
    {}
    //: Base constructor.
    // If 'widge' is a FrameC this creates a handle to it, if it is not
    // an invalid handle is created.
    
    bool GUISetLabel(const StringC &name)
    { return Body().GUISetLabel(name); }
    //: Set label
    // Call with GUI thread only!
    
    void SetShadowType(GtkShadowType shadow)
    { Body().SetShadowType(shadow); }
    //: Set frame type.
    // Values for shadow are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSHADOWTYPE">here</A>.
    
    void SetLabel(const StringC &name)
    { Body().SetLabel(name); }
    //: Set label
    
    friend class FrameBodyC;
  };

}
#endif
