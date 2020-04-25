// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPPOINT2D_HEADER
#define RAVLGUI_MARKUPPOINT2D_HEADER 1
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Calls.hh"

namespace RavlGUIN {

  enum MarkupPoint2dStyleT
    { MP2DS_Eye,
      MP2DS_CrossHair,
      MP2DS_DiagonalCross };

  class MarkupPoint2dC;
  
  //! userlevel = Develop
  //: Polygon
  
  class MarkupPoint2dBodyC 
    : public MarkupInfoBodyC
  {
  public:
    MarkupPoint2dBodyC(Int64T id,IntT zOrder,const Point2dC & position,MarkupPoint2dStyleT style = MP2DS_Eye,bool isFixed = false);
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
      { return centre; }
    //: Access to current position of point

    bool Position(const Point2dC & newPosition);
    //: Force update of position
    
    Signal1C<Point2dC>& SigPosition()
    { return sigPosition; }
    //: Signal position change on mouse release
    
  protected:
    Point2dC centre;
    Point2dC oldPosition;    
    Signal1C<Point2dC> sigPosition;
    MarkupPoint2dStyleT style;
    bool m_fixed; // Is the point fixed, i.e. not movable
  };
  
  //! userlevel=Normal
  //: Polygon 
  //!cwiz:author
  
  class MarkupPoint2dC
    : public MarkupInfoC
  {
  public:
    MarkupPoint2dC()
    {}
    //: Default constructor
    // Creates in invalid handle.
    
    MarkupPoint2dC(Int64T id,IntT zOrder,const Point2dC & position,MarkupPoint2dStyleT style = MP2DS_Eye,bool isFixed = false)
      : MarkupInfoC(*new MarkupPoint2dBodyC(id,zOrder,position,style,isFixed))
    {}
    //: Constructor. 
    //!cwiz:author
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC &area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
  protected:
    MarkupPoint2dC(MarkupPoint2dBodyC &bod)
     : MarkupInfoC(bod)
    {}
    //: Body constructor. 
    
    MarkupPoint2dBodyC& Body()
    { return static_cast<MarkupPoint2dBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
    const MarkupPoint2dBodyC& Body() const
    { return static_cast<const MarkupPoint2dBodyC &>(MarkupInfoC::Body()); }
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
