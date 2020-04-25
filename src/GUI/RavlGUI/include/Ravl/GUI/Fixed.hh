// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_FIXED_HEADER
#define RAVLGUI_FIXED_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Fixed.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="17/06/99"
//! example=exFixed.cc
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/CWidget.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Threads/MessageQueue.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Hash.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Fixed layout widget.
  
  class FixedWidgetBodyC
    : public ContainerWidgetBodyC
    {
    public:
      FixedWidgetBodyC();
      //: Default constructor.
      
      virtual bool Create();
      //: Create the widget.
      
      bool GUIMove(WidgetC &widge,Index2dC where);
      //: Move a widget.
      // Must be called from the GUI thread.

      bool Move(WidgetC &widge,Index2dC where);
      //: Move a widget.
      
      virtual bool GUIAdd(const WidgetC &widge);
      //: Add a child widget.
      // Must be called from the GUI thread.
      
      bool AddWidget(const WidgetC &newun,Index2dC where);
      //: Add new widget.
      // Thread safe.
      
      bool GUIAddWidget(WidgetC &newun,Index2dC &where);
      //: Add new widget.
      // GUI thread only.
      
    protected:
      virtual Index2dC Position(const WidgetC &widge);
      //: Where should we place this widget.
      
      HashC<WidgetC,Index2dC> where; // Table of widget locations.
      MessageQueueC<Tuple2C<WidgetC,Index2dC> > newuns; // Widgets mods.
      
      int defaultPos;
    };
  
  //! userlevel=Normal
  //: Fixed layout widget.
  
  class FixedWidgetC
    : public ContainerWidgetC
  {
  public:
    FixedWidgetC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    FixedWidgetC(bool)
      : ContainerWidgetC(*new FixedWidgetBodyC())
    {}
    //: Default constructor.
    // Creates an invalid handle.
  
  protected:
    FixedWidgetC(FixedWidgetBodyC &bod)
      : ContainerWidgetC(bod)
    {}
    //: Body constructor.
    
    FixedWidgetBodyC &Body()
      { return static_cast<FixedWidgetBodyC &>(WidgetC::Body()); }
    //: Access body.
  
    const FixedWidgetBodyC &Body() const
      { return static_cast<const FixedWidgetBodyC &>(WidgetC::Body()); }
    //: Access body.  
    
    public:
    bool GUIAddWidget(WidgetC &newun,Index2dC &where)
    { return Body().GUIAddWidget(newun,where); }
    //: Add new widget.
    // GUI thread only.
    
    bool AddWidget(const WidgetC &newun,Index2dC where)
    { return Body().AddWidget(newun,where); }
    //: Add new widget.
    // Thread safe.
    
    bool GUIMove(WidgetC &newun,Index2dC where)
    { return Body().GUIMove(newun,where); }
    //: Move a widget
    // Thread safe.
    
    bool Move(WidgetC &newun,Index2dC where)
    { return Body().Move(newun,where); }
    //: Move a widget
    // Thread safe.
    
    friend class FixedWidgetBodyC;
  };
}

#endif
