// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_COMBO_HEADER
#define RAVLGUI_COMBO_HEADER 1
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Combo.hh"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! date="25/06/1999"

#include "Ravl/GUI/GTKTypes.hh"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/String.hh"
#include "Ravl/Hash.hh"

namespace RavlGUIN {
  using namespace RavlN;
  class ComboC;
  
  //! userlevel=Obsolete
  //: Combo body.
  
  class ComboBodyC 
    : public WidgetBodyC
  {
  public:
    ComboBodyC(const DListC<StringC> &choices,bool editable = true);
    //: Constructor.
    
    ComboBodyC(bool editable);
    //: Constructor.
    
    virtual ~ComboBodyC() { }
    //: Have virtual methods - need virtual destructor

    StringC Selected() const;
    //: Get currently selected string.
    
    bool Clear();
    //: Clear all entries from combo list.

    bool ClearSelection();
    //: Clear the current selection.

    bool AddEntry(const StringC &opt);
    //: Add new entry to combo list.

    bool DelEntry(const StringC &opt);
    //: Delete entry from combo list.

    bool GUIClear();
    //: Clear all entries from combo list.
    // Call on the GUI thread only.

    bool GUIClearSelection();
    //: Clear selection.
    // Call on the GUI thread only.

    bool GUIAddEntry(const StringC &opt);
    //: Add new entry to combo list.
    // Call on the GUI thread only.
    
    bool GUIDelEntry(const StringC &opt);
    //: Add new entry to combo list.
    // Call on the GUI thread only.
    
    bool SetSelection(const StringC &opt);
    //: Set selection string.
    
    bool GUISetSelection(const StringC &opt);
    //: Set selection string.
    
    bool GUIEntryExists(const StringC &entry);
    //: Test if an entry exists.
    // Call from the GUI thread only.
    
    bool GUISetMaxLength(IntT charactors);
    //: Set maximum length of text entry.
    //!param: chars - Maximum number of charactors in widget, set to -1 for no limits.
    
    IntT GetCursorPosition();
    //: Get the cursor position
    // 0 = left of first character
    
    bool GUISetCursorPosition(const IntT pos);
    //: Set the cursor position
    //  0 = left of first character
    // -1 = after the last character
    
    bool SetCursorPosition(const IntT pos);
    //: Set the cursor position
    //  0 = left of first character
    // -1 = after the last character
    
    bool GetCursorSelection(IntT &start, IntT &end);
    //: Get the text selection
    // Returns false if nothing selected
    
    bool GUISetCursorSelection(const IntT start, const IntT end);
    //: Set the text selection
    //  0 = left of first character
    // -1 = after the last character
    
    bool SetCursorSelection(const IntT start, const IntT end);
    //: Set the text selection
    //  0 = left of first character
    // -1 = after the last character
    
    Signal0C &SigSelected()
    { return sigSelected; }
    //: Short cut clicked signal.
    // This should be used instead of signals["combo_activate"],
    // as it filters unwanted GTK signals

    virtual bool GUISetTextColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set text colour.
    
    virtual bool GUISetBaseColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set base colour.
    
    virtual bool GUISetBackgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set background colour.
    
    virtual bool GUISetForgroundColour(const ByteRGBValueC &colour,GtkStateType &state);
    //: Set forground colour.
    
  protected:
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    bool FilterSignal(StringC& sel);    
    //: Blocks unwanted GTK "selected" signals 

    bool GUISetStyle(WidgetStyleC& style);
    //: Set style of button and child widgets

    DListC<StringC> choices;
    HashC<StringC,DListC<GtkWidget *> > cmap; // Widget map.
    bool editable;
    bool allowsignals;
    StringC selection;
    Signal1C<StringC> sigSelected;
    IntT maxEntryLength;
    
    //: Selection changed signal
    // This should be used instead of signals["combo_activate"],
    // as it filters unwanted GTK signals

    friend class ComboC;
  };
  
  //! userlevel=Obsolete
  //: Combo box for GTK 1.
  
  class ComboC
    : public WidgetC
  {
  public:
    ComboC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ComboC(const DListC<StringC> &lst,bool editable = true)
      : WidgetC(*new ComboBodyC(lst,editable))
    {}
    //: Create a Combo box.
    
    ComboC(const WidgetC &base)
      : WidgetC(dynamic_cast<const ComboBodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // If types don't match an invalid handle will be created.
    
  protected:
    ComboC(ComboBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    ComboBodyC &Body()
    { return static_cast<ComboBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ComboBodyC &Body() const
    { return static_cast<const ComboBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    Signal0C &SigSelected()
    { return Body().SigSelected(); }
    //: Short cut clicked signal.
    // This should be used instead of signals["combo_activate"],
    // as it filters unwanted GTK signals
    
    StringC Selected() const 
    { return Body().Selected(); }
    //: Get currently selected string.
    
    bool Clear()
    { return Body().Clear(); }
    //: Clear all entries from combo list.

    bool ClearSelection()
    { return Body().ClearSelection(); }
    //: Clear the current selection

    bool AddEntry(const StringC &opt)
    { return Body().AddEntry(opt); }
    //: Add new entry to combo list.
    
    bool DelEntry(const StringC &opt)
    { return Body().DelEntry(opt); }
    //: Add new entry to combo list.

    bool GUIClear()
    { return Body().GUIClear(); }
    //: Clear all entries from combo list.
    // Call on the GUI thread only.

    bool GUIClearSelection()
    { return Body().GUIClearSelection(); }
    //: Clear selection.
    // Call on the GUI thread only.

    bool GUIAddEntry(const StringC &opt)
    { return Body().GUIAddEntry(opt); }
    //: Add new entry to combo list.
    // Call on the GUI thread only.
    
    bool GUIDelEntry(const StringC &opt)
    { return Body().GUIDelEntry(opt); }
    //: Add new entry to combo list.
    // Call on the GUI thread only.

    bool SetSelection(const StringC &opt)
    { return Body().SetSelection(opt); }
    //: Set selection string.
    
    bool GUISetSelection(const StringC &opt)
    { return Body().GUISetSelection(opt); }
    //: Set selection string.
    
    bool GUIEntryExists(const StringC &entry)
    { return Body().GUIEntryExists(entry); }
    //: Test if an entry exists in the combo box.
    // Call from the GUI thread only.
    
    bool GUISetMaxLength(IntT chars)
    { return Body().GUISetMaxLength(chars); }
    //: Set maximum length of text entry.
    //!param: chars - Maximum number of charactors in widget.
    
    IntT GetCursorPosition()
    { return Body().GetCursorPosition(); }
    //: Get the cursor position
    // 0 = left of first character
    
    bool GUISetCursorPosition(const IntT pos)
    { return Body().GUISetCursorPosition(pos); }
    //: Set the cursor position
    //  0 = left of first character
    // -1 = after the last character
    
    bool SetCursorPosition(const IntT pos)
    { return Body().SetCursorPosition(pos); }
    //: Set the cursor position
    //  0 = left of first character
    // -1 = after the last character
    
    bool GetCursorSelection(IntT &start, IntT &end)
    { return Body().GetCursorSelection(start, end); }
    //: Get the text selection
    // Returns false if nothing selected
    
    bool GUISetCursorSelection(const IntT start, const IntT end)
    { return Body().GUISetCursorSelection(start, end); }
    //: Set the text selection
    //  0 = left of first character
    // -1 = after the last character
    
    bool SetCursorSelection(const IntT start, const IntT end)
    { return Body().SetCursorSelection(start, end); }
    //: Set the text selection
    //  0 = left of first character
    // -1 = after the last character
    
    friend class ComboBodyC;
  };
  
  template<class DataT>
  ComboC Combo(const DListC<StringC> &lst,bool (*func)(ComboC &, DataT &ref),const DataT &dat = DataT(),bool editable = true)
  { 
    ComboC ret(lst,editable);
    Connect(ret.SigSelected(),func,ret,dat);
    return ret;    
  }  
  //: Contruct a combo box, call back to function.
  
  template<class DataT>
  ComboC Combo(const DListC<StringC> &lst,const DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  { 
    ComboC ret(lst,editable);
    Connect(ret.SigSelected(),dat,func);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in class.

  template<class DataT>
  ComboC Combo(const DListC<StringC> &lst,const char *tooltip,const DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  {
    ComboC ret(lst,editable);
    Connect(ret.SigSelected(),dat,func);
    ret.SetToolTip(tooltip);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in class.
  // With tooltip.

  template<class DataT>
  ComboC ComboR(const DListC<StringC> &lst,DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  { 
    ComboC ret(lst,editable);
    ConnectRef(ret.SigSelected(),dat,func);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in referenced class.

  template<class DataT>
  ComboC ComboR(const DListC<StringC> &lst,char *tooltip,const DataT &dat,bool (DataT::*func)(StringC &ref),bool editable = true)
  {
    ComboC ret(lst,editable);
    ConnectRef(ret.SigSelected(),dat,func);
    ret.SetToolTip(tooltip);
    return ret;    
  }  
  //: Contruct a combo box, call back to method in referenced class.
  // With tooltip.
  
}



#endif
