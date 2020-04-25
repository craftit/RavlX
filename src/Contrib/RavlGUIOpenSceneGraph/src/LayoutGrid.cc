// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUIOpenSceneGraph

#include "Ravl/OpenSceneGraph/LayoutGrid.hh"
#include "Ravl/OpenSceneGraph/TypeConvert.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN {

  //: Constructor
  
  LayoutGridC::LayoutGridC(bool create)
   : LayoutC(create),
     m_stackAxis1(0),
     m_stackAxis2(1),
     m_gap(0.1),
     m_minSize(1.0),
     m_homogenousGrid(false)
  {
  }
  
  //: XML factory constructor
  
  LayoutGridC::LayoutGridC(const XMLFactoryContextC &factory)
   : LayoutC(true),
     m_stackAxis1(0),
     m_stackAxis2(1),
     m_gap(0.1),
     m_minSize(1.0),
     m_homogenousGrid(false)
  {
    Setup(factory);
  }
  
  //: Update the layout
  
  bool LayoutGridC::UpdateLayout()
  {
    ONDEBUG(rDebug("Updating layout. "));
    
    unsigned colMax = 1;
    unsigned rowMax = 1;
    const unsigned viewSize = m_nodes.size();
    rowMax = RavlN::Floor(RavlN::Sqrt(viewSize));
    colMax = rowMax;
    if(RavlN::Sqr(rowMax) < viewSize)
      colMax++;
    if((rowMax * colMax) < viewSize)
      rowMax++;
    ONDEBUG(rDebug("Nodes:%u RowMax:%u ColMax:%u ",viewSize,rowMax,colMax));
    RavlAssert((colMax * rowMax) >= viewSize);

    // Sort out grid displays
    Vector3dC rowStart(0,0,0);
    
    std::vector<float> colSizes(colMax);
    float maxWidth = m_minSize;
    float maxHeight = m_minSize;
    for(unsigned c = 0;c < colMax;c++) {
      float width = m_minSize;
      for(unsigned r = 0;r < rowMax;r++) {
        unsigned at = r * colMax + c;
        if(at >= m_nodes.size())
          break;
        float csize = m_nodes[at]->Bounds()._max[m_stackAxis2] - m_nodes[at]->Bounds()._min[m_stackAxis2];
        if(csize > width)
          width = csize;
        float rsize = m_nodes[at]->Bounds()._max[m_stackAxis1] - m_nodes[at]->Bounds()._min[m_stackAxis1];
        if(rsize > maxHeight)
          maxHeight = rsize;
      }
      colSizes[c] = width;
      if(width > maxWidth)
        maxWidth = width;
    }

    // If we want the grid to be homogeneous, just use the max sizes all round.
    float minHeight = m_minSize;
    if(m_homogenousGrid) {
      for(unsigned c = 0;c < colMax;c++)
        colSizes[c] = maxWidth;
      minHeight = maxHeight;
    }

    unsigned at = 0;
    ONDEBUG(rDebug("Max height:%f Max Width:%f  Nodes:%u RowMax:%u ColMax:%u ",maxHeight,maxWidth,viewSize,rowMax,colMax));

    for(unsigned r = 0;r < rowMax;r++) {
      float hight = minHeight;
      Vector3dC vat = rowStart;
      
      unsigned atSave = at;
      for(unsigned c = 0;c < colMax && at < m_nodes.size();c++,at++) {
        float rsize = m_nodes[at]->Bounds()._max[m_stackAxis1] - m_nodes[at]->Bounds()._min[m_stackAxis1];
        if(rsize > hight)
          hight = rsize;
      }
      at = atSave;
      
      if(m_invertAxis2) {
        rowStart[m_stackAxis1] -= (hight + m_gap);        
      }
      
      // Work out the row height.
      for(unsigned c = 0;c < colMax && at < m_nodes.size();c++,at++) {
        if(m_invertAxis2)
          vat[m_stackAxis2] -= (colSizes[c] + m_gap);
        Vector3dC correctedPosition = vat - MakeVector3d(m_nodes[at]->Bounds()._min);
        m_nodes[at]->SetPosition(correctedPosition);
        ONDEBUG(rDebug("Start %s ",RavlN::StringOf(correctedPosition).data()));
        if(!m_invertAxis2)
          vat[m_stackAxis2] += (colSizes[c] + m_gap);
      }
      if(!m_invertAxis2)
        rowStart[m_stackAxis1] += (hight + m_gap);
    }
    
    return true;
  }
  
  //: Process a callback.
  
  void LayoutGridC::DoCallback()
  {
    LayoutC::DoCallback();
  }
  
  //: Do setup from factory
  
  bool LayoutGridC::Setup(const XMLFactoryContextC &factory)
  {
    m_stackAxis1 = factory.AttributeInt("gridAxis1",1);
    m_invertAxis1 = m_stackAxis1 < 0;
    m_stackAxis1 = RavlN::Abs(m_stackAxis1)-1;
    if(m_stackAxis1 < 0 || m_stackAxis1 > 2) {
      rError("Invalid 1st grid axis %d ",m_stackAxis1);
      throw RavlN::ExceptionBadConfigC("Invalid stack axis. ");
    }
    m_stackAxis2 = factory.AttributeInt("gridAxis2",2);
    m_invertAxis2 = m_stackAxis2 < 0;
    m_stackAxis2 = RavlN::Abs(m_stackAxis2)-1;
    if(m_stackAxis2 < 0 || m_stackAxis2 > 2) {
      rError("Invalid 2nd grid axis  %d ",m_stackAxis2);
      throw RavlN::ExceptionBadConfigC("Invalid stack axis. ");
    }
    m_gap = factory.AttributeReal("gap",0.1);
    m_minSize = factory.AttributeReal("minSize",1.0);
    m_homogenousGrid = factory.AttributeBool("homogenousGrid",false);
    LayoutC::Setup(factory);
    return true;
  }
  
  //: Called when owner handles drop to zero.
  
  void LayoutGridC::ZeroOwners()
  {
    LayoutC::ZeroOwners();
  }
  
  void LinkLayoutGrid()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<LayoutGridC,LayoutC> g_registerXMLFactoryLayout("RavlOSGN::LayoutGridC");

}
