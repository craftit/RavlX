// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! docentry="Graphics.GTK"
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/VLabel.cc"

#include "Ravl/GUI/VLabel.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

namespace RavlGUIN {

  //: Constructor.
  
  VLabelBodyC::VLabelBodyC(const StringC &ntext)
    : sx(-1),sy(-1),
      text(ntext)
  {}

  StringC VLabelBodyC::Name() const { 
    return text; 
  }

  static const gint vcharsep = 3;

  static gint win_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data ) {
    VLabelBodyC &vlb = *((VLabelBodyC *) data);
    gdk_draw_rectangle(widget->window,
		       widget->style->bg_gc[0],
		       true,
		       0,0,
		       vlb.TextWidth(),
		       vlb.TextHeight());
    
    int aw = widget->allocation.width;
    int ah = widget->allocation.height;
    char cbuff[2];
    cbuff[1] = 0;
    StringC text = vlb.Name();
    int py = 0;
    
    GdkFont *cfont = gtk_style_get_font(widget->style);
    
    if(ah > vlb.TextHeight())
      py += (ah - vlb.TextHeight())/2;
    for(int i = 0;i < ((int) text.length());i++) {
      cbuff[0] = text[i];
      py += gdk_char_height(cfont,cbuff[0]) + vcharsep;
      //cerr << "Char height :" << gdk_char_height(widget->style->font,cbuff[0]) << "\n";
      int cw = aw - gdk_char_width(cfont,cbuff[0]);
      gdk_draw_string (widget->window,
		       cfont,
		       widget->style->fg_gc[0],
		       (cw/2),py,
		       cbuff);
      
    }
    return 1;
  }
  
  static gint win_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data ) {
    VLabelBodyC &vlb = *((VLabelBodyC *) data);
    vlb.SizeUpText();
    return 1;
  }
  
  //: Create the widget.
  
  bool VLabelBodyC::Create() {
    if(widget != 0)
      return true;
    widget = gtk_drawing_area_new ();
    //gtk_label_new (text);
    
    gtk_signal_connect (GTK_OBJECT (widget), "expose_event",
			(GtkSignalFunc) win_expose_event,(gpointer) this);
    
    gtk_signal_connect (GTK_OBJECT(widget),"configure_event",
			(GtkSignalFunc) win_configure_event,(gpointer) this);
    
    ConnectSignals();
    gtk_widget_show (widget);
    SizeUpText();
    return true;
  }
  
  
  //: Sort out the text size.
  
  void VLabelBodyC::SizeUpText() {
    if(widget == 0)
      return ;
    GtkStyle* style = widget->style;
    if(style == 0) {
      std::cerr << "VLabelBodyC::SizeUpText(), ERROR: No style found.  \n";
      return;
    }
    GdkFont *font = gtk_style_get_font(widget->style);
    if(font == 0) {
      std::cerr << "VLabelBodyC::SizeUpText(), ERROR: No font found.  \n";
      return;
    }
    int nsx = 0; // Width
    int nsy = 1; // Height.
    for(int i = 0;i < ((int) text.length());i++) {
      nsy += gdk_char_height(font,text[i]) + vcharsep;
      int w = gdk_char_width(font,text[i]);
      //cerr << "Width of '" << text[i] << "' " << gdk_char_width(font,text[i]) << " Height: " << gdk_char_height(font,text[i]) << " \n";
      if(w > nsx)
	nsx = w;
    }
    nsx++;
    //cerr << "Size " << nsx << "," << nsy << " was (" << sx << "," << sy << ") \n";
    if(nsx != sx || nsy != sy) {
      gtk_drawing_area_size(GTK_DRAWING_AREA(widget),nsx,nsy);
      sx = nsx;
      sy = nsy;
    }
  }

  bool VLabelBodyC::GUISetText(StringC &txt) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    text = txt; 
    return true;
  }
  
  //: Update the text in the label.
  
  bool VLabelBodyC::Label(const StringC &txt) {
    Manager.Queue(Trigger(VLabelC(*this),&VLabelC::GUISetText,txt));
    return true;
  }
}
