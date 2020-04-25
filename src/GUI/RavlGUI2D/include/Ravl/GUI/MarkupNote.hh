// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPNOTE_HEADER
#define RAVLGUI_MARKUPNOTE_HEADER 1
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/Polygon2d.hh"

namespace RavlGUIN {
  
  //! userlevel = Develop
  //: Polygon
  
  class MarkupNoteBodyC 
    : public MarkupInfoBodyC
  {
  public:
    MarkupNoteBodyC(Int64T id,IntT zOrder,const StringC &text,const MarkupInfoC &obj,const Vector2dC &offset = Vector2dC(0,0));
    //: Constructor.
    
    virtual RealRange2dC Coverage(GUIMarkupCanvasBodyC &mv) const;
    //: Range of pixels covered by this one (Garanteed) 
    
    virtual RealRange2dC Extent(GUIMarkupCanvasBodyC &mv) const;
    //: Extent of object.
    
    virtual bool Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected);
    //: Method for rendering frame.
    
    virtual bool MouseEvent(GUIMarkupCanvasBodyC &mv,const Point2dC &at,MouseEventTypeT eventType,const MouseEventC &me,IntT &state,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    void SetText(const StringC &newText)
    { text = newText; }
    //: Update text
  protected:
    bool ComputeRectangle(GUIMarkupCanvasBodyC &mv,IndexRange2dC &rect,Point2dC &txtPosition) const;
    //: Compute text rectangle and position
    
    StringC text;
    MarkupInfoC obj;
    Vector2dC offset;
  };
  
  //! userlevel=Normal
  //: Polygon 
  //!cwiz:author
  
  class MarkupNoteC
    : public MarkupInfoC
  {
  public:
    MarkupNoteC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    MarkupNoteC(Int64T id,IntT zOrder,const StringC & text,const MarkupInfoC & obj,const Vector2dC &offset = Vector2dC(0,0)) 
      : MarkupInfoC(*new MarkupNoteBodyC(id,zOrder,text,obj,offset))
    {}
    //: Constructor. 
    //!cwiz:author
    
    RealRange2dC Coverage(GUIMarkupCanvasBodyC & mv) const
    { return Body().Coverage(mv); }
    //: Range of pixels covered by this one (Garanteed)  
    //!cwiz:author
    
    RealRange2dC Extent(GUIMarkupCanvasBodyC & mv) const
    { return Body().Extent(mv); }
    //: Extent of object. 
    //!cwiz:author
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC & area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
    
    bool MouseEvent(GUIMarkupCanvasBodyC & mv,const Point2dC & at,MouseEventTypeT eventType,const MouseEventC & me,IntT & state,bool &refresh) 
    { return Body().MouseEvent(mv,at,eventType,me,state,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    void SetText(const StringC &newText)
    { Body().SetText(newText); }
    //: Update text
    
  protected:
    MarkupNoteC(MarkupNoteBodyC &bod)
     : MarkupInfoC(bod)
    {}
    //: Body constructor. 
    
    MarkupNoteBodyC& Body()
    { return static_cast<MarkupNoteBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
    const MarkupNoteBodyC& Body() const
    { return static_cast<const MarkupNoteBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
  };
  
}



#endif
