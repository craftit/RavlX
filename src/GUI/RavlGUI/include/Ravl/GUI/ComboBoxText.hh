// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_COMBOBOXTEXT_HEADER
#define RAVLGUI_COMBOBOXTEXT_HEADER
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! date="21/04/2008"
//! lib=RavlGUI

#include "Ravl/GUI/ComboBox.hh"
#include "Ravl/DList.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Combo box body.
  
  class ComboBoxTextBodyC
    : public ComboBoxBodyC
  {
  public:
    ComboBoxTextBodyC();
    //: Constructor
    
    ComboBoxTextBodyC(const DListC<StringC> &options,bool editable = true);
    //: Constructor
    
    Signal1C<StringC> &SigTextSelected()
    { return m_sigSelected; }
    //: Short cut clicked signal.
    
    StringC GUITextSelected() const;
    //: Access current selected text.
    
    StringC TextSelected() const;
    //: Access current selection.
    
    bool GUISetTextSelected(const StringC &str);
    //: Set selected entry.
    
    IntT GUIFindEntryPosition(const StringC &str);
    //: Find the row number of the str.
    // If -ve value is return the entry hasn't been found.
    
    bool GUIEntryExists(const StringC &str)
    { return GUIFindEntryPosition(str) >= 0; }
    //: Test if we have a particular entry.
    
    bool GUIAppendEntry(const StringC &str);
    //: Append new entry
    
    bool GUIPrependEntry(const StringC &str);
    //: Prepend a new entry
    
    bool GUIInsertEntry(IntT pos,const StringC &str);
    //: Insert an entry.

    bool GUIRemoveEntry(IntT pos);
    //: Remove an entry by index
    
    bool GUIRemoveEntry(const StringC &str);
    //: Remove an entry by name
    
    bool GUIClear();
    //: Clear all entries.
    
  protected:
    
    bool CommonCreate(GtkWidget *widget);
    //: Create with a widget possibly supplied from elsewhere.
    
    bool CBChanged();
    //: Process changed call back signals.
    
    DListC<StringC> m_options; //: List of preset options.
    bool m_editable;
    bool m_useGladeOptions;
    Signal1C<StringC> m_sigSelected; //: Sig selected for compatibility with old ComboC code.
    
    StringC m_selected;    
  };
  

  //! userlevel=Normal
  //: Combo box.
  // <img src="../../../html/combo-box.png"><br>
  // Like <a href="RavlGUIN.ComboBoxC.html">ComboBoxC</a> but uses simple text list for entries
  
  class ComboBoxTextC
    : public ComboBoxC
  {
  public:
    ComboBoxTextC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    ComboBoxTextC(const DListC<StringC> &options,bool editable = true)
      : ComboBoxC(new ComboBoxTextBodyC(options,editable))
    {}
    //: Constructor
    
  protected:
    ComboBoxTextC(const ComboBoxTextBodyC *body)
      : ComboBoxC(body)
    {}
    //: Body constructor
    
    ComboBoxTextBodyC &Body()
    { return dynamic_cast<ComboBoxTextBodyC &>(WidgetC::Body()); }
    //: Body access.
    
    const ComboBoxTextBodyC &Body() const
    { return dynamic_cast<const ComboBoxTextBodyC &>(WidgetC::Body()); }
    //: Body access.
    
  public:
    StringC GUITextSelected() const
    { return Body().GUITextSelected(); }
    //: Access current selected text.
    
    StringC TextSelected() const
    { return Body().TextSelected(); }
    //: Access current selection.
    
    Signal1C<StringC> &SigTextSelected()
    { return Body().SigTextSelected(); }
    //: Short cut clicked signal.
    
    bool GUISetTextSelected(const StringC &str)
    { return Body().GUISetTextSelected(str); }
    //: Set selected entry.
    
    IntT GUIFindEntryPosition(const StringC &str)
    { return Body().GUIFindEntryPosition(str); }
    //: Find the row number of the str.
    // If -ve value is return the entry hasn't been found.
    
    bool GUIEntryExists(const StringC &str)
    { return Body().GUIEntryExists(str); }
    //: Test if we have a particular entry.
    
    bool GUIAppendEntry(const StringC &str)
    { return Body().GUIAppendEntry(str); }
    //: Append new entry
    
    bool GUIPrependEntry(const StringC &str)
    { return Body().GUIPrependEntry(str); }
    //: Prepend a new entry
    
    bool GUIInsertEntry(IntT pos,const StringC &str)
    { return Body().GUIInsertEntry(pos,str); }
    //: Insert an entry.
    
    bool GUIRemoveEntry(IntT pos)
    { return Body().GUIRemoveEntry(pos); }
    //: Remove an entry by id
    
    bool GUIRemoveEntry(const StringC &text)
    { return Body().GUIRemoveEntry(text); }
    //: Remove an entry by name
    
    bool GUIClear()
    { return Body().GUIClear(); }
    //: Clear all entries.

  };

  
  template<class DataT>
  ComboBoxTextC ComboBoxText(const DListC<StringC> &lst,const char *tooltip,const DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  {
    ComboBoxTextC ret(lst,editable);
    Connect(ret.SigTextSelected(),dat,func);
    ret.SetToolTip(tooltip);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in class.
  // With tooltip.

  template<class DataT>
  ComboBoxTextC ComboBoxTextR(const DListC<StringC> &lst,DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  { 
    ComboBoxTextC ret(lst,editable);
    ConnectRef(ret.SigTextSelected(),dat,func);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in referenced class.

  template<class DataT>
  ComboBoxTextC ComboBoxTextR(const DListC<StringC> &lst,char *tooltip,const DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  {
    ComboBoxTextC ret(lst,editable);
    ConnectRef(ret.SigTextSelected(),dat,func);
    ret.SetToolTip(tooltip);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in referenced class.
  // With tooltip.

}

#endif
