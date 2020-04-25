// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid=$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/RadioButton.cc"

#include "Ravl/GUI/RadioButton.hh"
#include "Ravl/CDLIter.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  //: Create a button.
  
  RadioButtonC::RadioButtonC(const char *label,const char *tooltip,const RadioButtonGroupT &agroup)
    : ToggleButtonC(*new RadioButtonBodyC(label,tooltip,agroup))
{}

  //: Create a button.
  // If label is set to 0 none will be used.
  
  RadioButtonC::RadioButtonC(const PixmapC &pixm,const char *label,const RadioButtonGroupT &agroup)
    : ToggleButtonC(*new RadioButtonBodyC(label,pixm,agroup))
  {}
  
  /////// RadioButtonBodyC ////////////////////////////////////////////////
  
  //: Constructor.
  
  RadioButtonBodyC::RadioButtonBodyC(const char *nlabel,const char *tooltip,const RadioButtonGroupT &agroup)
    : ToggleButtonBodyC(nlabel),
      group(agroup),
      gtkgroup(0)
  {
    if(tooltip != 0)
      SetToolTip(tooltip);
    group.InsLast(this);
  }
  
  //: Constructor.
  
  RadioButtonBodyC:: RadioButtonBodyC(const char *nlabel,const PixmapC &pixm,const RadioButtonGroupT &agroup)
    : ToggleButtonBodyC(nlabel,pixm),
      group(agroup),
      gtkgroup(0)
  {
    group.InsLast(this);
  }
  
  //: Destructor.
  
  RadioButtonBodyC::~RadioButtonBodyC() {
  // Remove it from the group list.
    for(DLIterC<RadioButtonBodyC *> it(group);it;it++) {
      if(*it == this) {
	it.Del();
	break;
      }
    }
  }
  
  //: Create the widget.
  
  bool RadioButtonBodyC::Create() {
    return ToggleButtonBodyC::Create();
  }
  
  //: Create the actual widget.
  // This allows different types of buttons to
  // be created easily.
  
  GtkWidget *RadioButtonBodyC::BuildWidget(const char *lab) {
    ONDEBUG(std::cerr << "RadioButtonBodyC::BuildWidget() Start : " << ((void *) gtkgroup) << "\n");
    GSList *last = 0;
    for(DLIterC<RadioButtonBodyC *> it(group);it;it++) {
      if(*it == this)
	break;
      if((*it)->GtkGroup() == 0)
	(*it)->Create();
      last = (*it)->GtkGroup();
    }
    GtkWidget *ret;
    if(lab == 0)
      ret = gtk_radio_button_new (last);
    else
      ret = gtk_radio_button_new_with_label (last,lab);
    //if(gtkgroup == 0)
    //    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), true);
    gtkgroup = gtk_radio_button_group (GTK_RADIO_BUTTON (ret));
    ONDEBUG(std::cerr << "RadioButtonBodyC::BuildWidget() End : " << ((void *) gtkgroup) << "\n");
    return ret;
  }
}
