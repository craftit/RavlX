// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

#include "Ravl/GUI/ComboBoxEntry.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  //: Constructor
  
  ComboBoxEntryBodyC::ComboBoxEntryBodyC() 
    : m_textEntry("")
  {}
  
  //: Constructor
  
  ComboBoxEntryBodyC::ComboBoxEntryBodyC(TreeModelC &treeModel,UIntT textColumn) 
    : ComboBoxBodyC(treeModel),
      m_textEntry(""),
      m_textColumn(textColumn)
  {}
  
  //: Create with a widget possibly supplied from elsewhere.
  
  bool ComboBoxEntryBodyC::CommonCreate(GtkWidget *nwidget) {
    if(nwidget == 0) {
      if(m_treeModel.IsValid()) {
        nwidget = gtk_combo_box_entry_new_with_model (m_treeModel.TreeModel(),m_textColumn);
      } else {
        // There is currently no way to add to a model or cell layout if this condition is reached.
        nwidget = gtk_combo_box_entry_new();
      }
    }
    
    if(!ComboBoxBodyC::CommonCreate(nwidget))
      return false;
    
    GtkWidget *entryWidget = gtk_bin_get_child(GTK_BIN(widget));
    RavlAssert(entryWidget != 0);
    m_textEntry.Create(entryWidget);
    
    return true;
  }

}
