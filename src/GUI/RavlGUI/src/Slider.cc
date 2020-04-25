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
//! file="Ravl/GUI/GTK/Slider.cc"

#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

#define DPDEBUG 0

#if DPDEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  void value_changed( GtkAdjustment *adj,SliderBodyC *data ) {
    data->SigChanged()(adj->value); 
  }
  
  int value_released(GtkWidget *widget,GdkEvent *event,SliderBodyC *data ) {
    data->SigReleased()(); 
    return 0;
  }
  
  //: Constructor.
  
  SliderBodyC::SliderBodyC(RealT nvalue, 
			   RealT nlower, 
			   RealT nupper, 
			   RealT nstep_increment,
			   RealT npage_increment,
			   RealT npage_size)
    : setConfig(false),
      value(nvalue),
      lower(nlower),
      upper(nupper), 
      step_increment(nstep_increment),
      page_increment(npage_increment),
      page_size(npage_size),
      digits(0),
      updateCont(true),
      vert(false),
      sReq(false),
      numPos(GTK_POS_TOP),
      drawValue(true),
      setValue(false),
      sigChanged(true),
      sigReleased(true)
  {
    ONDEBUG(std::cerr << "Horiz Constructor. \n");
  }
  
  SliderBodyC::SliderBodyC(bool nvert,RealT nvalue, RealT nlower, 
			       RealT nupper, RealT nstep_increment,
			   RealT npage_increment,RealT npage_size)
    
    : value(nvalue),
      lower(nlower),
      upper(nupper), 
      step_increment(nstep_increment),
      page_increment(npage_increment),
      page_size(npage_size),
      digits(0),
      updateCont(true),
      vert(nvert),
      sReq(false),
      numPos(GTK_POS_TOP),
      drawValue(true),
      setValue(false),
      sigChanged(true),
      sigReleased(true)
  {
    if(!vert)
      numPos = GTK_POS_TOP;
    else
      numPos = GTK_POS_LEFT;
  }
  //: Constructor.
  
  
  //: Create the widget.

  bool SliderBodyC::CommonCreate(GtkWidget *_widget)
  {
    if (_widget == NULL)
    {
      ONDEBUG(std::cerr << "Slider create: Low:" << lower << " High:" << upper << " Inc:" << step_increment << "\n");
      adj = gtk_adjustment_new(value, 
                               lower, 
                               upper, 
                               step_increment,
                               page_increment,
                               page_size);
      if(vert) 
        widget = gtk_vscale_new(GTK_ADJUSTMENT (adj));
      else
        widget = gtk_hscale_new(GTK_ADJUSTMENT (adj));
      
      if(updateCont) {
        gtk_range_set_update_policy (GTK_RANGE (widget),
             GTK_UPDATE_CONTINUOUS);
      } else {
        gtk_range_set_update_policy (GTK_RANGE (widget),
             GTK_UPDATE_DISCONTINUOUS);    
      }
      // GTK_UPDATE_DELAYED
      
      gtk_scale_set_digits (GTK_SCALE (widget), digits);
      gtk_scale_set_value_pos (GTK_SCALE (widget), numPos);
      gtk_scale_set_draw_value (GTK_SCALE(widget), drawValue);
    }
    else
    {
      ONDEBUG(std::cerr << "Slider create from glade");
      widget = _widget;
      adj = GTK_OBJECT(gtk_range_get_adjustment(GTK_RANGE(widget)));
    }
    
    gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
			GTK_SIGNAL_FUNC (value_changed), this);
    gtk_signal_connect (GTK_OBJECT (widget), "button_release_event",
			GTK_SIGNAL_FUNC (value_released), this);
    gtk_signal_connect (GTK_OBJECT (widget), "key_release_event",
			GTK_SIGNAL_FUNC (value_released), this);
    
    return true;
  }
  
  //: Update the slider value.
  
  bool SliderBodyC::GUIUpdateValue(const RealT &val) {
    bool updateVal = val != value;
    value = val;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    GTK_ADJUSTMENT (adj)->value = val;
    ONDEBUG(std::cerr << "Slider setting: Value:" << value << " (L: " << GTK_ADJUSTMENT (adj)->lower << " U:" << GTK_ADJUSTMENT (adj)->upper << ") \n");
    gtk_signal_emit_by_name (GTK_OBJECT (adj), "changed");
    
    // FIX for slider value update bug.
    // A bit of a sledge hammer, but this ensures the number displayed above the slider is
    // correct.
    if(updateVal)
      gtk_widget_queue_draw (widget);
    return true;
  }
  
  //: Update the slider value.
  bool SliderBodyC::GUIUpdateRange(const RealT &nlower,const RealT &nupper) {
    lower = nlower;
    upper = nupper;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GTK_ADJUSTMENT (adj)->lower = lower;
    GTK_ADJUSTMENT (adj)->upper = (RealT) upper + step_increment;
    // Check range.
    if(GTK_ADJUSTMENT (adj)->lower > GTK_ADJUSTMENT (adj)->value) {
      GTK_ADJUSTMENT (adj)->value = GTK_ADJUSTMENT (adj)->lower;
    }
    if(GTK_ADJUSTMENT (adj)->upper < GTK_ADJUSTMENT (adj)->value) {
      GTK_ADJUSTMENT (adj)->value = GTK_ADJUSTMENT (adj)->upper;
    }
    
    gtk_signal_emit_by_name (GTK_OBJECT (adj), "changed");
    return true;
  }
  
  //: Update the slider value.
  
  bool SliderBodyC::GUIUpdate(const RealT &val,const RealT &nlower,const RealT &nupper,const RealT &inc) {
    lower = nlower;
    upper = nupper;
    step_increment = inc;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GTK_ADJUSTMENT (adj)->value = val;
    GTK_ADJUSTMENT (adj)->lower = lower;
    GTK_ADJUSTMENT (adj)->upper = (RealT) upper + step_increment;
    GTK_ADJUSTMENT (adj)->step_increment = step_increment;
    gtk_signal_emit_by_name (GTK_OBJECT (adj), "changed");
    return true;
  }

  //: Update the page size
  bool SliderBodyC::GUIUpdatePageSize(const RealT &pageSize)
  {
    page_size = pageSize;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_adjustment_set_page_size(GTK_ADJUSTMENT (adj),pageSize);
    //gtk_signal_emit_by_name (GTK_OBJECT (adj), "changed");
    return true;
  }

  //: Get the current value of the slider.
  
  RealT SliderBodyC::GUIValue() const {
    if(widget == 0)
      return value;
    RavlAssert(adj != 0);
    return GTK_ADJUSTMENT (adj)->value;
  }
  
  bool SliderBodyC::UpdateRange(RealT nlower,RealT nupper) {
    Manager.Queue(Trigger(SliderC(*this),&SliderC::GUIUpdateRange,nlower,nupper));
    return true;
  }
  
  //: Update the slider value.
  
  bool SliderBodyC::UpdateValue(RealT val) {
    Manager.Queue(Trigger(SliderC(*this),&SliderC::GUIUpdateValue,val));
    return true;
  }

  //: Update the slider value.
  
  bool SliderBodyC::Update(RealT val,RealT nlower,RealT nupper,RealT inc) {
    Manager.Queue(Trigger(SliderC(*this),&SliderC::GUIUpdate,val,nlower,nupper,inc));
    return true;  
  }

}

