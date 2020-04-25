// This file is part of Magellan, Applied Image Recognition
// Copyright (C) 2003, Ravlperception Ltd.
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id: exJob.cc 3697 2004-07-21 10:17:06Z charles $"
//! file = "Magellan/Workspace/GUI/GUIWsTag.cc"
//! author = "Warren Moore"
//! lib=RavlGUI2D

#include "Ravl/GUI/GUIMarkupLayerEditor.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/Hash.hh"

#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN
{
  
  
  
  using namespace RavlGUIN;



  enum MLETreeColT
  {
    MLEId = 0,
    MLEVisible,
    MLEVisibleInconsistent,
    MLEName,
    MLEColour,
    MLEColourFore,
    MLEColourBack,
    MLELine,
    MLELineFore
  };
  
  
  
  const static IntT g_TreeColMax = 9;
  
  
  
  GUIMarkupLayerEditorBodyC::GUIMarkupLayerEditorBodyC(GUIMarkupCanvasC &canvas, const bool showLine) :
    WindowBodyC(300, 300, "Canvas Layer Editor", GTK_WINDOW_DIALOG, 0, false),
    m_canvas(canvas),
    m_showLine(showLine),
    m_sigVisibility(-1, true),
    m_sigLayerChange(MarkupLayerInfoC())
  {
  }


  
  GUIMarkupLayerEditorBodyC::~GUIMarkupLayerEditorBodyC()
  {}



  bool GUIMarkupLayerEditorBodyC::Create()
  {
    // Connect the window kill signal
    ConnectRef(Signal("delete_event"), *this, &GUIMarkupLayerEditorBodyC::CBClose);
  
    // Create the tree store
    SArray1dC<AttributeTypeC> types(g_TreeColMax);
    types[MLEId] =                    AttributeTypeNumC<IntT>("Id","...");
    types[MLEVisible] =               AttributeTypeBoolC("Visible","...");
    types[MLEVisibleInconsistent] =   AttributeTypeBoolC("VisibleInconsistent","...");
    types[MLEName] =                  AttributeTypeStringC("Name","...");
    types[MLEColour] =                AttributeTypeStringC("Colour","...");
    types[MLEColourFore] =            AttributeTypeStringC("ColourFore","...");
    types[MLEColourBack] =            AttributeTypeStringC("ColourBack","...");
    types[MLELine] =                  AttributeTypeStringC("Line","...");
    types[MLELineFore] =              AttributeTypeStringC("LineFore","...");
    
    m_treeStore = TreeStoreC(types);
  
    // Set the tree columns
    DListC<StringC> cols;
    cols.InsLast("Visible");
    cols.InsLast("Name");
    cols.InsLast("Colour");
    if (m_showLine)
      cols.InsLast("Line");
    
    // Create a new tree view displaying the columns
    TreeViewC m_treeView(m_treeStore, cols, GTK_SELECTION_NONE);
    m_treeView.SetAttribute(0, "inconsistent", "VisibleInconsistent"); 
    m_treeView.SetAttribute(2, "foreground", "ColourFore");
    m_treeView.SetAttribute(2, "background", "ColourBack");
    if (m_showLine)
      m_treeView.SetAttribute(3, "foreground", "LineFore");

    // Load the data
    LoadTree();

    // Connect the signals
    ConnectRef(m_treeView.ChangedSignal(0), *this, &GUIMarkupLayerEditorBodyC::CBToggleVisibility);
    ConnectRef(m_treeView.Signal("row-activated"), *this, &GUIMarkupLayerEditorBodyC::CBActivated);
    
    // Add the button
    Add(VBox(PackInfoC(m_treeView, true, true) +
             PackInfoC(ButtonR("Done", *this, &GUIMarkupLayerEditorBodyC::CBClose), false, true)));

    return WindowBodyC::Create();
  }



  bool GUIMarkupLayerEditorBodyC::GUIRaise()
  {
    // Bit of a hack.
    if (widget != 0)
      gdk_window_raise(widget->window);
    
    return true;
  }


  
  bool GUIMarkupLayerEditorBodyC::CBClose()
  {
    GUIHide();
    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::GUISetLayerList(DListC<MarkupLayerInfoC> &layerList)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    MutexLockC lock(m_lock);
   
    m_layerList = layerList.Copy();
    m_selectedRow = TreeModelIterC();
    
    LoadTree();
    
    lock.Unlock();
    
    GUIRefresh();
    
    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::GUISetZOrderVisible(const IntT zOrder, const bool visible)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    MutexLockC lock(m_lock);
   
    // Get all effected layers
    HashC<IntT, bool> zOrderSet;
    for (DLIterC<MarkupLayerInfoC> it(m_layerList); it; it++)
    {
      bool showVisibility = true;
      if (it->ZOrderSet().Lookup(zOrder, showVisibility))
      {
        zOrderSet.Add(it->ZOrderSet());
      }
    }
    
    // Signal all the layers and the display
    UpdateZOrderLayerData(zOrderSet, visible);
    
    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::GUIRefresh()
  {
    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::Refresh()
  {
    Manager.Queue(Trigger(GUIMarkupLayerEditorC(*this), &GUIMarkupLayerEditorC::GUIRefresh));
    return true;
  }
  
  
  
  void GUIMarkupLayerEditorBodyC::LoadTree()
  {
    if (!m_treeStore.IsValid())
      return;
    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    m_treeStore.GUIEmpty();
    m_zOrderRows.Empty();
    
    for (DLIterC<MarkupLayerInfoC> it(m_layerList); it; it++)
    {
      if (it->Id() >= 0)
      {
        TreeModelIterC row;
        m_treeStore.AppendRow(row);

        // Id
        m_treeStore.GUISetValue(row, MLEId, it->Id());
        
        // Visible
        HashC<IntT, bool> zOrderList = it->ZOrderSet();
        if (zOrderList.Size() > 0)
        {
          bool visible = true;
          for (HashIterC<IntT, bool> it(zOrderList); it; it++)
          {
            if (it.Data() && m_canvas.ZOrderExcluded(it.Key()))
            {
              visible = false;
              break;
            }
          }
          
          // Set visible value true and editable
          m_treeStore.GUISetValue(row, MLEVisible, visible);
          m_treeStore.GUISetValue(row, MLEVisibleInconsistent, false);
          
          // Store the row
          for (HashIterC<IntT, bool> itZOrder(zOrderList); itZOrder; itZOrder++)
          {
            DListC<TreeModelIterC> rowList;
            m_zOrderRows.Lookup(itZOrder.Key(), rowList);
            rowList.InsLast(row);
            m_zOrderRows.Update(itZOrder.Key(), rowList);
          }
        }
        else
        {
          // Set visible value false and inconsistent
          m_treeStore.GUISetValue(row, MLEVisible, false);
          m_treeStore.GUISetValue(row, MLEVisibleInconsistent, true);
        }
        
        // Name
        m_treeStore.GUISetValue(row, MLEName, it->Name());
        
        // Colour
        m_treeStore.GUISetValue(row, MLEColour, "Default");
        if (it->ColourDefault())
        {
          m_treeStore.GUISetValue(row, MLEColourFore, "gray");
          m_treeStore.GUISetValue(row, MLEColourBack, "white");
        }
        else
        {
          StringC colourVal;
          colourVal.form("#%02x%02x%02x", it->Colour().Red(), it->Colour().Green(), it->Colour().Blue());
          m_treeStore.GUISetValue(row, MLEColourFore, colourVal);
          m_treeStore.GUISetValue(row, MLEColourBack, colourVal);
        }
        
        // Line
        if (it->LineDefault())
        {
          m_treeStore.GUISetValue(row, MLELine, "Default");
          m_treeStore.GUISetValue(row, MLELineFore, "gray");
        }
        else
        {
          m_treeStore.GUISetValue(row, MLELine, LineStyleName(it->Line()));
          m_treeStore.GUISetValue(row, MLELineFore, "black");
        }
      }
    }
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::CBToggleVisibility(TreeModelIterC &row)
  {
    ONDEBUG(std::cerr << "GUIMarkupLayerEditorBodyC::CBToggleVisibility" << endl);
    
    MutexLockC lock(m_lock);
   
    // Is the row inconsistent?
    bool inconsistent = true;
    m_treeStore.GetValue(row, MLEVisibleInconsistent, inconsistent);
    if (!inconsistent)
    {
      // Get the id
      IntT id = -1;
      m_treeStore.GetValue(row, MLEId, id);
      
      // Get and set the visibility
      bool visible = true;
      m_treeStore.GetValue(row, MLEVisible, visible);
      visible = !visible;
      
      // Do these layers effect any other selections?
      HashC<IntT, bool> zOrderSet;
      SArray1dC<bool> toggleFlag(m_layerList.Size());
      toggleFlag.Fill(false);
      SArray1dIterC<bool> itToggleFlag(toggleFlag);
      for (DLIterC<MarkupLayerInfoC> it(m_layerList); it;)
      {
        bool effected = false;
        if (!*itToggleFlag)
        {
          // Add the toggled row
          if (it->Id() == id)
          {
            zOrderSet.Add(it->ZOrderSet());
          }
          else
          {
            // Will toggling this layer effect all any other layers
            for (HashIterC<IntT, bool> itZOrder(it->ZOrderSet()); itZOrder; itZOrder++)
            {
              // If this markup layer is effected, add it's layers to the set
              bool showVisibility = true;
              if (zOrderSet.Lookup(*itZOrder, showVisibility))
              {
                zOrderSet.Add(it->ZOrderSet());
                effected = true;
                break;
              }
            }
          }
        }
        
        // Carry on or go back to the beginning?
        if (effected)
        {
          *itToggleFlag = true;
          it.First();
          itToggleFlag.First();
        }
        else
        {
          it++;
          itToggleFlag++;
        }
      }
      
      // Signal all the layers and the display
      UpdateZOrderLayerData(zOrderSet, visible);
    }

    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::CBActivated(TreeModelPathC &path, StringC &colName)
  {
    ONDEBUG(std::cerr << "GUIMarkupLayerEditorBodyC::CBActivated colName=" << colName << endl);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    // Get the row values
    TreeModelIterC row = m_treeStore.Path2Iter(path.Text());
    if (!row.IsValid())
    {
      return false;
    }
    
    // Colour
    if (colName == "Colour")
    {
      // Get the id
      IntT id = -1;
      m_treeStore.GetValue(row, MLEId, id);
      
      // Find the markup
      ByteRGBValueC colour(255, 255, 255);
      for (DLIterC<MarkupLayerInfoC> it(m_layerList); it; it++)
      {
        if (it->Id() == id)
        {
          if (it->ColourDefault() || !it->ColourEditable())
          {
            return true;
          }
          
          colour = it->Colour();
          break;
        }
      }
      
      // Create the selector
      m_selectedRow = row;
      m_colourSelector = ColourSelectorC("Colour Selector", UInt16RGBValueC(colour.Red() * 256, colour.Green() * 256, colour.Blue() * 256));
      ConnectRef(m_colourSelector.Selected(), *this, &GUIMarkupLayerEditorBodyC::CBColourSelected);
      m_colourSelector.GUIShow();
    }
    
    return true;
  }
  
  
  
  bool GUIMarkupLayerEditorBodyC::CBColourSelected(UInt16RGBValueC &colour)
  {
    ONDEBUG(std::cerr << "GUIMarkupLayerEditorBodyC::CBColourSelected colour=" << colour[0] / 256 << ", " << colour[1] / 256 << ", " << colour[2] / 256 << endl);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");

    MutexLockC lock(m_lock);
   
    m_colourSelector.GUIHide();
    
    if (m_selectedRow.IsValid())
    {
      // Get the id
      IntT selectedId = -1;
      m_treeStore.GetValue(m_selectedRow, MLEId, selectedId);
      
      for (DLIterC<MarkupLayerInfoC> it(m_layerList); it; it++)
      {
        if (it->Id() == selectedId)
        {
          if ((!it->ColourDefault()) && (it->ColourEditable()))
          {
            ByteRGBValueC layerCol(colour[0] / 256, colour[1] / 256, colour[2] / 256);
            
            // Update the markup info
            it->SetColour(layerCol);
            
            // Update the tree
            StringC colourVal;
            colourVal.form("#%02x%02x%02x", layerCol.Red(), layerCol.Green(), layerCol.Blue());
            m_treeStore.GUISetValue(m_selectedRow, MLEColourFore, colourVal);
            m_treeStore.GUISetValue(m_selectedRow, MLEColourBack, colourVal);
            
            // Update everything else
            m_sigLayerChange(*it);
          }
          
          break;
        }
      }
      
      m_selectedRow = TreeModelIterC();
    }
    
    return true;
  }
  
  

  void GUIMarkupLayerEditorBodyC::UpdateZOrderLayerData(HashC<IntT, bool> &zOrderSet, const bool visible)
  {
    ONDEBUG(std::cerr << "GUIMarkupLayerEditorBodyC::UpdateZOrderLayerData setSize=" << zOrderSet.Size() << " visible=" << (visible? "Y" : "N") << endl);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    // Signal all effected layers
    for (HashIterC<IntT, bool> it(zOrderSet); it; it++)
    {
      m_sigVisibility(it.Key(), visible);
      
      // Update all layer toggles
      DListC<TreeModelIterC> rowList;
      m_zOrderRows.Lookup(it.Key(), rowList);
      for (DLIterC<TreeModelIterC> it(rowList); it; it++)
      {
        m_treeStore.GUISetValue(*it, MLEVisible, visible);
      }
    }
  }
  
  
  
}
