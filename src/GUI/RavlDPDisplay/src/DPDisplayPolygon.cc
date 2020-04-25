// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDPDisplay

#include "Ravl/GUI/DPDisplayPolygon.hh"
#include "Ravl/GUI/MarkupPolygon2d.hh"
#include "Ravl/PolyLine2d.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/PolyLine2d.hh"
#include "Ravl/LinePP2d.hh"

namespace RavlGUIN {

  
  //: Constructor
  
  DPDisplayPolygon2dBodyC::DPDisplayPolygon2dBodyC(const Polygon2dC &polygon,bool openPoly) 
    : m_polygon(polygon),
      m_openPolygon(openPoly)
  {}
  
  //: Draw object to canvas.
  
  bool DPDisplayPolygon2dBodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupPolygon2dC(m_id,LayerNo(),m_polygon,m_openPolygon,true));
    return true;
  }
  
  //: Find bounding box for object.
  
  IndexRange2dC DPDisplayPolygon2dBodyC::Frame() const 
  { return m_polygon.BoundingRectangle().IndexRange(); }
  
  //: Query a point in the display.
  // Return true if point is within object.
  
  bool DPDisplayPolygon2dBodyC::Query(const Vector2dC &pnt,StringC &text) {
    if(m_polygon.Contains(pnt)) {
      StringC tmp = StringOf(m_polygon);
      tmp.gsub("\n",",");
      if(m_openPolygon) {
        if(m_polygon.Size() > 2)
          text = StringC("PolyLine:") + tmp;
        else
          text = StringC("Line:") + tmp;
      } else
        text = StringC("Polygon:") + tmp;
      return true;
    }
    return false;
  }
  
  //: Save to a file.
  
  bool DPDisplayPolygon2dBodyC::Save(const StringC &str) const {
    
    return false;
  }

  //------------------------------------------------------------------
  // Register some conversions as well.
  
  DPDisplayObjC Polygon2dPDisplayObj(const Polygon2dC &poly) 
  { return DPDisplayPolygon2dC(poly.Copy()); }
  
  DP_REGISTER_CONVERSION_NAMED(Polygon2dPDisplayObj,1,"DPDisplayObjC Polygon2d2DPDisplayObj(const Polygon2dC &) ");
  
  
  DPDisplayObjC PolyLine2dPDisplayObj(const PolyLine2dC &poly) 
  { 
    PointSet2dC pntSet = poly;
    return DPDisplayPolygon2dC(Polygon2dC(pntSet),true); 
  }
  
  DP_REGISTER_CONVERSION_NAMED(PolyLine2dPDisplayObj,1,"DPDisplayObjC PolyLine2d2DPDisplayObj(const PolyLine2dC &) ");

  DPDisplayObjC LinePP2dPDisplayObj(const LinePP2dC &line) 
  { 
    Polygon2dC poly;
    poly.InsFirst(line.P1());
    poly.InsFirst(line.P2());
    return DPDisplayPolygon2dC(poly,true); 
  }
  
  DP_REGISTER_CONVERSION_NAMED(LinePP2dPDisplayObj,1,"DPDisplayObjC LinePP2d2DPDisplayObj(const LinePP2dC &) ");
  
  DPDisplayObjC RealRange2dPDisplayObj(const RealRange2dC &rect) {
    Polygon2dC poly(rect);
    return DPDisplayPolygon2dC(poly);
  }
  
  DP_REGISTER_CONVERSION_NAMED(RealRange2dPDisplayObj,1,"DPDisplayObjC RealRange2d2DPDisplayObj(const RealRange2dC &) ");
  
  DPDisplayObjC IndexRange2dPDisplayObj(const IndexRange2dC &rect) {
    RealRange2dC rr(rect);
    Polygon2dC poly(rr);
    return DPDisplayPolygon2dC(poly);
  }
  
  DP_REGISTER_CONVERSION_NAMED(IndexRange2dPDisplayObj,1,"DPDisplayObjC IndexRange2d2DPDisplayObj(const RealRange2dC &) ");
  

  void InitDPDisplayPolygon2d() 
  {}
  
}
