// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPDISPLAYPOINTS_HEADER
#define RAVLGUI_DPDISPLAYPOINTS_HEADER 1
//! rcsid="$Id: DPDisplayImage.hh 5741 2006-07-13 10:27:51Z craftit $"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayImage.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Data Display"

#include "Ravl/GUI/DPDisplayObj.hh"
#include "Ravl/Point2d.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: DPDisplayObj to handle images.
  
  class DPDisplayPointsBodyC
    : public DPDisplayObjBodyC
  {
  public:
    DPDisplayPointsBodyC(const DListC<Point2dC> &points);
    //: Constructor
    
    DPDisplayPointsBodyC(const Point2dC &point);
    //: Constructor
    
    virtual bool Draw(FrameMarkupC &markup);
    //: Draw object to canvas.
    
    virtual IndexRange2dC Frame() const;
    //: Find bounding box for object.
    
    virtual bool Query(const Vector2dC &pnt,StringC &text);
    //: Query a point in the display.
    // Return true if point is within object.
    
    virtual bool Save(const StringC &str) const;
    //: Save to a file.
    
  protected:
    DListC<Point2dC> m_points;
  };
  

  class DPDisplayPointsC
    : public DPDisplayObjC
  {
  public:
    DPDisplayPointsC()
    {}
    //: Default constructor

    DPDisplayPointsC(const DListC<Point2dC> &points)
      : DPDisplayObjC(new DPDisplayPointsBodyC(points))
    {}
    //: Constructor
    
    DPDisplayPointsC(const Point2dC &point)
      : DPDisplayObjC(new DPDisplayPointsBodyC(point))
    {}
    //: Constructor
    
  };
}

#endif
