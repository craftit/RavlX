// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! docentry="Graphics.GTK"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/CWidget.cc"

#include "Ravl/GUI/CWidget.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/CallMethods.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  //: Create the widget.
  
  bool ContainerWidgetBodyC::Create() {
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next())
      it.Data().Create();
    return true;
  }



  bool ContainerWidgetBodyC::Create(GtkWidget *newWidget) {
    widget = newWidget;
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next())
    {
      it.Data().Create();
    }
    return true;
  }

  //: Undo all references.
  // Used to avoid problems with circluar references.
  // This should undo all references to RAVL and GUI
  // objects which are not in the destroy tree. 
  // The function is called by the root window in its
  // destructor.
  
  void ContainerWidgetBodyC::Destroy() {
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) 
      it.Data().Destroy();
    WidgetBodyC::Destroy();
  }
  
  //: Add widget.
  // GUI thread only.
  
  bool ContainerWidgetBodyC::GUIAdd(const WidgetC &awidge) {
    children.InsLast(awidge);
    WidgetC &widge = children.Last();
    if(widget == 0) {
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(widge.IsValid());
    if(widge.Widget() == 0) {
      if(!widge.Create()) {
        std::cerr << "ContainerWidgetBodyC::Create(), Widget create failed ! \n";
        return true;
      }
    }
    gtk_container_add(GTK_CONTAINER(widget),widge.Widget());
    return true;
  }
  
  //: Add child widget.
  
  bool ContainerWidgetBodyC::Add(const WidgetC &widge) {
    Manager.Queue(Trigger(ContainerWidgetC(*this),&ContainerWidgetC::GUIAdd,widge));
    return true;
  }
  
  
  //: Remove a child widget.
  
  bool ContainerWidgetBodyC::Remove(WidgetC &widge)  {
    Manager.Queue(Trigger(ContainerWidgetC(*this),&ContainerWidgetC::GUIRemove,widge));
    return true;
  }
  
  //: Remove a child widget.
  
  bool ContainerWidgetBodyC::GUIRemove(WidgetC &widge) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) {
      if(it.Data() == widge) {
	it.Del();
	break;
      }
    }
    if(widget == 0 || widge.Widget() == 0)
      return true; // Done !
    gtk_container_remove(GTK_CONTAINER(widget),widge.Widget());
    return true;
  }

  //: Add children.
  
  bool ContainerWidgetBodyC::GUIAddList(const DListC<WidgetC> &widges) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    for(ConstDLIterC<WidgetC> it(widges);it.IsElm();it.Next())
      GUIAdd(it.Data());
    return true;
  }

  //: Add children.
  
  bool ContainerWidgetBodyC::Add(const DListC<WidgetC> &widges) {
    Manager.Queue(Trigger(ContainerWidgetC(*this),&ContainerWidgetC::GUIAddList,widges));
    return true;
  }
  
}
