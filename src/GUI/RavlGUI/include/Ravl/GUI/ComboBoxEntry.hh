// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_COMBOBOXENTRY_HEADER
#define RAVLGUI_COMBOBOXENTRY_HEADER 1
//! lib=RavlGUI

//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/ComboBox.hh"
#include "Ravl/GUI/TextEntry.hh"

namespace RavlGUIN {
  
  //! userlevel=Normal
  //: Combo box with text entry.
  
  class ComboBoxEntryBodyC
    : public ComboBoxBodyC
  {
  public:
    ComboBoxEntryBodyC();
    //: Constructor
    
    ComboBoxEntryBodyC(TreeModelC &treeModel,UIntT textColumn);
    //: Constructor
    
    TextEntryC &TextEntry()
    { return m_textEntry; }
    //: Access text entry.
    
  protected:
    virtual bool CommonCreate(GtkWidget *widget);
    //: Create with a widget possibly supplied from elsewhere.
    
    TextEntryC m_textEntry; // Text entry.
    UIntT m_textColumn; // Text column in the tree store.
  };
  
  //! userlevel=Normal
  //: Combo box
  // <img src="../../../html/combo-box.png"><br>
  // Like <a href="RavlGUIN.ComboBoxC.html">ComboBoxC</a> but with editable entries
  
  class ComboBoxEntryC
    : public ComboBoxC
  {
  public:
    ComboBoxEntryC()
    {}
    //: Default constructor
    
    ComboBoxEntryC(TreeModelC &treeModel,UIntT textColumn)
      : ComboBoxC(new ComboBoxEntryBodyC(treeModel,textColumn))
    {}
    //: Constructor
    
  protected:
    ComboBoxEntryC(const ComboBoxEntryBodyC *body)
      : ComboBoxC(body)
    {}
    //: Body Constructor
    
    ComboBoxEntryBodyC &Body()
    { return dynamic_cast<ComboBoxEntryBodyC &>(WidgetC::Body()); }
    //: Body access.
    
    const ComboBoxEntryBodyC &Body() const
    { return dynamic_cast<const ComboBoxEntryBodyC &>(WidgetC::Body()); }
    //: Body access.
 
  public:
    TextEntryC &TextEntry()
    { return Body().TextEntry(); }
    //: Access text entry.
    
  };

  
}

#endif
