// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Menu.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <gtk/gtk.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN
{
  void MenuItemC_response ( GtkWidget *,MenuItemBodyC *data) { 
    if(data->SigSelected().IsValid())
      data->SigSelected()();
  }
  
  //: Constructor.
  
  MenuItemBodyC::MenuItemBodyC(const StringC &lab)
    : name(lab),
      selected(true)
  {}
  
  //: Constructor.
  static bool InvokeTrigger(TriggerC &trig) {
    trig.Invoke();
    return true;
  }

  MenuItemBodyC::MenuItemBodyC(const XMLFactoryContextC &factory)
   : WidgetBodyC(factory)
  {
    Signal0C sigClicked;
    if(factory.UseComponent("SigClicked",sigClicked,true,typeid(Signal0C)))
      Connect(Signal("activate"),sigClicked,&Signal0C::Invoke);

    XMLFactoryContextC childContext;
    if(factory.ChildContext("Triggers",childContext)) {
      for(RavlN::DLIterC<RavlN::XMLTreeC> it(childContext.Children());it;it++) {
        TriggerC trigger;
        if(!childContext.UseComponent(it->Name(),trigger)) {
          continue;
        }
        Connect(Signal("activate"),&InvokeTrigger,trigger);
      }
    }
  }


  //: Create the widget.
  
  bool MenuItemBodyC::Create() {
    ONDEBUG(std::cerr << "MenuItemBodyC::Create(), name(" << name.chars() << ")\n");
    /* Create a new menu-item with a name... */
    if(!name.IsEmpty()) {
      widget = gtk_menu_item_new_with_label(name);
      
      /* Do something interesting when the menuitem is selected */
      gtk_signal_connect(GTK_OBJECT(widget), "activate",
			 GTK_SIGNAL_FUNC(RavlGUIN::MenuItemC_response), this);
    } else { // Separator....
      widget = gtk_menu_item_new();
    }

    ConnectSignals();

    ONDEBUG(std::cerr << "MenuItemBodyC::Create(), Done. \n");
    return true;
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool MenuItemBodyC::Create(GtkWidget *newWidget) {
    widget = newWidget;
    gtk_signal_connect(GTK_OBJECT(widget), "activate",
     GTK_SIGNAL_FUNC(RavlGUIN::MenuItemC_response), this);
    ONDEBUG(std::cerr << "MenuItemBodyC::Create(widget), name(" << name.chars() << ")\n");
    ConnectSignals();
    return true;
  }
  
  //: Undo all references.
  
  void MenuItemBodyC::Destroy() {
    ONDEBUG(std::cerr << "MenuItemBodyC::Destroy(), Called. \n");  
    selected.Invalidate();
    WidgetBodyC::Destroy();
  }
  
  //////////////////////////////////////////////////
  
  //: Create the widget.
  
  bool MenuBodyC::Create()
  {
    ONDEBUG(std::cerr << "MenuBodyC::Create(), name(" << menuName << ") called \n");

    widget = gtk_menu_new();

    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) {
      if(!it.Data().Create()) 
        std::cerr << "MenuBodyC::Create(), name(" << menuName << ") widget create failed!\n";

      gtk_widget_show(it.Data().Widget());

      MenuC sm(it.Data());
      if(sm.IsValid()) {
        ONDEBUG(std::cerr << "MenuBodyC::Create(), name(" << menuName << ") submenu name(" << sm.MenuName().chars() << ")\n");
        GtkWidget *submenu = gtk_menu_item_new_with_label(sm.MenuName());
        gtk_widget_show(submenu);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu),it.Data().Widget());
        gtk_menu_append(GTK_MENU(widget),submenu);
      }
      else
      {
        ONDEBUG(std::cerr << "MenuBodyC::Create(), name(" << menuName << ") adding menu item\n");
        gtk_menu_append(GTK_MENU(widget), it.Data().Widget());
      }
    }

    ConnectSignals();

    ONDEBUG(std::cerr << "MenuBodyC::Create(), name(" << menuName << ") done\n");

    return true;
  }
  
  //: Add an item to the end of the menu

  bool MenuBodyC::GUIAdd(const WidgetC &widge)
  {
    RavlAssert(Manager.IsGUIThread());

    ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") called\n");
    
    children.InsLast(widge);

    if (widget != 0)
    {
      WidgetC &addedWidget = children.Last();

      ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") adding to menu\n");
      if (addedWidget.Widget() == 0)
      {
        ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") creating widget\n");
        if (!addedWidget.Create())
          std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") widget create failed!\n";

      }

      gtk_widget_show(addedWidget.Widget());

      MenuC sm(addedWidget);
      if (sm.IsValid()) {
        ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") submenu name(" << sm.MenuName().chars() << ")\n");
        GtkWidget *submenu = gtk_menu_item_new_with_label(sm.MenuName());
        gtk_widget_show(submenu);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu), addedWidget.Widget());
        gtk_menu_append(GTK_MENU(widget), submenu);
      }
      else
      {
        ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") adding menu item\n");
        gtk_menu_append(GTK_MENU(widget), addedWidget.Widget());
      }
    }

    ONDEBUG(std::cerr << "MenuBodyC::GUIAdd(), name(" << menuName << ") done\n");
    
    return true;
  }



  bool MenuBodyC::GUIRemove(WidgetC &widge)
  {
    RavlAssert(Manager.IsGUIThread());

    ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") called\n");

    bool widgetFound = false;
    for (DLIterC<WidgetC> iterChildren(children); iterChildren.IsElm(); iterChildren.Next())
    {
      if (iterChildren.Data() == widge)
      {
        widgetFound = true;
        
        iterChildren.Del();
        
        break;
      }
    }

    if (widgetFound && widget && widge.Widget())
    {
      MenuC subMenu(widge);
      if (subMenu.IsValid())
      {
        ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") searching for submenu\n");

        GList *iterContainer = gtk_container_get_children(GTK_CONTAINER(widget));
        while (iterContainer)
        {
          ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") searching submenu widget\n");

          GtkWidget *childWidget = reinterpret_cast<GtkWidget*>(iterContainer->data);
          if (childWidget && GTK_IS_MENU_ITEM(childWidget))
          {
            GtkWidget *subMenu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(childWidget));
            if (subMenu && subMenu == widge.Widget())
            {
              ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") submenu widget found\n");

              gtk_container_remove(GTK_CONTAINER(widget), childWidget);
              
              break;
            }
          }

          iterContainer = iterContainer->next;
        }

        g_list_free(iterContainer);
      }
      else
      {
        ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") removing menu item\n");

        gtk_container_remove(GTK_CONTAINER(widget), widge.Widget());
      }
    }

    ONDEBUG(std::cerr << "MenuBodyC::GUIRemove(), name(" << menuName << ") done\n");

    return true;
  }

  //: Make menu popup in 'parent' at 'where'.
  
  void MenuBodyC::Popup(GdkEvent *event) {
    if(widget == 0) 
      Create();
    if(widget == 0) {
      std::cerr << "ERROR: Failed to create menu. \n";
      return ;
    }
    if(event != 0) {
      if (event->type == GDK_BUTTON_PRESS) {
	GdkEventButton *bevent = (GdkEventButton *) event; 
	gtk_menu_popup (GTK_MENU(Widget()), NULL, NULL, NULL, NULL,
			bevent->button, bevent->time);
	return ;
      } 
      if (event->type == GDK_MOTION_NOTIFY) {
	GdkEventMotion *bevent = (GdkEventMotion *) event; 
	gtk_menu_popup (GTK_MENU(Widget()), NULL, NULL, NULL, NULL,
			0, bevent->time);
	return ;
      } 
#if 0
      gtk_menu_popup (GTK_MENU(Widget()), NULL, NULL, NULL, NULL,
		      0, event->time);
      return ;
#endif
    }
    gtk_menu_popup (GTK_MENU(Widget()), NULL, NULL, NULL, NULL,
		    0, GDK_CURRENT_TIME);
    return ;
  }



  void MenuBodyC::Popup(MouseEventC &me)
  {
    if(widget == 0)
      Create();
    if(widget == 0)
    {
      std::cerr << "ERROR: Failed to create menu.\n";
      return;
    }

    int button = 0;
    for (int testButton = 0; testButton < me.MaxButtons(); testButton++)
    {
      if (!me.IsPressed(testButton) && me.HasChanged(testButton))
      {
        button = testButton;
        break;
      }
    }
    
    gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL, button, me.Time());
  }
  
  //: Create a handle from a base class.
  // This will create an invalid reference if
  // the given object is not derived from MenuC
  
  MenuC::MenuC(WidgetC &widge)
    : ContainerWidgetC(widge)
  {
    if(IsValid()) {
      if(dynamic_cast<const MenuBodyC *>(&(WidgetC::Body())) == 0)
	      Invalidate();
    }
  }


  //////////////////////////////////////////////////
  
  //: Create the widget.
  
  bool MenuBarBodyC::Create() {
    ONDEBUG(std::cerr << "MenuBarBodyC::Create(), Called. \n");
    widget = gtk_menu_bar_new();
    
    for(DLIterC<WidgetC> it(children);it.IsElm();it.Next()) {
      if(!it.Data().Create()) 
	cerr << "MenubarBodyC::Create(), Widget create failed ! \n";
      gtk_widget_show (it.Data().Widget());
      MenuC subMenu(it.Data());
      if(subMenu.IsValid()) {
	GtkWidget *root_menu = gtk_menu_item_new_with_label(subMenu.MenuName());
	gtk_widget_show(root_menu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(root_menu),it.Data().Widget()); 
	gtk_menu_bar_append(GTK_MENU_BAR (widget), root_menu);
      } else
	gtk_menu_bar_append(GTK_MENU_BAR (widget), it.Data().Widget());
    }
    ONDEBUG(std::cerr << "MenuBarBodyC::Create(), Done. \n");  
    return true;
  }
  
  // This will always be invoked by the GUI thread.
  static bool MenuShowWidget(MenuItemC &,WidgetC &ref) { 
    ref.GUIShow(); 
    return true;
  }
  
  //: Function which invokes to show method of a widget.
  MenuItemC MenuItemShow(const StringC &label,const WidgetC &widge) { 
    return MenuItem(label,MenuShowWidget,widge); 
  }
  
  //: Simply call a function.
  MenuItemC MenuItem(const StringC &label,bool (*func)()) {
    MenuItemC ret(label);
    Connect(ret.SigSelected(),func);
    return ret;
  }
  
  static XMLFactoryRegisterHandleConvertC<MenuItemC,WidgetC> g_registerXMLFactoryMenuItem("RavlGUIN::MenuItemC");

}
