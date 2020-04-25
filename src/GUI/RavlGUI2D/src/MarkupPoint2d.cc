// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include "Ravl/GUI/MarkupPoint2d.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/Projection2d.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/RCWrap.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  MarkupPoint2dBodyC::MarkupPoint2dBodyC(Int64T id, IntT zOrder,const Point2dC & cn,MarkupPoint2dStyleT _style,bool isFixed)
    : MarkupInfoBodyC(id,zOrder),
      centre(cn),
      sigPosition(Point2dC()),
      style(_style),
      m_fixed(isFixed)
  {}
  
  
  //: Method for rendering frame.  
  bool MarkupPoint2dBodyC::Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected) { 
    // Set up the gc
    if (!SetDrawStyle(mv))
      return true;
    GdkGC *dc = mv.GcDrawContext();

    
    switch(style) {
    case MP2DS_Eye: {
      RealT size = 10;
      Point2dC ver(0,size/2.0);
      Point2dC hor(size/2.0,0);
      for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
          Point2dC start(centre);
          if (i==0) start += ver;
          else start -= ver;
          if (j==0) start += hor;
          else start -= hor;
          for (int k=0; k<2; k++) {
            Point2dC end(start);
            // Vertical
            if (k==0) {
              if (i==0) end += ver;
              else end -= ver;
            }
            // Horizontal
            else {
              if (j==0) end += hor;
              else end -= hor;
            }	  
            // Draw
            mv.GUIDrawLine(dc, start, end);
          }
        }
      }
      Point2dC horline(centre.Row(), centre.Col()-5.);
      Point2dC horline2(centre.Row(), centre.Col()+5);
      Point2dC verline(centre.Row()-5., centre.Col());
      Point2dC verline2(centre.Row()+5., centre.Col());
      
      mv.GUIDrawLine(dc, horline, horline2);
      mv.GUIDrawLine(dc, verline, verline2);
    } break;
    default:
    case MP2DS_CrossHair: {
      RealT size = 5;
      Vector2dC ver1(0,2);
      Vector2dC ver2(0,size);
      Vector2dC hor1(2,0);
      Vector2dC hor2(size,0);
      
      mv.GUIDrawLine(dc, Point2dC(centre + hor1),Point2dC(centre + hor2));
      mv.GUIDrawLine(dc, Point2dC(centre - hor1),Point2dC(centre - hor2));
      mv.GUIDrawLine(dc, Point2dC(centre + ver1),Point2dC(centre + ver2));
      mv.GUIDrawLine(dc, Point2dC(centre - ver1),Point2dC(centre - ver2));
    } break;
    case MP2DS_DiagonalCross: {
      RealT size = 3;
      Vector2dC p1(size,size);
      Vector2dC p2(size,-size);
      
      mv.GUIDrawLine(dc, Point2dC(centre + p1),Point2dC(centre - p1));
      mv.GUIDrawLine(dc, Point2dC(centre + p2),Point2dC(centre - p2));
    } break;
    }
      

    return true;
  }

  RealRange2dC MarkupPoint2dBodyC::Extent(GUIMarkupCanvasBodyC &mv) const {
    RealRange2dC rextent(centre.Row()-10, centre.Row()+10, centre.Col()-10, centre.Col()+10);
    // Involve extent of children

    return rextent;
  }

  RealT MarkupPoint2dBodyC::SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me) {
    RealRange2dC rextent(centre.Row()-5,centre.Row()+5,centre.Col()-5,centre.Col()+5);
    if(rextent.Contains(at))
      return 1;
    return -1;
  }

  bool MarkupPoint2dBodyC::MouseEventPress(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupPoint2dBodyC::MouseEventPress() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    if(!m_fixed && me.HasChanged(0)) { // Button 0 press ?
      mv.GUIClearSelect(false);
      mv.GUIAddSelect(Id());
      oldPosition = mv.MousePressAt();
      refresh = true;
      return true;
    }
    return false;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupPoint2dBodyC::MouseEventMove(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupPoint2dBodyC::MouseEventMove() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    if(m_fixed) 
      return true;
    Point2dC offset = (at - oldPosition);
    centre += offset;
    oldPosition = centre;
    refresh = true;
    return true;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupPoint2dBodyC::MouseEventRelease(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    //cerr << "MarkupPoint2dBodyC::MouseEventRelease() At=" << at << " State=" << state << " " << " ";
    //cerr << " Press " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << " ";
    if(m_fixed) 
      return true;
    //: Signal position change on mouse release
    sigPosition(centre);
    return false;
  }


  bool MarkupPoint2dBodyC::Position(const Point2dC & newPosition) {
    centre = newPosition;
    oldPosition = centre;
    return true;
  }
 

  
}
