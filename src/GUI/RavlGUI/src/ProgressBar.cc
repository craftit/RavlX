// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ProgressBar.cc"

#include "Ravl/GUI/ProgressBar.hh"
#include "Ravl/GUI/Manager.hh"

#include <gtk/gtkprogressbar.h>

namespace RavlGUIN {

  //: Create the widget.
  
  bool ProgressBarBodyC::Create() {
    if(widget != 0)
      return true;
    widget = gtk_progress_bar_new();
    
    switch(type) 
      {
      case GUIPROGBAR_CONTINUOUS_WITH_TEXT:
	gtk_progress_set_show_text (GTK_PROGRESS(widget),true);
	/* no break */
      case GUIPROGBAR_CONTINUOUS:
	gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR(widget),GTK_PROGRESS_CONTINUOUS);
	break;
      case GUIPROGBAR_DISCRETE:
	gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR(widget),GTK_PROGRESS_DISCRETE);
	gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(widget),blocks);
	break;
      case GUIPROGBAR_ACTIVITY:
	gtk_progress_set_activity_mode (GTK_PROGRESS(widget),true);
	gtk_progress_bar_set_activity_blocks(GTK_PROGRESS_BAR(widget),blocks);
	break;
      default:
	std::cerr << "ProgressBarBodyC::Create(), WARNING: Unknow style : " << ((int) type) << "\n";
	break;
      }
    
    GtkProgressBarOrientation orientmap[] = {
      GTK_PROGRESS_LEFT_TO_RIGHT,
      GTK_PROGRESS_RIGHT_TO_LEFT,
      GTK_PROGRESS_BOTTOM_TO_TOP,
      GTK_PROGRESS_TOP_TO_BOTTOM
    };
    if(((int) orient) < 0 && ((int) orient) > 3)
      std::cerr << "ProgressBarBodyC::Create(), WARNING: Unknow orientation : " << ((int) orient) << "\n";
    else
      gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(widget),orientmap[(int) orient]);
    ConnectSignals();
    return true;
  }



  bool ProgressBarBodyC::Create(GtkWidget *_widget)
  {
    RavlAssert(_widget != NULL);
    widget = _widget;
    ConnectSignals();
    return true;
  }
  
  //: Update percentage done on bar.
  
  void ProgressBarBodyC::Update(float percentage) {
    Manager.Queue(Trigger(ProgressBarC(*this),&ProgressBarC::GUIUpdate,percentage));
  }
  
  
  //: Update percentage done on bar.
  
  bool ProgressBarBodyC::GUIUpdate(const float &percentage) {
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_progress_bar_update(GTK_PROGRESS_BAR(widget),percentage);
    return true;
  }

  //: Set type of progress bar.
  
  bool ProgressBarBodyC::GUISetType(ProgBarTypeT ntype) {
    type = ntype;
    if(widget == 0)
      return true;    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    switch(type) 
      {
      case GUIPROGBAR_CONTINUOUS_WITH_TEXT:
	gtk_progress_set_show_text (GTK_PROGRESS(widget),true);
	/* no break */
      case GUIPROGBAR_CONTINUOUS:
	gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR(widget),GTK_PROGRESS_CONTINUOUS);
	break;
      case GUIPROGBAR_DISCRETE:
	gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR(widget),GTK_PROGRESS_DISCRETE);
	gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(widget),blocks);
	break;
      case GUIPROGBAR_ACTIVITY:
	gtk_progress_set_activity_mode (GTK_PROGRESS(widget),true);
	gtk_progress_bar_set_activity_blocks(GTK_PROGRESS_BAR(widget),blocks);
	break;
      default:
	std::cerr << "ProgressBarBodyC::GUISetStyle(), WARNING: Unknow style : " << ((int) type) << "\n";
	break;
      }
    return true;
  }
  
  //: Set type of progress bar.
  
  bool ProgressBarBodyC::SetType(ProgBarTypeT ntype) {
    Manager.Queue(Trigger(ProgressBarC(*this),&ProgressBarC::GUISetType,ntype));
    return true;
  }
  
}
