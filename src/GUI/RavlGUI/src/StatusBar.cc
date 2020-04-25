// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/StatusBar.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/StatusBar.hh"
#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  StatusBarBodyC::StatusBarBodyC(const StringC &nInitMsg)
    : initMsg(nInitMsg)
  {}
  
  //: Create the widget.
  
  bool StatusBarBodyC::Create() {
    if(widget != 0)
      return true;
    widget = gtk_statusbar_new();
    defaultContext = gtk_statusbar_get_context_id	(GTK_STATUSBAR(widget),"Default");
    gtk_statusbar_push(GTK_STATUSBAR(widget),defaultContext,initMsg.chars());
    return true;
  }
  
  //: Create the widget from another.
  
  bool StatusBarBodyC::Create(GtkWidget *_widget) {
    widget = _widget;
    defaultContext = gtk_statusbar_get_context_id	(GTK_STATUSBAR(widget),"Default");
    gtk_statusbar_push(GTK_STATUSBAR(widget),defaultContext,initMsg.chars());
    return true;
  }
  
  //: Change context.
  // GUI Thread only.
  
  IntT StatusBarBodyC::GUIGetContext(const char *name)  { 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(widget == 0) 
      if(!Create())
	return 0;
    return gtk_statusbar_get_context_id(GTK_STATUSBAR(widget),name); 
  }
  
  //: Push message.
  // GUI Thread only.
  
  bool StatusBarBodyC::GUIPush(IntT &context,StringC &str) {
    if(widget == 0) {
      ONDEBUG(std::cerr << "Msg early: " << str << "\n");
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(context == 0)
      context = defaultContext;
    ONDEBUG(std::cerr << "StatusBarBodyC::GUIPush()  Ctxt:" << context << " Msg:" << str << "\n");
    gtk_statusbar_push(GTK_STATUSBAR(widget),context,str.chars());
    return true;
  }
  
  //: Pop message.
  // GUI Thread only.
  
  bool StatusBarBodyC::GUIPop(IntT &context) {
    if(widget == 0) {
      ONDEBUG(std::cerr << "Pop early Ctxt:" << context << "\n");
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(context == 0)
      context = defaultContext;
    ONDEBUG(std::cerr << "StatusBarBodyC::GUIPop()  Ctxt:" << context << " \n");
    gtk_statusbar_pop(GTK_STATUSBAR(widget),context);
    return true;
  }
  
  
  //: Push message.
  
  void StatusBarBodyC::Push(IntT context,const StringC &str) 
  { 
    ONDEBUG(std::cerr << "StatusBarBodyC::Push()  Ctxt:" << context << " Msg:" << str << "\n");
    Manager.QueueOnGUI(Trigger(StatusBarC(*this),&StatusBarC::GUIPush,context,str));
  }
  
  //: Push message for a give period.
  // Thread safe.
  
  void StatusBarBodyC::PushTimed(RealT t,IntT context,const StringC &str)  {
    ONDEBUG(std::cerr << "StatusBarBodyC::PushTimed(), t:" << t << " Ctxt" << context << " Msg:" << str << "\n");
    Push(context,str);
    Manager.Queue(t,Trigger(StatusBarC(*this,context),&StatusBarC::Pop));
  }
  
  //: Pop message.
  
  bool StatusBarBodyC::Pop(IntT context) 
  {
    ONDEBUG(std::cerr << "StatusBarBodyC::Pop()   Ctxt:" << context << " \n");
    Manager.QueueOnGUI(Trigger(StatusBarC(*this),&StatusBarC::GUIPop,context));
    return true;
  }
  
  //////////////////////////////////////////////
  
#if 0
#include "Ravl/PThread/Semaphore.hh"

  class StatusBarCtxtUpdateC  {
  public:
    StatusBarCtxtUpdateC(SemaphoreC &nsema,StatusBarC &nsb,const char *nname)
      : sema(nsema),
      sb(nsb),
      name(nname)
      {}
    //: Constructor.
    
    void ChangeContext() {
      sb.GUIChangeContext(name);
      sema.Post();
    }
    
    SemaphoreC &sema;
    StatusBarC &sb;
    const char *name;
  };
#endif
  //: Change context.
  
  void StatusBarC::ChangeContext(const char *name) {
    if(Manager.IsGUIThread()) {
      GUIChangeContext(name);
      return ;
    }
#if 0
    SemaphoreC sema(0);
    
    // Could event be lost, and we wait for ever ?
    
    Manager.Queue(Trigger(StatusBarCtxtUpdateC(sema,*this,name),
			      &StatusBarCtxtUpdateC::ChangeContext));
    sema.Wait(); // Wait for change...
#else
    std::cerr << "StatusBarC::ChangeContext(), Not Implemented for non-gui threads \n";
#endif
  }

}
