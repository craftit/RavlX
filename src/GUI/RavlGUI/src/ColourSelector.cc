// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! docentry="Graphics.GTK"
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ColourSelector.cc"

#include "Ravl/GUI/ColourSelector.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  /* Color changed handler */
  
  static void color_changed_cb( GtkWidget         *widget,
				ColourSelectorBodyC *colorsel )
  { colorsel->HandleSelect(); }
  
  static void cancel (GtkWidget *widget, ColourSelectorBodyC *fs) {    
    fs->HandleCancel(); // We're in the GUI thread.
  } 
  
  //: Constructor.
  
  ColourSelectorBodyC::ColourSelectorBodyC(const StringC &nname,const UInt16RGBValueC &_colour)
    : name(nname),
      colour(_colour),
      hideOnSelect(true),
      selected(_colour)
  {}
  
  
  bool ColourSelectorBodyC::Create() {
    
    widget = gtk_color_selection_dialog_new (name);
    
    // Connect default signal, to handle widget destructon correctly
    ConnectSignals(); 
    
    GtkColorSelection *colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (widget)->colorsel);
    
    GdkColor ncolor;
    ncolor.red = colour.Red();
    ncolor.green = colour.Green();
    ncolor.blue = colour.Blue();
    
    gtk_color_selection_set_previous_color (colorsel, &ncolor);
    gtk_color_selection_set_current_color (colorsel, &ncolor);
    
    //g_signal_connect (G_OBJECT (colorsel), "color_changed",
    //G_CALLBACK (color_changed_cb), (gpointer) this);
    
    /* Connect the cancel button to the destroyed function */
    
    gtk_signal_connect (GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG (widget)->cancel_button),
			"clicked", (GtkSignalFunc) cancel, this );
    
    gtk_signal_connect (GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG (widget)->ok_button),
			"clicked", (GtkSignalFunc) color_changed_cb, this );
    
    ConnectSignals();
    
    return true;
  }

  //: Handle selection cancel.
  void ColourSelectorBodyC::HandleCancel() {
    GUIHide();
  }

  //: Handle selection 
  
  void ColourSelectorBodyC::HandleSelect() {
    GdkColor ncolor;
    GtkColorSelection *colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (widget)->colorsel);
    gtk_color_selection_get_current_color (colorsel, &ncolor);
    selected(UInt16RGBValueC(ncolor.red,ncolor.green,ncolor.blue));
    GUIHide();
  }
  
  
  //: Dissconnect all signals.
  
  void ColourSelectorBodyC::Destroy() {
    selected.DisconnectAll();
    WidgetBodyC::Destroy();
  }


  //: Set the title of the window.
  // GUI thread only.
  
  bool ColourSelectorBodyC::GUISetTitle(const StringC &nname) {
    //cerr << "ColourSelectorBodyC::GUISetTitle(), Called. \n";
    name = nname;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_window_set_title(GTK_WINDOW(widget),nname);    
    return true;
  }
  
  //: Set the title of the window.
  
  bool ColourSelectorBodyC::SetTitle(const StringC &nname) {
    Manager.Queue(Trigger(ColourSelectorC(*this),&ColourSelectorC::GUISetTitle,nname));
    return true;
  }
  
}

