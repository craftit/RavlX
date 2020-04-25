// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/EventBox.cc"

#include "Ravl/GUI/EventBox.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  //: Create the widget.
  
  bool EventBoxBodyC::Create() {
    //cerr << "EventBoxBodyC::Create(), Called. \n";
    widget = gtk_event_box_new ();
    if(child.IsValid()) {
      if(!child.Create())
	return false;
      gtk_container_add (GTK_CONTAINER (widget), child.Widget());
      gtk_widget_show (child.Widget());
    }
    ConnectSignals();
    return true;
  }
}
