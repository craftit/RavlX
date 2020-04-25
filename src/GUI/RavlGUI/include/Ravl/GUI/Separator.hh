// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_SEPARATOR_HEADER
#define RAVLGUI_SEPARATOR_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Separator.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Vertical or horizontal separator.
  
  class SeparatorBodyC
    : public WidgetBodyC
  {
  public:
    SeparatorBodyC(bool nvert)
      : vert(nvert)
      {}
    //: Constructor.
    
    virtual ~SeparatorBodyC() { }
    //: Have virtual methods - need virtual destructor

    virtual bool Create();
    //: Create the widget.
    
  protected:
    bool vert;
  };
  
  //! userlevel=Normal
  //: Vertical or horizontal separator.
  
  class SeparatorC
    : public WidgetC
  {
  public:
    SeparatorC()
    {}
    //: Default constructor.
    
    SeparatorC(bool nvert)
      : WidgetC(*new SeparatorBodyC(nvert))
      {}
    //: Construct a separator.
    
  };

  inline 
  SeparatorC Separator(bool vert) { 
    return SeparatorC(vert);
  }
  //: Create a separator.

  inline 
  SeparatorC VSeparator() { 
    return SeparatorC(true);
  }
  //: Create a vertical separator.

  inline 
  SeparatorC HSeparator() { 
    return SeparatorC(false);
  }
  //: Create a horizontal separator.
  
}



#endif
