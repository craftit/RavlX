// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/GUI/MarkupLayerInfo.hh"
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

namespace RavlGUIN
{
  
  const static IntT g_lineStyleNameSize = 3;
  const static char *g_lineStyleName[] = 
  {
    "Normal",
    "None",
    "Dotted"
  };
  
  const StringC LineStyleName(const MLILineStyleT line)
  {
    StringC name("Unknown");
    if (line < g_lineStyleNameSize)
    {
      name = g_lineStyleName[line];
    }
    return name;
  }
  
  const static ByteRGBValueC g_colourDefaultValue(0, 0, 0);

  MarkupLayerInfoC::MarkupLayerInfoC() :
    m_layerId(-1),
    m_name(),
    m_colour(g_colourDefaultValue),
    m_colourEditable(false),
    m_colourDefault(false),
    m_colourCanDefault(true),
    m_line(MLI_LineNormal),
    m_lineEditable(false),
    m_lineDefault(false),
    m_lineCanDefault(false)
  {
  }
                      
  MarkupLayerInfoC::MarkupLayerInfoC(const IntT layerId,
                                     const StringC &name) :
    m_layerId(layerId),
    m_name(name),
    m_colour(g_colourDefaultValue),
    m_colourEditable(false),
    m_colourDefault(true),
    m_colourCanDefault(true),
    m_line(MLI_LineNormal),
    m_lineEditable(false),
    m_lineDefault(false),
    m_lineCanDefault(false)
  {
  }
                      
  MarkupLayerInfoC::MarkupLayerInfoC(const IntT layerId,
                                     const StringC &name,
                                     const ByteRGBValueC colour,
                                     const MLILineStyleT line,
                                     const bool editable) :
    m_layerId(layerId),
    m_name(name),
    m_colour(colour),
    m_colourEditable(editable),
    m_colourDefault(false),
    m_colourCanDefault(false),
    m_line(line),
    m_lineEditable(editable),
    m_lineDefault(false),
    m_lineCanDefault(false)
  {
  }
                      
  MarkupLayerInfoC::MarkupLayerInfoC(const IntT layerId,
                                     const StringC &name,
                                     const ByteRGBValueC colour) :
    m_layerId(layerId),
    m_name(name),
    m_colour(colour),
    m_colourEditable(true),
    m_colourDefault(false),
    m_colourCanDefault(false),
    m_line(MLI_LineNormal),
    m_lineEditable(false),
    m_lineDefault(true),
    m_lineCanDefault(true)
  {
  }
  
  MarkupLayerInfoC::MarkupLayerInfoC(const IntT layerId,
                                     const StringC &name,
                                     const MLILineStyleT line) :
    m_layerId(layerId),
    m_name(name),
    m_colour(g_colourDefaultValue),
    m_colourEditable(false),
    m_colourDefault(true),
    m_colourCanDefault(true),
    m_line(line),
    m_lineEditable(true),
    m_lineDefault(false),
    m_lineCanDefault(false)
  {
  }
  
  void MarkupLayerInfoC::AddZOrder(const IntT zOrder, const bool showVisibility)
  {
    m_zOrderList.Update(zOrder, showVisibility);
  }
    
  const bool MarkupLayerInfoC::ZOrderVisibility(const IntT zOrder)
  {
    return m_zOrderList.Lookup(zOrder) != 0;
  }
    
  bool MarkupLayerInfoC::SetColour(const ByteRGBValueC colour)
  {
    if (!m_colourEditable)
      return false;
    
    m_colourDefault = false;
    m_colour = colour;
    return true;
  }

  bool MarkupLayerInfoC::SetColourDefault(const bool colourDefault)
  {
    if (!m_colourEditable || !m_colourCanDefault)
      return false;
    
    m_colourDefault = colourDefault;
    return true;
  }

  bool MarkupLayerInfoC::SetLine(const MLILineStyleT line)
  {
    if (!m_lineEditable)
      return false;
    
    m_lineDefault = false;
    m_line = line;
    return true;
  }

  bool MarkupLayerInfoC::SetLineDefault(const bool lineDefault)
  {
    if (!m_lineEditable || !m_lineCanDefault)
      return false;
    
    m_lineDefault = lineDefault;
    return true;
  }

}
