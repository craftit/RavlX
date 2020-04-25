// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.Graphics.GTK.Control"
//! lib=RavlGUI
//! date="23/9/2003"
//! file="Ravl/GUI/GTK/TreeModel.cc"

#include "Ravl/GUI/TreeModel.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"

#include "Ravl/HashIter.hh"
#include <gtk/gtk.h>
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  GType Ravl2GTKType(AttributeValueTypeT vtype) {
    switch(vtype) {
    case AVT_Bool:     return G_TYPE_BOOLEAN;
    case AVT_Int:      return G_TYPE_INT;
    case AVT_Int64:    return G_TYPE_INT64;
    case AVT_Real:     return G_TYPE_FLOAT;
    case AVT_String:   return G_TYPE_STRING;
    case AVT_Enum:     return G_TYPE_ENUM;
    case AVT_Abstract: return G_TYPE_NONE;
    case AVT_ByteRGBImage: return GDK_TYPE_PIXBUF;
    case AVT_None:     return G_TYPE_NONE;
    case AVT_Invalid:  return G_TYPE_INVALID;
    case AVT_Component: return G_TYPE_NONE;
    }
    return G_TYPE_INVALID;
  }

  //:----------------------------------------------------------------------------

  //: Constructor.

  TreeModelIterBodyC::TreeModelIterBodyC()
    : model(0),
      isElm(true)
  {
    memset(&treeIter,0,sizeof(treeIter));
  }
  
  //: Constructor.

  TreeModelIterBodyC::TreeModelIterBodyC(GtkTreeIter *ntreeIter)
    : model(0),
      treeIter(*ntreeIter),
      isElm(true) // FIXME:- How to check ?
  {}
  
  //: Construct from tree model.

  TreeModelIterBodyC::TreeModelIterBodyC(TreeModelC &ntreeModel)
    : model(0),
      isElm(false)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    model = ntreeModel.Body().model;
    if(gtk_tree_model_get_iter_first (model,&treeIter))
      isElm = true;
  }
  
  TreeModelIterBodyC::TreeModelIterBodyC(GtkTreeModel *nmodel,GtkTreeIter *ntreeIter)
    : model(nmodel),
      treeIter(*ntreeIter),
      isElm(true) // FIXME:- How to check
  {}
  
  //: Construct from tree model and path.

  TreeModelIterBodyC::TreeModelIterBodyC(GtkTreeModel *nmodel,GtkTreePath *treePath)
    : model(nmodel),
      isElm(false)
  {
    RavlAssert(model != 0);
    RavlAssert(treePath != 0);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(gtk_tree_model_get_iter(model,&treeIter,treePath))
      isElm = true;
  }
  
  //: Destructor.

  TreeModelIterBodyC::~TreeModelIterBodyC()
  {}
  
  //: Goto next element at current level.
  // Returns true if succeeded.

  bool TreeModelIterBodyC::Next() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(model != 0);
    if(gtk_tree_model_iter_next(model,&treeIter)) {
      isElm = true;
      return true;
    }
    isElm = false;
    return false;
  }

  //: Return iterator for first child.
  // Will return an invalid iterator if none.

  TreeModelIterC TreeModelIterBodyC::Children() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(model != 0);
    TreeModelIterC ret;
    ret.Model(model);
    if(!gtk_tree_model_iter_children(model,ret.TreeIter(),&treeIter))
      ret.Invalidate();
    return ret;
  }

  //: Does current node have children ?

  bool TreeModelIterBodyC::HasChildren() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(model != 0);
    return gtk_tree_model_iter_has_child(model,&treeIter) != 0;
  }

  //: Get iterator for parent.
  // Returns an invalid handle if none.

  TreeModelIterC TreeModelIterBodyC::Parent() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(model != 0);
    TreeModelIterC ret;
    ret.Model(model);
    if(!gtk_tree_model_iter_parent(model,ret.TreeIter(),&treeIter))
      ret.Invalidate();
    return ret;
  }

  //: Create a copy of the iterator.

  TreeModelIterC TreeModelIterBodyC::Copy() {
    RavlAssert(model != 0);
    return TreeModelIterC(model,&treeIter);
  }

  //: Test if we think we're on a valid element.

  bool TreeModelIterBodyC::IsElm() const
  { return isElm; }

  //: Flag as invalid.

  void TreeModelIterBodyC::Clear() {
    isElm = false;
    memset(&treeIter,0,sizeof(treeIter));
  }

  //:----------------------------------------------------------------------------

  //: Constructor.

  TreeModelIterC::TreeModelIterC(TreeModelC &model,const TreeModelPathC &treePath)
    : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC(model.Body().TreeModel(),const_cast<TreeModelPathC &>(treePath).TreePath()))
  {}

  //:----------------------------------------------------------------------------

  //: Constructor.

  TreeModelPathBodyC::TreeModelPathBodyC()
    : treePath(0)
  { 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    treePath = gtk_tree_path_new (); 
  }

  //: Constructor.

  TreeModelPathBodyC::TreeModelPathBodyC(TreeModelIterC treeIter)
    :  treePath(0)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssertMsg(treeIter.Model()!=0, "TreeModelPathBodyC::TreeModelPathBodyC(TreeModelIterC) - tree iterator has an invalid model!");
    RavlAssertMsg(treeIter.TreeIter()!=0, "TreeModelPathBodyC::TreeModelPathBodyC(TreeModelIterC) - tree iterator is invalid!");
    treePath = gtk_tree_model_get_path(treeIter.Model(), treeIter.TreeIter());
  }
  //: Constructor.

  TreeModelPathBodyC::TreeModelPathBodyC(GtkTreePath *ntreePath,bool canFree)
    : treePath(0)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(canFree)
      treePath = ntreePath;
    else
      treePath = gtk_tree_path_copy (ntreePath);
  }

  //: Destructor.

  TreeModelPathBodyC::~TreeModelPathBodyC() {
    if(treePath != 0) {
      ReadBackLockC lock;
      gtk_tree_path_free (treePath);
    }
  }

  //: Path as text.

  StringC TreeModelPathBodyC::Text() const {
    // I've lookat the source for the following function and it looks thread safe. CG.
    char *str = gtk_tree_path_to_string(treePath);
    StringC ret(str);
    g_free(str);
    return ret;
  }

  //:----------------------------------------------------------------------------

  //"row-changed"
  static void handle_row_changed(GtkTreeModel *treemodel,
         GtkTreePath *arg1,
         GtkTreeIter *arg2,
         gpointer user_data) {
    TreeModelPathC path(arg1,false);
    TreeModelIterC row(arg2);
    (*((Signal2C<TreeModelPathC,TreeModelIterC> *) user_data))(path,row);
  }

  //"row-deleted"
  static void handle_row_deleted(GtkTreeModel *treemodel,
         GtkTreePath *arg1,
         gpointer user_data) {
    TreeModelPathC path(arg1,false);
    TreeModelIterC row;
    (*((Signal2C<TreeModelPathC,TreeModelIterC> *) user_data))(path,row);
  }

  //"row-has-child-toggled"
  static void handle_row_has_child_toggled(GtkTreeModel *treemodel,
             GtkTreePath *arg1,
             GtkTreeIter *arg2,
             gpointer user_data) {
    TreeModelPathC path(arg1,false);
    TreeModelIterC row(arg2);
    (*((Signal2C<TreeModelPathC,TreeModelIterC> *) user_data))(path,row);
  }

  //"row-inserted"
  static void handle_row_inserted(GtkTreeModel *treemodel,
          GtkTreePath *arg1,
          GtkTreeIter *arg2,
          gpointer user_data) {
    TreeModelPathC path(arg1,false);
    TreeModelIterC row(arg2);
    (*((Signal2C<TreeModelPathC,TreeModelIterC> *) user_data))(path,row);
  }

  //"rows-reordered"
  static void handle_rows_reordered(GtkTreeModel *treemodel,
           GtkTreePath *arg1,
           GtkTreeIter *arg2,
           gpointer arg3,
           gpointer user_data) {

    TreeModelPathC path(arg1,false);
    TreeModelIterC row(arg2);
    (*((Signal2C<TreeModelPathC,TreeModelIterC> *) user_data))(path,row);
  }


  //:----------------------------------------------------------------------------

  //: Constructor.

  TreeModelBodyC::TreeModelBodyC()
    : model(0)
  {}

  //: Constructor from a list of types.

  TreeModelBodyC::TreeModelBodyC(const SArray1dC<AttributeTypeC> &ncolTypes)
    : model(0),
      colTypes(ncolTypes)
  {}

  //: Constructor.

  TreeModelBodyC::TreeModelBodyC(GtkTreeModel *aModel)
    : model(aModel)
  {}
  
  //: Destructor.

  TreeModelBodyC::~TreeModelBodyC()
  {
  }


  //: Look up column number of named column.

  UIntT TreeModelBodyC::ColNumber(const StringC &name) const {

    for(SArray1dIterC<AttributeTypeC> it(colTypes);it;it++)
      if(it->Name() == name)
        return it.Index().V();
    return ((UIntT) -1);
  }

  //: Access number of rows in store

  UIntT TreeModelBodyC::GUIRows() const {
    UIntT i = 0;
    GtkTreeIter treeIter;
    if(!gtk_tree_model_get_iter_first (model,&treeIter))
      return 0;
    i++;
    while(gtk_tree_model_iter_next(model,&treeIter))
      i++;
    return i;
  }


  //: Create the widget.

  bool TreeModelBodyC::Create() {
    // Connect signals...
    for(HashIterC<StringC,Signal2C<TreeModelPathC,TreeModelIterC> > it(signals);it;it++)
      ConnectUp(it.Key());
    return true;
  }

  //: Create a new signal.

  void TreeModelBodyC::ConnectUp(StringC name) {

    //, "row-deleted","row-has-child-toggled","row-inserted","rows-reordered"
    Signal2C<TreeModelPathC,TreeModelIterC> &ret =  signals[name];
    if(name == "row-changed") {
      g_signal_connect (model, name,G_CALLBACK (handle_row_changed),&ret);
      return;
    }
    if(name == "row-deleted") {
      g_signal_connect (model, name,G_CALLBACK (handle_row_deleted),&ret);
      return;
    }
    if(name == "row-has-child-toggled") {
      g_signal_connect (model, name,G_CALLBACK (handle_row_has_child_toggled),&ret);
      return;
    }
    if(name == "row-inserted") {
      g_signal_connect (model, name,G_CALLBACK (handle_row_inserted),&ret);
      return;
    }
    if(name == "rows-reordered") {
      g_signal_connect (model, name,G_CALLBACK (handle_rows_reordered),&ret);
      return;
    }
    RavlAssertMsg(0,"Unknown signal type. ");
  }

  //: Get real value

  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, RealT &value) {
    if(!rowIter.IsElm())
      return false;
    ReadBackLockC lock;
    gtk_tree_model_get(model,rowIter.TreeIter(),col,&value,-1);
    return true;
  }



  //: Set int value.

  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, IntT &value) {
    if(!rowIter.IsElm())
      return false;
    ReadBackLockC lock;
    gtk_tree_model_get(model,rowIter.TreeIter(),col,&value,-1);
    return true;
  }



  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, Int64T &value) {
    if(!rowIter.IsElm())
      return false;
    ReadBackLockC lock;
    gtk_tree_model_get(model,rowIter.TreeIter(),col,&value,-1);
    return true;
  }

  //: Set bool value.

  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, bool &value) {
    if(!rowIter.IsElm())
      return false;
    int tmp;
    ReadBackLockC lock;
    gtk_tree_model_get(model,rowIter.TreeIter(),col,&tmp,-1);
    value = tmp != 0;
    return true;
  }

  //: Set bool value.

  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, StringC &value) {
    if(!rowIter.IsElm())
      return false;
    guchar *text;
    ReadBackLockC lock;
    gtk_tree_model_get(model,rowIter.TreeIter(),col,&text,-1);
    value = StringC((char *) text);
    g_free(text);
    return true;
  }

  //: Set bool value.

  bool TreeModelBodyC::GetValue(TreeModelIterC &rowIter,IntT col, PixbufC &value) {
    RavlAssertMsg(0,"TreeModelBodyC::GetValue(,,PixbufC&), Not implemented.");
    return true;
  }

  //: Set int value.

  bool TreeModelBodyC::SetValue(TreeModelIterC &rowIter,IntT col, RealT value) {
    ONDEBUG(RavlAssert(0)); // This method is a bad idea. Row's can be deleted unexpectedly when method is queued
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUISetValueInt,rowIter,col,static_cast<int>(value)));
    return false;
  }

  //: Set int value.

  bool TreeModelBodyC::SetValue(TreeModelIterC &rowIter,IntT col, IntT value) {
    ONDEBUG(RavlAssert(0)); // This method is a bad idea. Row's can be deleted unexpectedly when method is queued
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUISetValueInt,rowIter,col,value));
    return false;
  }

  //: Set bool value.

  bool TreeModelBodyC::SetValue(TreeModelIterC &rowIter,IntT col, bool value) {
    ONDEBUG(RavlAssert(0)); // This method is a bad idea. Row's can be deleted unexpectedly when method is queued
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUISetValueBool,rowIter,col,value));
    return false;
  }

  //: Set bool value.

  bool TreeModelBodyC::SetValue(TreeModelIterC &rowIter,IntT col, const StringC &value) {
    ONDEBUG(RavlAssert(0)); // This method is a bad idea. Row's can be deleted unexpectedly when method is queued
    //if(!Manager.IsManagerStarted())
    //return GUISetValue(rowIter,col,value);
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUISetValueString,rowIter,col,value));
    return true;
  }

  //: Set bool value.

  bool TreeModelBodyC::SetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value) {
    ONDEBUG(RavlAssert(0)); // This method is a bad idea. Row's can be deleted unexpectedly when method is queued
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUISetValuePixbuf,rowIter,col,value));
    return true;
  }

  //: Clear store of all values.

  void TreeModelBodyC::Empty() {
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUIEmpty));
  }

  //: Append a row.

  bool TreeModelBodyC::AppendRow(TreeModelIterC &rowHandle) {
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }



  bool TreeModelBodyC::InsertRowBefore(TreeModelIterC &rowHandle, TreeModelIterC &targetRowHandle) {
    RavlAssertMsg(0,"Not implemented. ");
    return false;
  }

  //: Delete a row.

  bool TreeModelBodyC::DeleteRow(TreeModelIterC &rowHandle) {
    //if(!Manager.IsManagerStarted())
    //return GUIDeleteRow(rowHandle);
    Manager.Queue(Trigger(TreeModelC(*this),&TreeModelC::GUIDeleteRow,rowHandle));
    return true;
  }


  //: Delete a row.

  bool TreeModelBodyC::GUIDeleteRow(TreeModelIterC &rowHandle) {
    RavlAssert(0);
    return false;
  }

  //: Set int value.

  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, RealT value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,IntT) Not implemented. ");
    return false;
  }


  //: Set int value.

  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,IntT) Not implemented. ");
    return false;
  }



  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, Int64T value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,Int64T) Not implemented. ");
    return false;
  }

  //: Set bool value.

  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, bool value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,bool) Not implemented. ");
    return false;
  }

  //: Set bool value.

  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,const StringC &) Not implemented. ");
    return false;
  }

  //: Set bool value.

  bool TreeModelBodyC::GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value) {
    RavlAssertMsg(0,"TreeModelBodyC::SetValue(TreeModelIterC &,IntT,const PixbufC &) Not implemented. ");
    return false;
  }

  //: Clear store of all values.

  void TreeModelBodyC::GUIEmpty() {
    RavlAssertMsg(0,"TreeModelBodyC::Empty() Not implemented. ");
    return ;
  }

  //: Access tree signal.
  // Where name is one of "row-changed", "row-deleted","row-has-child-toggled","row-inserted","rows-reordered"

  Signal2C<TreeModelPathC,TreeModelIterC> &TreeModelBodyC::Signal(const char *nname) {
    Signal2C<TreeModelPathC,TreeModelIterC> *ret;
    StringC name(nname);
    if((ret = signals.Lookup(name)) != 0)
      return *ret;
    if(name == "row-changed" ||
       name == "row-deleted" ||
       name == "row-has-child-toggled" ||
       name == "row-inserted" ||
       name == "rows-reordered") {
      TreeModelIterC tmp2;
      TreeModelPathC tmp1;
      signals[name] = Signal2C<TreeModelPathC,TreeModelIterC>(tmp1,tmp2);
      ret = &signals[name];
    } else {
      std::cerr << "TreeModelBodyC::Signal(), WARNING: Unknown signal '" << nname << "'\n";
      RavlAssert(0);
    }
    if(model != 0)
      ConnectUp(name);
    return *ret;
  }

  //: Get iterator from path text.

  TreeModelIterC TreeModelBodyC::Path2Iter(const char *pathName) {
    TreeModelIterC ret;
    ret.Model(model);
    GtkTreePath *path = gtk_tree_path_new_from_string (pathName);
    gtk_tree_model_get_iter (model, ret.TreeIter(), path);
    gtk_tree_path_free (path);
    return ret;
  }

  //: Get the root iterator

  TreeModelIterC TreeModelBodyC::GetRoot()
  {
    TreeModelIterC ret;
    ret.Model(model);
    if(!gtk_tree_model_get_iter_root(model, ret.TreeIter()))
      ret.Invalidate();
    return ret;
  }

}

