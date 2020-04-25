// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Fixed.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Fixed.hh"
#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN {

  //: Default constructor.
  
  FixedWidgetBodyC::FixedWidgetBodyC()
    : defaultPos(5)
  {}
  
  //: Create the widget.
  
  bool FixedWidgetBodyC::Create() {
    //cerr << "FixedWidgetBodyC::Create(), Called ! \n";
    widget = gtk_fixed_new();
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) {
      WidgetC widge = it.Data();
      Index2dC at;
      if(!where.IsElm(widge)) { // Setup default position.
	at = Index2dC(defaultPos,defaultPos);
	where[widge] = at;
	defaultPos += 10;
	return false;
      } else
	at = where[it.Data()];
      if(it.Data().Widget() == 0) {
	if(!it.Data().Create()) {
	  std::cerr << "FixedWidgetBodyC::Create(), Widget create failed ! \n";
	  continue;
	}
      }
      ONDEBUG(std::cerr << "Placing widget " << it.Data().Hash() << " at " << at << "\n");
      gtk_fixed_put (GTK_FIXED (widget), it.Data().Widget(), at.Row().V(),at.Col().V());
      gtk_widget_show (it.Data().Widget());
    }
    ConnectSignals();
    return true;
  }
  
  //: Add new widget.
  // Thread safe.
  
  bool FixedWidgetBodyC::AddWidget(const WidgetC &newun,Index2dC where) {
    Manager.Queue(Trigger(FixedWidgetC(*this),&FixedWidgetC::GUIAddWidget,newun,where));
    return true;
  }
  
  //: Add new widget.
  // GUI thread only.
  
  bool FixedWidgetBodyC::GUIAddWidget(WidgetC &newun,Index2dC &loc) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(where.IsElm(newun)) {
      where[newun] = loc;
      GUIMove(newun,loc);
    } else {
      ONDEBUG(std::cerr << "Adding new widget \n");
      where[newun] = loc;
      GUIAdd(newun);
    }
    return true;
  }
  
  //: Where should we place this widget.
  
  Index2dC FixedWidgetBodyC::Position(const WidgetC &) { 
    return Index2dC(1,1); 
  }
  
  //: Add a child widget.
  
  bool FixedWidgetBodyC::GUIAdd(const WidgetC &awidge) {
    WidgetC widge(awidge);
    if(!ContainerWidgetBodyC::GUIAdd(widge))
      return false;
    if(!where.IsElm(widge)) { // Setup default position.
      where[widge] = Index2dC(defaultPos,defaultPos);
      defaultPos += 10;
      return false;
    }
    if(widget != 0) { // Are we adding a widget later ?
      ONDEBUG(std::cerr << "FixedWidgetBodyC::GUIAdd(), Adding widget to existing window. \n");
      // Nope, do setup now.
      if(widge.Widget() == 0) {
	if(!widge.Create()) {
	  std::cerr << "FixedWidgetBodyC::Create(), Widget create failed ! \n";
	}
      }
      Index2dC &at = where[widge];
      gtk_fixed_put (GTK_FIXED (widget), widge.Widget(), at.Row().V(),at.Col().V());
      gtk_widget_show (widge.Widget());
    }
    return true;
  }
  
  //: Move a widget.
  
  bool FixedWidgetBodyC::GUIMove(WidgetC &widge,Index2dC at) {
    ONDEBUG(std::cerr << "Moveing widget :" << widge.IsValid() << " to " << at << "\n");
    // Should check the widget is actual inside.
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_fixed_move(GTK_FIXED (widget), widge.Widget(),at.Row().V(),at.Col().V());
    return true;
  }
  
  //: Move a widget.
  
  bool FixedWidgetBodyC::Move(WidgetC &widge,Index2dC where) {
    Manager.Queue(Trigger(FixedWidgetC(*this),&FixedWidgetC::GUIMove,widge,where));
    return true;
  }

}
