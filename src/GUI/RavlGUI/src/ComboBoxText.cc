// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

#include "Ravl/GUI/ComboBoxText.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/GUI/ListStore.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  //: Constructor
  
  ComboBoxTextBodyC::ComboBoxTextBodyC()
    : m_editable(false),
      m_useGladeOptions(true),
      m_sigSelected(StringC(""))
  {}
  
  //: Constructor
  
  ComboBoxTextBodyC::ComboBoxTextBodyC(const DListC<StringC> &options,bool editable)
    : m_options(options),
      m_editable(editable),
      m_useGladeOptions(true),
      m_sigSelected(StringC(""))
  {}
  
  //: Create with a widget possibly supplied from elsewhere.
  
  bool ComboBoxTextBodyC::CommonCreate(GtkWidget *nwidget) {
    if(nwidget == 0) {
      ONDEBUG(std::cerr << "ComboBoxTextBodyC::CommonCreate, Creating widget. \n");
      if(m_editable)
        nwidget = gtk_combo_box_entry_new_text();
      else
        nwidget = gtk_combo_box_new_text();
    }


    if(!ComboBoxBodyC::CommonCreate(nwidget))
      return false;
    
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(widget));
    if(model == 0) {
      ONDEBUG(std::cerr << "ComboBoxTextBodyC::CommonCreate, Generating new tree store. \n");
      SArray1dC<AttributeTypeC> colTypes(1);
      colTypes[0] = AttributeTypeStringC("Another","...");
      m_treeModel = ListStoreC(colTypes);
      gtk_combo_box_set_model (GTK_COMBO_BOX(widget),m_treeModel.TreeModel());
    } else {
      ONDEBUG(std::cerr << "ComboBoxTextBodyC::CommonCreate, Using old handle. \n");
      m_treeModel = ListStoreC(model);
      // Remove anything put there by glade if we have some predefined contents.
      if(m_options.Size() > 0)
        m_treeModel.GUIEmpty();
    }

    
    
    
    
    RavlAssert(widget != 0);
    ONDEBUG(std::cerr << "ComboBoxTextBodyC::CommonCreate, Adding " << m_options.Size() << " options to combobox. \n");
    
    for(DLIterC<StringC> it(m_options);it;it++)
      gtk_combo_box_append_text(GTK_COMBO_BOX(widget),it->chars());

    ConnectRef(Signal("changed"),*this,&ComboBoxTextBodyC::CBChanged);
    
    if(!m_selected.IsEmpty()) {
      GUISetTextSelected(m_selected);
    }
    
    return true;
  }

  //: Access current selected text.
  
  StringC ComboBoxTextBodyC::GUITextSelected() const {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    return StringC(gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget)));
  }
  
  //: Access current selection.
  
  StringC ComboBoxTextBodyC::TextSelected() const {
    ReadBackLockC lock;    
    return m_selected;
  }
  
  //: Process changed call back signals.
  
  bool ComboBoxTextBodyC::CBChanged() {
    //std::cerr << "Selection changed. \n";
    StringC selStr = GUITextSelected();
    m_selected = selStr;
    m_sigSelected(selStr);
    return true;
  }

  //: Set selected entry.
  
  bool ComboBoxTextBodyC::GUISetTextSelected(const StringC &str) {
    if(widget == 0) {
      m_selected = str;
      return true;
    }    
    int pos = GUIFindEntryPosition(str);
    if(pos < 0)
      return false;
    return GUISetSelectionIndex(pos);
  }
  
  //: Find the row number of the str.
  // If -ve value is return the entry hasn't been found.
  
  IntT ComboBoxTextBodyC::GUIFindEntryPosition(const StringC &str) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    RavlAssert(m_treeModel.IsValid());
    
    TreeModelIterC it(m_treeModel);
    IntT ret = -1;
    int i = 0;
    
    // Are there any entries ?
    if(it.IsElm()) {
      do {
        StringC value;
        // Get String value for row.
        if(m_treeModel.GetValue(it,0,value)) {
          if(value == str) {
            // Found it!
            ret = i;
            break;
          }
        }
        i++;
      } while(it.Next());
    }
    
    return ret;
  }
  
  //: Append new entry
  
  bool ComboBoxTextBodyC::GUIAppendEntry(const StringC &str) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_combo_box_append_text(GTK_COMBO_BOX(widget),str.chars());
    return true;
  }
  
  //: Prepend a new entry
  
  bool ComboBoxTextBodyC::GUIPrependEntry(const StringC &str) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_combo_box_prepend_text(GTK_COMBO_BOX(widget),str.chars());
    return true;
  }
  
  //: Insert an entry.
  
  bool ComboBoxTextBodyC::GUIInsertEntry(IntT pos,const StringC &str) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_combo_box_insert_text(GTK_COMBO_BOX(widget),pos,str.chars());
    return true;
  }
  
  //: Remove an entry.
  bool ComboBoxTextBodyC::GUIRemoveEntry(IntT pos) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_combo_box_remove_text(GTK_COMBO_BOX(widget),pos);
    return true;
  }
  
  //: Remove an entry.
  bool ComboBoxTextBodyC::GUIRemoveEntry(const StringC &str) {
    IntT index = GUIFindEntryPosition(str);
    if (index >= 0)
      return GUIRemoveEntry(index);
    return true;
  }

  //: Clear all entries.
  
  bool ComboBoxTextBodyC::GUIClear() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    m_treeModel.GUIEmpty();    
    return true;
  }

}
