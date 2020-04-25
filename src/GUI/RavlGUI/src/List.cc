// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/List.cc"

#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/GUI/List.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  static const char *ListItemKey = "ListDataKey";
  
  //: Constructor.
  
  ListBodyC::ListBodyC(const DListC<Tuple2C<IntT,StringC> > &nChoices,GtkSelectionMode nSelMode)
    : selMode(nSelMode)
  {
    //signals["list_activate"] = Signal1C<StringC>("-none-");
    for(ConstDLIterC<Tuple2C<IntT,StringC> > it(nChoices);it.IsElm();it.Next())
      children.InsLast(Tuple2C<IntT,WidgetC>(it->Data1(),LabelC(it->Data2())));
  }
  
  //: Constructor from a list of strings.
  
  ListBodyC::ListBodyC(const DListC<Tuple2C<IntT,WidgetC> > &lst,GtkSelectionMode nSelMode)
    : children(lst),
      selMode(nSelMode)    
  {}
  
  ListBodyC::ListBodyC(GtkSelectionMode nSelMode)
    : selMode(nSelMode)
  {
    //signals["list_activate"] = Signal1C<StringC>("-none-");  
  }

  //: Get currently selected string.
  
  DListC<IntT> ListBodyC::Selected() const {
    DListC<IntT> ret; 
    GList   *dlist=GTK_LIST(widget)->selection;
    
    while (dlist) {
      GtkObject       *list_item;
      
      list_item=GTK_OBJECT(dlist->data);
#if RAVL_CPUTYPE_64
      IntT id =(Int64T)gtk_object_get_data(list_item, ListItemKey); 
#else
      IntT id =(IntT)gtk_object_get_data(list_item, ListItemKey); 
#endif
      
      ret.InsLast(id);
      dlist=dlist->next;
    }
    return ret;
  }
  
  //: Add new widget to list.
  
  void ListBodyC::AppendLine(IntT id,WidgetC &widge) {
    Manager.Queue(Trigger(ListC(*this),&ListC::GUIAppendLine,id,widge));
  }
  
  //: Del string from list.
  
  void ListBodyC::RemoveLine(IntT id) {
    Manager.Queue(Trigger(ListC(*this),&ListC::GUIRemoveLine,id));
  }
  
  //: Add new string to window.
  
  void ListBodyC::AppendLine(IntT id,const StringC &text)  { 
    LabelC lab(text);
    AppendLine(id,lab);
  }
  
  //: Add new widget to list.
  
  bool ListBodyC::GUIAppendLine(const IntT &id,WidgetC &widge) {
    if(widget == 0) { // List created yet ?
      children.InsLast(Tuple2C<IntT,WidgetC>(id,widge));
      return true;
    }
    if(widge.Widget() == 0) {
      if(!widge.Create()) {
	cerr << "WARNING: Failed to create list widget.\n";
	return false; // Can't continue.
      }
    }
    GtkWidget *li = gtk_list_item_new();
    gtk_container_add(GTK_CONTAINER(li), widge.Widget());
    gtk_widget_show(widge.Widget());
    gtk_container_add (GTK_CONTAINER (widget), li);
    gtk_widget_show (li);
    gtk_object_set_data(GTK_OBJECT(li),
			ListItemKey,
			(void *) id);
    
    ONDEBUG(std::cerr << "Added " << widge.Name() << " \n");
    return true;
  }
  
  //: Del widget from list.
  
  bool ListBodyC::GUIRemoveLine(const IntT &id) {
    //ONDEBUG(std::cerr << "CListBodyC::GUIRemoveLine(), ID:" << id << " \n");
    GList   *dlist=GTK_LIST(widget)->children;    
    while (dlist) {
      GtkObject       *list_item;
      
      list_item=GTK_OBJECT(dlist->data);
#if RAVL_CPUTYPE_64
      IntT rid =(Int64T)gtk_object_get_data(list_item,ListItemKey); 
#else
      IntT rid =(IntT)gtk_object_get_data(list_item,ListItemKey); 
#endif      

if(rid == id) {	
	GList   static_dlist;
	static_dlist.data=list_item;
	static_dlist.next=NULL;
	static_dlist.prev=NULL;
	gtk_list_remove_items (GTK_LIST(widget),&static_dlist);
	return true;
      }
      dlist=dlist->next;
    }
    return true;
  }
  
  //: Create the widget.
  
  bool ListBodyC::Create() {
    if(widget != 0)
      return true; // Done already!
    ONDEBUG(std::cerr << "ListBodyC::Create(), Called. \n");
    widget = gtk_list_new();
    gtk_list_set_selection_mode(GTK_LIST(widget),selMode);  
    for(DLIterC<Tuple2C<IntT,WidgetC> > it(children);it;it++)
      GUIAppendLine(it->Data1(),it->Data2());
    //gtk_signal_connect(GTK_OBJECT(GTK_COMBO(widget)->entry), "changed",
    //GTK_SIGNAL_FUNC (combo_activate),& signals["combo_activate"]);
    ConnectSignals();
    ONDEBUG(std::cerr << "ListBodyC::Create(), Done. \n");
    return true;
  }
  
  //: Undo all references.
  
  void ListBodyC::Destroy()  {
    for(DLIterC<Tuple2C<IntT,WidgetC> > it(children);it;it++)
      it->Data2().Destroy();
    WidgetBodyC::Destroy();
  }

}

