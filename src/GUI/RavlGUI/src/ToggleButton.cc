// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! docentry="Graphics.GTK.Widget"
//! lib=RavlGUI
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/ToggleButton.cc"

#include "Ravl/GUI/ToggleButton.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  ToggleButtonBodyC::ToggleButtonBodyC(const char *nlabel,bool nInitState)
    : ButtonBodyC(nlabel),
      initState(nInitState),
      initInconsistant(false),
      sigChanged(true)
  {}
  
  //: Constructor.
  
  ToggleButtonBodyC::ToggleButtonBodyC(const char *nlabel,const PixmapC &pixm,bool nInitState)
    : ButtonBodyC(nlabel,pixm),
      initState(nInitState),
      initInconsistant(false),
      sigChanged(true)
  {}
  
  //: Create the actual widget.
  // This allows different types of buttons to
  // be created easily.
  
  GtkWidget *ToggleButtonBodyC::BuildWidget(const char *aLab) {
    ONDEBUG(std::cerr << "Built toggle button. \n");
    if(aLab == 0)
      return gtk_toggle_button_new ();
    return gtk_toggle_button_new_with_label (aLab);
  }
  
  // Signal state to clients with 'sigChanged'
  
  bool ToggleButtonBodyC::SignalState() { 
    ONDEBUG(std::cerr << "ToggleButtonBodyC::SignalState() : " << GUIIsActive() << "\n");
    bool isActive = GUIIsActive();
    sigChanged(isActive); 
    return true;
  }
  
  //: Create the widget.
  
  bool ToggleButtonBodyC::Create() {
    if(!ButtonBodyC::Create())
      return false;
    if(initState)  // Default state is off.
      GUISetActive(initState); // This will actual cause a signal ??
    if(initInconsistant)
      GUISetInconsistent(initInconsistant);
    Connect(Signal("toggled"),ToggleButtonC(*this),&ToggleButtonC::SignalState);
    //ConnectSignals();
    
    return true;
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool ToggleButtonBodyC::Create(GtkWidget *newwidget) {
    if(!ButtonBodyC::Create(newwidget))
       return false;
    if(initState)  // Default state is off.
      GUISetActive(initState); // This will actual cause a signal ??
    if(initInconsistant)
      GUISetInconsistent(initInconsistant);
    Connect(Signal("toggled"),ToggleButtonC(*this),&ToggleButtonC::SignalState);
    return true;
  }
  
  //: Undo all references.
  
  void ToggleButtonBodyC::Destroy() {
    sigChanged.DisconnectAll();
    WidgetBodyC::Destroy();
  }
  
  
  //: Set button active.
  // GUI thread only.
  
  bool ToggleButtonBodyC::GUISetActive(bool x) {
    initState = x;
    if(widget == 0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),initState);
    return true;
  }
  
  //: Test if button is active.
  

  bool ToggleButtonBodyC::GUIIsActive() const { 
    if(widget == 0) {
      ONDEBUG(std::cerr << "ToggleButtonBodyC::GUIIsActive() No widget. \n");
      return initState;
    }
	
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
#if 1
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) != 0;
#else
    return GTK_TOGGLE_BUTTON(widget)->active;
#endif
  }

  //: Test if button is active.
  
  bool ToggleButtonBodyC::IsActive() const { 
    if(widget == 0) {
      ONDEBUG(std::cerr << "ToggleButtonBodyC::GUIIsActive() No widget. \n");
      return initState;
    }
    return GTK_TOGGLE_BUTTON(widget)->active;
  }
  
  //: Set toggle state.
  // GUI thread only.
  
  bool ToggleButtonBodyC::GUISetToggle(const bool &val) {
    initState = val;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if((gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) != 0) != val)
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),val);
    return true;
  }
  
  //: Set toggle state.
  
  void ToggleButtonBodyC::SetToggle(const bool &val) 
  { Manager.Queue(Trigger(ToggleButtonC(*this),&ToggleButtonC::GUISetToggle,val)); }
  
  //: Set inconsistant state
  // GUI thread only.
  
  bool ToggleButtonBodyC::GUISetInconsistent(bool val) {
    initInconsistant = val;
    if(widget == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(widget),val);
    return true;
  }
  
  //: Set inconsistant state
  
  bool ToggleButtonBodyC::SetInconsistent(const bool &val) { 
    Manager.Queue(Trigger(ToggleButtonC(*this),&ToggleButtonC::GUISetInconsistent,val)); 
    return true;
  }
  
  //: Create a button.
  
  ToggleButtonC::ToggleButtonC(const char *nlabel,bool nInitState)
    : ButtonC(*new ToggleButtonBodyC(nlabel,nInitState))
  {}

  ///////////////////////////////////////////////////////////
  
  //: Create the actual widget.
  // This allows different types of buttons to
  // be created easily.
  
  GtkWidget *CheckButtonBodyC::BuildWidget(const char *aLab) {
    //cerr << "Built toggle button. \n";
    if(aLab == 0)
      return gtk_check_button_new ();
    return gtk_check_button_new_with_label (aLab);
  }
  
  
  //: Create a button.
  
  CheckButtonC::CheckButtonC(const char *nlabel,bool nInitState)
    : ToggleButtonC(*new CheckButtonBodyC(nlabel,nInitState))
  {}
}
