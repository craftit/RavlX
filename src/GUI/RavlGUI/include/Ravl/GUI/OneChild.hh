// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_ONECHILD_HEADER 
#define RAVLGUI_ONECHILD_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Internal"
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/OneChild.hh"
//! date="08/04/1999"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Base class for all widgets with exactly one child.
  // This is an abstract class.
  
  class OneChildBodyC 
    : public WidgetBodyC
  {
  public:
    OneChildBodyC(const WidgetC &widge);
    //: Constructor.
    
    OneChildBodyC()
    {}
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.
    
    const WidgetC &Child() const { return child; }
    //: Access window contents;
    
    WidgetC &Child() { return child; }
    //: Access window contents;
    
    bool Add(const WidgetC &nchild);
    //: Add widget.
    
  protected:
    virtual void Destroy();
    //: Undo all references.
    // Used to avoid problems with circluar references.
    // This should undo all references to RAVL and GUI
    // objects which are not in the destroy tree. 
    // The function is called by the root window in its
    // destructor.
    
    WidgetC child;
  };
  
  //! userlevel=Normal
  //: Base class for all widgets with exactly one child.
  // This is an abstract class.
  
  class OneChildC
    : public WidgetC
  {
  public:
    OneChildC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

  protected:
    OneChildC(OneChildBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor
    
    OneChildC(const OneChildBodyC *bod)
      : WidgetC(bod)
    {}
    //: Body constructor
    
    OneChildC(WidgetC &widge)
      : WidgetC(dynamic_cast<const WidgetBodyC *>(BodyPtr(widge)))
    {}
    //: Base class constructor
    // Casts 'widge' to a 'OneChildC', if types don't match an invalid handle 
    // will be created.
    
    OneChildBodyC &Body() 
    { return static_cast<OneChildBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const OneChildBodyC  &Body() const
    { return static_cast<const OneChildBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    
    bool Add(const WidgetC &widge) 
    { return Body().Add(widge); }
    //: Add widget.
    // Only has effect before create is called.
    
    const WidgetC &Child() const 
    { return Body().Child(); }
    //: Access window contents;
  };

}

#endif
