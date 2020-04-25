// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/MenuCheck.cc"

#include "Ravl/GUI/MenuCheck.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"

#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN
{
  extern void MenuItemC_response ( GtkWidget *,MenuItemBodyC *data);

  int MenuCheckItemBodyC::doSelected(GtkWidget *widget,Signal1C<bool> *data){ 
    if(data->IsValid()) {
      bool isActive(GTK_CHECK_MENU_ITEM(widget)->active);
      (*data)(isActive);
    }
    return 1;
  }
  
  //: Constructor.

  MenuCheckItemBodyC::MenuCheckItemBodyC(const StringC &lab,bool initActive)
    : MenuItemBodyC(lab),
      selectedToggle(true),
      active(initActive)
  {}
  
  //: Set active.
  
  bool MenuCheckItemBodyC::GUISetActive(const bool &val) {
    active = val;
    if(widget == 0)  return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget),val);
    return true;
  }
  
  //: Test if toggle is active.
  
  bool MenuCheckItemBodyC::IsActive() const {
    if(widget == 0)
      return active;
    ReadBackLockC lock;
    return GTK_CHECK_MENU_ITEM(widget)->active;
  }
  
  //: Undo all references
  
  void MenuCheckItemBodyC::Destroy() {
    selectedToggle.Invalidate();
    MenuItemBodyC::Destroy();
  }
  
  //: Set active.
  
  void MenuCheckItemBodyC::SetActive(bool val) {
    Manager.Queue(Trigger(MenuCheckItemC(*this),&MenuCheckItemC::GUISetActive,val));
  }
  
  //: Create the widget.
  
  bool MenuCheckItemBodyC::Create() {
    ONDEBUG(std::cerr << "MenuItemBodyC::Create(), Called. \n");
    
    /* Create a new menu-item with a name... */
    widget = gtk_check_menu_item_new_with_label(name);
    
    gtk_check_menu_item_set_show_toggle (GTK_CHECK_MENU_ITEM(widget),true);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(widget),active);
    
    /* Do something interesting when the menuitem is selected */
    gtk_signal_connect(GTK_OBJECT(widget), "activate",
		       GTK_SIGNAL_FUNC(RavlGUIN::MenuItemC_response), this);
    
    gtk_signal_connect(GTK_OBJECT(widget), "activate",
		       GTK_SIGNAL_FUNC(MenuCheckItemBodyC::doSelected), &selectedToggle);
    
    ONDEBUG(std::cerr << "MenuItemBodyC::Create(), Done. \n");
    return true;
  }

}
