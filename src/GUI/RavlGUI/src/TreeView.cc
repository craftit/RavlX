// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/9/2003"
//! file="Ravl/GUI/GTK/TreeView.cc"

#include "Ravl/GUI/TreeView.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/StringList.hh"
#include "Ravl/GUI/DragAndDrop.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

#include  <gtk/gtk.h>

#include "WidgetDNDInfo.hh"
 
#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN {

  //:---------------------------------------------------------------------------------
  
  //: Default constructor.
  
  TreeViewColumnC::TreeViewColumnC()
    : renderers(1),
      column(0),
      sort(false),
      ascending(false),
      columnId(-1)
  {}
  
  //:---------------------------------------------------------------------------------

  static void tree_view_toggle_cb(GtkCellRendererToggle *cellrenderertoggle,gchar *arg1,gpointer treeCol) {
    //cerr << "tree_view_toggle(), Called. \n";
    Signal2C<TreeModelIterC, StringC> sig = ((TreeViewColumnRendererC *) treeCol)->SignalChanged();
    RavlAssert(sig.IsValid());
    RavlAssert(((TreeViewColumnRendererC *) treeCol)->TreeBody() != 0);
    TreeModelIterC iter = ((TreeViewColumnRendererC *) treeCol)->TreeBody()->Path2Iter(arg1);
    StringC strColumnName = ((TreeViewColumnRendererC *) treeCol)->Name();
    sig(iter, StringC(strColumnName));
  }
  
  static void tree_view_edit_cb(GtkCellRendererToggle *cellrenderertoggle,gchar *arg1,gchar *arg2,gpointer treeCol) {
    //cerr << "tree_view_edit(), Called. \n";
    Signal2C<TreeModelIterC,StringC> sig = ((TreeViewColumnRendererC *) treeCol)->SignalChanged();
    RavlAssert(sig.IsValid());
    RavlAssert(((TreeViewColumnRendererC *) treeCol)->TreeBody() != 0);
    TreeModelIterC iter = ((TreeViewColumnRendererC *) treeCol)->TreeBody()->Path2Iter(arg1);
    sig(iter,StringC(arg2));
  }
  
  static void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer obj) {
    TreeViewBodyC *bod = (TreeViewBodyC *) obj;
    // Filter first selection event
    if (bod->FilterFirstSelections()) {
      bod->GUIDeselectAll();
      return ;
    }
    TreeModelIterC rowIter;
    DListC<TreeModelIterC> ret = bod->GUISelected();
    bod->SelectionChanged()(ret);
    return ;
  }

  static const StringC attrText("text");
  static const StringC attrPixbuf("pixbuf");
  static const StringC attrActive("active");
  
  
  //: Constructor.
  
  TreeViewBodyC::TreeViewBodyC(const TreeModelC &tm,
                               const DListC<StringC> &ndisplayColumns,
                               GtkSelectionMode nselMode,
                               bool ignoreInitialSelectionSignals) 
    : treeModel(tm),
      selection(0),
      m_preselection(NULL),
      selectionChanged(DListC<TreeModelIterC>()),
      firstSelection(ignoreInitialSelectionSignals ? 2 : 0),
      selMode(nselMode)
  {
    UIntT nCols = ndisplayColumns.Size();
    DListC<StringC> dispList = ndisplayColumns;
    if(nCols == 0) {
      dispList = DListC<StringC>(); // Don't modify the input list.
      nCols = treeModel.Cols();
      for(UIntT i = 0;i < nCols;i++)
        dispList.InsLast(treeModel.ColumnType(i).Name());
    }
    
    //std::cerr << "TreeViewBodyC::TreeViewBodyC(), Called. " << nCols << "\n";
    displayColumns = SArray1dC<TreeViewColumnC>(dispList.Size());
    SArray1dIterC<TreeViewColumnC> ait(displayColumns);
    for(DLIterC<StringC> it(dispList);it;it++,ait++) {
      StringListC subCols(*it,"|");
      UIntT noSubCols = subCols.Size();
      SArray1dC<TreeViewColumnRendererC> renderers(noSubCols);
      
      SArray1dIterC<TreeViewColumnRendererC> rit(renderers);
      
      for(DLIterC<StringC> sit(subCols);sit;sit++) {
        StringC name = *sit;
        //cerr << "Name=" << name << "\n";
        RavlAssert(!name.IsEmpty());
        char key = name[0];
        bool mainName = false;
        if(key == '@') {
          name = name.after(0);
          key = name[0];
        }
        else
          mainName = true;
        
        if(key == '-' || key == '+') {
          name = name.after(0);
          rit->SetExpand(key == '+');
        }
        
        if(mainName)
          ait->Name(name); // Set the name of the column
        rit->Name(name);
        
        UIntT sourceCol = treeModel.ColNumber(name);
        if(sourceCol == ((UIntT) -1)) {
          std::cerr << "Unknown column '" << name << "'\n";
          continue;
        }
        
        AttributeTypeC colType = treeModel.ColumnType(sourceCol);
        StringC renderType;
        switch(colType.ValueType()) {
          case AVT_Bool: 
            renderType ="bool";
            // Get value from 'active' from column '*it'
            rit->Attributes()[attrActive] = Tuple2C<StringC,bool>(name,true);
            break;
          case AVT_ByteRGBImage: 
            renderType ="pixbuf";
            // Get value from 'pixbuf' from column '*it'
            rit->Attributes()[attrPixbuf] = Tuple2C<StringC,bool>(name,true);
            break;
          case AVT_Enum:
#if 0
            renderType ="combo";
            // Get value from 'text' from column '*it'
            rit->Attributes()[attrText] = Tuple2C<StringC,bool>(name,true);            
            break;
#endif
          case AVT_String:  
          case AVT_Int:
          case AVT_Int64:
          case AVT_Real:
            renderType ="text";
            // Get value from 'text' from column '*it'
            rit->Attributes()[attrText] = Tuple2C<StringC,bool>(name,true);
            break;
          case AVT_Abstract:
          case AVT_None:
          case AVT_Invalid:
          case AVT_Component:
            break;
        }
        rit->SetRenderType(renderType);
        //std::cerr << "Setting '" << ait->Name() << "' to render type '" << ait->RenderType() << "'\n";
      
        rit++;
      }
      renderers = SArray1dC<TreeViewColumnRendererC>(renderers,rit.Index().V());
      ait->SetRenderers(renderers);
    }
  }
  
  //: Constructor.
  
  TreeViewBodyC::TreeViewBodyC(const TreeModelC &tm,const SArray1dC<TreeViewColumnC> &displayColumns, GtkSelectionMode nselMode,
                               bool ignoreInitialSelectionSignals) 
    : treeModel(tm),
      selection(0),
      m_preselection(NULL),
      selectionChanged(DListC<TreeModelIterC>()),
      displayColumns(displayColumns),
      firstSelection(ignoreInitialSelectionSignals ? 2 : 0),
      selMode(nselMode)
  {}
  
  //: Default constructor.
  
  TreeViewBodyC::TreeViewBodyC(bool ignoreInitialSelectionSignals) 
    : selection(0),
      m_preselection(NULL),
      selectionChanged(DListC<TreeModelIterC>()),
      firstSelection(ignoreInitialSelectionSignals ? 2 : 0),
      selMode(GTK_SELECTION_NONE)
  {}

  static StringC FindMajorColumnName(const StringC &name) {
    if(name.IsEmpty() || name[0] != '@')
      return name; // Not a composite name.
    StringListC lst(StringC(name).after(0),"|");
    for(DLIterC<StringC> it(lst);it;it++) {
      if(it->IsEmpty() || (*it)[0] == '-') continue;
      return *it;
    }
    return name;
  }
  
  //: Get the column number for a given name 
  // Returns -1 if not found.
  
  IntT TreeViewBodyC::ColumnName2Number(const StringC &rawName) const {
    StringC name = FindMajorColumnName(rawName);
    for(SArray1dIterC<TreeViewColumnC> it(displayColumns);it;it++)
      if (it->Name() == name)
        return it.Index().V();
    return -1;
  }
  
  //: Get the column number for a given name 
  // Returns -1 if not found.
  
  IntT TreeViewBodyC::SubColumnName2Number(const StringC &rawName, const IntT &col) {
    StringC name = FindMajorColumnName(rawName);    
    RavlAssertMsg(col >= 0 && col < (IntT)displayColumns.Size(), "TreeViewBodyC::SubColumnName2Number invalid column");
    TreeViewColumnC &treecol = displayColumns[col];
    for(SArray1dIterC<TreeViewColumnRendererC> rit(treecol.Renderers());rit;rit++)
      if (rit->Name() == name)
        return rit.Index().V();
    return -1;
  }
  
  //: Set an attribute for a column
  // Possible keys include: "editable", "sortable", "activateable", "foreground", "background", "reorderable", "resizable"
  
  bool TreeViewBodyC::GUISetAttribute(IntT colNum,const StringC &key,const StringC &value,bool proxy) {
    displayColumns[colNum].Attributes()[key] = Tuple2C<StringC,bool>(value,proxy);
    
    StringC name = displayColumns[colNum].Name();    
    GtkTreeViewColumn *column = const_cast<GtkTreeViewColumn *>(displayColumns[colNum].TreeViewColumn());
    if(column == 0) return true; // Column not setup yet.
    RavlAssert(Manager.IsGUIThread());
    IntT subColNum = SubColumnName2Number(name,colNum);
    if(subColNum < 0) subColNum = 0;
    GtkCellRenderer *renderer = displayColumns[colNum].Renderers()[subColNum].Renderer();
    
    return SetAttribute(column,     /* Current column. */
                        renderer,   /* Current renderer */
                        name,       /* Col name */
                        key,        /* Attr name */
                        value,      /* Attr value */
                        proxy       /* proxy. */
                        );    
  }
  
  
  //: Set an attribute for a column
  
  bool TreeViewBodyC::SetAttribute(IntT colNum,const StringC &key,const StringC &value,bool proxy) {
    displayColumns[colNum].Attributes()[key] = Tuple2C<StringC,bool>(value,proxy);
    
    StringC name = displayColumns[colNum].Name();
    GtkTreeViewColumn *column = const_cast<GtkTreeViewColumn *>(displayColumns[colNum].TreeViewColumn());
    if(column == 0) return true; // Column not setup yet.
    RavlAssert(Manager.IsGUIThread());
    IntT subColNum = SubColumnName2Number(name,colNum);
    if(subColNum < 0) subColNum = 0;
    GtkCellRenderer *renderer = displayColumns[colNum].Renderers()[subColNum].Renderer();
    
    return SetAttribute(column,     /* Current column. */
                        renderer,   /* Current renderer */
                        name,       /* Col name */
                        key,        /* Attr name */
                        value,      /* Attr value */
                        proxy       /* proxy. */
                        );    
  }
  
  //: Set an attribute for a column
  // Possible keys include: "editable", "sortable", "activateable", "foreground", "background", "reorderable", "resizable"
  
  bool TreeViewBodyC::SetAttribute(IntT colNum,UIntT subCol,const StringC &key,const StringC &value,bool proxy) {
    displayColumns[colNum].Renderers()[subCol].Attributes()[key] = Tuple2C<StringC,bool>(value,proxy);
    
    StringC name = displayColumns[colNum].Name();
    GtkTreeViewColumn *column = const_cast<GtkTreeViewColumn *>(displayColumns[colNum].TreeViewColumn());
    if(column == 0) return true; // Column not setup yet.
    RavlAssert(Manager.IsGUIThread());
    GtkCellRenderer *renderer = displayColumns[colNum].Renderers()[subCol].Renderer();
    
    return SetAttribute(column,     /* Current column. */
                        renderer,   /* Current renderer */
                        name,       /* Col name */
                        key,        /* Attr name */
                        value,      /* Attr value */
                        proxy       /* proxy. */
                        );
  }
  
  //: Set an attribute for a column
  // Possible keys include: "editable", "sortable", "activateable", "foreground", "background", "reorderable", "resizable"
  
  bool TreeViewBodyC::SetAttribute(const StringC &colName,const StringC &key,const StringC &value,bool proxy) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IntT colNum = ColumnName2Number(colName);
    if (colNum >= 0 && displayColumns[colNum].Renderers().Size() == 1)
    {
      ONDEBUG(std::cerr << "TreeViewBodyC::SetAttribute name(" << colName << ") col(" << colNum << ")\n");
      return SetAttribute(colNum,key,value,proxy);
    }
    
    for (UIntT i = 0; i < displayColumns.Size(); i++)
      if ((colNum = SubColumnName2Number(colName, i)) >= 0)
      {
        ONDEBUG(std::cerr << "TreeViewBodyC::SetAttribute name(" << colName << ") col(" << i << ") sub(" << colNum << ")\n");
        return SetAttribute(i, colNum,key,value,proxy);
      }

    return false;
  }
  
  //: Set an attribute for a column
  // Possible keys include: "editable", "sortable", "activateable", "foreground", "background", "reorderable", "resizable"
  
  bool TreeViewBodyC::SetAttribute(const StringC &colName,UIntT subCol,const StringC &key,const StringC &value,bool proxy) {
    IntT colNum = ColumnName2Number(colName);
    if(colNum < 0) {
      RavlSysLog(SYSLOG_ERR) << "TreeViewBodyC::SetAttribute(), Unknown column '" << colName << "' ";
      return false;
    }    
    return SetAttribute(colNum,subCol,key,value,proxy);
  }
  
  //: Helper for setting attributes.
  
  bool TreeViewBodyC::SetAttribute(GtkTreeViewColumn *column,GtkCellRenderer *renderer,const StringC &colName,const StringC &attrName,const StringC &attrValue,bool proxy) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::SetAttribute(colName=" << colName << ",attrName=" << attrName << ",attrValue=" << attrValue << ",proxy=" << proxy);
    RavlAssert(column != 0);
    if(proxy) { // Proxy ?
      UIntT sourceCol = treeModel.ColNumber(attrValue);
      if(sourceCol == ((UIntT) -1)) {
        RavlSysLog(SYSLOG_ERR) << "Failed to find column '" << colName << "' \n";
        RavlAssert(0);
        return false;
      }
      RavlAssert(renderer != 0);
      gtk_tree_view_column_add_attribute(column,renderer,attrName,sourceCol);
      return true;
    }
    gboolean val = (attrValue == "1") ? 1 : 0;

    //cerr << "Setting attribute '" << ait.Key() << "' to '" << at.Data1() << "'\n";
    if(attrName == "editable" || attrName == "activatable") {
      RavlAssert(renderer != 0);
      g_object_set(G_OBJECT(renderer), attrName,val,NULL);
    }
    // Enable sorting
    else if (attrName == "sortable") {
      IntT colNum;
      if(attrValue == "1") // For backward compatibility
        colNum = treeModel.ColNumber(colName);
      else
        colNum = treeModel.ColNumber(attrValue);
      if(colNum >= 0)
        gtk_tree_view_column_set_sort_column_id(GTK_TREE_VIEW_COLUMN(column), colNum);
      else 
        std::cerr << "TreeViewBodyC::SetAttribute, Failed to find column '" << attrValue << "' for '" << colName << "'";
    }
    // Enable column reordering
    else if (attrName == "reorderable") {
      gtk_tree_view_column_set_reorderable(GTK_TREE_VIEW_COLUMN(column), true);
    }	  
    // Enable column resizing
    else if (attrName == "resizable") {
      gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), true);
    }
    else if (attrName == "visible") {
      gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), val);
    }
    else if (attrName == "renderer") 
    {} 
    else  { // Don't know!!
      RavlSysLog(SYSLOG_ERR) << "TreeViewBodyC::SetAttribute, Unknown attribute '" << attrName << "' ";
      RavlAssert(0);
      return false;
    }

    return true;
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool TreeViewBodyC::Create(GtkWidget *nwidget) {
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::Create()";)
    widget = nwidget;
    
    // Setup tree model.
    
    RavlAssert(treeModel.IsValid());
    treeModel.Create();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widget),treeModel.TreeModel());
    
    // Build view 
    for(SArray1dIterC<TreeViewColumnC> it(displayColumns);it;it++) {
      //const StringC &colRenderType = it->Renderer();
      GtkCellRenderer *renderer = 0;
      IntT col_offset = 0;

      GtkTreeViewColumn *column = gtk_tree_view_column_new();
      gtk_tree_view_column_set_title(column, it->Name());
      col_offset = (gtk_tree_view_append_column(GTK_TREE_VIEW(widget),column) - 1);
      RavlAssert(col_offset >= 0);
      it->SetColumnId(col_offset);
      it->ColumnView(column);

      //: See if the default renderer has been overidden....
      Tuple2C<StringC,bool> rendererAttribute;
      it->Attributes().Lookup("renderer", rendererAttribute);

      for(SArray1dIterC<TreeViewColumnRendererC> rit(it->Renderers());rit;rit++) {

        //: See if the default col renderer has been overriden
        Tuple2C<StringC,bool> rendererColAttribute;
        rit->Attributes().Lookup("renderer", rendererColAttribute); 
        
        // use the overriden renderer always
        if (!rendererColAttribute.Data1().IsEmpty())
          rit->SetRenderType(rendererColAttribute.Data1());
        else if (!rendererAttribute.Data1().IsEmpty())
          rit->SetRenderType(rendererAttribute.Data1());
       
        const StringC &renderType = rit->RenderType();

        renderer = 0;
        if (renderType == "bool") { // Bool render
          renderer = gtk_cell_renderer_toggle_new(); 
          if(rit->SignalChanged().IsValid()) {
            g_signal_connect(G_OBJECT(renderer),
                             "toggled",
                             G_CALLBACK(tree_view_toggle_cb),
                             &(*rit));
            rit->TreeBody(this);
          }
        } else if (renderType == "text") {
          renderer = gtk_cell_renderer_text_new();
          if(rit->SignalChanged().IsValid()) {
            g_signal_connect(G_OBJECT(renderer),
                             "edited",
                             G_CALLBACK(tree_view_edit_cb),
                             &(*rit));
            rit->TreeBody(this);
          }
        } else if (renderType == "pixbuf") {
          renderer = gtk_cell_renderer_pixbuf_new();
        } else if (renderType == "combo") {
#ifdef __GTK_CELL_RENDERER_COMBO_H__
          renderer = gtk_cell_renderer_combo_new(); 
#endif
        } else if (renderType == "progress") {
#ifdef  __GTK_CELL_RENDERER_PROGRESS_H__
          renderer = gtk_cell_renderer_progress_new(); 
#endif
        } else if (renderType == "spin") {
#ifdef  __GTK_CELL_RENDERER_SPIN_H__
          renderer = gtk_cell_renderer_spin_new (); 
#endif
        }
        if(renderer == 0) {
          std::cerr << "Unknown or unsupported render type '" << rit->RenderType() << "'\n";
          RavlAssert(0);
        }
        
        // Store a pointer to the renderer
        if(renderer != 0)
          rit->SetRenderer(renderer);
        
        gtk_tree_view_column_pack_start(column,
                                        renderer,
                                        rit->Expand());

        if (it->Sort())
        {
          gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(treeModel.TreeModel()),
                                               col_offset,
                                               it->SortAscending() ? GTK_SORT_ASCENDING : GTK_SORT_DESCENDING);
        }
        
        // Setup attributes.
        for(HashIterC<StringC,Tuple2C<StringC,bool> > ait(rit->Attributes());ait;ait++) {
          SetAttribute(column,     /* Current column. */
                       renderer,   /* Current renderer */
                       it->Name(), /* Col name */
                       ait.Key(),  /* Attr name */
                       ait.Data().Data1(), /* Attr value */
                       ait.Data().Data2()  /* proxy. */
                       );
        }
      }
    }
    
    // Setup selection
    
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
    gtk_tree_selection_set_mode(selection, selMode);
    g_signal_connect(G_OBJECT(selection),
                     "changed",
		                 G_CALLBACK(tree_selection_changed_cb),
                     this);
    
    // Finalise widget
    
    ConnectSignals();

    // If a selection was made prior to Create() being called we need to redo the selection now...
    if (m_preselection != NULL) {
      gtk_tree_selection_select_iter(selection, m_preselection);
    }
    return true;
  }
  
  //: Create widget.
  
  bool TreeViewBodyC::Create() {
    if(widget != 0)
      return true; // Already constructed.
    return Create(gtk_tree_view_new());
  }
  
  //: Access changed signal for a column
  Signal0C &TreeViewBodyC::ChangedSignal(UIntT colNum,UIntT subColNo) {
    TreeViewColumnC &col = displayColumns[colNum];
    TreeViewColumnRendererC &colRender = col.Renderers()[subColNo];
    Signal0C &sig = colRender.SignalChanged();
    if(!sig.IsValid()) {
	if(colRender.RenderType() == "bool" || colRender.RenderType() == "text")
	{
	  sig = Signal2C<TreeModelIterC,StringC>(TreeModelIterC(),StringC(""));
	}
    }
    return sig;
  }
  
  //: Access changed signal for a column
  
  Signal0C &TreeViewBodyC::ChangedSignal(const StringC &colName) {
    IntT colNum = ColumnName2Number(colName);
    IntT subColNum = 0;
    if (colNum < 0 || displayColumns[colNum].Renderers().Size() != 1)
    {
      for (UIntT i = 0; i < displayColumns.Size(); i++)
        if ((subColNum = SubColumnName2Number(colName, i)) >= 0)
        {
          colNum = i;
          break;
        }
    }
    RavlAssertMsg(colNum >= 0, "TreeViewBodyC::ChangedSignal() unknown column");
    TreeViewColumnC &col = displayColumns[colNum];
    TreeViewColumnRendererC &colRender = col.Renderers()[subColNum];
    Signal0C &sig = colRender.SignalChanged();
    if(!sig.IsValid()) {
	if(colRender.RenderType() == "bool" || colRender.RenderType() == "text")
	  {
	    sig = Signal2C<TreeModelIterC,StringC>(TreeModelIterC(),StringC(""));
	  }
    }
    return sig;
  }


  //: Sort treeview by column colNum.
  
  void TreeViewBodyC::Sort(UIntT colNum, bool bAscending) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISortNum,colNum,bAscending));
  }

  //: Sort treeview by column colNum.
  // GUI thread only
  
  bool TreeViewBodyC::GUISort(UIntT colNum, bool bAscending) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(colNum >= 0);
    displayColumns[colNum].SetSort(true,bAscending);
    // Set sorting
    if(widget != 0) {
      IntT colId = displayColumns[colNum].ColumnId();
      ONDEBUG(std::cerr << "TreeViewBodyC::GUISort colNum=" << colNum << " colId=" << colId << endl);
      RavlAssert(colId >= 0);
      gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(treeModel.TreeModel()),
                                           colId,
                                           bAscending ? GTK_SORT_ASCENDING : GTK_SORT_DESCENDING);
    }
    return true;
  }
  
  //: Sort treeview by named column
  
  void TreeViewBodyC::Sort(const StringC &colName, bool bAscending) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISortNamed,colName,bAscending));
  }
  
  //: Sort treeview by named column 
  // GUI thread only
  
  bool TreeViewBodyC::GUISort(const StringC &colName, bool bAscending) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IntT colNum = treeModel.ColNumber(colName);
    if(colNum < 0) {
      std::cerr << "TreeViewBodyC::GUISort(), Unknown column '" << colName << "' \n";
      return false;
    }
    return GUISort(colNum,bAscending);
  }
  
  //: Expand to the specified path
  
  void TreeViewBodyC::Expand(TreeModelPathC path) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIExpand,path));  
  }

  //: Expand to the specified row iterator

  void TreeViewBodyC::Expand(TreeModelIterC iter) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIExpandIter,iter));
  }

  //: Expand to the specified row iterator
  
  bool TreeViewBodyC::GUIExpandIter(TreeModelIterC iter) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Create path
    TreeModelPathC path(iter);
    // Expand to path    
    GUIExpand(path);
    return true;
  }

  //: Expand to the specified path
  // GUI thread only

  bool TreeViewBodyC::GUIExpand(TreeModelPathC path) {
    // Check validity of widget
    if(widget == 0) {
      //Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIExpand,path));
      std::cerr << "TreeViewBodyC::GUIExpand: Can't expand, widget does not exist yet. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Expand    
    gtk_tree_view_expand_to_path(GTK_TREE_VIEW(widget),path.TreePath());
    return true;
  }
  
  //: Is the specified path expanded?
  // GUI thread only
  
  bool TreeViewBodyC::GUIExpanded(TreeModelPathC path) {    
    // Check validity of widget
    if(widget == 0)
      return false;
    // Check status
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    return gtk_tree_view_row_expanded(GTK_TREE_VIEW(widget),path.TreePath()) != 0;
  }
  
  //: Is the specified iterator expanded?
  // GUI thread only
  
  bool TreeViewBodyC::GUIExpanded(TreeModelIterC iter) {
    // Convert to path
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    TreeModelPathC path(iter);
    // Check status
    return GUIExpanded(path);
  }

  //: Expand the entire tree
  
  void TreeViewBodyC::ExpandAll() {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIExpandAll));
  }
  
  //: Expand the entire tree
  // GUI thread only
  
  bool TreeViewBodyC::GUIExpandAll() {
    // Check validity of widget
    if(widget == 0) {
      //Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIExpandAll));
      std::cerr << "TreeViewBodyC::GUIExpandAll: Can't expand, widget does not exist yet. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Expand all
    gtk_tree_view_expand_all(GTK_TREE_VIEW(widget));
    return true;
  }
  
  //: Collapse the entire tree
  
  void TreeViewBodyC::CollapseAll() {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUICollapseAll));
  }
  
  //: Collapse the entire tree
  // GUI thread only
  
  bool TreeViewBodyC::GUICollapseAll() {
    // Check validity of widget
    if(widget == 0) {
      //Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUICollapseAll));
      std::cerr << "TreeViewBodyC::GUICollapseAll: Can't collapse, widget does not exist yet. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Collapse all
    gtk_tree_view_collapse_all(GTK_TREE_VIEW(widget));
    return true;
  }

  //: Select the specified path
  
  void TreeViewBodyC::Select(TreeModelPathC path) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISelectPath,path));  
  }

  //: Select the specified row iterator

  void TreeViewBodyC::Select(TreeModelIterC iter) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISelectIter,iter));  
  }

  //: Select the specified path
  // GUI thread only

  bool TreeViewBodyC::GUISelectPath(TreeModelPathC path) {
    if (selection == 0) {
      ONDEBUG(std::cerr << "Warning No selection setup!\n");
      return false;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    ONDEBUG(std::cerr << "Selecting path... \n");
    gtk_tree_selection_select_path(selection,path.TreePath());
    return true;
  }
  
  //: Select the specified row iterator
  // GUI thread only
  
  bool TreeViewBodyC::GUISelectIter(TreeModelIterC iter) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::GUISelectIter()");
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if (selection == 0) {
      ONDEBUG(std::cerr << "TreeViewBodyC::GUISelectedIter, No selection setup.");
      m_preselection = iter.TreeIter();
      return false;
    }
    if(!iter.IsValid()) {
      std::cerr << "TreeViewBodyC::GUISelectIter(), Warning: Asked to select invalid iterator. \n";
      return false;
    }
    ONDEBUG(std::cerr << "Setting selection.\n");
    gtk_tree_selection_select_iter(selection,iter.TreeIter());
    return true;
  }

  //: Is the specified path selected?
  // GUI thread only
  
  bool TreeViewBodyC::GUISelectedPath(TreeModelPathC path) {
    if (selection == 0) {
      ONDEBUG(std::cerr << "TreeViewBodyC::GUISelectedIter, No selection setup.");
      return false;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    return gtk_tree_selection_path_is_selected(selection,path.TreePath()) != 0;
  }

  //: Is the specified row iterator selected?
  // GUI thread only
  
  bool TreeViewBodyC::GUISelectedIter(TreeModelIterC iter) {
    ONDEBUG(std::cerr << "TreeViewBodyC::GUISelectedIter, Called.");
    if (selection == 0) {
      ONDEBUG(std::cerr << "TreeViewBodyC::GUISelectedIter, No selection setup.");
      return false;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    return gtk_tree_selection_iter_is_selected(selection,iter.TreeIter()) != 0;
  }

  //: Get list of selected rows.
  
  DListC<TreeModelIterC> TreeViewBodyC::GUISelected() {
    DListC<TreeModelIterC> ret;
    if(selection == 0)
      return ret; // Not created yet!.
    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Get selection
    GtkTreeModel* model;
    GList* list = gtk_tree_selection_get_selected_rows(selection,&model);    
    GList* ptr = list;

    // Extract iterators
    while (ptr) {
      GtkTreePath* path(static_cast<GtkTreePath*>(ptr->data));
      TreeModelIterC iter(model,path);
      ret.InsLast(iter);
      gtk_tree_path_free(path);
      ptr = ptr->next;
    }

    // Free list
    g_list_free(list);

    // Done
    return ret;
  }

  //: Deselect the specified path
  void TreeViewBodyC::Deselect(TreeModelPathC path)  {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIDeselectPath,path));  
  }
  
  //: Deselect the specified row iterator
  
  void TreeViewBodyC::Deselect(TreeModelIterC iter) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIDeselectIter,iter));
  }
  
  //: Deselect the specified path
  // GUI thread only
  
  bool TreeViewBodyC::GUIDeselectPath(TreeModelPathC path) {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_unselect_path(selection,path.TreePath());
    return true;
  }

  //: Deselect the specified row iterator
  // GUI thread only
  
  bool TreeViewBodyC::GUIDeselectIter(TreeModelIterC iter) {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_unselect_iter(selection,iter.TreeIter());
    return true;
  }

  //: Select all rows
  
  void TreeViewBodyC::SelectAll() {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISelectAll));
  }

  //: Select all rows
  // GUI thread only

  bool TreeViewBodyC::GUISelectAll() {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_select_all(selection);
    return true;
  }
    
  //: Deselect all rows

  void TreeViewBodyC::DeselectAll() {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIDeselectAll));
  }
    
  //: Deselect all rows
  // GUI thread only

  bool TreeViewBodyC::GUIDeselectAll() {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_unselect_all(selection);
    return true;
  }

  //: Select the rows between the two paths
  
  void TreeViewBodyC::SelectRange(TreeModelPathC from, TreeModelPathC to) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUISelectRange,from,to));
  }

  //: Select the rows between the two paths
  // GUI thread only
  
  bool TreeViewBodyC::GUISelectRange(TreeModelPathC from, TreeModelPathC to) {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_select_range(selection,from.TreePath(),to.TreePath());
    return true;
  }

  //: Deselect the rows between the two paths
  
  void TreeViewBodyC::DeselectRange(TreeModelPathC from, TreeModelPathC to) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIDeselectRange,from,to));
  }

  //: Deselect the rows between the two paths
  // GUI thread only
  
  bool TreeViewBodyC::GUIDeselectRange(TreeModelPathC from, TreeModelPathC to) {
    if (selection == 0)
      return false;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_selection_unselect_range(selection,from.TreePath(),to.TreePath());
    return true;
  }

  //: Scroll the treeview to the specified path

  void TreeViewBodyC::ScrollTo(TreeModelPathC path) {
    Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIScrollTo,path));  
  }

  //: Scroll the treeview to the specified row iterator
  
  void TreeViewBodyC::ScrollTo(TreeModelIterC iter) {
    // Create path
    TreeModelPathC path(iter);
    // Expand to path    
    ScrollTo(path);
  }

  //: Scroll the treeview to the specified path
  // GUI thread only
  
  bool TreeViewBodyC::GUIScrollTo(TreeModelPathC path) {
    // Check validity of widget
    if(widget == 0) {
      //Manager.Queue(Trigger(TreeViewC(*this),&TreeViewC::GUIScrollTo,path));
      std::cerr << "TreeViewBodyC::GUIScrollTo: Can't scroll to, widget does not exist yet. \n";
      return true;
    }
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Expand
    gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(widget), path.TreePath(), NULL, true, 0, 0);
    return true;
  }
  
  //: Get path to a position in the tree.
  // 'pos' must be widget coordinates.
  
  TreeModelPathC TreeViewBodyC::GUIGetPathTo(const Index2dC &pos) {
    if(widget == 0) return TreeModelPathC(); // Nothing yet!
    GtkTreePath *path = 0;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");    
    if(!gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
                                          pos[1].V(),pos[0].V(),
                                          &path,
                                          0)) {
      return TreeModelPathC();
    }
    // Generate the path class.
    return TreeModelPathC(path,true);
  }
  
  //: Get path to a position in the tree.
  // 'pos' must be widget coordinates.
  
  TreeModelPathC TreeViewBodyC::GUIGetPathTo(const Index2dC &pos,IntT &colNo,Index2dC &cellPos) {
    if(widget == 0) return TreeModelPathC(); // Nothing yet!
    GtkTreePath *path = 0;
    GtkTreeViewColumn *column = 0;
    IntT cellRow = cellPos.Row().V() ; 
    IntT cellCol = cellPos.Col().V() ; 
    if(!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
                                      pos[1].V(),pos[0].V(),
                                      &path,
                                      &column,
                                      &cellCol,
                                      &cellRow
                                      ))

      return TreeModelPathC();
    for(SArray1dIterC<TreeViewColumnC> it(displayColumns);it;it++) {
      if(it->TreeViewColumn() == column) {
        colNo = it.Index().V();
        break;
      }
    }
    
    // Generate the path class.
    return TreeModelPathC(path,true);
  }
  
  
  //: Get iter for row at position 'pos' in the tree.
  // 'pos' must be widget coordinates.
  
  TreeModelIterC TreeViewBodyC::GUIGetIter(const Index2dC &pos) {
    TreeModelPathC path = GUIGetPathTo(pos);
    
    if(!path.IsValid() || path.Text().IsEmpty()) {
      ONDEBUG(std::cerr << "TreeViewBodyC::GUIGetIter, Path empty. \n");
      return TreeModelIterC();
    }
    ONDEBUG(std::cerr << "TreeViewBodyC::GUIGetIter, Path='" << path.Text() << "' \n");
    return  treeModel.Path2Iter(path.Text());
  }

  //: Send cursor to cell and optionally start editing.
  
  bool TreeViewBodyC::GUISetCursorOnCell(TreeModelPathC &path,bool startEditing,IntT colId,IntT subColId) {
    if(!path.IsValid())
      return false;
    
    GtkTreeViewColumn *focus_column = 0;
    GtkCellRenderer *focus_cell = 0;
    if(colId >= 0) {
      RavlAssert(colId < (IntT) displayColumns.Size());
      TreeViewColumnC &treeViewCol = displayColumns[colId];
      focus_column = treeViewCol.ColumnView();
      if(subColId >= 0) {
        RavlAssert(subColId < (IntT) treeViewCol.Renderers().Size());
        focus_cell = treeViewCol.Renderers()[subColId].Renderer();
      }
    }
    
    gtk_tree_view_set_cursor_on_cell(GTK_TREE_VIEW(widget),
                                     path.TreePath(),
                                     focus_column,
                                     focus_cell,
                                     startEditing);
    
    return true;
  }


  
  //: Access the current cursor selection
  bool TreeViewBodyC::GUIGetCurrentCursor(TreeModelPathC & path, StringC & colName)
  {
    GtkTreePath * tPath = 0; 
    GtkTreeViewColumn * tCol = 0;
    gtk_tree_view_get_cursor((GtkTreeView*) widget, &tPath, &tCol);
    // path must be freed when used, so wrap it....
    TreeModelPathC retPath(tPath, true);
    if ((tPath == NULL) || (tCol==NULL))
      return false; 
    path = retPath; 
    colName = tCol->title;
    return true;
  }


  
  //: Setup widget as drag and drop source.
  
  bool TreeViewBodyC::GUIDNDSource(ModifierTypeT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::GUIDNDSource() - entry";)
    if(dndInfo == 0)
      dndInfo = new WidgetDndInfoC();
    dndInfo->isSource = true;
    dndInfo->SrcFlags = flags;
    dndInfo->SrcEntries = entries;
    dndInfo->SrcActions = actions;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_tree_view_enable_model_drag_source(GTK_TREE_VIEW(widget),
                                           (GdkModifierType) flags,
                                           &(entries[0]),
                                           entries.Size(),
                                           (GdkDragAction) actions
                                           );
    
    
    return true;
  }
  
  //: Disable widget as a drag and drop source.
  
  bool TreeViewBodyC::GUIDNDSourceDisable() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::GUIDNDSourceDisable() - entry";)
    if(dndInfo != 0)
      dndInfo->isSource = false;
    if(widget == 0) return true;
    gtk_tree_view_unset_rows_drag_source(GTK_TREE_VIEW(widget));
    return true;
  }
    
  //: Setup widget as drag and drop target.
  
  bool TreeViewBodyC::GUIDNDTarget(DestDefaultsT flags,const SArray1dC<GtkTargetEntry> &entries,DragActionT actions) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::GUIDNDTarget() - entry";)
    if(dndInfo == 0)
      dndInfo = new WidgetDndInfoC();
    dndInfo->TargFlags = flags;
    dndInfo->TargEntries = entries;
    dndInfo->TargActions = actions;
    dndInfo->isTarget = true;
    if(widget == 0) 
      return true;
    std::cerr << "TreeViewBodyC::GUIDNDTarget(), Called. \n";
    
    gtk_tree_view_enable_model_drag_dest(GTK_TREE_VIEW(widget),
                                         &(entries[0]),
                                         entries.Size(),
                                         (GdkDragAction) actions);
    return true;
  }
  
  //: Disable widget as a drag and drop source.
  
  bool TreeViewBodyC::GUIDNDTargetDisable() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "TreeViewBodyC::GUIDNDTargetDisable() - entry";)
    if(dndInfo != 0)
      dndInfo->isSource = false;
    if(widget != 0)
      gtk_tree_view_unset_rows_drag_dest(GTK_TREE_VIEW(widget));
    return true;
  }
  
}

