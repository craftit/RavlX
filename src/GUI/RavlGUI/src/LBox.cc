// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/LBox.cc"

#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/Stream.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN {
  

  //: Create the widget.
  
  bool LBoxBodyC::CommonCreate(GtkWidget *_widget) {
    if (_widget == NULL)
    {
      if(widget != 0) 
        return true;
      
      if(vert)
        widget = gtk_vbox_new (homogeneous,spacing);
      else
        widget = gtk_hbox_new (homogeneous,spacing);
      
      if(border != 0)
        gtk_container_set_border_width (GTK_CONTAINER (widget), border);
    }
    else
    {
      widget = _widget;
    }
    
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) {
      if(!it->IsValid()) {
        std::cerr << "LBoxBodyC::Create(), WARNING: Invalid widget in child list. \n";
        continue;
      }
      WidgetC &child = *it;
      if(child.Widget() == 0) {
        if(!child.Create()) {
          std::cerr << "LBoxBodyC::Create(), WARNING: Widget create failed ! \n";
          continue;
        }
      }
      RavlAssert(child.Widget() != 0);
      PackInfoC pi(child);
      if(pi.IsValid()) {
        ONDEBUG(std::cerr << "LBoxBodyC::Create(), Found pack info :" << pi.Expand() << " " << pi.Fill() << " " << pi.Padding() << "\n");
        gtk_box_pack_start(GTK_BOX (widget), child.Widget(),pi.Expand(), pi.Fill(), pi.Padding());
      } else {
        gtk_box_pack_start(GTK_BOX (widget), child.Widget(), true, true, 0);
      }
      gtk_widget_show (child.Widget());
    }
    
    ConnectSignals();
    
    return true;
  }

  //: Add widget.
  // GUI thread only.
  
  bool LBoxBodyC::GUIAdd(const WidgetC &achild) {
    children.InsLast(achild);
    WidgetC &child = children.Last();
    if(widget == 0) 
      return true;
    if(child.Widget() == 0) {
      if(!child.Create()) {
        std::cerr << "LBoxBodyC::Create(), Widget create failed ! \n";
        return false;
      }
    }
    PackInfoC pi(child);
    if(pi.IsValid()) {
      ONDEBUG(std::cerr << "LBoxBodyC::Create(), Found pack info :" << pi.Expand() << " " << pi.Fill() << " " << pi.Padding() << "\n");
      gtk_box_pack_start(GTK_BOX (widget), child.Widget(), 
                          pi.Expand(), pi.Fill(), pi.Padding());
    } else {
      gtk_box_pack_start(GTK_BOX (widget), child.Widget(), true, true, 0);
    }
    gtk_widget_show (child.Widget());
    return true;
  }
  
  ///////////////////////////////////////////
}

