// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! docentry="Graphics.GTK.Widget"
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/Paned.cc"

#include "Ravl/GUI/Paned.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  //: Constructor.
  
  PanedBodyC::PanedBodyC(const WidgetC &widge1,const WidgetC &widge2,bool nvertical)
    : child1(widge1),
      child2(widge2),
      vertical(nvertical)
  {}
  
  //: Create the widget.
  
  bool PanedBodyC::Create() {
    if(vertical)
      widget = gtk_vpaned_new();
    else
      widget = gtk_hpaned_new();
    if(child1.IsValid()) {
      child1.GUIShow();
      gtk_paned_add1(GTK_PANED(widget),child1.Widget());
    }
    if(child2.IsValid()) {
      child2.GUIShow();
      gtk_paned_add2(GTK_PANED(widget),child2.Widget());
    }
    return true;
  }
  
  void PanedBodyC::Destroy() {
    if(child1.IsValid())
      child1.Destroy();
    if(child2.IsValid())
      child2.Destroy();
    WidgetBodyC::Destroy();
  }
}
