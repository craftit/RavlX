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
//! file="Ravl/GUI/GTK/OneChild.cc"

#include "Ravl/GUI/OneChild.hh"

namespace RavlGUIN {

  //: Constructor.
  
  OneChildBodyC::OneChildBodyC(const WidgetC &widge)
    : child(widge)
  {}
  
  //: Create the widget.
  
  bool OneChildBodyC::Create() {
    RavlAssert(0);
    return false;
  }

  //: Create with a widget supplied from elsewhere.
  
  bool OneChildBodyC::Create(GtkWidget *widget) {
    return WidgetBodyC::Create(widget);
  }
  
  //: Add widget.
  
  bool OneChildBodyC::Add(const WidgetC &nchild) {
    child=nchild;
    return true;
  }
  

  void OneChildBodyC::Destroy() {
    if(child.IsValid())
      child.Destroy();
    WidgetBodyC::Destroy();
  }
}
