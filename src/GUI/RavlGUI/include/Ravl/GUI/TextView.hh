// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/TextView.hh"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Control"
//! example = "exTextView.cc"

#ifndef RAVLGUI_TEXTVIEW_HEADER
#define RAVLGUI_TEXTVIEW_HEADER 1

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"

namespace RavlGUIN
{

  //! userlevel=Develop
  //: Text View
  
  class TextViewBodyC :
    public WidgetBodyC
  {
  public:
    TextViewBodyC(const StringC &text);
    //: Constructor.
    
    virtual ~TextViewBodyC()
    {}
    //: Destructor

    virtual bool Create()
    { return CommonCreate(); }
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget)
    { return CommonCreate(_widget); } 
    //: Create the widget.
    
  private:
    bool CommonCreate(GtkWidget *_widget = NULL);
    //: Common widget creation function.
    
  private:
    StringC m_text;
  };

  //! userlevel=Normal
  //: Text View
  // BIG OBJECT
  // This is the GTK 2.x mutli-line text edit widget. Currently it
  // only supports setting read-only text on initialisation.
  
  class TextViewC :
    public WidgetC
  {
  public:
    TextViewC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TextViewC(const StringC &text) :
      WidgetC(*new TextViewBodyC(text))
    {}
    //: Constructor
    
  private:
    TextViewC(TextViewBodyC &body) :
      WidgetC(body)
    {}
    //: Body constructor.
    
    TextViewBodyC &Body() 
    { return dynamic_cast<TextViewBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const TextViewBodyC &Body() const
    { return dynamic_cast<const TextViewBodyC &>(WidgetC::Body()); }
    //: Access body.  
  };

}

#endif
