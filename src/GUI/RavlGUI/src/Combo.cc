// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Combo.cc"
//! example=exCombo.cc

#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/GUI/WidgetStyle.hh"
#include "Ravl/GUI/Combo.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"

#include <gtk/gtk.h>

// TODO :- Make new wrapper

namespace RavlGUIN {

  //: Constructor.
  
  ComboBodyC::ComboBodyC(const DListC<StringC> &nChoices,bool neditable)
    : choices(nChoices),
      editable(neditable),
      allowsignals(true),
      sigSelected(StringC()),
      maxEntryLength(-1)
  {
    signals["combo_activate"] = Tuple2C<Signal0C,IntT>(Signal1C<StringC>(StringC("-none-")),-1);
    ConnectRef(signals["combo_activate"].Data1(),*this,&ComboBodyC::FilterSignal);
  }
  
  //: Constructor.
  
  ComboBodyC::ComboBodyC(bool neditable)
    : editable(neditable),
      allowsignals(true),
      sigSelected(StringC()),
      maxEntryLength(-1)
  {
    signals["combo_activate"] = Tuple2C<Signal0C,IntT>(Signal1C<StringC>(StringC("-none-")),-1);
    ConnectRef(signals["combo_activate"].Data1(),*this,&ComboBodyC::FilterSignal);
  }
  
  //: Get currently selected string.
  
  StringC ComboBodyC::Selected() const { 
    ReadBackLockC lock(true);
    return StringC(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(widget)->entry)));
  }
  
  //: Clear all entries from combo list.

  bool ComboBodyC::Clear() {
    Manager.Queue(Trigger(ComboC(*this),&ComboC::GUIClear));
    return true;
  }

  //: Clear selection
  bool ComboBodyC::ClearSelection() {
    Manager.Queue(Trigger(ComboC(*this),&ComboC::GUIClearSelection));
    return true;    
  }

  //: Add new entry to combo list.
  
  bool ComboBodyC::AddEntry(const StringC &opt) {
    Manager.Queue(Trigger(ComboC(*this),&ComboC::GUIAddEntry,opt));
    return true;
  }
  
  //: Add new entry to combo list.
  
  bool ComboBodyC::DelEntry(const StringC &opt) {
    Manager.Queue(Trigger(ComboC(*this),&ComboC::GUIDelEntry,opt));
    return true;
  }
  
  //: Clear all entries from combo list
  // Call on the GUI thread only.
  
  bool ComboBodyC::GUIClear() {
    choices.Empty();
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(GTK_COMBO(widget)->list == 0)
      return true;
    // Disable signals
    allowsignals = false;
    cmap.Empty();
    // Clear list
    gtk_list_clear_items(GTK_LIST (GTK_COMBO(widget)->list),0,-1);
    // Re-enable signals
    allowsignals = true;
    // Done
    return true;
  }

  //: Clear selection.
  // Call on the GUI thread only.
  
  bool ComboBodyC::GUIClearSelection() {
    if(widget == 0) {
      selection = StringC("");
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Clear selection
    StringC str("");
    gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(widget)->entry), str.chars());
    // Done
    return true;
  }

  //: Add new entry to combo list.
  // Call on the GUI thread only.
  
  bool ComboBodyC::GUIAddEntry(const StringC &opt) {
    choices.InsLast(opt);
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GtkWidget *li = gtk_list_item_new_with_label ((gchar *) opt.chars());
    cmap[opt].InsLast(li);
    // Disable signals
    allowsignals = false;
    // Add widget
    gtk_widget_show (li);
    gtk_container_add (GTK_CONTAINER (GTK_COMBO(widget)->list), li);
    // Re-enable signals
    allowsignals = true;
    // Done
    return true;
  }
  
  //: Add new entry to combo list.
  // Call on the GUI thread only.
  
  bool ComboBodyC::GUIDelEntry(const StringC &opt) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    choices.Del(opt);
    //    RavlAssertMsg(0,"ComboBodyC::GUIDelEntry(), Not implemented. ");
    DListC<GtkWidget *> *widgetEntries = cmap.Lookup(opt);
    if(widgetEntries == 0 || widgetEntries->IsEmpty()) 
      return true; // item not in list.
    GtkWidget *li = (*widgetEntries).PopFirst();
    if(widgetEntries->IsEmpty())
      cmap.Del(opt);
    
    // Disable signals
    allowsignals = false;
    // Remove widget
    gtk_widget_hide((GtkWidget *) li);
    gtk_container_remove (GTK_CONTAINER (GTK_COMBO(widget)->list), li);
    // Re-enable signals
    allowsignals = true;
    // Done
    return true;
  }

  //: Set selection string.
  
  bool ComboBodyC::SetSelection(const StringC &opt) {
    Manager.Queue(Trigger(ComboC(*this),&ComboC::GUISetSelection,opt));
    return true;
  }
  
  //: Set selection string.
  
  bool ComboBodyC::GUISetSelection(const StringC &opt) {
    if(widget == 0) {
      selection = opt;
      return false;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(selection != opt) // Do we actually need to change the selection ?
      gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(widget)->entry), opt.chars());
    return true;
  }
  
  //: Test if an entry exists.
  // Call from the GUI thread only.
  
  bool ComboBodyC::GUIEntryExists(const StringC &entry) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    return cmap.IsElm(entry);
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool ComboBodyC::Create(GtkWidget *nwidget) {
    widget = nwidget;
    
    for(DLIterC<StringC> it(choices);it.IsElm();it.Next()) {
      GtkWidget *li = gtk_list_item_new_with_label ((gchar *) it.Data().chars());
      gtk_widget_show (li);
      gtk_container_add (GTK_CONTAINER (GTK_COMBO(widget)->list), li);
      cmap[*it].InsLast(li);
    }
    
    if(!editable)
      gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(widget)->entry), 0);
    
    if(!selection.IsEmpty())
      gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(widget)->entry), selection.chars());
    
    ConnectSignals();
    gtk_signal_connect(GTK_OBJECT(GTK_COMBO(widget)->entry), "changed",
		       GTK_SIGNAL_FUNC (WidgetBodyC::gtkString),& signals["combo_activate"]);    
    
    return true;
  }

  //: Create the widget.
  
  bool ComboBodyC::Create() {
    widget = gtk_combo_new();
    //cerr << "  bool ComboBodyC::Create() " << widget ;
    // gtk_list_clear_items (GTK_LIST (combo->list), 0, -1);
    
    for(DLIterC<StringC> it(choices);it.IsElm();it.Next()) {
      GtkWidget *li = gtk_list_item_new_with_label ((gchar *) it.Data().chars());
      gtk_widget_show (li);
      gtk_container_add (GTK_CONTAINER (GTK_COMBO(widget)->list), li);
      cmap[*it].InsLast(li);
    }
    
    if(maxEntryLength >= 0) {
      gtk_entry_set_max_length (GTK_ENTRY (GTK_COMBO(widget)->entry), maxEntryLength);
      GtkStyle *entryStyle = gtk_widget_get_style(GTK_COMBO(widget)->entry);
      

#if 0
      entryStyle->font_desc;
      PangoFontMetrics *fontMetrics = pango_fontset_get_metrics (PangoFontset *fontset);
      IntT digitWidth = pango_font_metrics_get_approximate_digit_width(metrics);
#else
      GdkFont *entryFont = gtk_style_get_font(entryStyle);
      IntT digitWidth = gdk_string_width (entryFont,"0123456789") / 5;
#endif 

      gtk_widget_set_size_request(widget,maxEntryLength * digitWidth,-1);

   }
    
    if(!editable)
      gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(widget)->entry),0);
    
    if(!selection.IsEmpty())
      gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(widget)->entry), selection.chars());
    
    ConnectSignals();
    gtk_signal_connect(GTK_OBJECT(GTK_COMBO(widget)->entry), "changed",
		       GTK_SIGNAL_FUNC (WidgetBodyC::gtkString),& signals["combo_activate"]);
    
    return true;
  }
  
  bool ComboBodyC::FilterSignal(StringC &sel) {
    //cerr << "ComboBodyC::FilterSignal(), Sel='" << sel << "'\n";
    if(sel == selection)
      return true; // Its not changed!
    selection = sel;
    if (allowsignals) {
      // Send signal
      sigSelected(sel);
    }
    return true;
  }

  bool ComboBodyC::GUISetStyle(WidgetStyleC& style) {    
    // Set widget style
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    WidgetBodyC::GUISetStyle(style);
    // Set style of child label
    if (widget != 0) {
      // Set entry style
      GtkWidget *entry = GTK_COMBO(widget)->entry;
      if(entry != 0) {
	gtk_widget_set_style(GTK_WIDGET(entry),style.Style());
      }
      // Set listbox style
      GtkWidget *list = GTK_COMBO(widget)->list;
      if(list != 0) {
	gtk_widget_set_style(GTK_WIDGET(list),style.Style());
      }
    }
    return true;
  }
  
  //: Set maximum length of text entry.
  //!param: chars - Maximum number of charactors in widget.
  
  bool ComboBodyC::GUISetMaxLength(IntT chars) {
    //cerr << "\n GUISetMaxLength"   << widget; 
    maxEntryLength = chars;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_entry_set_max_length (GTK_ENTRY (GTK_COMBO(widget)->entry), maxEntryLength);
    GtkStyle *entryStyle = gtk_widget_get_style(GTK_COMBO(widget)->entry);
    GdkFont *entryFont = gtk_style_get_font(entryStyle);
    IntT digitWidth = gdk_string_width (entryFont,"0123456789") / 5;
    gtk_widget_set_size_request(widget,maxEntryLength * digitWidth,-1);
    return true;
  }
  
  IntT ComboBodyC::GetCursorPosition()
  {
    ReadBackLockC lock;
    return gtk_editable_get_position(GTK_EDITABLE(GTK_COMBO(widget)->entry));
  }
  
  bool ComboBodyC::GUISetCursorPosition(const IntT pos)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    StringC str(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(widget)->entry)));
    gtk_editable_set_position(GTK_EDITABLE(GTK_COMBO(widget)->entry), pos);
    return true;
  }
  
  bool ComboBodyC::SetCursorPosition(const IntT pos)
  {
    Manager.Queue(Trigger(ComboC(*this), &ComboC::GUISetCursorPosition, pos));
    return true;
  }
  
  bool ComboBodyC::GetCursorSelection(IntT &start, IntT &end)
  {
    ReadBackLockC lock;
    return gtk_editable_get_selection_bounds(GTK_EDITABLE(GTK_COMBO(widget)->entry), &start, &end) != 0;
  }
  
  bool ComboBodyC::GUISetCursorSelection(const IntT start, const IntT end)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_editable_select_region(GTK_EDITABLE(GTK_COMBO(widget)->entry), start, end);
    return true;
  }
  
  bool ComboBodyC::SetCursorSelection(const IntT start, const IntT end)
  {
    Manager.Queue(Trigger(ComboC(*this), &ComboC::GUISetCursorSelection, start, end));
    return true;
  }

  bool ComboBodyC::GUISetTextColour(const ByteRGBValueC &colour,GtkStateType &state) {
    if(widget == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GdkColor color;
    color.red = (IntT) colour.Red() * 255;
    color.green = (IntT) colour.Green() * 255;
    color.blue = (IntT) colour.Blue() * 255;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_widget_modify_text (GTK_COMBO(widget)->entry, state, &color);
    gtk_widget_modify_text (GTK_COMBO(widget)->list, state, &color);    
    gtk_widget_modify_text (widget, state, &color);    
    return true;
  }

  //: Set base colour.
  
  bool ComboBodyC::GUISetBaseColour(const ByteRGBValueC &colour,GtkStateType &state) {
    if(widget == 0)
      return false;
    GdkColor color;
    color.red = (IntT) colour.Red() * 255;
    color.green = (IntT) colour.Green() * 255;
    color.blue = (IntT) colour.Blue() * 255;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_widget_modify_base (GTK_COMBO(widget)->entry, state, &color);    
    gtk_widget_modify_base (GTK_COMBO(widget)->list, state, &color);    
    gtk_widget_modify_base (widget, state, &color);    
    return true;
  }
  
  //: Set background colour.
  
  bool ComboBodyC::GUISetBackgroundColour(const ByteRGBValueC &colour,GtkStateType &state) {
    if(widget == 0)
      return false;
    GdkColor color;
    color.red = (IntT) colour.Red() * 255;
    color.green = (IntT) colour.Green() * 255;
    color.blue = (IntT) colour.Blue() * 255;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_widget_modify_bg (GTK_COMBO(widget)->entry, state, &color);
    gtk_widget_modify_bg (GTK_COMBO(widget)->list, state, &color);    
    gtk_widget_modify_bg (widget, state, &color);    
    return true;
  }
  
  //: Set background colour.
  
  bool ComboBodyC::GUISetForgroundColour(const ByteRGBValueC &colour,GtkStateType &state) {
    if(widget == 0)
      return false;
    GdkColor color;
    color.red = (IntT) colour.Red() * 255;
    color.green = (IntT) colour.Green() * 255;
    color.blue = (IntT) colour.Blue() * 255;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_widget_modify_fg (GTK_COMBO(widget)->entry, state, &color);
    gtk_widget_modify_fg (GTK_COMBO(widget)->list, state, &color);    
    gtk_widget_modify_fg (widget, state, &color);    
    return true;
  }

  
}
