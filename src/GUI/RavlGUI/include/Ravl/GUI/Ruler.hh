// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIRULER_HEADER
#define RAVLGUIRULER_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Ruler.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/09/99"
//! example=exRuler.cc
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Ruler body.
  
  class RulerBodyC
    : public WidgetBodyC
  {
  public:
    RulerBodyC(bool nVert,RealT nlower,RealT nupper,RealT npos,RealT nmaxSize)
      : vert(nVert),
      lower(nlower),
      upper(nupper),
      position(npos),
      max_size(nmaxSize)
    {}
    //: Ruler body
    
    
    bool GUIAttachTo(WidgetC &widge);
    //: Get motion events from widge.
    // GUI Thread only.
    
    bool GUISetRange(RealT &lower,RealT &upper);
    //: Set range of ruler.
    // GUI Thread only.
    
    bool GUISetRangeAndPosition(RealT &lower,RealT &upper,RealT &pos);
    //: Set range and position for the ruler.
    // GUI Thread only.
    
  protected:
    virtual bool Create();
    //: Create widget
    
    bool vert;  // true = Vertical.
    RealT lower;    // The upper limit of the ruler (in points) 
    RealT upper;    // The lower limit of the ruler 
    RealT position; // The position of the mark on the ruler 
    RealT max_size; // The maximum size of the ruler 
  };
  
  
  //! userlevel=Normal
  //: Ruler .
  
  class RulerC
    : public WidgetC
  {
  public:
    RulerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    RulerC(bool nVert,RealT nlower = 0,RealT nupper = 1,RealT npos = 0.5,RealT nmaxSize = 1)
      : WidgetC(* new RulerBodyC(nVert,nlower,nupper,npos,nmaxSize))
    {}
    //: Constructor.
    
  protected:
    RulerBodyC &Body() 
    { return static_cast<RulerBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const RulerBodyC &Body() const
    { return static_cast<const RulerBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    
  public:
    bool GUIAttachTo(WidgetC &widge)
    { return Body().GUIAttachTo(widge); }
    //: Get motion events from widge.
    // GUI Thread only.
    
    bool GUISetRange(RealT &lower,RealT &upper)
    { return Body().GUISetRange(lower,upper); }
    //: Set range of ruler.
    // GUI Thread only.
    
    bool GUISetRangeAndPosition(RealT &lower,RealT &upper,RealT &pos)
    { return Body().GUISetRangeAndPosition(lower,upper,pos); }
    //: Set range and position for the ruler.
    // GUI Thread only.
    
    bool AttachTo(WidgetC &widge);
    //: Get motion events from 'widge', for ruler position. 
    // Thread safe.
    
    void SetRange(RealT lower,RealT upper);
    //: Set range for ruler.
    // Thread safe.
  };
}
#endif
