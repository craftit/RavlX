// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/ComboBoxEntryText.hh"
//! lib=RavlGUI
//! author = "Charles Galambos"
//! docentry = "Ravl.API.Graphics.GTK.Control"
//! example = "exComboBoxEntryText.cc"

#ifndef RAVLGUI_COMBOBOXENTRYTEXT_HEADER
#define RAVLGUI_COMBOBOXENTRYTEXT_HEADER

#include "Ravl/GUI/ComboBoxText.hh"
#include "Ravl/GUI/TextEntry.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Combo box with a text entry body.
  
  class ComboBoxEntryTextBodyC
    : public ComboBoxTextBodyC
  {
  public:
    ComboBoxEntryTextBodyC();
    //: Constructor
    
    ComboBoxEntryTextBodyC(const DListC<StringC> &options);
    //: Constructor
    
    TextEntryC &TextEntry()
    { return m_textEntry; }
    //: Access text entry.

  protected:
    bool CommonCreate(GtkWidget *widget);
    //: Create with a widget possibly supplied from elsewhere.
    
    TextEntryC m_textEntry; // Text entry.

  };

  //! userlevel=Normal
  //: Combo box with a text entry.
  // <img src="../../../html/combo-box.png"><br>
  // Like <a href="RavlGUIN.ComboBoxTextC.html">ComboBoxTextC</a> but with editable entries
  
  class ComboBoxEntryTextC
    : public ComboBoxTextC
  {
  public:
    ComboBoxEntryTextC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    ComboBoxEntryTextC(const DListC<StringC> &options)
      : ComboBoxTextC(new ComboBoxEntryTextBodyC(options))
    {}
    //: Constructor
    
  protected:
    ComboBoxEntryTextC(const ComboBoxEntryTextBodyC *body)
      : ComboBoxTextC(body)
    {}
    //: Body constructor
    
    ComboBoxEntryTextBodyC &Body()
    { return dynamic_cast<ComboBoxEntryTextBodyC &>(WidgetC::Body()); }
    //: Body access.
    
    const ComboBoxEntryTextBodyC &Body() const
    { return dynamic_cast<const ComboBoxEntryTextBodyC &>(WidgetC::Body()); }
    //: Body access.
    
  public:
    TextEntryC &TextEntry()
    { return Body().TextEntry(); }
    //: Access text entry.
    
    
  };
  

}

#endif
