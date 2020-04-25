// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MENUCHECKITEM_HEADER
#define RAVLGUI_MENUCHECKITEM_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/MenuCheck.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! date="26/07/1999"

#include "Ravl/GUI/Menu.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Menu check item body
  
  class MenuCheckItemBodyC 
    : public MenuItemBodyC 
  {
  public:
    MenuCheckItemBodyC(const StringC &lab,bool initActive= false);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    bool IsActive() const;
    //: Test if toggle is active.
    
    bool GUISetActive(const bool &val);
    //: Set active.
    // This will cause a 'SelectedToggle' signal to 
    // be issued. <p>
    // Should only be called by the GUI thread.
    
    void SetActive(bool val);
    //: Set active.
    // This will cause a 'SelectedToggle' signal to 
    // be issued. <p>
    
    Signal1C<bool> &SelectedToggle()
    { return selectedToggle; }
    //: Toggle select signal.
    
  protected:
    virtual void Destroy();
    //: Undo all references
    
    static int doSelected(GtkWidget *widget,Signal1C<bool> *data);
    
    Signal1C<bool> selectedToggle;
    
    bool active;
  };
  
  
  //! userlevel=Normal
  //: Menu item
  
  class MenuCheckItemC 
    : public MenuItemC 
  {
  public:
    MenuCheckItemC()
    {}
    //: Default constructor
    
    MenuCheckItemC(const StringC &name,bool initActive = false)
      : MenuItemC(*new MenuCheckItemBodyC(name,initActive))
    {}
    //: Constructor
    
    MenuCheckItemC(MenuCheckItemBodyC &bod)
      : MenuItemC(bod)
    {}
    //: Body constructor
    
  protected:
    MenuCheckItemBodyC &Body() 
    { return static_cast<MenuCheckItemBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const MenuCheckItemBodyC &Body() const
    { return static_cast<const MenuCheckItemBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool IsActive() const
    { return Body().IsActive(); }
    //: Test if toggle is active.
    
    bool GUISetActive(const bool &val)
    { return Body().GUISetActive(val); }
    //: Set active status.
    // This will cause a 'SelectedToggle' signal to 
    // be issued. <p>
    // GUI Thread only.
    
    void SetActive(bool val)
    { Body().SetActive(val); }
    //: Set active status.
    // This will cause a 'SelectedToggle' signal to 
    // be issued. <p>
    // Thread safe. 
    
    Signal1C<bool> &SelectedToggle()
    { return Body().SelectedToggle(); }
    //: Toggle select signal.
    
  };
  
  //: Call a function.

  template<class Data1T>
  MenuCheckItemC MenuCheckItem(const StringC &label,bool (*func)(bool &, Data1T &),const Data1T &dat1 = Data1T())
  {
    MenuCheckItemC ret(label);
    Connect(ret.SelectedToggle(),func,false,dat1);
    return ret;
  }
  
  template<class DataT>
  MenuCheckItemC MenuCheckItem(const StringC &label,const DataT &obj,bool (DataT::*func)(bool &))
  {  
    MenuCheckItemC ret(label);
    Connect(ret.SelectedToggle(),obj,func);
    return ret;
  }
  
  template<class DataT>
  MenuCheckItemC MenuCheckItemR(const StringC &label,DataT &obj,bool (DataT::*func)(bool &))
  {  
    MenuCheckItemC ret(label);
    ConnectRef(ret.SelectedToggle(),obj,func);
    return ret;
  }
  //: Menu check item, call referenced class's method.
  
  template<class DataT>
  MenuCheckItemC MenuCheckItem(const StringC &label,bool initActive,const DataT &obj,bool (DataT::*func)(bool &))
  {  
    MenuCheckItemC ret(label,initActive);
    Connect(ret.SelectedToggle(),obj,func);
    return ret;
  }
  
  template<class DataT>
  MenuCheckItemC MenuCheckItemR(const StringC &label,bool initActive,DataT &obj,bool (DataT::*func)(bool &))
  {  
    MenuCheckItemC ret(label,initActive);
    ConnectRef(ret.SelectedToggle(),obj,func);
    return ret;
  }
}

#endif
