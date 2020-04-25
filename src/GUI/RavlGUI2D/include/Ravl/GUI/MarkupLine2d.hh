// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPLINE2D_HEADER
#define RAVLGUI_MARKUPLINE2D_HEADER 1
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Calls.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlGUIN {
  using namespace RavlImageN;
  
  class MarkupLine2dC;
  
  //! userlevel = Develop
  //: Line 2d
  
  class MarkupLine2dBodyC 
    : public MarkupInfoBodyC
  {
  public:
    MarkupLine2dBodyC(Int64T id,IntT overlay,const Point2dC & center,const Vector2dC &normal,RealT length,bool selectable = true);
    //: Constructor.
    
    MarkupLine2dBodyC(Int64T id,IntT overlay,const Point2dC & center,const Vector2dC &normal,RealT length,const ByteRGBValueC &colour,bool selectable = true);
    //: Constructor.
    
    virtual bool Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected);
    //: Method for rendering frame.
    
    virtual RealRange2dC Extent(GUIMarkupCanvasBodyC &mv) const;
    //: The extent

    virtual RealT SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me);
    //: Check if we've selected something in this object.
    //!return: Score on how close the selection is, the smaller the value the better (can be negative.) must be less than 100 to be accepted.

    virtual bool MouseEventPress(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    virtual bool MouseEventMove(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    virtual bool MouseEventRelease(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    Point2dC Position() const
    { return m_centre; }
    //: Access to current position of point
    
    bool Position(const Point2dC & newPosition);
    //: Force update of position
    
    Signal1C<Point2dC>& SigPosition() 
    { return m_sigPosition; }
    //: Signal position change on mouse release

  protected:
    Point2dC m_centre;
    Point2dC m_moveOffset;
    Vector2dC m_normal;
    RealT m_length;
    bool m_selectable;
    
    bool m_setColour;          // Set the colour.
    ByteRGBValueC m_colour;   // Colour to use.
    
    Signal1C<Point2dC> m_sigPosition;
  };
  
  //! userlevel=Normal
  //: Line 2d Markup
  //!cwiz:author
  
  class MarkupLine2dC
    : public MarkupInfoC
  {
  public:
    MarkupLine2dC()
    {}
    //: Default constructor
    // Creates in invalid handle.
    
    MarkupLine2dC(Int64T id,IntT overlay, const Point2dC & center,const Vector2dC &normal,RealT length,bool selectable = true) 
      : MarkupInfoC(*new MarkupLine2dBodyC(id,overlay,center,normal,length,selectable))
    {}
    //: Constructor. 
    //!cwiz:author

    MarkupLine2dC(Int64T id,IntT overlay, const Point2dC & center,const Vector2dC &normal,RealT length,const ByteRGBValueC &colour,bool selectable = true) 
      : MarkupInfoC(*new MarkupLine2dBodyC(id,overlay,center,normal,length,colour,selectable))
    {}
    //: Constructor. 
    //!cwiz:author
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC &area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
  protected:
    MarkupLine2dC(MarkupLine2dBodyC &bod)
     : MarkupInfoC(bod)
    {}
    //: Body constructor. 
    
    MarkupLine2dBodyC& Body()
    { return static_cast<MarkupLine2dBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
    const MarkupLine2dBodyC& Body() const
    { return static_cast<const MarkupLine2dBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 

  public:

    Point2dC Position() const
      { return Body().Position(); }
    //: Access to the current position point

    bool Position(const Point2dC & newPosition) 
      { return Body().Position(newPosition); }
    //: Force update of position

    Signal1C<Point2dC>&SigPosition() 
      { return Body().SigPosition(); }
    //: Signal position change on mouse release
    
  };
  
  
}


#endif
