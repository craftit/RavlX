// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ListStore.cc"

#include "Ravl/GUI/ListStore.hh"
#include "Ravl/GUI/Manager.hh"

#include "Ravl/SArray1dIter2.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  extern GType Ravl2GTKType(AttributeValueTypeT vtype);


  //: Constructor.

  ListStoreBodyC::ListStoreBodyC()
  : m_sortColumn(-1),
    m_sortAscending(true)
  {}

  //: Constructor. 
  // It the user's resonsiblity to check the model is of the
  // correct type.
  
  ListStoreBodyC::ListStoreBodyC(GtkTreeModel *aModel)
  : m_sortColumn(-1),
    m_sortAscending(true)
  {
    model = aModel;
  }
  
  //: List store.

  ListStoreBodyC::ListStoreBodyC(const SArray1dC<AttributeTypeC> &nColTypes,
                                     const IntT sortColumn,
                                     const bool sortAscending)
  : TreeModelBodyC(nColTypes),
    m_sortColumn(sortColumn),
    m_sortAscending(sortAscending)
  {
    Create();
  }

  //: Create the widget.

  bool ListStoreBodyC::Create() {
    if(model != 0)
      return true;

    SArray1dC<GType> types(colTypes.Size());
    for(SArray1dIter2C<GType,AttributeTypeC> it(types,colTypes);it;it++)
      it.Data1() = Ravl2GTKType(it.Data2().ValueType());
    model = GTK_TREE_MODEL(gtk_list_store_newv(colTypes.Size(),&(types[0])));

    if (m_sortColumn >= 0 && static_cast<UIntT>(m_sortColumn) < colTypes.Size())
    {
      gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(model), m_sortColumn, m_sortAscending ? GTK_SORT_ASCENDING : GTK_SORT_DESCENDING);
    }

    return TreeModelBodyC::Create();
  }

  //: Append a row.

  bool ListStoreBodyC::AppendRow(TreeModelIterC &rowHandle) {
    RavlAssert(model != 0);

    gtk_list_store_append(GTK_LIST_STORE(model),rowHandle.TreeIter());

    return true;
  }



  bool ListStoreBodyC::GUIDeleteRow(TreeModelIterC &rowHandle)
  {
    RavlAssert(model != 0);

    return gtk_list_store_remove(GTK_LIST_STORE(model), rowHandle.TreeIter());
  }



  bool ListStoreBodyC::InsertRowBefore(TreeModelIterC &rowHandle, TreeModelIterC &targetRowHandle)
  {
    RavlAssert(model != 0);

    gtk_list_store_insert_before(GTK_LIST_STORE(model), rowHandle.TreeIter(), targetRowHandle.TreeIter());

    return true;
  }

  //: Set int value.

  bool ListStoreBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!rowIter.IsElm())
      return false;
    gtk_list_store_set(GTK_LIST_STORE(model),rowIter.TreeIter(),col,value,-1);
    return true;
  }
  //: Set bool value.

  bool ListStoreBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, Int64T value) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!rowIter.IsElm())
      return false;
    gtk_list_store_set(GTK_LIST_STORE(model),rowIter.TreeIter(),col,value,-1);
    return true;
  }
  //: Set bool value.

  bool ListStoreBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, bool value) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!rowIter.IsElm())
      return false;
    gtk_list_store_set(GTK_LIST_STORE(model),rowIter.TreeIter(),col,value,-1);
    return true;
  }

  //: Set bool value.

  bool ListStoreBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!rowIter.IsElm())
      return false;
    gtk_list_store_set(GTK_LIST_STORE(model),rowIter.TreeIter(),col,const_cast<char *>(value.chars()),-1);
    return true;
  }

  //: Set bool value.

  bool ListStoreBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!rowIter.IsElm())
      return false;
    gtk_list_store_set(GTK_LIST_STORE(model),rowIter.TreeIter(),col,value.Pixbuf(),-1);
    return true;
  }

  //: Clear store of all values.

  void ListStoreBodyC::GUIEmpty() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_list_store_clear(GTK_LIST_STORE(model));
  }

  //: Move target row before position
  // if position is null, it will be moved to the end of list.

  bool ListStoreBodyC::GUIMoveBefore(const TreeModelIterC &target,const TreeModelIterC &position) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    const GtkTreeIter *positionIter = 0;
    if(position.IsElm())
      positionIter = position.TreeIter();
    gtk_list_store_move_before (GTK_LIST_STORE(model),
                                 const_cast<GtkTreeIter *>(target.TreeIter()),
                                 const_cast<GtkTreeIter *>(positionIter));
    return true;
  }

  //: Move target row after position
  // if position is null, it will be moved to the start of list.

  bool ListStoreBodyC::GUIMoveAfter(const TreeModelIterC &target,const TreeModelIterC &position) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    const GtkTreeIter *positionIter = 0;
    if(position.IsElm())
      positionIter = position.TreeIter();
    gtk_list_store_move_after (GTK_LIST_STORE(model),
                               const_cast<GtkTreeIter *>(target.TreeIter()),
                               const_cast<GtkTreeIter *>(positionIter));
    return true;
  }

  //: Reorder list.

  bool ListStoreBodyC::GUIReorder(const SArray1dC<IntT> &newOrder) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(newOrder.Size() != GUIRows())
      return false;
    gtk_list_store_reorder(GTK_LIST_STORE(model),(gint *)&newOrder[0]);
    return true;
  }

}

