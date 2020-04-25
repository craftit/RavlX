// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Table.cc"

#include "Ravl/GUI/Table.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  //: Add object to table.
  
  bool TableBodyC::AddObject(const WidgetC &widge,
			     UIntT left_attach,UIntT right_attach,
			     UIntT top_attach,UIntT bottom_attach,
			     GtkAttachOptions nxoptions,GtkAttachOptions nyoptions,
			     UIntT nxpadding,UIntT nypadding)
  {
    WidgeInfoC wi(widge,left_attach,right_attach,top_attach,bottom_attach,
		  nxoptions,nyoptions,
		  nxpadding,nypadding);
    Manager.Queue(Trigger(TableC(*this),&TableC::GUIAddObject,wi));
    return true;
 }

  //: Add object to table.
  // Values for attach options are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKATTACHOPTIONS">here</A>.
  // Call on the GUI thread only.
  
  bool TableBodyC::GUIAddObject(WidgeInfoC &wi) {
    children.InsLast(wi);
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(wi.widge.Widget() == 0) {
      if(!wi.widge.Create()) {
	cerr << "TableBodyC::Create(), Widget create failed. \n";
	return false;
      }
    }
    gtk_table_attach(GTK_TABLE(widget),
		     wi.widge.Widget(),
		     wi.left_attach,wi.right_attach,
		     wi.top_attach,wi.bottom_attach,
		     wi.xoptions,wi.yoptions,
		     wi.xpadding,wi.ypadding);
    gtk_widget_show (wi.widge.Widget());
    return true;
  }


  //: Create widget.
  
  bool TableBodyC::Create() {
    RavlAssert(widget == 0);
    widget = gtk_table_new(sx,sy,homogeneous);
    for(DLIterC<WidgeInfoC> it(children);it;it++) {
      WidgeInfoC &child = *it;
      if(child.widge.Widget() == 0) {
	if(!child.widge.Create()) {
	  std::cerr << "TableBodyC::Create(), Widget create failed. \n";
	  return false;
	}
      }
      //cerr << "TableBodyC::Create(), Making entry " << it.Data().widge.WidgetName()<< "\n";
      gtk_table_attach(GTK_TABLE(widget),
		       child.widge.Widget(),
		       child.left_attach,child.right_attach,
		       child.top_attach,child.bottom_attach,
		       child.xoptions,child.yoptions,
		       child.xpadding,child.ypadding);
      gtk_widget_show (child.widge.Widget());
    }
    //children.Empty(); // Might as well free the memory.
    ConnectSignals();
    return true;
  }
  
  bool TableBodyC::Create(GtkWidget *_widget) {
    widget = _widget;
    for(DLIterC<WidgeInfoC> it(children);it;it++) {
      WidgeInfoC &child = *it;
      if(child.widge.Widget() == 0) {
	if(!child.widge.Create()) {
	  std::cerr << "TableBodyC::Create(), Widget create failed. \n";
	  return false;
	}
      }
      //cerr << "TableBodyC::Create(), Making entry " << it.Data().widge.WidgetName()<< "\n";
      gtk_table_attach(GTK_TABLE(widget),
		       child.widge.Widget(),
		       child.left_attach,child.right_attach,
		       child.top_attach,child.bottom_attach,
		       child.xoptions,child.yoptions,
		       child.xpadding,child.ypadding);
      gtk_widget_show (child.widge.Widget());
    }
    //children.Empty(); // Might as well free the memory.
    ConnectSignals();
    return true;
  }
  
  //: Undo all references.
  
  void TableBodyC::Destroy() {
    for(DLIterC<WidgeInfoC> it(children);it.IsElm();it.Next()) 
      it.Data().widge.Destroy();
    WidgetBodyC::Destroy();
  }



  
}
