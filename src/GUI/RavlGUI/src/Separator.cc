// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Separator.cc"

#include "Ravl/GUI/Separator.hh"
#include <gtk/gtkhseparator.h>
#include <gtk/gtkvseparator.h>

namespace RavlGUIN {
  //: Create the widget.
  
  bool SeparatorBodyC::Create() {
    if(widget != 0)
      return true;
    if(vert)
      widget = gtk_vseparator_new();
    else
      widget = gtk_hseparator_new();
    ConnectSignals();
    return true;
  }
}
