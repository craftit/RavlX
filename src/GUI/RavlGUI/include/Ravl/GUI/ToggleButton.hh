// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_TOGGLEBUTTON_HEADER
#define RAVLGUI_TOGGLEBUTTON_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Graphics.GTK.Control"
//! lib=RavlGUI
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/ToggleButton.hh"
//! date="17/06/99"

#include "Ravl/GUI/Button.hh"

namespace RavlGUIN {

  class ToggleButtonC;
  
  //! userlevel=Develop
  //: Toggle Button body.
  
  class ToggleButtonBodyC
    : public ButtonBodyC
  {
  public:
    ToggleButtonBodyC(const char *nlabel = 0,bool nInitState = false);
    //: Constructor.
    
    ToggleButtonBodyC(const char *nlabel,const PixmapC &pixm,bool nInitState = false);
    //: Constructor.
    
    bool GUISetActive(bool x);
    //: Set button active.
    
    bool GUIIsActive() const;
    //: Test if button is active.

    bool IsActive() const;
    //: Test if button is active.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.

    Signal1C<bool> &SigChanged()
    { return sigChanged; }
    //: Access changed signal.
    
    bool GUISetToggle(const bool &val);
    //: Set toggle state.
    // GUI thread only.
  
    void SetToggle(const bool &val);
    //: Set toggle state.
    
    bool GUISetInconsistent(bool val);
    //: Set inconsistant state
    // GUI thread only. 
    
    bool SetInconsistent(const bool &val);
    //: Set inconsistant state
    
  protected:
    bool SignalState();
    // Signal state to clients with 'sigChanged'
    
    virtual void Destroy();
    //: Undo all references.
    
    virtual GtkWidget *BuildWidget(const char *lab = 0);
    //: Create the actual widget.
    // This allows different types of buttons to
    // be created easily.
    
    bool initState; // Inital state.
    bool initInconsistant;
    
    Signal1C<bool> sigChanged;
    
    friend class ToggleButtonC;
  };
  
  //! userlevel=Normal
  //: Toggle Button.
  // <img src="../../../html/toggle-button.png"><br>
  
  class ToggleButtonC
    : public ButtonC
  {
  public:
    ToggleButtonC()
    {}
    //: Default constructor.
    // Creates an invalud handle.
    
    ToggleButtonC(const char *label,bool nInitState = false);
    //: Create a button.
    
    ToggleButtonC(const PixmapC &pixm,const char *label = 0)
      : ButtonC(*new ToggleButtonBodyC(label,pixm))
    {}
    //: Create a button.
    // If label is set to 0 none will be used.
  
    ToggleButtonC(const WidgetC &widge)
      : ButtonC(dynamic_cast<const ToggleButtonBodyC *>(BodyPtr(widge)))
    {}
    //: Create from base class.
    // Creates an invalid handle if types don't match.
    
  protected:
    ToggleButtonC(ButtonBodyC &bod)
      : ButtonC(bod)
    {}
    //: Body constructor.
    
    ToggleButtonC(const ButtonBodyC *bod)
      : ButtonC(bod)
    {}
    //: Body constructor.
    
    ToggleButtonBodyC &Body() 
    { return static_cast<ToggleButtonBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ToggleButtonBodyC &Body() const
    { return static_cast<const ToggleButtonBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool SignalState()
    { return Body().SignalState(); }
    // Signal state to clients with 'sigChanged'
    
  public:
    bool GUIIsActive() const
    { return Body().GUIIsActive(); }
    //: Test if button is active.
    // GUI thread only.
    
    bool IsActive() const
    { return Body().IsActive(); }
    //: Test if button is active.
    
    Signal1C<bool> &SigChanged()
    { return Body().SigChanged(); }
    //: Access changed signal.
    
    bool GUISetToggle(bool val)
    { return Body().GUISetToggle(val); }
    //: Set toggle state.
    // GUI thread only.
    
    void SetToggle(bool val)
    { Body().SetToggle(val); }
    //: Set toggle state.
    
    bool GUISetInconsistent(bool val)
    { return Body().GUISetInconsistent(val); }
    //: Set inconsistant state
    // GUI thread only.
    
    bool SetInconsistent(const bool &val)
    { return Body().SetInconsistent(val); }
    //: Set inconsistant state
    
    friend class ToggleButtonBodyC;
  };
  
  ////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Check Button body.
  
  class CheckButtonBodyC
    : public ToggleButtonBodyC
  {
  public:
    CheckButtonBodyC(const char *nlabel = 0,bool nInitState = false)
      : ToggleButtonBodyC(nlabel,nInitState)
    {}
    //: Constructor.
    
    CheckButtonBodyC(const char *nlabel,const PixmapC &pixm)
      : ToggleButtonBodyC(nlabel,pixm)
    {}
    //: Constructor.
    
  protected:
    virtual GtkWidget *BuildWidget(const char *lab = 0);
    //: Create the actual widget.
    // This allows different types of buttons to
    // be created easily.
  };
  
  //! userlevel=Normal
  //: Check Button.
  // <img src="../../../html/check-button.png"><br>
  
  class CheckButtonC
    : public ToggleButtonC
  {
  public:
    CheckButtonC(const char *label = 0,bool nInitState = false);
    //: Create a button.
    
    CheckButtonC(const PixmapC &pixm,const char *label = 0)
      : ToggleButtonC(*new CheckButtonBodyC(label,pixm))
    {}
    //: Create a button.
    // If label is set to 0 none will be used.
    
    CheckButtonC(const WidgetC &base)
      : ToggleButtonC(dynamic_cast<const CheckButtonBodyC *>(BodyPtr(base)))
    {}
    //: Create for base class.
    // Creates an invalid handle if classes don't match.
    
  protected:  
    CheckButtonC(CheckButtonBodyC &bod)
      : ToggleButtonC(bod)
    {}
    //: Body constructor.
    
  };
  
  inline
  ToggleButtonC ToggleButton(const char *label,bool initState)
  { return  ToggleButtonC(label,initState); }
  //: Construct a toggle button.
  
  template<class ObjT>
  ToggleButtonC ToggleButton(const char *label,bool initState,const ObjT &obj,bool (ObjT::*func)(bool))
  { 
    ToggleButtonC ret = ToggleButtonC(label,initState);
    Connect(ret.SigChanged(),obj,func);
    return ret;    
  }
  
  inline
  CheckButtonC CheckButton(const char *label,bool initState)
  { return CheckButtonC(label,initState); }
  
  template<class ObjT>
  CheckButtonC CheckButton(const char *label,bool initState,const ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    CheckButtonC ret = CheckButtonC(label,initState);
    Connect(ret.SigChanged(),obj,func);
    return ret;    
  }

  template<class ObjT>
  ToggleButtonC ToggleButton(const char *label,const char *tooltip,bool initState,const ObjT &obj,bool (ObjT::*func)(bool))
  { 
    ToggleButtonC ret = ToggleButtonC(label,initState);
    ret.SetToolTip(tooltip);
    Connect(ret.SigChanged(),obj,func);
    return ret;    
  }
  
  template<class ObjT>
  CheckButtonC CheckButton(const char *label,const char *tooltip,bool initState,const ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    CheckButtonC ret = CheckButtonC(label,initState);
    ret.SetToolTip(tooltip);
    Connect(ret.SigChanged(),obj,func);
    return ret;    
  }

  ///// Reference //////////////////////////

  template<class ObjT>
  ToggleButtonC ToggleButtonR(const char *label,bool initState,ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    ToggleButtonC ret = ToggleButtonC(label,initState);
    ConnectRef(ret.SigChanged(),obj,func);
    return ret;    
  }
  
  template<class ObjT,class DataT>
  ToggleButtonC ToggleButtonR(const char *label,bool initState,ObjT &obj,bool (ObjT::*func)(bool &,DataT &dat),const DataT &dat)
  { 
    ToggleButtonC ret = ToggleButtonC(label,initState);
    ConnectRef(ret.SigChanged(),obj,func,initState,dat);
    return ret;    
  }
  
  template<class ObjT>
  CheckButtonC CheckButtonR(const char *label,bool initState,ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    CheckButtonC ret = CheckButtonC(label,initState);
    ConnectRef(ret.SigChanged(),obj,func);
    return ret;    
  }

  template<class ObjT,class DataT>
  CheckButtonC CheckButtonR(const char *label,bool initState,ObjT &obj,bool (ObjT::*func)(bool &,DataT &dat),const DataT &dat)
  { 
    CheckButtonC ret = CheckButtonC(label,initState);
    ConnectRef(ret.SigChanged(),obj,func,initState,dat);
    return ret;    
  }

  template<class ObjT>
  ToggleButtonC ToggleButtonR(const char *label,const char *tooltip,bool initState,ObjT &obj,bool (ObjT::*func)(bool))
  { 
    ToggleButtonC ret = ToggleButtonC(label,initState);
    ret.SetToolTip(tooltip);
    ConnectRef(ret.SigChanged(),obj,func);
    return ret;    
  }
  
  template<class ObjT>
  CheckButtonC CheckButtonR(const char *label,const char *tooltip,bool initState,ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    CheckButtonC ret = CheckButtonC(label,initState);
    ret.SetToolTip(tooltip);
    ConnectRef(ret.SigChanged(),obj,func);
    return ret;    
  }

}

#endif
