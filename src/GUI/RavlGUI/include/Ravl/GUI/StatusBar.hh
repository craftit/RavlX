// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUISTATUSBAR_HEADER
#define RAVLGUISTATUSBAR_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/StatusBar.hh"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! date="25/06/99"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Assert.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Status bar body.
  
  class StatusBarBodyC 
    : public WidgetBodyC
  {
  public:
    StatusBarBodyC(const StringC &initMsg);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget);
    //: Create the widget from another.
    
    void Push(IntT context,const StringC &str);
    //: Push message.
    // Thread safe.
    
    void PushTimed(RealT t,IntT context,const StringC &str);
    //: Push message for a give period.
    // Thread safe.
    
    bool Pop(IntT context);
    //: Pop message.
    // Thread safe.
    
    bool GUIPush(IntT &context,StringC &str);
    //: Push message.
    // GUI Thread only.
    
    bool GUIPop(IntT &context);
    //: Pop message.
    // GUI Thread only.
    
    IntT GUIGetContext(const char *name);
    //: Change context.
    // GUI Thread only.
    
  protected:
    StringC initMsg;
    IntT defaultContext; // Default context.
  
    friend class StatusBarC;
  };

  //! userlevel=Normal
  //: Status bar.
  // Note: Messages sent to an invalid StatusBar are redirected
  // to std::cerr.
  
  class StatusBarC 
    : public WidgetC
  {
  public:
    StatusBarC()
      : context(0)
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    StatusBarC(const StringC &initMsg)
      : WidgetC(*new StatusBarBodyC(initMsg)),
      context(0)
    {}
    //: Constructor.
    
    StatusBarC(const StatusBarC &oth)
      : WidgetC(oth),
      context(oth.context)
      {}
    //: Copy constructor.
    // New handle inherits parent handles context.
    
  protected:
    StatusBarC(StatusBarBodyC &bod,IntT ctxt = 0)
      : WidgetC(bod),
      context(ctxt)
      {}
    //: Body constructor.
    
    StatusBarBodyC &Body()
      { return static_cast<StatusBarBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const StatusBarBodyC &Body() const
      { return static_cast<const StatusBarBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    StatusBarC(const StatusBarC &oth,const char *name)
      : WidgetC(oth)
      { 
	RavlAssert(IsValid());
	context = Body().GUIGetContext(name); 
      }
    //: Copy, but with new context.
    // GUI Thread only.
    
    bool GUIChangeContext(const char *name) { 
      if(!IsValid()) {
	cerr << "Msg WARNING Context changes on invalid handles are ignored. Ctxt:'" << name << "'\n";
	return true;
      }
      context = Body().GUIGetContext(name); 
      return true;
    }
    //: Change context.
    // Call with GUI thread only!
    // 'Default' if none set.
    
    void ChangeContext(const char *name);
    //: Change context.
    // Thread safe. <p>
    // NB: This can ONLY be called AFTER the GUI thread has been 
    // started, and MUST NOT be called from the GUI thread itself.
    // (Use GUIChangeContext if this is the case.) <p>
    // Note: This call locks until the context is set,
    // this may be delayed by other GUI activity.
    
    bool Push(const StringC &str) { 
      if(!IsValid()) {
	cerr << "Msg:" << str << "\n";
	return true;
      }
      Body().Push(context,str); 
      return true;
    }
    //: Push message.
    // Thread safe.
    
    bool PushTimed(RealT t,const StringC &str){ 
      if(!IsValid()) {
	cerr << "Msg:" << str << "\n";
	return true;
      }
      Body().PushTimed(t,context,str); 
      return true;
    }
    //: Display a message for 't' seconds. 
    // Does Push, wait t, Pop.
    
    bool PushTimedF(RealT &t,StringC &str){ 
      if(!IsValid()) {
	cerr << "FMsg:" << str << "\n";
	return true;
      }
      Body().PushTimed(t,context,str); 
      return true;
    }
    //: Display a message for 't' seconds. 
    // Does Push, wait t, Pop.
    
    bool PushF(StringC &str) { 
      if(!IsValid()) {
	cerr << "Msg:" << str << "\n";
	return true;
      }
      Body().Push(context,str); 
      return true;
    }
    //: Push message.
    // Included for signal funcs.
    // Thread safe.
    
    bool Pop() { 
      if(!IsValid())
	return true;
      return Body().Pop(context); 
    }
    //: Pop message.
    // Thread safe.
    
    void Next(const StringC &str) {
      if(!IsValid()) {
	cerr << "Msg:" << str << "\n";
	return ;
      }
      Body().Pop(context);
      Body().Push(context,str);
    }
    //: Replace current message with next one.
    // Thread safe.
    
    bool GUIPush(IntT &context,StringC &str) { 
      if(!IsValid()) {
	cerr << "Msg:" << str << "\n";
	return true;
      }
      return Body().GUIPush(context,str); 
    }
    //: Push message.
    // GUI Thread only.
    
    bool GUIPop(IntT &context) { 
      if(!IsValid()) 
	return true;
      return Body().GUIPop(context); 
    }
    //: Pop message.
    // GUI Thread only.
    
  protected:
    IntT context; // Context id.
    
    friend class StatusBarBodyC;
  };

}

#endif

