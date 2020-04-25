// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

#include "Ravl/GUI/ComboBoxEntryText.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  //: Constructor
  
  ComboBoxEntryTextBodyC::ComboBoxEntryTextBodyC()
    : m_textEntry("")
  {}
  
  //: Constructor
  
  ComboBoxEntryTextBodyC::ComboBoxEntryTextBodyC(const DListC<StringC> &options)
    : ComboBoxTextBodyC(options, true),
      m_textEntry("")
  {}
  
  //: Create with a widget possibly supplied from elsewhere.
  
  bool ComboBoxEntryTextBodyC::CommonCreate(GtkWidget *nwidget) {
    if (nwidget == 0) {
      nwidget = gtk_combo_box_entry_new_text();
    }
    
    if(!ComboBoxTextBodyC::CommonCreate(nwidget))
      return false;

    GtkWidget *entryWidget = gtk_bin_get_child(GTK_BIN(widget));
    RavlAssert(entryWidget != 0);
    m_textEntry.Create(entryWidget);
    
    return true;
  }

}
