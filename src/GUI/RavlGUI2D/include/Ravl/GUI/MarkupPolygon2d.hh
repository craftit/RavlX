// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPPOLYGON2D_HEADER
#define RAVLGUI_MARKUPPOLYGON2D_HEADER 1
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Calls.hh"

namespace RavlN {
  class Projection2dC;
}

namespace RavlGUIN {
  using namespace RavlImageN;
  
  class MarkupPolygon2dC;
  
  //! userlevel = Develop
  //: Polygon
  
  class MarkupPolygon2dBodyC 
    : public MarkupInfoBodyC
  {
  public:
    MarkupPolygon2dBodyC(Int64T id,IntT zOrder,const Polygon2dC &poly,bool openPoly = false,bool isFixed = false);
    //: Constructor.

    MarkupPolygon2dBodyC(Int64T id,IntT zOrder,const Polygon2dC &poly,const ByteRGBValueC &colour,bool openPoly = false,bool isFixed = false);
    //: Constructor.
    
    void SetColour(const ByteRGBValueC &colour);
    //: Set colour for polygon.
    
    void UnsetColour();
    //: Unset colour for polygon.
    
    virtual RealRange2dC Extent(GUIMarkupCanvasBodyC &mv) const;
    //: Extent of object.
    
    virtual bool Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected);
    //: Method for rendering frame.
    
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
    
    void SetUpdate(const CallFunc2C<Polygon2dC,Int64T> &_trigUpdate)
    { trigUpdate = _trigUpdate; }
    //: Set update trigger.
    
    const DListC<MarkupPolygon2dC> &Children() const
    { return children; }
    //: Access list of children
    
    bool Insert(const MarkupPolygon2dC &markupPoly);
    //: Add child markup.
    
    bool ApplyTransform(const Projection2dC &proj);
    //: Apply transform to this polygon
    // This replaces poly with orgPoly transformed by the given projection.
    
    bool UpdateDb();
    //: Update database with changes to this polygon
    
    bool HasChild(Int64T id) const;
    //: Does polygon have child with given id ?
    
    Polygon2dC &Polygon()
    { return poly; }
    //: Access polygon.

    const Polygon2dC &Polygon() const
    { return poly; }
    //: Access polygon.

  protected:
    bool FindClosestPoint(const Point2dC &pnt,RealT &score,IntT &pntNo);
    //: Find a point on the polygon
    // Returns true if its close enough
    
    bool MovePoly();
    //: Call when some part of the polygon has been updated.
    // This will update the position of any child polygons appropriatly.
    
    bool QuickMarkPoint(int pnt,Point2dC pos,bool markVert);
    //: Move a point with quick markup.
    
    Polygon2dC poly;
    Polygon2dC orgPoly; // Original polygon, used for drags
    
    CallFunc2C<Polygon2dC,Int64T> trigUpdate;
    
    DListC<MarkupPolygon2dC> children; // Child polygon's

    bool useCustomColour;
    ByteRGBValueC colour; // Colour for markup.
    bool m_fixed; // Is this polygon unmovable ?
    bool m_openPoly; // Is this open polygon
  };
  
  //! userlevel=Normal
  //: Polygon 
  //!cwiz:author
  
  class MarkupPolygon2dC
    : public MarkupInfoC
  {
  public:
    MarkupPolygon2dC()
    {}
    //: Default constructor
    // Creates in invalid handle.
    
    MarkupPolygon2dC(Int64T id,IntT zOrder,const Polygon2dC & poly,bool openPoly = false,bool isFixed = false) 
      : MarkupInfoC(*new MarkupPolygon2dBodyC(id,zOrder,poly,openPoly,isFixed))
    {}
    //: Constructor. 
    //!cwiz:author
    
    MarkupPolygon2dC(Int64T id,IntT zOrder,const Polygon2dC & poly,const ByteRGBValueC &colour,bool openPoly = false,bool isFixed = false) 
      : MarkupInfoC(*new MarkupPolygon2dBodyC(id,zOrder,poly,colour,openPoly,isFixed))
    {}
    //: Constructor. 
    //!cwiz:author
    
    void SetColour(const ByteRGBValueC &colour)
    { return Body().SetColour(colour); }
    //: Set colour for polygon.
    
    void UnsetColour()
    { Body().UnsetColour(); }
    //: Unset colour for polygon.
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC &area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
    
    void SetUpdate(const CallFunc2C<Polygon2dC,Int64T> &trigUpdate)
    { Body().SetUpdate(trigUpdate); }
    //: Set update trigger.
    
    const DListC<MarkupPolygon2dC> &Children() const
    { return Body().Children(); }
    //: Access list of children
    
    bool Insert(const MarkupPolygon2dC &markupPoly)
    { return Body().Insert(markupPoly); }
    //: Add child markup.
    
    bool ApplyTransform(const Projection2dC &proj)
    { return Body().ApplyTransform(proj); }
    //: Apply transform to this polygon
    // This replaces poly with orgPoly transformed by the given projection.
    
    bool UpdateDb()
    { return Body().UpdateDb(); }
    //: Update database with changes to this polygon
    
    
    bool MouseEventPress(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventPress(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    bool MouseEventMove(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventMove(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    bool MouseEventRelease(GUIMarkupCanvasBodyC & mv,const Point2dC & at,const MouseEventC & me,IntT & mouseState,bool & refresh) 
    { return Body().MouseEventRelease(mv,at,me,mouseState,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    bool HasChild(Int64T id) const
    { return Body().HasChild(id); }
    //: Does polygon have child with given id ?
    
    Polygon2dC &Polygon()
    { return Body().Polygon(); }
    //: Access polygon.

    const Polygon2dC &Polygon() const
    { return Body().Polygon(); }
    //: Access polygon.

  protected:
    MarkupPolygon2dC(MarkupPolygon2dBodyC &bod)
     : MarkupInfoC(bod)
    {}
    //: Body constructor. 
    
    MarkupPolygon2dBodyC& Body()
    { return static_cast<MarkupPolygon2dBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
    const MarkupPolygon2dBodyC& Body() const
    { return static_cast<const MarkupPolygon2dBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
  };
  
  
}


#endif
