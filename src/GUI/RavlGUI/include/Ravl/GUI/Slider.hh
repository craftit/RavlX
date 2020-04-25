// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_SLIDER_HEADER
#define RAVLGUI_SLIDER_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Slider.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/99"
//! rcsid="$Id$"
//! example=exSlider.cc
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"

namespace RavlGUIN {

  class SliderC;
  
  //! userlevel=Develop
  //: Slider body
  
  class SliderBodyC
    : public WidgetBodyC
  {
  public:
    SliderBodyC(RealT nvalue, RealT nlower, RealT nupper, 
		RealT nstep_increment = 1,
		RealT npage_increment = 1,
		RealT npage_size = 1);
    //: Constructor for horizontal slider.
    
    SliderBodyC(bool nvert,RealT nvalue, RealT nlower, RealT nupper, 
		RealT nstep_increment = 1,
		RealT npage_increment = 1,
		RealT npage_size = 1);
    //: Constructor for horizontal | vertical slider.
    
    virtual bool Create()
    { return CommonCreate(); }
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget)
    { return CommonCreate(_widget); }
    //: Create with a widget supplied from elsewhere.
    
    bool UpdateValue(RealT val);
    //: Update the slider value.
    
    bool UpdateRange(RealT lower,RealT upper);
    //: Update the slider value.
    
    bool Update(RealT val,RealT lower,RealT upper,RealT inc = 1);
    //: Update the slider value.

    bool GUIUpdateValue(const RealT &val);
    //: Update the slider value.
    
    bool GUIUpdateRange(const RealT &lower,const RealT &upper);
    //: Update the slider value.
    
    bool GUIUpdate(const RealT &val,const RealT &lower,const RealT &upper,const RealT &inc);
    //: Update the slider value.

    bool GUIUpdatePageSize(const RealT &pageSize);
    //: Update the page size

    inline
    RealT Upper() const 
    { return upper; }
    //: Read upper limit
    
    inline
    RealT Lower() const 
    { return lower; }
    //: Read lower limit
    
    void SetValuePosition(GtkPositionType pos)
    { numPos = pos; }
    //: Set number position.
    // Only has effect BEFORE Create() is called.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOSITIONTYPE">here</A>.
    
    void SetDrawValue(bool ndrawValue)
    { drawValue = ndrawValue; }  
    //: Draw the value of the slider ?
    // Only has effect BEFORE Create() is called.
    
    Signal1C<RealT> &SigChanged() { return sigChanged; }
    //: Access changed signal.
    
    Signal0C &SigReleased() { return sigReleased; }
    //: Access released signal.
    
    RealT GUIValue() const;
    //: Get the current value of the slider.
    
  protected:
    bool CommonCreate(GtkWidget *_widget = NULL);
    //: Common widget creation
    
    bool setConfig;
    
    RealT value;
    RealT lower;  
    RealT upper;  
    RealT step_increment;
    RealT page_increment;  
    RealT page_size;
    GtkObject *adj;
    IntT digits; // No of digits after decimal point.
    bool updateCont; // Update continously ?
    bool vert;  // Vertical ?
    bool sReq; // Serive request pending.
    GtkPositionType numPos;
    bool drawValue; // Draw the current value ?
    bool setValue;  // Flag if value has been set.
    Signal1C<RealT> sigChanged; // Signal value change.
    Signal0C sigReleased; // Signal value released.
    
    friend class SliderC;
  };
  
  //! userlevel=Normal
  //: Slider

  class SliderC 
    : public WidgetC
  {
  public:
    SliderC()
    {}
    //: Default constructor.
    
    SliderC(RealT nvalue, RealT nlower, RealT nupper, 
	    RealT nstep_increment = 1,RealT npage_increment = 1,
	    RealT npage_size = 1)
      : WidgetC(*new SliderBodyC(nvalue,nlower,nupper, 
				 nstep_increment,npage_increment,
				 npage_size))
    {}
    //: Constructor
    
    SliderC(bool nvert,RealT nvalue, RealT nlower, RealT nupper, 
	    RealT nstep_increment = 1,RealT npage_increment = 1,
	    RealT npage_size = 1)
      : WidgetC(*new SliderBodyC(nvert,nvalue,nlower,nupper, 
				 nstep_increment,npage_increment,
				 npage_size))
    {}
    //: Constructor
    
    SliderC(SliderBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body Constructor.
    
    SliderC(const WidgetC &base)
      : WidgetC(dynamic_cast<const SliderBodyC *>(BodyPtr(base)))
    {}
    //: Create from base class.
    // Creates an invalid handle if class types don't match.
    
  protected:
    SliderBodyC &Body() 
    { return static_cast<SliderBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const SliderBodyC &Body() const 
    { return static_cast<const SliderBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool GUIUpdateValue(RealT val)
    { return Body().GUIUpdateValue(val); }
    //: Update the slider value.
    
    bool GUIUpdateRange(RealT lower,RealT upper)
    { return Body().GUIUpdateRange(lower,upper); }
    //: Update the slider value.
    
    bool GUIUpdate(RealT val,RealT lower,RealT upper,RealT inc)
    { return Body().GUIUpdate(val,lower,upper,inc); }
    //: Update the slider value.

    bool GUIUpdatePageSize(RealT pageSize)
    { return Body().GUIUpdatePageSize(pageSize); }
    //: Update the page size

    void SetValuePosition(GtkPositionType pos)
    { Body().SetValuePosition(pos); }
    //: Set number position.
    // Only has effect BEFORE Create() is called.
    // Values for pos are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOSITIONTYPE">here</A>.
    
    void SetDrawValue(bool ndrawValue)
    { Body().SetDrawValue(ndrawValue); }
    //: Set draw value.
    
    bool UpdateValue(RealT val)
    { return Body().UpdateValue(val); }
    //: Update the slider value.
    
    bool UpdateRange(RealT lower,RealT upper)
    { return Body().UpdateRange(lower,upper); }
    //: Update the slider value.
    
    bool Update(RealT val,RealT lower,RealT upper,RealT inc = 1)
    { return Body().Update(val,lower,upper,inc); }
    //: Update the slider value.
    
    RealT Upper() const 
    { return Body().Upper(); }
    //: Read upper limit
    
    RealT Lower() const 
    { return Body().Lower(); }
    //: Read lower limit
    
    RealT GUIValue() const
    { return Body().GUIValue(); }
    //: Get the current value of the slider.
    
    Signal1C<RealT> &SigChanged() 
    { return Body().SigChanged(); }
    //: Access changed signal.
    
    Signal0C &SigReleased() 
    { return Body().SigReleased(); }
    //: Access released signal.
    
    friend class SliderBodyC;
  };
  
  template<class DataT>
  SliderC SliderV(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  const DataT &obj,bool (DataT::*func)(RealT &value))
  {
    SliderC ret(true,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),obj,func,nvalue);
    return ret;
  }
  //: Vertical slider with call back to class.

  template<class DataT,class ParmT>
  SliderC SliderV(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  bool (*func)(RealT &value,ParmT &par),const ParmT &pt = ParmT())
  {  
    SliderC ret(true,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),func,nvalue,pt);
    return ret;
  }
  //: Vertical slider with call back to function.

  inline SliderC SliderH(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  bool (*func)(RealT &value))
  {
    SliderC ret(false,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),func,nvalue);
    return ret;
  }
  //: Horizontal slider with call back to function.

  template<class DataT>
  SliderC SliderH(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  const DataT &obj,bool (DataT::*func)(RealT &value))
  {
    SliderC ret(false,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),obj,func,nvalue);
    return ret;
  }
  //: Horizontal slider with call back to class.

  template<class DataT,class ParmT>
  SliderC SliderH(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  bool (*func)(RealT &value,ParmT &par),const ParmT &pt = ParmT())
  {  
    SliderC ret(false,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),func,nvalue,pt);
    return ret;
  }
  //: Horizontal slider with call back to function.

  template<class DataT>
  SliderC SliderV(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  bool (*func)(RealT &value,DataT &dat),
		  const DataT &data = DataT())
  {  
    SliderC ret(true,nvalue,nlower,nupper,nstep_increment);
    RealT tmp = 0;
    Connect(ret.SigChanged(),func,tmp,data);
    return ret;
  }
  //: Veritical slider with call back to function.
  
  template<class DataT>
  SliderC SliderH(RealT nvalue, 
		  RealT nlower, 
		  RealT nupper, 
		  RealT nstep_increment,
		  bool (*func)(RealT &value,DataT &dat),
		  const DataT &data = DataT())
  {  
    SliderC ret(false,nvalue,nlower,nupper,nstep_increment);
    RealT tmp = 0;
    Connect(ret.SigChanged(),func,tmp,data);
    return ret;
  }
  //: Horizontal slider with call back to function.
  
  template<class DataT>
  SliderC SliderVR(RealT nvalue, 
		   RealT nlower, 
		   RealT nupper, 
		   RealT nstep_increment,
		   DataT &obj,bool (DataT::*func)(RealT &value),RealT defVal = 0)
  {
    SliderC ret(true,nvalue,nlower,nupper,nstep_increment);
    ConnectRef(ret.SigChanged(),obj,func,defVal);
    return ret;
  }
  //: Vertical slider with reference call back to class.

  template<class DataT>
  SliderC SliderHR(RealT nvalue, 
		   RealT nlower, 
		   RealT nupper, 
		   RealT nstep_increment,
		   DataT &obj,bool (DataT::*func)(RealT &value),RealT defVal = 0)
  {
    SliderC ret(false,nvalue,nlower,nupper,nstep_increment);
    ConnectRef(ret.SigChanged(),obj,func,defVal);
    return ret;
  }
  //: Horizontal slider with reference call back to class.
  
}

#endif
