// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_LBOX_HEADER
#define RAVLGUI_LBOX_HEADER 1
////////////////////////////////////////////
//! file="Ravl/GUI/GTK/LBox.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! rcsid="$Id$"
//! example=exLBox.cc

#include "Ravl/GUI/CWidget.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Layout box
  
  class LBoxBodyC 
    : public ContainerWidgetBodyC 
  {
  public:
    LBoxBodyC()
      : vert(true),
        border(0),
        homogeneous(false),
        spacing(0)
    {}
    //: Default constructor.
    
    LBoxBodyC(const DListC<WidgetC> &widges,bool nvert = false,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetBodyC(widges),
        vert(nvert),
        border(nborder),
        homogeneous(nhomogeneous),
        spacing(nspacing)
    {}
    //: Constructor
    // Constructs box of list of widgets
    
    LBoxBodyC(const WidgetC &widges,bool nvert = false,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetBodyC(),
        vert(nvert),
        border(nborder),
        homogeneous(nhomogeneous),
        spacing(nspacing)
    { children.InsLast(widges); }
    //: Constructor
    // Constructs box around single widget
    
    LBoxBodyC(bool nvert,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetBodyC(),
        vert(nvert),
        border(nborder),
        homogeneous(nhomogeneous),
        spacing(nspacing)
    {}
    //: Constructor
    // Constructs empty box
    
    virtual bool Create()
    { return CommonCreate(); }
    //: Create widget.
    
    virtual bool Create(GtkWidget *_widget)
    {return CommonCreate(_widget);}
    //: Create with a widget supplied from elsewhere.
    
    virtual bool GUIAdd(const WidgetC &widge);
    //: Add widget.
    // GUI thread only.
    
  protected:
    bool CommonCreate(GtkWidget *_widget = NULL);
    //: Common widget create function
    
    virtual void Destroy()
    { ContainerWidgetBodyC::Destroy(); }
    //: Undo all references.
    
    bool vert; // Vertical layout ?
    IntT border;
    bool homogeneous;
    IntT spacing;
  };
  
  //! userlevel=Normal
  //: Layout box
  // A box that can contain a horizontal (default) or vertical list of widgets

  class LBoxC 
    : public ContainerWidgetC 
  {
  public:
    LBoxC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    LBoxC(const DListC<WidgetC> &widges,bool vert = false,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetC(*new LBoxBodyC(widges,vert,nborder,nhomogeneous,nspacing))
    {}
    //: Constructor
    // Constructs box containing widgets from list

    LBoxC(const WidgetC &widget,bool vert = false,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetC(*new LBoxBodyC(widget,vert,nborder,nhomogeneous,nspacing))
    {}
    //:  Constructor
    // Constructs box around single widget.
    
    LBoxC(bool vert,IntT nborder = 0,bool nhomogeneous = false,IntT nspacing = 0)
      : ContainerWidgetC(*new LBoxBodyC(vert,nborder,nhomogeneous,nspacing))
    {}
    //:  Constructor
    // Constructs empty box for widgets
    // If 'vert' is true a vertical layout box is created.
    
  protected:
    LBoxC(LBoxBodyC &bod)
      : ContainerWidgetC(bod)
    {}
    //: Body constructor.
    
    LBoxBodyC &Body()
    { return static_cast<LBoxBodyC &>(WidgetC::Body()); }
    //: Access body.

    const LBoxBodyC &Body() const
    { return static_cast<const LBoxBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  };
  
  inline 
  LBoxC VBox(const DListC<WidgetC> &widges,int border = 0,bool nhomogeneous = false,IntT nspacing = 0)
  { return LBoxC(widges,true,border,nhomogeneous,nspacing); }
  //: Lays out widgets in a vertical box.
  // Helper function calls: <a href="RavlGUIN.LBoxC.html">LBoxC</a>(widges,true,border,nhomogeneous,nspacing)

  inline 
  LBoxC VBox(const WidgetC &widge,int border = 0,bool nhomogeneous = false,IntT nspacing = 0)
  { 
    DListC<WidgetC> widges;
    widges.InsLast(widge);
    return LBoxC(widges,true,border,nhomogeneous,nspacing); }
  //: Puts a widget in a vertical box.
  // Helper function calls: <a href="RavlGUIN.LBoxC.html">LBoxC</a>(widge,true,border,nhomogeneous,nspacing)
  
  inline 
  LBoxC HBox(const DListC<WidgetC> &widges,int border = 0,bool nhomogeneous = false,IntT nspacing = 0)
  { return LBoxC(widges,false,border,nhomogeneous,nspacing); }
  //: Lays out widgets in a horizontal box.
  // Helper function calls: <a href="RavlGUIN.LBoxC.html">LBoxC</a>(widges,false,border,nhomogeneous,nspacing)

  inline
  LBoxC HBox(const WidgetC &widge,int border = 0,bool nhomogeneous = false,IntT nspacing = 0)
  { 
    DListC<WidgetC> widges;
    widges.InsLast(widge);
    return LBoxC(widges,false,border,nhomogeneous,nspacing); 
  }
  //: Puts a widget in a horizontal box.
  // Helper function calls: <a href="RavlGUIN.LBoxC.html">LBoxC</a>(widge,false,border,nhomogeneous,nspacing)

  inline 
  LBoxC Box(const WidgetC &widge,int border = 0,bool nhomogeneous = false,IntT nspacing = 0)
  { return LBoxC(widge,false,border,nhomogeneous,nspacing); }
  //: Put a border around a widget.
  // Helper function calls: <a href="RavlGUIN.LBoxC.html">LBoxC</a>(widge,false,0,nhomogeneous,nspacing)
  
}

#endif
