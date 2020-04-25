// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/SpinButton.cc"

#include "Ravl/GUI/SpinButton.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"
#include <gtk/gtk.h>

#define DPDEBUG 0

#if DPDEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  extern "C" {
    void spinbutton_value_changed( GtkAdjustment *adj,SpinButtonBodyC *data )
    { data->SigChanged()(adj->value); }
  }
  
  SpinButtonBodyC::SpinButtonBodyC(IntT ndigits,
				   RealT nclimbRate,
				   RealT nvalue,
				   RealT nlower,RealT nupper,RealT nstep_increment)
    :  digits(ndigits),
       climbRate(nclimbRate),
       value(nvalue),
       lower(nlower),  
       upper(nupper),  
       step_increment(nstep_increment),
       adj(0),
       sigChanged(true)
  {}
  

  //: Get current value.
  
  RealT SpinButtonBodyC::Value() const { 
    if(adj == 0) return value; 
    ReadBackLockC lock;
    return gtk_adjustment_get_value(GTK_ADJUSTMENT (adj));
  }

  //: Create with a widget supplied from elsewhere.
  
  bool SpinButtonBodyC::Create(GtkWidget *nwidget) {
    widget = nwidget;
    
    adj = GTK_OBJECT (gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(widget)));
    
    gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
			GTK_SIGNAL_FUNC (spinbutton_value_changed), this);
    
    ConnectSignals();
    return true;
  }
  
  //: Create widget.
  
  bool SpinButtonBodyC::Create() {
    // Setup adjustment.
    
    adj = gtk_adjustment_new (value, 
			      lower, 
			      upper,
			      step_increment,
			      1,
			      0);
    
    widget = gtk_spin_button_new(GTK_ADJUSTMENT (adj),
				 climbRate,
				 digits);
    
    gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
			GTK_SIGNAL_FUNC (spinbutton_value_changed), this);
    
    ConnectSignals();
    return true;
  }
  
  //: Undo all references.
  
  void SpinButtonBodyC::Destroy() {
    sigChanged.DisconnectAll();
    WidgetBodyC::Destroy();
  }
  
  
  //: Set value 
  // Thread safe.
  
  void SpinButtonBodyC::SetValue(RealT val) {
    Manager.Queue(Trigger(SpinButtonC(*this),&SpinButtonC::GUISetValue,val));
  }
  
  //: Set value 
  // GUI thread only.
  
  bool SpinButtonBodyC::GUISetValue(RealT val) {
    if(widget == 0) {
      value = val;
      return true;
    }
    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget),val);
    //GTK_ADJUSTMENT (adj)->value = val;
    //gtk_signal_emit_by_name (GTK_OBJECT (adj), "changed");  
    return true;
  }

  //: Set range of spin button
  // If current value is outside range, it is clipped to be inside.
  
  void SpinButtonBodyC::SetRange(RealT nlower, RealT nupper) {
    Manager.Queue(Trigger(SpinButtonC(*this),&SpinButtonC::GUISetRange,nlower,nupper));    
  }

  //: Set range of spin button
  // GUI thread only
  // If current value is outside range, it is clipped to be inside.

  bool SpinButtonBodyC::GUISetRange(RealT nlower, RealT nupper) {

    // Store
    lower = nlower;
    upper = nupper;      

    if(adj == 0) 
      return true;
    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    // Clip new value
    bool bValueChanged(false);
    RealT val = Value();
    if (val > nupper) {
      val = nupper;
      bValueChanged = true;
    }
    else if (val < nlower) {
      val = nlower;
      bValueChanged = true;
    }

    // Modify adjustment
    GTK_ADJUSTMENT(adj)->lower = lower;
    GTK_ADJUSTMENT(adj)->upper = upper;
    gtk_adjustment_changed(GTK_ADJUSTMENT(adj));
    gtk_spin_button_update(GTK_SPIN_BUTTON(widget));

    // Update value
    if (bValueChanged)
      GUISetValue(val);

    // Done
    return true;
  }

}
  
