// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_TEXTBOX_HEADER
#define RAVLGUI_TEXTBOX_HEADER 1
//////////////////////////////////////////////////
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/TextBox.hh"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"

namespace RavlGUIN {

  class TextBoxC;
  
  //! userlevel=Develop
  //: Text box body
  // See handle documentation for more information.
  
  class TextBoxBodyC 
    : public WidgetBodyC
  {
  public:
    TextBoxBodyC(const StringC &ntext,bool editable = true);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    StringC Text() const;
    //: Access text
    
    bool Insert(const StringC &txt);
    //: Update text.
    // This is thread safe.
    
  protected:
    bool GUIInsert(StringC &txt);
    //: Insert text in window.
    
    StringC text; // text.
    bool editable; // Is text editable ?
    
    friend class TextBoxC;
  };


  //! userlevel=Normal
  //: Text box 
  // This is intended to edit multi-line text. Due to some problems with the
  // underlying gtk widget it is unreliable at the moment and it is recommended 
  // that it not be used.  When the move to gtk2 is complete it will be changed 
  // to something more usable.
  
  class TextBoxC 
    : public WidgetC
  {
  public:
    TextBoxC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TextBoxC(const StringC &text,bool editable = true)
      : WidgetC(*new TextBoxBodyC(text,editable))
    {}
    //: Constructor
    
  protected:
    TextBoxC(TextBoxBodyC &body)
      : WidgetC(body)
    {}
    //: Body constructor.
    
    TextBoxBodyC &Body() 
    { return dynamic_cast<TextBoxBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const  TextBoxBodyC &Body() const
    { return dynamic_cast<const TextBoxBodyC &>(WidgetC::Body()); }
    //: Access body.  

    bool GUIInsert(StringC &txt)
    { return Body().GUIInsert(txt); }
    //: Insert text in window.
    
  public:
    StringC Text() const 
    { return Body().Text(); }
    //: Access text
    
    bool Insert(const StringC &txt)
    { return Body().Insert(txt); }
    //: Insert text at cursor
    // This is thread safe.

    friend class TextBoxBodyC;
  };

}

#endif
