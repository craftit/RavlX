// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ScrolledArea.cc"

#include "Ravl/GUI/ScrolledArea.hh"
#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN {

  /////////////////////////////////
  //: Constructor.
  
  ScrolledAreaBodyC::ScrolledAreaBodyC(const WidgetC &achild,IntT sx,IntT sy,bool nVauto,bool nHauto)
    : OneChildBodyC(achild),
      isx(sx),isy(sy)
  {
    if(nVauto)
      vpolicy = GTK_POLICY_AUTOMATIC;
    else
      vpolicy = GTK_POLICY_ALWAYS;
    if(nHauto)
      hpolicy = GTK_POLICY_AUTOMATIC;
    else
      hpolicy = GTK_POLICY_ALWAYS;
  }
  
  ScrolledAreaBodyC::ScrolledAreaBodyC(const WidgetC &achild,
				       IntT sx,IntT sy,
				       GtkPolicyType nvpolicy,GtkPolicyType nhpolicy) 
    : OneChildBodyC(achild),
      isx(sx),isy(sy),
      vpolicy(nvpolicy),
      hpolicy(nhpolicy)    
  {}
  
  
  //////////////////////////////////
  //: Create the widget.
  
  bool ScrolledAreaBodyC::Create() {
    if(widget != 0)
      return true; // Done already!
    
    widget = gtk_scrolled_window_new(0,0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (widget),
				   hpolicy,vpolicy);
    
    if(child.IsValid()) {
      if(child.Widget() == 0)
	if(!child.Create())
	  return false;
      gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(widget),
					     child.Widget());
      child.GUIShow();
    }
    
    if(isx >= 0 || isy >= 0) {
      if(isy < 0)
	isy = 0;
      if(isx < 0)
	isx = 0;
      GUISetUSize(isx,isy);
    } 
    
    ConnectSignals();
    return true;
  }
  
}
