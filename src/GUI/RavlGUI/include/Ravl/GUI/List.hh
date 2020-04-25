// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_LIST_HEADER
#define RAVLGUI_LIST_HEADER 1
//////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/List.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="02/07/1999"
//! example=exList.cc
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/GTKTypes.hh"
#include "Ravl/GUI/CWidget.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/String.hh"

namespace RavlGUIN {

  class ListC;
  
  
  //! userlevel=Develop
  //: List body.
  // NOTE: This widget has been replaced by CListC. Use CListC in all new code. <br>
  // GtkSelectionMode:
  //  GTK_SELECTION_SINGLE
  //  GTK_SELECTION_BROWSE
  //  GTK_SELECTION_MULTIPLE
  //  GTK_SELECTION_EXTENDED
  
  class ListBodyC 
    : public WidgetBodyC
  {
  public:
    ListBodyC(const DListC<Tuple2C<IntT,StringC> > &choices,GtkSelectionMode selMode = GTK_SELECTION_SINGLE);
    //: Constructor from a list of strings.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    ListBodyC(const DListC<Tuple2C<IntT,WidgetC> > &lst,GtkSelectionMode selMode = GTK_SELECTION_SINGLE);
    //: Constructor from a list of strings.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    ListBodyC(GtkSelectionMode selMode = GTK_SELECTION_SINGLE);
    //: Constructor from a list of strings.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    virtual ~ListBodyC() { }
    //: Have virtual methods - need virtual destructor

    virtual bool Create();
    //: Create the widget.
    
    DListC<IntT> Selected() const;
    //: Get current selection
    // Should only be called by the GUI thread !
    
    bool GUIAppendLine(const IntT &id,WidgetC &widge);
    //: Add new widget to list.
    
    bool GUIRemoveLine(const IntT &id);
    //: Del line from list.
    
    void AppendLine(IntT id,const StringC &text);
    //: Add new string to list.
    
    void AppendLine(IntT id,WidgetC &widge);
    //: Add new widget to list.
    
    void RemoveLine(IntT id);
    //: Delete line id from list.
    
    void RemoveLine(WidgetC &widge);
    //: Del widget from list.
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    
    DListC<Tuple2C<IntT,WidgetC> > children;
    GtkSelectionMode selMode;
    friend class ListC;
  };
  
  //! userlevel=Obsolete
  //: List widget.
  // NOTE: This widget has been replaced by CListC. Use CListC in all new code.
  
  class ListC
    : public WidgetC
  {
  public:
    ListC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ListC(DListC<Tuple2C<IntT,StringC> > lst,GtkSelectionMode selMode = GTK_SELECTION_SINGLE)
      : WidgetC(*new ListBodyC(lst,selMode))
    {}
    //: Create a List box.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
    
    ListC(const DListC<Tuple2C<IntT,WidgetC> > &lst,GtkSelectionMode selMode = GTK_SELECTION_SINGLE)
      : WidgetC(*new ListBodyC(lst,selMode))
    {}
    //: Constructor from a list of strings.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
  
    ListC(GtkSelectionMode selMode)
      : WidgetC(*new ListBodyC(selMode))
    {}
    //: Create an empty List box.
    // Values for selMode are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKSELECTIONMODE">here</A>.
  
  protected:
    ListC(ListBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    ListBodyC &Body()
    { return static_cast<ListBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ListBodyC &Body() const
    { return static_cast<const ListBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool GUIAppendLine(const IntT &id,WidgetC &widget)
    { return Body().GUIAppendLine(id,widget); }
    //: Add new string to window.
    // GUI Thread only.
    
    bool GUIRemoveLine(const IntT &id)
    { return Body().GUIRemoveLine(id); }
    //: Add new string to window.
    // GUI Thread only.
    
  public:
    Signal0C &SigSelected()
    { return Body().Signal("list_activate"); }
    //: Short cut clicked signal.
    
    DListC<IntT> Selected() const 
    { return Body().Selected(); }
    //: Get currently selected string.
    // Should only be called by the GUI thread !
    
    void AppendLine(IntT id,const StringC &text)
    { Body().AppendLine(id,text); }
    //: Add new string to window.
    // Thread safe.

    bool AddString(IntT id,const StringC &text) { 
      Body().AppendLine(id,text); 
      return true;
    }
    //: Add new string to window.
    // Thread safe.
    // This is setup to be easly callable from a signal.
    
    void AppendLine(IntT id,WidgetC &text)
    { Body().AppendLine(id,text); }
    //: Add new string to window.
    // Thread safe.
    
    void RemoveLine(IntT line)
    { Body().RemoveLine(line); }
    //: Add new string to window.
    // Thread safe.
    
    void RemoveLine(WidgetC &obj)
    { Body().RemoveLine(obj); }
    //: Add new string to window.
    // Thread safe.
    
    friend class ListBodyC;
  };
  
  
  template<class DataT>
  ListC List(const DListC<Tuple2C<IntT,StringC> > &lst,void (*func)(ListC &, DataT &ref),const DataT &dat = DataT()) { 
    ListC ret(lst);
    Connect(ret.SigSelected(),func,ret,dat);
    return ret;    
  }  

  template<class DataT>
  ListC List(const DListC<Tuple2C<IntT,StringC> > &lst,const DataT &dat,void (DataT::*func)(StringC &ref)) { 
    ListC ret(lst);
    Connect(ret.SigSelected(),dat,func);
    return ret;    
  }  
}



#endif
