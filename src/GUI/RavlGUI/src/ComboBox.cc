// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

#include "Ravl/GUI/ComboBox.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  //: Constructor
  
  ComboBoxBodyC::ComboBoxBodyC()
  : m_sigRowSelected(TreeModelIterC())
  {}
  
  //: Constructor
  
  ComboBoxBodyC::ComboBoxBodyC(TreeModelC &treeModel, const DListC<IntT> treeColumns)
   : m_treeModel(treeModel),
    m_treeColumns(treeColumns),
    m_sigRowSelected(TreeModelIterC())
  {}
  
  ComboBoxBodyC::ComboBoxBodyC(TreeModelC &treeModel, const DListC<StringC> &treeColumns)
   : m_treeModel(treeModel),
     m_sigRowSelected(TreeModelIterC())
  {
    for(DLIterC<StringC> it(treeColumns);it;it++) {
      UIntT colNum = treeModel.ColNumber(*it);
      if(colNum == ((UIntT)-1)) {
        RavlAssertMsg(0,"Unknown tree column.");
        continue;
      }
      m_treeColumns.InsLast(colNum);
    }

  }
  //: Constructor

  //: Set the tree model to use.
  // This can only be changed before the box is 'Created'.
  
  void ComboBoxBodyC::SetTreeModel(TreeModelC &treeModel, const DListC<IntT> treeColumns)
  {
    if (widget == 0)
    {
      m_treeModel = treeModel;
      m_treeColumns = treeColumns;
    }
  }
  
  //: Access model iterator for the current selection.
  
  TreeModelIterC ComboBoxBodyC::GUISelection() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    TreeModelIterC iter(m_treeModel);
    if(!gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget),iter.TreeIter())) {
      // If nothing is selected invalidate the iterator.
      iter.Invalidate();
    }
    return iter;
  }
  
  //: Set the current selection
  
  bool ComboBoxBodyC::GUISetSelection(TreeModelIterC &selection) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    RavlAssert(selection.IsValid());
    gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget),selection.TreeIter());
    return true;
  }
  
  //: Get index of active selection.
  
  IntT ComboBoxBodyC::GUIGetSelectionIndex() { 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    return gtk_combo_box_get_active (GTK_COMBO_BOX(widget)); 
  }
  
  //: Get index of active selection.
  
  bool ComboBoxBodyC::GUISetSelectionIndex(IntT selectionIndex) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    gtk_combo_box_set_active (GTK_COMBO_BOX(widget),selectionIndex);
    return true;
  }
  
  //: Create the widget.
  
  bool ComboBoxBodyC::Create() {
    return CommonCreate(NULL);
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool ComboBoxBodyC::Create(GtkWidget *widget) {
    return CommonCreate(widget);
  }
  
  //: Create with a widget possibly supplied from elsewhere.
  
  bool ComboBoxBodyC::CommonCreate(GtkWidget *nwidget)
  {
    if (nwidget == 0)
    {
      if (m_treeModel.IsValid())
      {
        widget = gtk_combo_box_new_with_model(m_treeModel.TreeModel());
      }
      else
      {
        // There is currently no way to add to a model or cell layout if this condition is reached.
        widget = gtk_combo_box_new();
      }
    }
    else
    {
      widget = nwidget;
      if (m_treeModel.IsValid())
      {
        // The assumption is that the constructed widget is supplied with the cell layout.
        gtk_combo_box_set_model(GTK_COMBO_BOX(widget), m_treeModel.TreeModel());
      }
    }
    
    // Set up requested renders.
    if (m_treeModel.IsValid()) {
      if(m_treeModel.Cols() > 0 && m_treeColumns.Size() > 0) {
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(widget));
        
        for (DLIterC<IntT> colIter(m_treeColumns); colIter; ++colIter)
          {
            if (*colIter >= 0 && static_cast<UIntT>(*colIter) < m_treeModel.Cols())
              {
                GtkCellRenderer *cell = NULL;
                StringC typeAttribute;
                switch (m_treeModel.ColumnValueType(*colIter))
                  {
                  case AVT_String:
                  case AVT_Int:
                  case AVT_Int64:
                  case AVT_Real:
                  case AVT_Bool:
                    cell = gtk_cell_renderer_text_new();
                    typeAttribute = "text";
                    break;
                  case AVT_ByteRGBImage:
                    cell = gtk_cell_renderer_pixbuf_new();
                    typeAttribute = "pixbuf";
                    break;
                  default:
                    break;
                  }
                
                if (cell != NULL)
                  {
                    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), cell, TRUE);
                    gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), cell, typeAttribute.chars(), *colIter);
                  }
              }
          }
      }
    }


    ConnectSignals();

    ConnectRef(Signal("changed"), *this, &ComboBoxBodyC::CBRowChanged);

    return true;
  }

  bool ComboBoxBodyC::CBRowChanged()
  {
    RavlAssert(widget);

    GtkTreeIter row;
    if (m_treeModel.IsValid() && gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &row))
      m_sigRowSelected(TreeModelIterC(m_treeModel.TreeModel(), &row));
    else
      m_sigRowSelected(TreeModelIterC());

    return true;
  }
  
}
