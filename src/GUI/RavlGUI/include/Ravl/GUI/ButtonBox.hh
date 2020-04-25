// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_BUTTONBOX_HEADER
#define RAVLGUI_BUTTONBOX_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/ButtonBox.hh"
//! lib=RavlGUI
//! author="Kieron Messer"
//! date="22/10/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Dialogs"
//! example="exButtonBox.cc"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/SArray1d.hh"

namespace RavlGUIN {
  
  using namespace RavlN;

  class ButtonBoxC;
  
  //! userlevel=Develop
  //: Window body.
  
  class ButtonBoxBodyC 
    : public WindowBodyC
  {
  public:
    ButtonBoxBodyC(StringC message, SArray1dC<StringC> & buttons, const char *title = 0, const WindowC* parent = NULL);
    //: Constructor.
    // If parent is not NULL, the dialog will be kept above it by the window manager
    
    virtual bool Create();
    //: Create the widget.
    
    Signal1C<UIntT>& SigDone() {return sigDone;}
    //: "Finished" signal

  protected:

    bool OnClick(UIntT & buttonNo);
    //: Handles button presses

    StringC m_strMessage;
    SArray1dC<StringC>m_buttons;
    Signal1C<UIntT> sigDone;

    friend class ButtonBoxC;
  };
  
  //! userlevel=Normal
  //: Message box handle.
  
  class ButtonBoxC 
    : public WindowC
  {
  public:
    ButtonBoxC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ButtonBoxC(StringC message, SArray1dC<StringC> & buttons, const char *title = 0, const WindowC* parent = NULL)
      : WindowC( *new ButtonBoxBodyC(message, buttons,title,parent))
    {}
    //: Constructor.
    // If parent is not NULL, the dialog will be kept above it by the window manager
    
  protected:
    ButtonBoxC(ButtonBoxBodyC &bod)
      : WindowC(bod)
    {}
    //: Body constructor.
    
    ButtonBoxBodyC &Body() 
    { return static_cast<ButtonBoxBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
    const ButtonBoxBodyC  &Body() const
    { return static_cast<const ButtonBoxBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
  public:

    Signal1C<UIntT>& SigDone() {return Body().SigDone();}
    //: "Finished" signal

    void Destroy() { 
      WidgetC::Destroy(); 
      Invalidate();
    }
    //: Destroy this window.
    
    friend class ButtonBoxBodyC;
  };

  inline
  ButtonBoxC ButtonBox(StringC message, SArray1dC<StringC> & buttons, const char *title, bool (*func)(UIntT & buttonNo), const WindowC* parent = NULL)
  { 
    ButtonBoxC ret = ButtonBoxC(message,buttons,title,parent);
    Connect(ret.SigDone(),func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
  template<class ObjT>
  ButtonBoxC ButtonBox(StringC message,SArray1dC<StringC> & buttons, const char *title, const ObjT &obj,bool (ObjT::*func)(UIntT & buttonNo), const WindowC* parent = NULL)
  { 
    ButtonBoxC ret = ButtonBoxC(message,buttons,title,parent);
    Connect(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

  template<class ObjT,class DataT>
  ButtonBoxC ButtonBoxR(StringC message,SArray1dC<StringC>&buttons,const char *title, ObjT &obj,bool (ObjT::*func)(UIntT &buttonNo), const WindowC* parent = NULL)
  { 
    ButtonBoxC ret = ButtonBoxC(message,buttons,title,parent);
    ConnectRef(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
}

#endif
