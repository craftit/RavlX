// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_ASPECTFRAME_HEADER
#define RAVLGUI_ASPECTFRAME_HEADER 1
//////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/AspectFrame.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="7/10/2003"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! example=exFrame.cc

#include "Ravl/GUI/Frame.hh"
#include "Ravl/String.hh"

namespace RavlGUIN {
  
  class AspectFrameC;
  
  //! userlevel=Develop
  //: Put a frame around some other widgets.
  
  class AspectFrameBodyC
    : public FrameBodyC
  {
  public:
    AspectFrameBodyC();
    //: Default constructor.
    
    AspectFrameBodyC(const WidgetC &widge,RealT aspect,int nborder = 0,const StringC &ntitle = "");
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    bool GUIAspect(RealT ratio);
    //: Set aspect ratio.
    // GUI thread only.
    
    bool Aspect(RealT ratio);
    //: Set aspect ratio.
    
    RealT Aspect() const
    { return aspect; }
    //: Get current aspect ratio.
    // Only really safe on the GUI thread.
    
    void GUIObeyChild(bool obeyChild);
    //: Set obey child.
    
    void SetUseTrueAspect(bool _useTrueAspect)
    { useTrueAspect = _useTrueAspect; }
    //: Set use true aspect ratio flag
    
  protected:
    RealT alignx;
    RealT aligny;
    RealT screenCorrection;
    RealT aspect;
    bool obeyChild;
    bool useTrueAspect; // Display so aspect is relative screen, not pixels
    friend class AspectFrameC;
  };
  
  //! userlevel=Normal
  //: Put a frame around some other widgets.
  
  class AspectFrameC
    : public FrameC
  {
  public:
    AspectFrameC()
    {}
    //: Default constructor.

    AspectFrameC(const WidgetC &widge,RealT aspect,const StringC &title,int border = 2)
      : FrameC(*new AspectFrameBodyC(widge,aspect,border,title))
      {}
    //: Constructor.
    
    AspectFrameC(const WidgetC &widge,RealT aspect,int border)
      : FrameC(*new AspectFrameBodyC(widge,aspect,border))
      {}
    //: Constructor.
    
  protected:
    AspectFrameC(AspectFrameBodyC &body)
      : FrameC(body)
    {}
    //: Body constructor.
    
    AspectFrameBodyC &Body() 
    { return static_cast<AspectFrameBodyC &>(WidgetC::Body()); }
    //: Access body.

    const AspectFrameBodyC &Body() const
    { return static_cast<const AspectFrameBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    AspectFrameC(const WidgetC &widge)
      : FrameC(dynamic_cast<const AspectFrameBodyC *>(BodyPtr(widge)))
    {}
    //: Base constructor.
    // If 'widge' is a AspectFrameC this creates a handle to it, if it is not
    // an invalid handle is created.
    
    bool GUIAspect(RealT ratio)
    { return Body().GUIAspect(ratio); }
    //: Set aspect ratio.
    // GUI thread only.
    
    bool Aspect(RealT ratio)
    { return Body().Aspect(ratio); }
    //: Set aspect ratio.
    
    RealT Aspect() const
    { return Body().Aspect(); }
    //: Get current aspect ratio.
    // Only really safe on the GUI thread.
    
    void GUIObeyChild(bool obeyChild)
    { Body().GUIObeyChild(obeyChild); }
    //: Set obey child.
    
    void SetUseTrueAspect(bool _useTrueAspect)
    { Body().SetUseTrueAspect(_useTrueAspect); }
    //: Set use true aspect ratio flag
    
    friend class AspectFrameBodyC;
  };

}
#endif
