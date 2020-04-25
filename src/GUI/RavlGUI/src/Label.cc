// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Label.cc"

#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN  {

  //: Constructor.
  
  LabelBodyC::LabelBodyC(const StringC &ntext, const bool markup)
    : text(ntext.Copy()),
      lineWrap(false),
      justification(0),
      m_markup(markup)
  {}
  
  StringC LabelBodyC::Name() const
  { return text; }
  
  //: Create the widget.
  
  bool LabelBodyC::Create() {
    if(widget != 0)
      return true;
    widget = gtk_label_new (text);
    if(lineWrap)
      gtk_label_set_line_wrap (GTK_LABEL(widget),lineWrap);
    if(justification != 0)
      gtk_label_set_justify (GTK_LABEL(widget),(GtkJustification) justification);
    if (m_markup)
      gtk_label_set_markup(GTK_LABEL(widget), text.chars());

    ConnectSignals();
    return true;
  }

  //: Create with a widget supplied from elsewhere.
  
  bool LabelBodyC::Create(GtkWidget *_widget) {
    widget = _widget;
    ConnectSignals();    
    return true;
  }
  
  //: Set label.
  
  bool LabelBodyC::GUISetLabel(const StringC &txt) {
    text = txt;
    m_markup = false;
    if(widget == 0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_label_set (GTK_LABEL(widget),text);
    return true;
  }
  
  //: Update the text in the label. 
  bool LabelBodyC::Label(const StringC &txt) 
  {
    Manager.QueueOnGUI(Trigger(LabelC(*this),&LabelC::GUISetLabel,txt));
    return true;
  }
  
  bool LabelBodyC::GUIJustify(GtkJustification& justify) {
    justification = (IntT) justify;
    if(widget == 0) return true; // Maybe on shutdown ?
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_label_set_justify (GTK_LABEL(widget),justify);
    return true;
  }

  bool LabelBodyC::Justify(GtkJustification& justify) {
    Manager.Queue(Trigger(LabelC(*this),&LabelC::GUIJustify,justify));
    return true;
  }
  
  bool LabelBodyC::GUIWrap(const bool& wrap) {
    lineWrap = wrap;
    if(widget == 0) return true; // Maybe on shutdown ?
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_label_set_line_wrap (GTK_LABEL(widget),wrap);
    return true;    
  }

  bool LabelBodyC::Wrap(const bool& wrap) {
    Manager.Queue(Trigger(LabelC(*this),&LabelC::GUIWrap,wrap));
    return true;
  }

  bool LabelBodyC::SetMarkup(const StringC &text)
  {
    Manager.Queue(Trigger(LabelC(*this), &LabelC::GUISetMarkup, text));
    return true;
  }

  bool LabelBodyC::GUISetMarkup(const StringC &newText)
  {
    m_markup = true;
    text = newText;

    if (widget == 0)
      return true;

    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");

    gtk_label_set_markup(GTK_LABEL(widget), text.chars());

    return true;
  }

}
