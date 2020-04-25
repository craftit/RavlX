// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MENUBAR_HEADER
#define RAVLGUI_MENUBAR_HEADER 1
////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Menu.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! example=exMenu.cc
//! docentry="Ravl.API.Graphics.GTK.Control"
//! rcsid="$Id$"

#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Signal3.hh"
#include "Ravl/GUI/CWidget.hh"
#include "Ravl/String.hh"
#include "Ravl/GUI/MouseEvent.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Menu item body
  
  class MenuItemBodyC 
    : public WidgetBodyC
  {
  public:
    MenuItemBodyC(const StringC &lab);
    //: Constructor.
    
    MenuItemBodyC(const XMLFactoryContextC &factory);
    //: Constructor.

    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    Signal0C &SigSelected()
    { return selected; }
    //: Access selected signal.
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    
    StringC name;
    Signal0C selected;
  };
  
  
  //! userlevel=Normal
  //: Menu item

  class MenuItemC 
    : public WidgetC
  {
  public:
    MenuItemC()
    {}
    //: Default constructor
    
    MenuItemC(const StringC &name)
      : WidgetC(*new MenuItemBodyC(name))
    {}
    //: Constructor
    
    MenuItemC(MenuItemBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor

    MenuItemC(const XMLFactoryContextC &factory)
      : WidgetC(*new MenuItemBodyC(factory))
    {}
    //: Body constructor

  protected:
    MenuItemBodyC &Body() 
    { return static_cast<MenuItemBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    Signal0C &SigSelected()
    { return Body().SigSelected(); }
    //: Access selected signal.
    
  };
  
  //////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Pixmap body
  
  class MenuBodyC 
    : public ContainerWidgetBodyC
  {
  public:
    MenuBodyC(const StringC &nmenuName,const DListC<WidgetC> &widges)
      : ContainerWidgetBodyC(widges),
	menuName(nmenuName)
    {}
    //: Constructor.
    
    MenuBodyC(const StringC &nmenuName,const WidgetC &widge)
      : menuName(nmenuName)
    { GUIAdd(const_cast<WidgetC &>(widge)); }
    //: Constructor.
    
    MenuBodyC(const StringC &nmenuName)
      : menuName(nmenuName)
    {}
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool GUIAdd(const WidgetC &widge);
    //: Add an item to the end of the menu
    // GUI thread only.

    virtual bool GUIRemove(WidgetC &widge);
    //: Remove an item from the menu
    // GUI thread only.

    const StringC &MenuName() const 
    { return menuName; }
    //: Menu name
    
    void Popup(GdkEvent *event = 0);
    //: Make menu popup in 'parent' at 'where'.

    void Popup(MouseEventC &me);
    //: Popup the menu.

  protected:
    StringC menuName;
  };
  

  //! userlevel=Normal
  //: Pixmap 
  
  class MenuC 
    : public ContainerWidgetC
  {
  public:
    MenuC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    MenuC(const StringC &menuName,const DListC<WidgetC> &widges)
      : ContainerWidgetC(*new MenuBodyC(menuName,widges))
    {}
    //: Constructor

    MenuC(const StringC &menuName,const WidgetC &widge)
      : ContainerWidgetC(*new MenuBodyC(menuName,widge))
    {}
    //: Constructor
  
    MenuC(const StringC &menuName)
      : ContainerWidgetC(*new MenuBodyC(menuName))
    {}
    //: Constructor
    
    MenuC(WidgetC &widge);
    //: Create a handle from a base class.
    // This will create an invalid reference if
    // the given object is not derived from MenuC
    
  protected:
    MenuBodyC &Body() 
    { return static_cast<MenuBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const MenuBodyC &Body() const 
    { return static_cast<const MenuBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    const StringC &MenuName() const 
    { return Body().MenuName(); }
    //: Get name of menu.
    
    void Popup(GdkEvent *event = 0) 
    { Body().Popup(event); }
    //: Popup a menu.
    // If caused by an event you should pass 'event' ptr,
    // otherwise set to 0.   This is used for some heuristics
    // about how to display the menu. (Setting to 0 is fine
    // in most cases.)
    
    void Popup(GdkEventButton *event)
    { Body().Popup((GdkEvent *) event); }
    //: Popup a menu.
    // This is just a way of avoiding some casts.
    // See Popup(GdkEvent *) for details.

    void Popup(MouseEventC &me)
    { Body().Popup(me); }
    //: Popup the menu.

  };

  //////////////////////////////////////////////////

  //! userlevel=Develop
  //: Pixmap body
  
  class MenuBarBodyC 
    : public ContainerWidgetBodyC
  {
  public:
    MenuBarBodyC()
    {}
    //: Default constuctor.
  
    MenuBarBodyC(const DListC<WidgetC> &widges)
      : ContainerWidgetBodyC(widges)
    {}
    //: Constructor.
  
    MenuBarBodyC(const MenuC &menu)
    { GUIAdd(menu); }
    //: Default constuctor.
    
    virtual bool Create();
    //: Create the widget.
    
  protected:
  };


  //! userlevel=Normal
  //: Pixmap 
  
  class MenuBarC 
    : public ContainerWidgetC
  {
  public:
    MenuBarC(const DListC<WidgetC> &widges)
      : ContainerWidgetC(*new MenuBarBodyC(widges))
    {}
    //: Constructor
  
    MenuBarC(const MenuC &menu)
      : ContainerWidgetC(*new MenuBarBodyC(menu))
    {}
    //: Constructor
  
    MenuBarC()
      : ContainerWidgetC(*new MenuBarBodyC())
    {}
    //: Default constructor
    
  };
  
  
  template<class Data1T,class Data2T>
  MenuItemC MenuItem(const StringC &label,bool (*func)(Data1T &, Data2T &),const Data1T &dat1,const Data2T &dat2) {
    MenuItemC ret(label);
    Connect(ret.SigSelected(),func,dat1,dat2);
    return ret;
  }
  //: Create new menu item, connect to a function.
  
  template<class Data1T>
  MenuItemC MenuItem(const StringC &label,bool (*func)(Data1T &),const Data1T &dat1) {
    MenuItemC ret(label);
    Connect(ret.SigSelected(),func,dat1);
    return ret;
  }
  //: Create new menu item, connect to a function.
  
  template<class DataT>
  MenuItemC MenuItem(const StringC &label,bool (*func)(MenuItemC &, DataT &ref),const DataT &dat)
  {  
    MenuItemC ret(label);
    Connect(ret.SigSelected(),func,ret,dat);
    return ret;
  }
  //: Create new menu item, connect to a function.
  
  template<class DataT>
  MenuItemC MenuItem(const StringC &label,const DataT &dat,bool (DataT::*func)())
  {  
    MenuItemC ret(label);
    Connect(ret.SigSelected(),dat,func);
    return ret;
  }
  //: Create new menu item, connect to a function.
  
  template<class DataT>
  MenuItemC MenuItemR(const StringC &label,DataT &dat,bool (DataT::*func)())
  {  
    MenuItemC ret(label);
    ConnectRef(ret.SigSelected(),dat,func);
    return ret;
  }
  //: Create new menu item, connect to a function.
  // This does NOT hold a reference to item 'dat'
  
  template<class ObjT,class DataT>
  MenuItemC MenuItem(const StringC &label,const ObjT &obj,bool (ObjT::*func)(DataT &),const DataT &dat)
  {  
    MenuItemC ret(label);
    Connect(ret.SigSelected(),obj,func,dat);
    return ret;
  }
  //: Create new menu item, connect to a function.

  template<class ObjT,class DataT>
  MenuItemC MenuItemR(const StringC &label,ObjT &obj,bool (ObjT::*func)(DataT &),const DataT &dat = DataT())
  {  
    MenuItemC ret(label);
    ConnectRef(ret.SigSelected(),obj,func,dat);
    return ret;
  }
  //: Create new menu item, connect to a function.
  // This does NOT hold a reference to item 'dat'

  template<class ObjT,class Data1T,class Data2T>
  MenuItemC MenuItem(const StringC &label,const ObjT &obj,bool (ObjT::*func)(Data1T &,Data2T &),const Data1T &dat1,const Data2T &dat2)
  {  
    MenuItemC ret(label);
    Connect(ret.SigSelected(),obj,func,dat1,dat2);
    return ret;
  }
  //: Create new menu item, connect to a function.

  template<class ObjT,class Data1T,class Data2T>
  MenuItemC MenuItemR(const StringC &label,ObjT &obj,bool (ObjT::*func)(Data1T &,Data2T &),const Data1T &dat1 = Data1T(),const Data2T &dat2 = Data2T())
  {  
    MenuItemC ret(label);
    ConnectRef(ret.SigSelected(),obj,func,dat1,dat2);
    return ret;
  }
  //: Create new menu item, connect to a function.

  inline
  MenuItemC MenuItem(const StringC &label)
  { return MenuItemC(label); }  
  //: Do nothing...

  inline
  MenuItemC MenuItemSeparator()
  { return MenuItemC(StringC()); }
  //: Do nothing...
  
  MenuItemC MenuItem(const StringC &label,bool (*func)());
  //: Simply call a function.
  
  MenuItemC MenuItemShow(const StringC &label,const WidgetC &widge);
  //: Send 'Show' request to a widget, from a menu item.
  // Calls 'show' of another widget when menu item is selected.
}



#endif
