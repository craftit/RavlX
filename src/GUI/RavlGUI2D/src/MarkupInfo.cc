// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include <gdk/gdk.h>

namespace RavlGUIN {
  
  const static ByteRGBValueC g_defaultColour(255, 255, 255);
  const static MLILineStyleT g_defaultLineStyle = MLI_LineNormal;
  const static IntT g_defaultLineWidth = 2;

  //: Default constructor.
  
  MarkupInfoBodyC::MarkupInfoBodyC(Int64T _id,IntT _zOrder)
    : id(_id),
      zOrder(_zOrder)
  {
    m_layerId.InsFirst(_zOrder);
  }
    
  //: Find center of object
  
  Point2dC MarkupInfoBodyC::Centroid(GUIMarkupCanvasBodyC &mv) const 
  { return Extent(mv).Center(); }
  
  //: Range of pixels covered by this one (Garanteed) 
  
  RealRange2dC MarkupInfoBodyC::Coverage(GUIMarkupCanvasBodyC &mv) const
  { return coverage; }
  
  //: Extent of object.
  
  RealRange2dC MarkupInfoBodyC::Extent(GUIMarkupCanvasBodyC &mv) const
  { return extent; }
  
  //: Method for rendering frame.
  
  bool MarkupInfoBodyC::Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected) {
    return true;
  }
  
  //: Check if we've selected something in this object.
  //!return: Score on how close the selection is, the smaller the value the better (can be negative.)
  
  RealT MarkupInfoBodyC::SelectTest(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me) {
    return -1;
  }
  
  //: Handle mouse event.
  
  bool MarkupInfoBodyC::MouseEventPress(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &state,bool &refresh) {
    return false;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupInfoBodyC::MouseEventMove(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh) {
    return false;
  }
    
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupInfoBodyC::MouseEventRelease(GUIMarkupCanvasBodyC &mv,const Point2dC &at,const MouseEventC &me,IntT &mouseState,bool &refresh) {
    return false;
  }
    


  void MarkupInfoBodyC::PushLayer(const IntT layerId)
  {
    m_layerId.InsFirst(layerId);
  }


    
  IntT MarkupInfoBodyC::PopLayer()
  {
    return m_layerId.PopFirst();
  }
    

    
  void MarkupInfoBodyC::SetLayerList(DListC<IntT> layerId)
  {
    m_layerId = layerId.Copy();
  }

    
    
  bool MarkupInfoBodyC::SetDrawStyle(GUIMarkupCanvasBodyC &mv)
  {
    // Set the default colour
    GdkGC *dc = mv.GcDrawContext();
    GdkColor gcolour;
    gcolour.red = (IntT) g_defaultColour.Red() * 256;
    gcolour.green = (IntT) g_defaultColour.Green() * 256;
    gcolour.blue = (IntT) g_defaultColour.Blue() * 256;
    MLILineStyleT lineStyle = g_defaultLineStyle;
    
    // Loop through the affecting layers to get the final colour
    DLIterC<IntT> layerId(m_layerId);
    for (layerId.Last(); layerId; layerId--)
    {
      MarkupLayerInfoC layerInfo;
      if (mv.LayerInfo(*layerId, layerInfo))
      {
        if (!layerInfo.ColourDefault())
        {
          gcolour.red = (IntT) layerInfo.Colour().Red() * 256;
          gcolour.green = (IntT) layerInfo.Colour().Green() * 256;
          gcolour.blue = (IntT) layerInfo.Colour().Blue() * 256;
        }
        if (!layerInfo.LineDefault())
        {
          lineStyle = layerInfo.Line();
        }
      }
    }
    
    // Set the colour and line
    if (lineStyle == MLI_LineNone)
      return false;
    
    // Apply settings
    gdk_gc_set_rgb_fg_color(dc, &gcolour);
    gdk_gc_set_line_attributes(dc, g_defaultLineWidth, (lineStyle == MLI_LineNormal ? GDK_LINE_SOLID : GDK_LINE_ON_OFF_DASH), GDK_CAP_ROUND, GDK_JOIN_MITER);
    
    return true;
  }
  
  
  
  std::ostream &operator<<(std::ostream &out,const MarkupInfoC &obj) {
    out << obj.Id();
    return out;
  }
  //: Stream operator.
  
  BinOStreamC &operator<<(BinOStreamC &out,const MarkupInfoC &obj) {
    out << obj.Id();
    return out;
  }
  //: Binary stream operator.
  
  std::istream &operator>>(std::istream &in,MarkupInfoC &obj) {
    Int64T id;
    in >> id;
    return in;
  }
  //: Stream operator.
  
  BinIStreamC &operator>>(BinIStreamC &in,MarkupInfoC &obj) {
    Int64T id;
    in >> id;
    return in;    
  }
  //: Binary stream operator.

}
