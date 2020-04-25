// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_SPINBUTTON_HEADER
#define RAVLGUI_SPINBUTTON_HEADER 1
//////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/SpinButton.hh"
//! lib=RavlGUI
//! date="29/08/1999"
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/Threads/Signal1.hh"

namespace RavlGUIN {
  class SpinButtonC;
  
  //! userlevel=Develop
  //: Spin button body.
  
  class SpinButtonBodyC
    : public WidgetBodyC
  {
  public:
    SpinButtonBodyC(IntT digits,RealT nclimbRate,RealT nvalue,RealT nlower,RealT nupper,RealT nstep_increment);
    //: Default constructor.
    
    Signal1C<RealT> &SigChanged() { return sigChanged; }
    //: Access changed signal.
    
    void SetValue(RealT val);
    //: Set value 
    // Thread safe.
    
    bool GUISetValue(RealT val);
    //: Set value 
    // GUI thread only.
    
    RealT Value() const;
    //: Get current value.
    
    void SetRange(RealT nlower, RealT nupper);
    //: Set range of spin button
    // If current value is outside range, it is clipped to be inside.

    bool GUISetRange(RealT nlower, RealT nupper);
    //: Set range of spin button
    // GUI thread only
    // If current value is outside range, it is clipped to be inside.

  protected:
    virtual bool Create();
    //: Create widget.

    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    virtual void Destroy();
    //: Undo all references.
    
    IntT  digits;
    RealT climbRate;
    RealT value;
    RealT lower;  
    RealT upper;  
    RealT step_increment;
    
    GtkObject *adj;
    
    Signal1C<RealT> sigChanged; // Signal value change.
    
    friend class SpinButtonC;
  };
  
  //! userlevel=Normal
  //: Spin button<br>
  // <img src="../../../html/spinbutton.png">
  
  class SpinButtonC
    : public WidgetC
  {
  public:
    SpinButtonC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SpinButtonC(IntT digits,RealT climbRate = 1,RealT value = 0,RealT lower = 0,RealT upper = 100,RealT step_increment = 1)
      : WidgetC(*new SpinButtonBodyC(digits,climbRate,value,lower,upper,step_increment))
    {}
    //: Constructor.
    //!param: digits - no. of digits displayed
    //!param: climbRate - the acceleration rate when you hold down a button
    //!param: value - initial value
    //!param: lower - lower bound
    //!param: upper - upper bound
    //!param: step_increment - step increment
    
    SpinButtonC(const WidgetC &base)
      : WidgetC(dynamic_cast<const SpinButtonBodyC *>(BodyPtr(base)))
    {}
    //: Create from base class.
    // Creates an invalid handle if types don't match.
    
  protected:
    SpinButtonC(SpinButtonBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    SpinButtonBodyC &Body()
    { return static_cast<SpinButtonBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const SpinButtonBodyC &Body() const
    { return static_cast<const SpinButtonBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    Signal1C<RealT> &SigChanged() 
    { return Body().SigChanged(); }
    //: Access changed signal.
    
    void SetValue(RealT val)
    { Body().SetValue(val); }
    //: Set value 
    // Thread safe.
    
    bool GUISetValue(RealT val)
    { return Body().GUISetValue(val); }
    //: Set value 
    // GUI thread only.

    RealT Value() const
    { return Body().Value(); }
    //: Get current value.
    
    void SetRange(RealT nlower, RealT nupper)
    { Body().SetRange(nlower,nupper); }
    //: Set range of spin button
    // If current value is outside range, it is clipped to be inside.

    bool GUISetRange(RealT nlower, RealT nupper)
    { return Body().GUISetRange(nlower,nupper); }
    //: Set range of spin button
    // GUI thread only
    // If current value is outside range, it is clipped to be inside.

    friend class SpinButtonBodyC;
  };
  

  template<class DataT>
  SpinButtonC SpinButton(RealT nvalue,
			 RealT nlower, 
			 RealT nupper, 
			 RealT nstep_increment,
			 bool (*func)(RealT &value))
  {
    SpinButtonC ret(2,1,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),func,nvalue);
    return ret;
  }
  //: Construct spin button with callback to class method.

  template<class DataT,class ParamT>
  SpinButtonC SpinButton(RealT nvalue,
			 RealT nlower, 
			 RealT nupper, 
			 RealT nstep_increment,
			 bool (*func)(RealT &value,ParamT &parm),const ParamT &parm)
  {
    SpinButtonC ret(2,1,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),func,nvalue,parm);
    return ret;
  }
  //: Construct spin button with callback to class method.
  
  template<class DataT>
  SpinButtonC SpinButton(RealT nvalue,
			 RealT nlower, 
			 RealT nupper, 
			 RealT nstep_increment,
			 const DataT &obj,bool (DataT::*func)(RealT &value))
  {
    SpinButtonC ret(2,1,nvalue,nlower,nupper,nstep_increment);
    Connect(ret.SigChanged(),obj,func,nvalue);
    return ret;
  }
  //: Construct spin button with callback to class method.
  
  template<class DataT>
  SpinButtonC SpinButtonR(RealT nvalue,
			  RealT nlower, 
			  RealT nupper, 
			  RealT nstep_increment,
			  DataT &obj,bool (DataT::*func)(RealT &value),RealT defVal = 0)
  {
    SpinButtonC ret(2,1,nvalue,nlower,nupper,nstep_increment);
    ConnectRef(ret.SigChanged(),obj,func,defVal);
    return ret;
  }
  //: Construct spin button with callback to referenced class method.
  
}
#endif
