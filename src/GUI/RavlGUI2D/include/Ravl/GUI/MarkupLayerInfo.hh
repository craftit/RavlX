// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_GUIMARKUPLAYERINFO_HEADER
#define RAVLGUI_GUIMARKUPLAYERINFO_HEADER 1
//! author="Warren Moore"
//! rcsid="$Id: GUIMarkupViewer.hh 8474 2006-04-27 16:56:41 +0100 (Thu, 27 Apr 2006) warren $"
//! lib=RavlGUI2D
//! file="RavlSoft/GUI/GUIMarkupLayerInfo.hh"
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/String.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Tuple2.hh"

namespace RavlGUIN
{
  
  
  
  using namespace RavlN;
  using namespace RavlImageN;
  
  
  
  enum MLILineStyleT
  {
    MLI_LineNormal = 0,
    MLI_LineNone,
    MLI_LineDotted
  };


  
  const StringC LineStyleName(const MLILineStyleT line);



  //: MarkupLayerInfoC
  // Class containing markup canvas layer modifiers for visual
  // parameters, such as colour and line style.
  // Parameters are currently hardwired for the sake of simpilicity
  // however it would be good to abstract out the modifiers at a later
  // date as more parameters are added
  
  class MarkupLayerInfoC
  {
    public:
      MarkupLayerInfoC();
      //: Create an empty info layer
      
      MarkupLayerInfoC(const IntT layerId,
                       const StringC &name);
      //: Create a layer where the colour and line are 'default' and can't be edited

      MarkupLayerInfoC(const IntT layerId,
                       const StringC &name,
                       const ByteRGBValueC colour,
                       const MLILineStyleT line,
                       const bool editable);
      //: Create a layer where the colour and line can be changed, but not set 'default'
                          
      MarkupLayerInfoC(const IntT layerId,
                       const StringC &name,
                       const ByteRGBValueC colour);
      //: Create a layer where the colour can be set, but the line is fixed 'default'
    
      MarkupLayerInfoC(const IntT layerId,
                       const StringC &name,
                       const MLILineStyleT line);
      //: Create a layer where the line can be set, but the colour is fixed 'default'
    
      const IntT Id() const
      { return m_layerId; }
      //: Get the layer id
      
      const StringC Name() const
      { return m_name; }
      //: Get the name
      
      void AddZOrder(const IntT zOrder, const bool showVisibility = true);
      //: Add the Z order layer
      
      const HashC<IntT, bool> ZOrderSet() const
      { return m_zOrderList; }
      //: Get the associated Z order
      
      const bool ZOrderVisibility(const IntT zOrder);
      //: Should the layer visibility be shown in the layer dialog?
      
      const ByteRGBValueC Colour() const
      { return m_colour; }
      //: Get the colour
      
      bool SetColour(const ByteRGBValueC colour);
      //: Set the colour
      // Return false if not editable
    
      const bool ColourDefault() const
      { return m_colourDefault; }
      //: Get the colour default setting
      
      const bool ColourEditable() const
      { return m_colourEditable; }
      //: Get the colour editable setting
      
      bool SetColourDefault(const bool colourDefault);
      //: Set the colour default state
      // Return false if can't be set default

      const MLILineStyleT Line() const
      { return m_line; }
      //: Get the line style
      
      bool SetLine(const MLILineStyleT line);
      //: Set the line style
      // Return false if not editable
    
      const bool LineDefault() const
      { return m_lineDefault; }
      //: Get the line default setting
      
      const bool LineEditable() const
      { return m_lineEditable; }
      //: Get the line editable setting
      
      bool SetLineDefault(const bool lineDefault);
      //: Set the colour default state
      // Return false if can't be set default

    private:
      IntT m_layerId;                 //: The layer id
      StringC m_name;                 //: Layer text name
      HashC<IntT, bool> m_zOrderList;       //: Z-order layer list
      ByteRGBValueC m_colour;         //: Layer colour
      bool m_colourEditable;          //: Can the colour be changed?
      bool m_colourDefault;           //: Is the colour marked as default?
      bool m_colourCanDefault;        //: Can the colour be set as default?
      MLILineStyleT m_line;           //: Line style
      bool m_lineEditable;            //: Can the line be changed?
      bool m_lineDefault;             //: Is the line marked as default?
      bool m_lineCanDefault;          //: Can the line be set as default?
  };
  
  
  
}

#endif

