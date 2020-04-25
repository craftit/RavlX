// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPDISPLAYPOLYGON_HEADER
#define RAVLGUI_DPDISPLAYPOLYGON_HEADER 1
//! rcsid="$Id: DPDisplayImage.hh 5741 2006-07-13 10:27:51Z craftit $"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayImage.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Data Display"

#include "Ravl/GUI/DPDisplayObj.hh"
#include "Ravl/Polygon2d.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: DPDisplayObj to handle images.
  
  class DPDisplayPolygon2dBodyC
    : public DPDisplayObjBodyC
  {
  public:
    DPDisplayPolygon2dBodyC(const Polygon2dC &polygon,bool openPoly = false);
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
    Polygon2dC m_polygon;
    bool m_openPolygon;
  };
  

  class DPDisplayPolygon2dC
    : public DPDisplayObjC
  {
  public:
    DPDisplayPolygon2dC()
    {}
    //: Default constructor

    DPDisplayPolygon2dC(const Polygon2dC &polygon,bool openPoly = false)
      : DPDisplayObjC(new DPDisplayPolygon2dBodyC(polygon,openPoly))
    {}
    //: Constructor
    
  };
}

#endif
