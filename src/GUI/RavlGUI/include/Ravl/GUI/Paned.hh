// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_PANED_HEADER 
#define RAVLGUI_PANED_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/Paned.hh"
//! date="08/04/1999"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Base class for all widgets with exactly one child.
  // This is an abstract class.
  
  class PanedBodyC 
    : public WidgetBodyC
  {
  public:
    PanedBodyC(const WidgetC &widge1,const WidgetC &widge2,bool vertical = false);
    //: Constructor.
    
    PanedBodyC()
    {}
    //: Default constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    const WidgetC &Child1() const 
    { return child1; }
    //: Access window contents;
    
    WidgetC &Child1() 
    { return child1; }
    //: Access window contents;

    const WidgetC &Child2() const 
    { return child2; }
    //: Access window contents;
    
    WidgetC &Child2() 
    { return child2; }
    //: Access window contents;
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    // Used to avoid problems with circluar references.
    // This should undo all references to RAVL and GUI
    // objects which are not in the destroy tree. 
    // The function is called by the root window in its
    // destructor.
    
    WidgetC child1;
    WidgetC child2;
    bool vertical;
  };
  
  //! userlevel=Normal
  //: Base class for all widgets with exactly one child.
  // This is an abstract class.
  
  class PanedC
    : public WidgetC
  {
  public:
    PanedC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    PanedC(const WidgetC &child1,const WidgetC &child2,bool vertical = false)
      : WidgetC(*new PanedBodyC(child1,child2,vertical))
    {}
    //: Default constructor.
    // Creates an invalid handle.

  protected:
    PanedC(PanedBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor
    
    PanedC(const WidgetC &bod)
      : WidgetC(dynamic_cast<const PanedBodyC *>(BodyPtr(bod)))
    {}
    //: Base class constructor
    // If object is of wrong type an invalid handle will be
    // created.
    
    PanedBodyC &Body() 
    { return static_cast<PanedBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const PanedBodyC  &Body() const
    { return static_cast<const PanedBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    const WidgetC &Child1() const 
    { return Body().Child1(); }
    //: Access window contents;
    
    const WidgetC &Child2() const 
    { return Body().Child2(); }
    //: Access window contents;
  };

}

#endif
