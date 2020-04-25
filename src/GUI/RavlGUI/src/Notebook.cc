// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Notebook.cc"
//! docentry="Ravl.Graphics.GTK.Layout"
//! userlevel=Normal

#include "Ravl/GUI/Notebook.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Stream.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  extern "C" 
  {
    void notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num, NotebookBodyC *user_data)
    {
      user_data->SigChanged()(page_num);
    }
  }

  //: Default constructor.
  
  NotebookBodyC::NotebookBodyC(GtkPositionType ntabpos,bool nshowtabs,bool nshowborder)
    : tabpos(ntabpos),
      showborder(nshowborder),
      showtabs(nshowtabs),
      m_sigChanged(true)
  {}
  
  //: Constructor
  
  NotebookBodyC::NotebookBodyC(const DListC<WidgetC> &widges,GtkPositionType ntabpos,bool nshowtabs,bool nshowborder)
    : ContainerWidgetBodyC(widges),
      tabpos(ntabpos),
      showborder(nshowborder),
      showtabs(nshowtabs),
      m_sigChanged(true)
  {}
  
  //: Constructor
  
  NotebookBodyC::NotebookBodyC(const WidgetC &widges,GtkPositionType ntabpos,bool nshowtabs,bool nshowborder)
    : tabpos(ntabpos),
      showborder(nshowborder),
      showtabs(nshowtabs),
      m_sigChanged(true)
  { children.InsLast(widges); }
  
  //: Create the widget.
  
  bool NotebookBodyC::Create() {
    if(widget != 0)
      return true;
    
    widget = gtk_notebook_new();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (widget),tabpos);  
    if(!showtabs)
      gtk_notebook_set_show_tabs(GTK_NOTEBOOK (widget), showtabs);
    if(!showborder)
      gtk_notebook_set_show_border(GTK_NOTEBOOK (widget), showborder);
    
    gtk_widget_show(widget);
    //cerr << "NotebookBodyC::Create(), Creating pages. Widget=" << ((void *) this) << "\n";
    for(DLIterC<WidgetC> it(children);it;it++) {
      WidgetC tab;    
      // Check we have a tab widget.
      if(tabWidges.IsElm(*it))
	tab = tabWidges[*it];
      if(!tab.IsValid()) {
	tab = LabelC("Tmp...");
	tabWidges[*it] = tab;
      }
      FixupPage(*it,tab);
    }
    //cerr << "NotebookBodyC::Create(), Done with pages. \n";
    
    gtk_signal_connect(GTK_OBJECT(widget), "switch-page", GTK_SIGNAL_FUNC(notebook_switch_page), this);
    
    ConnectSignals();
    return true;
  }
  
  //: Create the widget.
  
  bool NotebookBodyC::Create(GtkWidget *_widget)
  {
    widget = _widget;

    if(!showtabs)
      gtk_notebook_set_show_tabs(GTK_NOTEBOOK (widget), showtabs);
    if(!showborder)
      gtk_notebook_set_show_border(GTK_NOTEBOOK (widget), showborder);
    
    gtk_widget_show(widget);

    //cerr << "NotebookBodyC::Create(), Creating pages. Widget=" << ((void *) this) << "\n";
    for(DLIterC<WidgetC> it(children);it;it++) {
      WidgetC tab;    
      // Check we have a tab widget.
      if(tabWidges.IsElm(*it))
	tab = tabWidges[*it];
      if(!tab.IsValid()) {
	tab = LabelC("Tmp...");
	tabWidges[*it] = tab;
      }
      FixupPage(*it,tab);
    }
    //cerr << "NotebookBodyC::Create(), Done with pages. \n";
    
    gtk_signal_connect(GTK_OBJECT(widget), "switch-page", GTK_SIGNAL_FUNC(notebook_switch_page), this);
    
    ConnectSignals();

    return true;
  }
  
  //: Undo all references.
  
  void NotebookBodyC::Destroy() {
    DListC<WidgetC> keys;
    for(HashIterC<WidgetC,WidgetC> it(tabWidges);it;it++) {
      it.Data().Destroy();
      keys += it.Key();
    }
    for(DLIterC<WidgetC> it(keys);it;it++) 
      it.Data().Destroy();
    tabWidges.Empty();
    children.Empty();
    WidgetBodyC::Destroy();
  }

  //: Setup tab widgets.
  // GUI Thread only.
  
  bool NotebookBodyC::GUISetTab(const WidgetC &parent,const WidgetC &tabw) { 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    tabWidges[parent] = tabw; 
    return true;
  }
  
  //: Remove page number 'pageNo'
  // GUI Thread only.
  
  bool NotebookBodyC::GUIRemovePage(const IntT &pageNo) {
    if(widget == 0) {
      std::cerr << "NotebookBodyC::GUIRemovePage(), ERROR: Called before widget initalised. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_notebook_remove_page(GTK_NOTEBOOK (widget),pageNo);
    return true;
  }
  
  
  //: Remove page number 'pageNo'
  
  void NotebookBodyC::RemovePage(const IntT &pageNo) {
    Manager.Queue(Trigger(NotebookC(*this),&NotebookC::GUIRemovePage,pageNo));
  }
  
  
  //: Remove page number 'pageNo'
  // GUI Thread only.
  
  bool NotebookBodyC::GUIRemovePageW(WidgetC &page) {
    if(widget == 0) {
      std::cerr << "NotebookBodyC::GUIRemovePage(), ERROR: Called before widget initalised. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    int pageNo = gtk_notebook_page_num(GTK_NOTEBOOK (widget),page.Widget());
    if(pageNo < 0) {
      std::cerr << "NotebookBodyC::GUIRemovePage(), ERROR: Asked to remove unknown page. \n";
      return true;
    }
    gtk_notebook_remove_page(GTK_NOTEBOOK (widget),pageNo);
    return true;
  }
  
  //: Remove page number 'pageNo'
  
  void NotebookBodyC::RemovePage(WidgetC &pageNo) {
    Manager.Queue(Trigger(NotebookC(*this),&NotebookC::GUIRemovePageW,pageNo));  
  }
  
  //: Do the main bit of AppendPage.
  bool NotebookBodyC::FixupPage(WidgetC &page,WidgetC &tab) {
    //cerr << "NotebookBodyC::FixupPage(), Start. this=" << ((void*) this ) << " Widget=" << page.Hash() << "\n";
    if(!page.GUIShow())
      return false;
    if(!tab.GUIShow())
      return false;
    // Add page.
    //cerr << "NotebookBodyC::FixupPage(), Adding this=" << ((void*) this ) << " Widget=" << page.Hash() << "\n";
    gtk_notebook_append_page(GTK_NOTEBOOK (widget),page.Widget(),tab.Widget());
    return true;
  }

  //: Append a new page.
  
  bool NotebookBodyC::GUIAppendPage(const WidgetC &_page,const WidgetC &_tab) {
    WidgetC page(_page),tab(_tab);
    
    tabWidges[page] = tab;
    children.InsLast(page);
    //cerr << "NotebookBodyC::GUIAppendPage(), this=" << ((void*) this ) << " PageWidget=" << page.Hash() << " Widget=" << widget << "\n";
    if(widget == 0)
      return true;
    return FixupPage(page,tab);
  }
  
  //: Append a new page.
  
  void NotebookBodyC::AppendPage(const WidgetC &page,const WidgetC &tab) {
    //cerr << "NotebookBodyC::AppendPage(), this=" << ((void*) this ) << " Widget=" << page.Hash() << "\n";
    Manager.Queue(Trigger(NotebookC(*this),&NotebookC::GUIAppendPage,
			  const_cast<WidgetC &>(page),const_cast<WidgetC &>(tab))
		  );
  }
  
  //: Show a page from the notebook.
  // GUI Thread only.
  
  bool NotebookBodyC::GUIShowPageNo(IntT pageNo) {
    if(widget == 0) return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_notebook_set_page(GTK_NOTEBOOK (widget),pageNo);
    return true;
  }
  
  //: Show a page from the notebook.
  
  bool NotebookBodyC::ShowPageNo(IntT pageNo) {
    Manager.Queue(Trigger(NotebookC(*this),&NotebookC::GUIShowPageNo,pageNo));
    return true;
  }
  
  //: Show a page from the notebook.
  // GUI Thread only.
  
  bool NotebookBodyC::GUIShowPage(WidgetC &page) {
    if(widget == 0) {
      std::cerr << "NotebookBodyC::GUIShowPage(), ERROR: Called before widget initalised. \n";
      return true;
    }
    if(page.Widget() == 0) {
      ONDEBUG(std::cerr << "NotebookBodyC::GUIShowPage(), Creating page. \n");
      page.Create();
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    int pageNo = gtk_notebook_page_num(GTK_NOTEBOOK (widget),page.Widget());
    if(pageNo < 0) {
      ONDEBUG(std::cerr << "NotebookBodyC::GUIShowPage(), Initalising page. \n");
      WidgetC tab;    
      // Check we have a tab widget.
      if(tabWidges.IsElm(page))
	tab = tabWidges[page];
      if(!tab.IsValid()) {
	tab = LabelC("Tmp...");
	tabWidges[page] = tab;
      }
      GUIAppendPage(page,tab);
      pageNo = gtk_notebook_page_num(GTK_NOTEBOOK (widget),page.Widget());
      if(pageNo < 0) {
	cerr << "NotebookBodyC::GUIShowPage(), Failed to find page for widget. \n";
	return true;
      }
    }
    ONDEBUG(std::cerr << "NotebookBodyC::GUIShowPage(), Showing " << pageNo << "\n");
    gtk_notebook_set_page(GTK_NOTEBOOK (widget),pageNo);
    ONDEBUG(std::cerr << "NotebookBodyC::GUIShowPage(), Done \n");
    return true;
  }
  
  //: Show a page from the notebook.
  
  bool NotebookBodyC::ShowPage(WidgetC &page) {
    Manager.Queue(Trigger(NotebookC(*this),&NotebookC::GUIShowPage,page));
    return true;
  }
  
  //: Get the id of the current page being displayed.
  IntT NotebookBodyC::GetCurrentPage() {
    ReadBackLockC readBackLock; // Lock the GUI thread.
    return gtk_notebook_get_current_page(GTK_NOTEBOOK (widget));
  }

  //: Get the id of a page
  IntT NotebookBodyC::GetPageId(WidgetC & page) {
    ReadBackLockC readBackLock; // Lock the GUI thread.
    return gtk_notebook_page_num(GTK_NOTEBOOK (widget), page.Widget());
  }
  


  ///////////////////////////////////////////
  
  //: Default constructor
  
  NotebookC::NotebookC()
  {}
}
