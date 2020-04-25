// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ToolTips.cc"

#include "Ravl/GUI/ToolTips.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/CallMethods.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  ToolTipsC guiGlobalToolTips(true);
  
  //: Constructor.
  
  ToolTipsBodyC::ToolTipsBodyC()
    : enabled(true),
      widget(0),
      delay(-1)
  { 
    //Manager.Queue(Trigger(ToolTipsC(*this),&ToolTipsC::Create));
  }
  
  //: Create the widget.
  
  bool ToolTipsBodyC::Create() {
    //cerr << "ToolTipsBodyC::Create(), Called. \n";
    if(widget != 0)
      return true;
    widget = gtk_tooltips_new();
    if(!enabled)
      gtk_tooltips_disable(GTK_TOOLTIPS(widget));
    if(delay >= 0)
      gtk_tooltips_set_delay(widget,(gint)((RealT)delay*1000000.0));
    //ConnectSignals();
    return true;
  }
  
  //: Undo all references.
  
  void ToolTipsBodyC::Destroy()  {
  }
  
  
  //: Enable tool tips.
  // Thread safe.
  
  void ToolTipsBodyC::Enable() {
    Manager.Queue(Trigger(ToolTipsC(*this),&ToolTipsC::GUIEnable));
  }
  
  //: Disable tool tips.
  // Thread safe.
  
  void ToolTipsBodyC::Disable() {
    Manager.Queue(Trigger(ToolTipsC(*this),&ToolTipsC::GUIDisable));
  }
  
  //: Enable tool tips.
  // GUI Thread only.
  
  bool ToolTipsBodyC::GUIEnable() {
    if(widget == 0) 
      return true;  
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tooltips_enable(GTK_TOOLTIPS(widget));
    return true;
  }
  
  //: Disable tool tips.
  // GUI Thread only.
  
  bool ToolTipsBodyC::GUIDisable() {
    enabled = false;
    if(widget == 0) 
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tooltips_disable(GTK_TOOLTIPS(widget));
    return true;
  }
  
  //: Set tips delay.
  // Thread safe.
  
  void ToolTipsBodyC::SetDelay(RealT secs) {
    Manager.Queue(Trigger(ToolTipsC(*this),&ToolTipsC::GUISetDelay,secs));
  }
  
  
  //: Set tips delay.
  // GUI Thread only.
  
  bool ToolTipsBodyC::GUISetDelay(RealT secs)  {
    delay = secs;
    if(widget == 0)
      return true;
    gtk_tooltips_set_delay(widget,(gint)((RealT)secs*1000000.0));
    return true;
  }
  

  //: Set tips delay.
  // Thread safe.
  
  void ToolTipsBodyC::AddToolTip(WidgetC &widge,const char *text,const char *context) {
    char *t = (char *) text;
    char *c = (char *) context;
    Manager.Queue(Trigger(ToolTipsC(*this),&ToolTipsC::GUIAddToolTip,widge,t,c));
  }
  
  //: Set tips delay.
  // GUI Thread only.
  
  bool ToolTipsBodyC::GUIAddToolTip(WidgetC &cwidge,const char *text,const char *context) {
    if(widget == 0)
      Create();
    if(cwidge.Widget() == 0) {
      if(!cwidge.Create()) {
	cerr << "ERROR: ToolTipsBodyC::GUIAddToolTip(), Failed to create widget. \n";
	return true;
      }
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tooltips_set_tip(widget,
			 cwidge.Widget(),
			 text,
			 context);
    return true;
  }

}
