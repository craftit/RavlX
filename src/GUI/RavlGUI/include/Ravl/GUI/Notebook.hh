// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_NOTEBOOK_HEADER
#define RAVLGUI_NOTEBOOK_HEADER 1
////////////////////////////////////////////
//! file="Ravl/GUI/GTK/Notebook.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! rcsid="$Id$"
//! example=exNotebook.cc
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/CWidget.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Threads/Signal1.hh"

namespace RavlGUIN 
{

  class NotebookC;
  
  //! userlevel=Develop
  //: Notebook
  
  class NotebookBodyC 
    : public ContainerWidgetBodyC 
  {
  public:
    NotebookBodyC(GtkPositionType ntabpos = GTK_POS_TOP,bool nshowtabs = true,bool nshowborder = true);
    //: Default constructor.
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOSITIONTYPE">here</A>.
    
    NotebookBodyC(const DListC<WidgetC> &widges,GtkPositionType ntabpos =GTK_POS_TOP,bool nshowtabs = true,bool nshowborder = true);
    //: Constructor
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOSITIONTYPE">here</A>.
  
    NotebookBodyC(const WidgetC &widges,GtkPositionType ntabpos =GTK_POS_TOP,bool nshowtabs = true,bool nshowborder = true);
    //: Constructor
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOSITIONTYPE">here</A>.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    bool GUISetTab(const WidgetC &parent,const WidgetC &tabw);
    //: Setup tab widgets.
    // GUI Thread only.
    
    bool GUIRemovePage(const IntT &pageNo);
    //: Remove page number 'pageNo'
    // GUI Thread only.
    
    void RemovePage(const IntT &pageNo);
    //: Remove page number 'pageNo'
  
    bool GUIRemovePageW(WidgetC &page);
    //: Remove page number 'pageNo'
    // GUI Thread only.
    
    void RemovePage(WidgetC &pageNo);
    //: Remove page number 'pageNo'
    
    bool GUIAppendPage(const WidgetC &page,const WidgetC &tab);
    //: Append a new page.
    
    void AppendPage(const WidgetC &page,const WidgetC &tab);
    //: Append a new page.
    
    bool GUIShowPage(WidgetC &page);
    //: Show a page from the notebook.
    // GUI Thread only.
    
    bool ShowPage(WidgetC &page);
    //: Show a page from the notebook.
    
    bool GUIShowPageNo(IntT pageNo);
    //: Show a page from the notebook.
    // GUI Thread only.
    
    bool ShowPageNo(IntT pageNo);
    //: Show a page from the notebook.
    
    IntT GetCurrentPage();
    //: Get the id of the current page being displayed.
    
    IntT GetPageId(WidgetC &page);
    //: Get the id of the page

    Signal1C<IntT> &SigChanged()
    { return m_sigChanged; }
    //: Access page changed signal.
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    
    bool FixupPage(WidgetC &page,WidgetC &tab);
    //: Do the main bit of AppendPage.
    
    HashC<WidgetC,WidgetC> tabWidges;
    
    GtkPositionType tabpos;
    bool showborder;
    bool showtabs;
    
    Signal1C<IntT> m_sigChanged;
    
    friend class NotebookC;
  };
  
  //! userlevel=Normal
  //: Notebook
  
  class NotebookC 
    : public ContainerWidgetC 
  {
  public:
    NotebookC();
    //: Default constructor
    // Creates an invalid handle.
    
    NotebookC(GtkPositionType ntabpos,bool nshowtabs = true,bool nshowborder = true)
      : ContainerWidgetC(*new NotebookBodyC(ntabpos,nshowtabs,nshowborder))
    {}
    //: Constructor
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/2.10/gtk-Standard-Enumerations.html#GtkPositionType">here</A>.
  
    NotebookC(const DListC<WidgetC> &widges,GtkPositionType ntabpos = GTK_POS_TOP,bool nshowtabs = true,bool nshowborder = true)
      : ContainerWidgetC(*new NotebookBodyC(widges,ntabpos,nshowtabs,nshowborder))
    {}
    //: Constructor
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/2.10/gtk-Standard-Enumerations.html#GtkPositionType">here</A>.
    
    NotebookC(const WidgetC &widges,GtkPositionType ntabpos = GTK_POS_TOP,bool nshowtabs = true,bool nshowborder = true)
      : ContainerWidgetC(*new NotebookBodyC(widges,ntabpos,nshowtabs,nshowborder))
    {}
    //: Constructor
    // Values for position types are listed <A HREF="http://library.gnome.org/devel/gtk/2.10/gtk-Standard-Enumerations.html#GtkPositionType">here</A>.
  
  protected:
    NotebookC(NotebookBodyC &bod)
      : ContainerWidgetC(bod)
    {}
    //: Body construcotr.
  
    NotebookBodyC &Body() 
    { return static_cast<NotebookBodyC &>(WidgetC::Body()); }
    //: Access body.

    const NotebookBodyC &Body() const
    { return static_cast<const NotebookBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool GUISetTab(const WidgetC &parent,const WidgetC &tabw)
    { return Body().GUISetTab(parent,tabw); }
    //: Setup tab widgets.

    bool GUIRemovePage(const IntT &pageNo)
    { return Body().GUIRemovePage(pageNo); }
    //: Remove page number 'pageNo'
    // GUI Thread only.
    
    void RemovePage(const IntT &pageNo)
    { Body().RemovePage(pageNo); }
    //: Remove page number 'pageNo'

    bool GUIRemovePageW(WidgetC &page)
    { return Body().GUIRemovePageW(page); }
    //: Remove page by widget.
    // GUI Thread only.
    
    void RemovePage(WidgetC &page)
    { Body().RemovePage(page); }
    //: Remove page by widget.
    
    bool GUIAppendPage(const WidgetC &page,const WidgetC &tab)
    { return Body().GUIAppendPage(page,tab); }
    //: Append a new page.
    // GUI Thread only.
    
    void AppendPage(const WidgetC &page,const WidgetC &tab)
    { Body().AppendPage(page,tab); }
    //: Append a new page.
    
    bool GUIShowPage(WidgetC &page)
    { return Body().GUIShowPage(page); }
    //: Show a page from the notebook.
    // GUI Thread only.
    
    bool ShowPage(WidgetC &page)
    { return Body().ShowPage(page); }
    //: Show a page from the notebook.

    bool GUIShowPageNo(IntT pageNo)
    { return Body().GUIShowPageNo(pageNo); }
    //: Show a page from the notebook.
    // GUI Thread only.
    
    bool ShowPageNo(IntT pageNo)
    { return Body().ShowPageNo(pageNo); }
    //: Show a page from the notebook.
    
    IntT GetCurrentPage()
    { return Body().GetCurrentPage(); }
    //: Get the id of the current page being displayed.

    IntT GetPageId(WidgetC &page) 
    { return Body().GetPageId(page); }
    //: Get the id of the page

    Signal1C<IntT> &SigChanged()
    { return Body().SigChanged(); }
    //: Access page changed signal.
    
    friend class NotebookBodyC;
  };
  
}

#endif
