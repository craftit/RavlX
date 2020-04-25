// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_EVENTBOX_HEADER
#define RAVLGUI_EVENTBOX_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/EventBox.hh"
//! lib=RavlGUI
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! date="17/06/1999"

#include "Ravl/GUI/OneChild.hh"

namespace RavlGUIN {
  //! userlevel=Develop
  //: Event box widget.
  
  class EventBoxBodyC
    : public OneChildBodyC 
  {
  public:
    EventBoxBodyC()
    {}
    //: Default constructor.
    
    EventBoxBodyC(const WidgetC &widge)
      : OneChildBodyC(widge)
    {}
    //: Default constructor.
    
    virtual bool Create();
    //: Create the widget.
  };
  
  //! userlevel=Normal
  //: Event box.
  
  class EventBoxC
    : public OneChildC 
  {
  public:
    EventBoxC()
    {}
    //: Default constructor
    
    EventBoxC(const WidgetC &widge,bool)
      : OneChildC(*new EventBoxBodyC(widge))
    {}
    //: Constructor
  
    EventBoxC(bool)
      : OneChildC(*new EventBoxBodyC())
    {}
    //: Constructor.
    
  protected:
    EventBoxBodyC &Body() 
    { return static_cast<EventBoxBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const EventBoxBodyC &Body() const
    { return static_cast<const EventBoxBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    explicit EventBoxC(const WidgetC &widge)
      : OneChildC(dynamic_cast<const EventBoxBodyC *>(BodyPtr(widge)))
    {}
    //: Base constructor.
    
  };
}

#endif
