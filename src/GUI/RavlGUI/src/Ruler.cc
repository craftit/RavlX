// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Ruler.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Ruler.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

namespace RavlGUIN {

  bool RulerBodyC::GUIAttachTo(WidgetC &widge) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!widge.IsValid()) {
      std::cerr << "RulerBodyC::GUIAttachTo(), Passed invalid widget. \n";
      return false;
    }
    if(widge.Widget() == 0) {
      ONDEBUG(std::cerr << "RulerBodyC::GUIAttachTo(), Creating target. \n");
      if(!widge.Create()) {
	cerr << "RulerBodyC::GUIAttachTo(), Create failed. \n";
	return false;
      }
    }
    ONDEBUG(std::cerr << "RulerBodyC::GUIAttachTo(): " << widge.WidgetName() << "\n");
    if(Widget() == 0) {
      ONDEBUG(std::cerr << "RulerBodyC::GUIAttachTo(), Create self. \n");
      if(!Create()) {
	cerr << "RulerBodyC::GUIAttachTo(), Self create failed. \n";
	return false;
      }
    }
    
    gtk_widget_add_events(widge.Widget(), GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    g_signal_connect_swapped (G_OBJECT (widge.Widget()), "motion_notify_event",
			      G_CALLBACK (EVENT_METHOD (Widget(), motion_notify_event)),
			      Widget());
    ConnectSignals();
    
    return true;
  }
  
  //: Set range of ruler.
  // GUI Thread only.
  
  bool RulerBodyC::GUISetRange(RealT &nlower,RealT &nupper) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    lower = nlower;
    upper = nupper;
    gtk_ruler_set_range(GTK_RULER(widget),
			lower,upper,
			position,max_size);
    return true;
  }
  
  bool RulerBodyC::Create() {
    if(widget != 0)
      return true; // Done already.
    if(vert)
      widget = gtk_vruler_new();
    else
      widget = gtk_hruler_new();
    gtk_ruler_set_metric( GTK_RULER(widget), GTK_PIXELS );
    gtk_ruler_set_range(GTK_RULER(widget),
			lower,
			upper,
			position,
			max_size);
    return true;
  }
  
  //: Set range and position for the ruler.
  // GUI Thread only.
  
  bool RulerBodyC::GUISetRangeAndPosition(RealT &nlower,RealT &nupper,RealT &pos) {
    
    lower = nlower;
    upper = nupper;
    position = pos;
    if(widget == 0)
      return true;
    gtk_ruler_set_range(GTK_RULER(widget),
                        lower,upper,
                        position,max_size);
    
    return true;
  }

  /////////////////////////////////////////////////////////////////////
  
  void RulerC::SetRange(RealT lower,RealT upper) { 
    Manager.Queue(Trigger(*this,&RulerC::GUISetRange,lower,upper)); 
  }
  
  bool RulerC::AttachTo(WidgetC &widge) {
    ONDEBUG(std::cerr << "RulerC::AttachTo(), Queueing... \n");
    Manager.Queue(Trigger(*this,&RulerC::GUIAttachTo,widge));
    return true;
  }
}
