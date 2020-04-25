// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id$"
//! file = "Magellan/Workspace/GUI/GUIWsTag.hh"
//! author = "Warren Moore"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#ifndef RAVLGUI_GUIMARKUPLAYEREDITOR_HEADER
#define RAVLGUI_GUIMARKUPLAYEREDITOR_HEADER 1

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/MarkupLayerInfo.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/GUI/TreeStore.hh"
#include "Ravl/GUI/TreeView.hh"
#include "Ravl/GUI/ColourSelector.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"


namespace RavlGUIN
{
  
  using namespace RavlGUIN;
  
  class GUIMarkupLayerEditorBodyC :
    public WindowBodyC
  {
  public:
    GUIMarkupLayerEditorBodyC(GUIMarkupCanvasC &canvas, const bool showLine = true);
    //: Glade constructor.
    
    virtual ~GUIMarkupLayerEditorBodyC();
    //: Destructor
    
    virtual bool Create();
    //: Create widget.
    
    bool GUIRaise();
    //: Raise window to top
    
    bool GUISetLayerList(DListC<MarkupLayerInfoC> &layerList);
    //: Set the layer list to display
    
    bool GUISetZOrderVisible(const IntT zOrder, const bool visible);
    //: Set the Z order visible
    
    bool GUIRefresh();
    //: Refresh the GUI
    // GUI thread

    bool Refresh();
    //: Refresh the GUI
    
    Signal2C<IntT, bool>& SigVisibilityChanged()
    { return m_sigVisibility; }
    //: Get the visibility signal
    
    Signal1C<MarkupLayerInfoC>& SigLayerInfoChanged()
    { return m_sigLayerChange; }
    //: Get the markup layer info signal
    
  private:
    bool CBClose();
    //: Close window.
    
    void LoadTree();
    //: Load the layer list into the tree view

    bool CBToggleVisibility(TreeModelIterC &row);
    //: Layer visibility toggled
    
    bool CBActivated(TreeModelPathC &path, StringC &colName);
    //: Row element clicked
    
    bool CBColourSelected(UInt16RGBValueC &colour);
    //: Colour chosen in colour selector
    
    void UpdateZOrderLayerData(HashC<IntT, bool> &zOrderSet, const bool visible);
    //: Signal the layers of the update and update the tree toggles
    
  private:
    GUIMarkupCanvasC m_canvas;
    
    DListC<MarkupLayerInfoC> m_layerList;
    HashC<IntT, DListC<TreeModelIterC> > m_zOrderRows;
    MutexC m_lock;
    
    bool m_showLine;
    TreeStoreC m_treeStore;
    TreeViewC m_teeView;
    ColourSelectorC m_colourSelector;
    TreeModelIterC m_selectedRow;
    
    Signal2C<IntT, bool> m_sigVisibility;
    Signal1C<MarkupLayerInfoC> m_sigLayerChange;
  };
  
  
  
  class GUIMarkupLayerEditorC :
    public WindowC
  {
  public:
    GUIMarkupLayerEditorC()
    {}
    //: Default constructor
    // Creates an invalid handle

    GUIMarkupLayerEditorC(GUIMarkupCanvasC &canvas, const bool showLine = true) :
      WindowC(*new GUIMarkupLayerEditorBodyC(canvas, showLine))
    {}
    //: Constructor
    
    bool Create() 
    { return Body().Create(); }
    //: Create widget

    bool GUISetLayerList(DListC<MarkupLayerInfoC> &layerList)
    { return Body().GUISetLayerList(layerList); }
    //: Set the layer list to display
    
    bool GUISetZOrderVisible(const IntT zOrder, const bool visible)
    { return Body().GUISetZOrderVisible(zOrder, visible); }
    //: Set the Z order visible
    
    bool GUIRefresh()
    { return Body().GUIRefresh(); }
    //: Refresh the GUI
    // GUI thread

    bool Refresh()
    { return Body().Refresh(); }
    //: Refresh the GUI
    
    Signal2C<IntT, bool>& SigVisibilityChanged()
    { return Body().SigVisibilityChanged(); }
    //: Get the visibility signal
    
    Signal1C<MarkupLayerInfoC>& SigLayerInfoChanged()
    { return Body().SigLayerInfoChanged(); }
    //: Get the markup layer info signal
    
  private:    
    GUIMarkupLayerEditorC(GUIMarkupLayerEditorBodyC &body) :
      WindowC(body)
    {}
    //: Body constructor.
    
    GUIMarkupLayerEditorBodyC &Body()
    { return static_cast<GUIMarkupLayerEditorBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const GUIMarkupLayerEditorBodyC &Body() const
    { return static_cast<const GUIMarkupLayerEditorBodyC &>(WidgetC::Body()); }
    //: Access body.

    friend class GUIMarkupLayerEditorBodyC;
  };

  
  
}

#endif
