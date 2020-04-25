// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MESSAGEBOX_HEADER
#define RAVLGUI_MESSAGEBOX_HEADER 1
/////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/MessageBox.hh"
//! lib=RavlGUI
//! author="James Smith"
//! date="22/10/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Dialogs"
//! example="exMessageBox.cc"

#include "Ravl/GUI/Window.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/GUI/Manager.hh"

namespace RavlGUIN {
  
  using namespace RavlN;

  class MessageBoxC;
  
  //! userlevel=Develop
  //: Window body.
  
  class MessageBoxBodyC 
    : public WindowBodyC
  {
  public:
    MessageBoxBodyC(StringC message, bool bYesNo = false, const char *title = 0, const WindowC* parent = NULL);
    //: Constructor.
    // If parent is not NULL, the dialog will be kept above it by the window manager
    
    virtual bool Create();
    //: Create the widget.
    
    Signal1C<bool>& SigDone() { return m_sigDone; }
    //: "Finished" signal

  protected:

    bool OnClick(bool &bResult);
    //: Handles button presses

    StringC m_strMessage;
    bool m_bYesNo;
    Signal1C<bool> m_sigDone;
    
    bool m_gotSelfRef;
    
    friend class MessageBoxC;
  };
  
  //! userlevel=Normal
  //: Message box handle.
  
  class MessageBoxC 
    : public WindowC
  {
  public:
    MessageBoxC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    MessageBoxC(StringC message, bool bYesNo = false, const char *title = 0, const WindowC* parent = NULL)
      : WindowC( *new MessageBoxBodyC(message,bYesNo,title,parent))
    {}
    //: Constructor.
    // If parent is not NULL, the dialog will be kept above it by the window manager
    
  protected:
    MessageBoxC(MessageBoxBodyC &bod)
      : WindowC(bod)
    {}
    //: Body constructor.
    
    MessageBoxBodyC &Body() 
    { return static_cast<MessageBoxBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
    const MessageBoxBodyC  &Body() const
    { return static_cast<const MessageBoxBodyC  &>(WidgetC::Body()); }
    //: Access body.
    
  public:

    Signal1C<bool>& SigDone() {return Body().SigDone();}
    //: "Finished" signal

    void Destroy() { 
      WidgetC::Destroy(); 
      Invalidate();
    }
    //: Destroy this window.
    
    friend class MessageBoxBodyC;
  };

  inline
  MessageBoxC QuestionBox(StringC message,bool (*func)(bool &result), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,true,"Question",parent);
    Connect(ret.SigDone(),func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

  template<class ObjT>
  MessageBoxC QuestionBoxR(StringC message,ObjT &obj,bool (ObjT::*func)(bool &result), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,true,"Question",parent);
    ConnectRef(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
  template<class ObjT>
  MessageBoxC QuestionBox(StringC message,const ObjT &obj,bool (ObjT::*func)(bool &result), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,true,"Question",parent);
    Connect(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
  template<class ObjT,class DataT>
  MessageBoxC QuestionBoxR(StringC message,ObjT &obj,bool (ObjT::*func)(bool &result,DataT &),const DataT &data,const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,true,"Question",parent);
    bool tmp = true;
    ConnectRef(ret.SigDone(),obj,func,tmp,data);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

  template<class ObjT,class DataT>
  MessageBoxC QuestionBox(StringC message,const ObjT &obj,bool (ObjT::*func)(bool &result,DataT &),const DataT &data,const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,true,"Question",parent);
    bool tmp = true;
    Connect(ret.SigDone(),obj,func,tmp,data);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
  inline
  MessageBoxC AlertBox(StringC message, const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,false,"Alert",parent);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

  inline
  MessageBoxC AlertBox(StringC message,bool (*func)(), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,false,"Alert",parent);
    Connect(ret.SigDone(),func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }
  
  template<class ObjT>
  MessageBoxC AlertBox(StringC message,const ObjT &obj,bool (ObjT::*func)(), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,false,"Alert",parent);
    Connect(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

  template<class ObjT>
  MessageBoxC AlertBoxR(StringC message,ObjT &obj,bool (ObjT::*func)(), const WindowC* parent = NULL)
  { 
    MessageBoxC ret = MessageBoxC(message,false,"Alert",parent);
    ConnectRef(ret.SigDone(),obj,func);
    Manager.QueueOnGUI(Trigger(WidgetC(ret), &WidgetC::GUIShow));
    return ret;
  }

}

#endif
