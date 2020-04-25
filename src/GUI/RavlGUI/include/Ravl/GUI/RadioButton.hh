// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIRADIOBUTTON_HEADER
#define RAVLGUIRADIOBUTTON_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! example=exRadioButton.cc
//! docentry="Ravl.API.Graphics.GTK.Control"
//! lib=RavlGUI
//! userlevel=Normal
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/RadioButton.hh"

#include "Ravl/GUI/ToggleButton.hh"


typedef struct _GSList           GSList;

namespace RavlGUIN {

  class RadioButtonC;
  class RadioButtonBodyC;
  typedef DListC<RadioButtonBodyC *> RadioButtonGroupT;
  
  //! userlevel=Normal
  //: Radio button
  // <img src="../../../html/radio-group.png"><br>
  // Use <a href="RavlGUIN.html#RadioButtonGroupT">RadioButtonGroupT</a> to create a mutually exclusive group of buttons, i.e. such that, when one of them is pressed, the others all get turned off.
  
  class RadioButtonC
    : public ToggleButtonC
  {
  public:
    RadioButtonC(const char *label = 0,const char *tooltip = 0,const RadioButtonGroupT &group = RadioButtonGroupT());
    //: Create a button.
    // If <code>label</code> is set to 0, none will be used.<br>
    // If <code>group</code> is not used, buttons will behave independently.

    
    RadioButtonC(const PixmapC &pixm,const char *label = 0,const RadioButtonGroupT &group = RadioButtonGroupT());
    //: Create a button.
    // If <code>label</code> is set to 0, none will be used.<br>
    // If <code>group</code> is not used, buttons will behave independently.
    
  protected:
    inline RadioButtonBodyC &Body();
    //: Access body.
    
    inline const RadioButtonBodyC &Body() const;
    //: Access body.
    
  public:
    
    inline RadioButtonGroupT &Group();
    //: Access button group.
    
    friend class RadioButtonBodyC;
  };
  
  //! userlevel=Develop
  //: Radio button body.
  
  class RadioButtonBodyC
    : public ToggleButtonBodyC
  {
  public:
    RadioButtonBodyC(const char *nlabel = 0,const char *tooltip = 0,const RadioButtonGroupT &group = RadioButtonGroupT());
    //: Constructor.
    
    RadioButtonBodyC(const char *nlabel,const PixmapC &pixm,const RadioButtonGroupT &group = RadioButtonGroupT());
    //: Constructor.
    
    virtual ~RadioButtonBodyC();
    //: Destructor.
    
    virtual bool Create();
    //: Create the widget.
    
    RadioButtonGroupT &Group()
      { return group; }
    //: Access the button group.
    
  protected:
    virtual GtkWidget *BuildWidget(const char *lab = 0);
    //: Create the actual widget.
    // This allows different types of buttons to
    // be created easily.
    
    GSList *GtkGroup()
      { return gtkgroup; }
    //: Access group.
    
    RadioButtonGroupT group;
    GSList *gtkgroup;
  };
  
  inline 
  RadioButtonBodyC &RadioButtonC::Body() { 
    return static_cast<RadioButtonBodyC &>(WidgetC::Body()); 
  }
  //: Access body.

  inline 
  const RadioButtonBodyC &RadioButtonC::Body() const
  { return static_cast<const RadioButtonBodyC &>(WidgetC::Body()); }
  //: Access body.

  inline 
  RadioButtonGroupT &RadioButtonC::Group()
  { return Body().Group(); }
  //: Access button group.
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! userlevel=Normal
  
  inline
  RadioButtonC RadioButton(const char *label,const RadioButtonGroupT &group)
  { return  RadioButtonC(label,0,group); }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class ObjT>
  RadioButtonC RadioButton(const char *label,const RadioButtonGroupT &group,const ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    Connect(ret.SigChanged(),obj,func);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  inline
  RadioButtonC RadioButton(const char *label,const RadioButtonGroupT &group,bool (*func)(bool &))
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    Connect(ret.SigChanged(),func,true);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class DataT>
  RadioButtonC RadioButton(const char *label,const RadioButtonGroupT &group,bool (*func)(bool &,DataT &),const DataT &v)
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    Connect(ret.SigChanged(),func,true,v);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class ObjT>
  RadioButtonC RadioButton(const char *label,const char *tooltip,const RadioButtonGroupT &group,const ObjT &obj,bool (ObjT::*func)(bool))
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    ret.SetToolTip(tooltip);
    Connect(ret.SigChanged(),obj,func,true);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class ObjT>
  RadioButtonC RadioButtonR(const char *label,const RadioButtonGroupT &group,ObjT &obj,bool (ObjT::*func)(bool &))
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    ConnectRef(ret.SigChanged(),obj,func,true);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class ObjT,class DataT>
  RadioButtonC RadioButtonR(const char *label,const RadioButtonGroupT &group,ObjT &obj,bool (ObjT::*func)(bool &,DataT &dat),const DataT &dat)
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    bool initState = false;
    ConnectRef(ret.SigChanged(),obj,func,initState,dat);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
  template<class ObjT>
  RadioButtonC RadioButtonR(const char *label,const char *tooltip,const RadioButtonGroupT &group,bool initState,ObjT &obj,bool (ObjT::*func)(bool))
  { 
    RadioButtonC ret = RadioButtonC(label,0,group);
    ret.SetToolTip(tooltip);
    ConnectRef(ret.SigChanged(),obj,func,true);
    return ret;    
  }
  //! userlevel=Normal
  //: Create a radio button in 'group'
  
}

#endif
