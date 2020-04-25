// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_CWIDGET_HEADER
#define RAVLGUI_CWIDGET_HEADER 1
////////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/CWidget.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Internal"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/DList.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Container Widget Body
  
  class ContainerWidgetBodyC 
    : public WidgetBodyC
  {
  public:
    ContainerWidgetBodyC()
    {}
    //: Default constructor.
    
    ContainerWidgetBodyC(const DListC<WidgetC> &widges)
      : children(widges)
    {}
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *newWidget);
    //: Create the widget.

    virtual bool Add(const WidgetC &widge);
    //: Add a child widget.
    
    bool Add(const DListC<WidgetC> &widges);
    //: Add children.
    
    bool GUIAddList(const DListC<WidgetC> &widges);
    //: Add children.
    
    virtual bool GUIAdd(const WidgetC &widge);
    //: Add widget.
    // GUI thread only.

    bool Remove(WidgetC &widge);
    //: Remove a child widget.
    
    virtual bool GUIRemove(WidgetC &widge);
    //: Remove a child widget.
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    // Used to avoid problems with circluar references.
    // This should undo all references to RAVL and GUI
    // objects which are not in the destroy tree. 
    // The function is called by the root window in its
    // destructor.
    
    DListC<WidgetC> children; // Child widgets.
    
    friend class ContainerWidgetC;
  };
  
  //! userlevel=Normal
  //: Container Widget
  
  class ContainerWidgetC 
    : public WidgetC
  {
  public:
    ContainerWidgetC()
    {}
    //: Default constructor.
    
    ContainerWidgetC(const WidgetC &body)
      : WidgetC(dynamic_cast<const ContainerWidgetBodyC *>(BodyPtr(body)))
    {}
    //: Base constructor.
    
  protected:
    ContainerWidgetC(ContainerWidgetBodyC &body)
      : WidgetC(body)
    {}
    //: Body constructor.
    
    ContainerWidgetC(const ContainerWidgetBodyC *body)
      : WidgetC(body)
    {}
    //: Body constructor.
    
    ContainerWidgetBodyC &Body() 
    { return static_cast<ContainerWidgetBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ContainerWidgetBodyC &Body() const
    { return static_cast<const ContainerWidgetBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:  
    bool Add(const WidgetC &widge)
    { return Body().Add(widge); }
    //: Add child widget.
    
    bool Remove(WidgetC &widge)
    { return Body().Remove(widge); }
    //: Remove child widget.
    
    bool GUIRemove(WidgetC &widge)
    { return Body().GUIRemove(widge); }
    //: Remove child widget.
    
    bool Add(const DListC<WidgetC> &widges)
    { return Body().Add(widges); }
    //: Add child widget.
    
    bool GUIAddList(const DListC<WidgetC> &widges)
    { return Body().GUIAddList(widges); }
    //: Add children.
    
    bool GUIAdd(const WidgetC &widge)
    { return Body().GUIAdd(widge); }
    //: Add widget.
    // GUI thread only.
    
    bool operator+=(const WidgetC &widge)
    { return Body().Add(widge); }
    //: Add child widget.
    
    friend class ContainerWidgetBodyC;
  };
}

#endif
