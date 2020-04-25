// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include "Ravl/GUI/MarkupLine2d.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/RCWrap.hh"

#include "Ravl/Curve2dLineSegment.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  MarkupLine2dBodyC::MarkupLine2dBodyC(Int64T id, IntT overlay,const Point2dC & center,const Vector2dC &normal,
                                       RealT length,bool selectable)
    : MarkupInfoBodyC(id,overlay),
      m_centre(center),
      m_normal(normal),
      m_length(length),
      m_selectable(selectable),
      m_setColour(false),
      m_sigPosition(Point2dC())
  {
    m_normal.MakeUnit();
  }
  
  MarkupLine2dBodyC::MarkupLine2dBodyC(Int64T id, IntT overlay,const Point2dC & center,const Vector2dC &normal,
                                       RealT length,const ByteRGBValueC &colour,bool selectable)
    : MarkupInfoBodyC(id,overlay),
      m_centre(center),
      m_normal(normal),
      m_length(length),
      m_selectable(selectable),
      m_setColour(true),
      m_colour(colour),
      m_sigPosition(Point2dC())
  {
    m_normal.MakeUnit();
  }
  
  //: Method for rendering frame.  
  
  bool MarkupLine2dBodyC::Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected) { 
    ONDEBUG(std::cerr << "MarkupLine2dBodyC::Render, Called. Length=" <<m_length << "\n");
    // Set up the gc
    if (!SetDrawStyle(mv))
      return true;
    GdkGC *dc = mv.GcDrawContext();

    if(m_length >= 0) {
      Vector2dC dir = m_normal.Perpendicular();
      Point2dC start = m_centre - dir * m_length;
      Point2dC end = m_centre + dir * m_length;
      ONDEBUG(std::cerr << "MarkupLine2dBodyC::Render, Start=" << start << " End=" << end << "\n");
      mv.GUIDrawLine(dc, start, end);
    } else {
      // Infinite line.
    }

    return true;
  }

  RealRange2dC MarkupLine2dBodyC::Extent(GUIMarkupCanvasBodyC &mv) const {
//    RealRange2dC rextent(m_centre.Row()-1, m_centre.Row()+1, m_centre.Col()-1, m_centre.Col()+1);
    RealRange2dC rextent;
    
    // Involve extent of children
    Vector2dC dir = m_normal.Perpendicular();
    Point2dC start = m_centre - dir * m_length;
    Point2dC end = m_centre + dir * m_length;
    rextent.Involve(start);
    rextent.Involve(end);
    
    return rextent;
  }

  RealT MarkupLine2dBodyC::SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me) {
    if(!m_selectable)
      return -1;
    
    Vector2dC dir = m_normal.Perpendicular();
    Point2dC start = m_centre - dir * m_length;
    Point2dC end = m_centre + dir * m_length;
    
    // Check distance to line.
    Curve2dLineSegmentC line(start,end);
    RealT sqrDist = line.SqrEuclidDistance(at);
    if(sqrDist > Sqr(10.0))
      return -1;
    
    // Only accept if its inside the segment
    RealT pat = line.Closest(at);
    if(pat < line.Start() || pat > line.End())
      return -1;
    
    return Sqrt(sqrDist);
  }

  bool MarkupLine2dBodyC::MouseEventPress(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupLine2dBodyC::MouseEventPress() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    if(me.HasChanged(0)) { // Button 0 press ?
      mv.GUIClearSelect(false);
      mv.GUIAddSelect(Id());
      m_moveOffset = m_centre - mv.MousePressAt();
      refresh = true;
      return true;
    }
    return false;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupLine2dBodyC::MouseEventMove(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupLine2dBodyC::MouseEventMove() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    m_centre = m_moveOffset + at;
    refresh = true;
    return true;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupLine2dBodyC::MouseEventRelease(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupLine2dBodyC::MouseEventRelease() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    //: Signal position change on mouse release
    m_sigPosition(m_centre);
    return false;
  }

  
  bool MarkupLine2dBodyC::Position(const Point2dC & newPosition) {
    m_centre = newPosition;
    return true;
  }
 

  
}
