// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_WIDGETDNDINFO_HEADER
#define RAVLGUI_WIDGETDNDINFO_HEADER 1
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/WidgetDNDInfo.hh"

#include "Ravl/SArray1d.hh"
#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {
  
  //: Extra drag and drop info, where its needed.
  
  class WidgetDndInfoC {
  public:
    WidgetDndInfoC()
      : isSource(false),
	isTarget(false)
    {}
    //: Constructor.
    
    bool isSource;
    bool isTarget;
    DestDefaultsT TargFlags;
    ModifierTypeT SrcFlags;
    SArray1dC<GtkTargetEntry> TargEntries;
    SArray1dC<GtkTargetEntry> SrcEntries;
    DragActionT TargActions,SrcActions;
  };
  
}


#endif
