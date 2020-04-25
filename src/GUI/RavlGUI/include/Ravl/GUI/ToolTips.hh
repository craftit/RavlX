// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUITOOLTIP_HEADER
#define RAVLGUITOOLTIP_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/ToolTips.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="11/10/99"
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/Widget.hh"

extern "C" {
  typedef struct _GtkTooltips	 GtkTooltips;
};

namespace RavlGUIN {

  class ToolTipsC;
  
  ////////////////////////////////////////
  
  //! userlevel=Develop
  //: Tool tip set body.
  
  class ToolTipsBodyC 
    : public RCBodyC
  {
  public:
    ToolTipsBodyC();
    //: Constructor.
    
    void Enable();
    //: Enable tool tips.
    // Thread safe.
    
    void Disable();
    //: Disable tool tips.
    // Thread safe.
    
    bool GUIEnable();
    //: Enable tool tips.
    // GUI Thread only.
    
    bool GUIDisable();
    //: Disable tool tips.
    // GUI Thread only.
    
    void SetDelay(RealT secs);
    //: Set tips delay.
    // Thread safe.
    
    bool GUISetDelay(RealT secs);
    //: Set tips delay.
    // GUI Thread only.
    
    void AddToolTip(WidgetC &widge,const char *text,const char *context = 0);
    //: Set tips delay.
    // Thread safe.
    
    bool GUIAddToolTip(WidgetC &widge,const char *text,const char *context = 0);
    //: Set tips delay.
    // GUI Thread only.
    
    bool IsEnabled() const
      { return enabled; }
    //: Is this group of tool tips enabled ?
    
  protected:
    
    bool Create();
    //: Create the widget.
    
    void Destroy();
    //: Undo all references.
    
  private:
    bool enabled;
    GtkTooltips *widget;
    RealT delay; 
    friend class ToolTipsC;
  };
  
  ////////////////////////////////////////
  //! userlevel=Normal
  //: Tool tip set handle.
  
  class ToolTipsC 
    : public RCHandleC<ToolTipsBodyC>
  {
  public:
    ToolTipsC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ToolTipsC(bool bod)
      : RCHandleC<ToolTipsBodyC>(*new ToolTipsBodyC())
      {}
    //: Constructor.
    
  protected:
    ToolTipsC(ToolTipsBodyC &bod)
      : RCHandleC<ToolTipsBodyC>(bod)
      {}
    //: Body Constructor.
    
    ToolTipsBodyC &Body()
      { return static_cast<ToolTipsBodyC &>(RCHandleC<ToolTipsBodyC>::Body ()); }
    //: Access body.
    
    const ToolTipsBodyC &Body() const
      { return static_cast<const ToolTipsBodyC &>(RCHandleC<ToolTipsBodyC>::Body()); }
    //: Access body.
    
    bool GUIEnable()
      { return Body().GUIEnable(); }
    //: Enable tool tips.
    // GUI Thread only.
    
    bool GUIDisable()
      { return Body().GUIDisable(); }
    //: Disable tool tips.
    // GUI Thread only.
    
    bool GUISetDelay(RealT &secs)
      { return Body().GUISetDelay(secs); }
    //: Set tips delay.
    
  public:
    bool Create()
      { return Body().Create(); }
    //: Create the widget.
    
    void Enable()
      { Body().Enable(); }
    //: Enable tool tips.
    // Thread safe.
  
    void Disable()
    { Body().Disable(); }
    //: Disable tool tips.
    // Thread safe.
    
    void SetDelay(RealT &secs)
      { Body().SetDelay(secs); }
    //: Set tips delay.
    // Thread safe.
  
    void AddToolTip(WidgetC &widge,const char *text,const char *context = 0)
      { Body().AddToolTip(widge,text,context); }
    //: Set tips delay.
    // Thread safe.
    
   bool GUIAddToolTip(WidgetC &widge,const char *text,const char *context = 0)
      { return Body().GUIAddToolTip(widge,text,context); }
    //: Set tips delay.
    // GUI Thread only.
    
    void ShowToolTips(bool &state) { 
      if(state)
	Enable();
      else
	Disable();
    }
    //: Show tool tips ?
    // true= yes.
    
    bool IsEnabled() const 
      { return Body().IsEnabled(); }
    //: Is this group of tool tips enabled ?
    
    friend class ToolTipsBodyC;
    
  };
  
  extern ToolTipsC guiGlobalToolTips;
}

#endif
