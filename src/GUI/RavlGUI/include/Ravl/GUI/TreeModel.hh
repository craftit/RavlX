// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_TREEMODEL_HEADER
#define RAVLGUI_TREEMODEL_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! lib=RavlGUI
//! example=exTreeStore.cc
//! date="23/9/2003"
//! file="Ravl/GUI/GTK/TreeModel.hh"

#include "Ravl/config.h"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/Threads/Signal2.hh"

#include <gtk/gtktreemodel.h> // Needed for GtkTreeIter

namespace RavlGUIN {

  class TreeModelIterC;
  class TreeModelC;
  class TreeModelPathC;

  //! userlevel=Develop
  //: Handle to row in tree model.
  // Available on GTK+-2.0 and above only.

  class TreeModelIterBodyC
    : public RCBodyC
  {
  public:
    TreeModelIterBodyC();
    //: Constructor.
 
    TreeModelIterBodyC(GtkTreeIter *treeIter);
    //: Constructor.
    
    TreeModelIterBodyC(TreeModelC &treeModel);
    //: Construct from tree model.

    TreeModelIterBodyC(GtkTreeModel *model,GtkTreeIter *treeIter);
    //: Constructor.

    TreeModelIterBodyC(GtkTreeModel *model,GtkTreePath *treePath);
    //: Constructor.

    ~TreeModelIterBodyC();
    //: Destructor.

    GtkTreeIter *TreeIter()
    { return &treeIter; }
    //: Access tree store.

    const GtkTreeIter *TreeIter() const
    { return &treeIter; }
    //: Access tree store.

    GtkTreeModel *Model()
    { return model; }
    //: Access tree model.

    void Model(GtkTreeModel * nModel)
    { model = nModel; }
    //: Set tree model.

    bool Next();
    //: Goto next element at current level.
    // Returns true if succeeded.

    TreeModelIterC Children();
    //: Return iterator for first child.
    // Will return an invalid iterator if none.

    bool HasChildren();
    //: Does current node have children ?

    TreeModelIterC Parent();
    //: Get iterator for parent.
    // Returns an invalid handle if none.

    TreeModelIterC Copy();
    //: Create a copy of the iterator.

    bool IsElm() const;
    //: Test if we think we're on a valid element.

    void Clear();
    //: Flag as invalid.

  protected:
    GtkTreeModel *model;
    GtkTreeIter treeIter;
    bool isElm;
  };

  //! userlevel=Normal
  //: Handle to row in tree model.
  // Available on GTK+-2.0 and above only.

  class TreeModelIterC
    : public RCHandleC<TreeModelIterBodyC>
  {
  public:
    TreeModelIterC()
      : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC())
    {}
    //: Default constructor.

    TreeModelIterC(TreeModelC &treeModel)
      : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC(treeModel))
    {}
    //: Construct from tree.
    // Create an iterator pointing to the first element in the tree.

    TreeModelIterC(GtkTreeIter *treeIter)
      : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC(treeIter))
    {}
    //: Constructor.
    
    TreeModelIterC(GtkTreeModel *model,GtkTreeIter *treeIter)
      : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC(model,treeIter))
    {}
    //: Constructor.

    TreeModelIterC(GtkTreeModel *model,GtkTreePath *treePath)
      : RCHandleC<TreeModelIterBodyC>(*new TreeModelIterBodyC(model,treePath))
    {}
    //: Constructor.

    TreeModelIterC(TreeModelC &model,const TreeModelPathC &treePath);
    //: Constructor.

    GtkTreeIter *TreeIter()
    { return Body().TreeIter(); }
    //: Access tree store.

    const GtkTreeIter *TreeIter() const
    { return Body().TreeIter(); }
    //: Access tree store.

    GtkTreeModel *Model()
    { return Body().Model(); }
    //: Access tree model.

    void Model(GtkTreeModel * nModel)
    { Body().Model(nModel); }
    //: Set tree model.

    bool Next()
    { return Body().Next(); }
    //: Goto next element at current level.
    // Returns true if succeeded.

    TreeModelIterC Children()
    { return Body().Children(); }
    //: Return iterator for first child.
    // Will return an invalid iterator if none.

    bool HasChildren()
    { return Body().HasChildren(); }
    //: Does current node have children ?

    TreeModelIterC Parent()
    { return Body().Parent(); }
    //: Get iterator for parent.
    // Returns an invalid handle if none.

    TreeModelIterC Copy()
    { return Body().Copy(); }
    //: Create a copy of the iterator.

    bool IsElm() const
    { if(!IsValid()) return false; return Body().IsElm(); }
    //: Check if its a valid entry.

    void Clear()
    { Body().Clear(); }
    //: Set to non element.

  };

  inline bool operator<(const TreeModelIterC& lhs, const TreeModelIterC& rhs)
  {
    if (!lhs.IsValid())
      return true;
    if (!rhs.IsValid())
      return false;
    return lhs.TreeIter()->stamp < rhs.TreeIter()->stamp;
  }

  //:-------------------------------------------------------------------------------------

  //! userlevel=Develop
  //: Handle to row in tree model.
  // Available on GTK+-2.0 and above only.

  class TreeModelPathBodyC
    : public RCBodyC
  {
  public:
    TreeModelPathBodyC();
    //: Constructor.

    TreeModelPathBodyC(TreeModelIterC treeIter);
    //: Constructor.

    TreeModelPathBodyC(GtkTreePath *treeIter,bool canFree);
    //: Constructor.

    ~TreeModelPathBodyC();
    //: Destructor.

    GtkTreePath *TreePath()
    { return treePath; }
    //: Access tree store.

    StringC Text() const;
    //: Path as text.
  protected:
    GtkTreePath *treePath;
  };


  //! userlevel=Normal
  //: Handle to row in tree model.
  // Available on GTK+-2.0 and above only.

  class TreeModelPathC
    : public RCHandleC<TreeModelPathBodyC>
  {
  public:
    TreeModelPathC()
      : RCHandleC<TreeModelPathBodyC>(*new TreeModelPathBodyC())
    {}
    //: Default constructor.

    TreeModelPathC(TreeModelIterC treeIter)
      : RCHandleC<TreeModelPathBodyC>(*new TreeModelPathBodyC(treeIter))
    {}
    //: Constructor.
    // GUI Thread only!

    TreeModelPathC(GtkTreePath *treeIter,bool canFree)
      : RCHandleC<TreeModelPathBodyC>(*new TreeModelPathBodyC(treeIter,canFree))
    {}
    //: Constructor.
    // If canFree is true, the path with be deleted with the body of this object.

    GtkTreePath  *TreePath()
    { return Body().TreePath(); }
    //: Access tree store.

    StringC Text() const
    { return Body().Text(); }
    //: Path as text.

  };

  //:-------------------------------------------------------------------------------------

  //! userlevel=Develop
  //: List store body.
  // Available on GTK+-2.0 and above only.

  class TreeModelBodyC
    : public RCBodyVC
  {
  public:
    TreeModelBodyC();
    //: Constructor.
    
    TreeModelBodyC(GtkTreeModel *aModel);
    //: Constructor.
    
    TreeModelBodyC(const SArray1dC<AttributeTypeC> &ncolTypes);
    //: Constructor.

    virtual ~TreeModelBodyC();
    //: Destructor.

    virtual bool Create();
    //: Create the widget.

    GtkTreeModel *TreeModel()
    { return model; }
    //: Access tree model.

    AttributeValueTypeT ColumnValueType(UIntT col) const
    { return colTypes[col].ValueType(); }
    //: Get the type of a given column.

    AttributeTypeC ColumnType(UIntT col) const
    { return colTypes[col] ; }
    //: Access column type info.

    UIntT ColNumber(const StringC &name) const;
    //: Look up column number of named column.

    UIntT Cols() const
    { return colTypes.Size(); }
    //: Access the number of columns .

    UIntT GUIRows() const;
    //: Access number of top level rows in store

    virtual bool AppendRow(TreeModelIterC &rowHandle);
    //: Append a row.

    virtual bool InsertRowBefore(TreeModelIterC &rowHandle, TreeModelIterC &targetRowHandle);
    //: Insert a row before the target row.

    bool DeleteRow(TreeModelIterC &rowHandle);
    //: Delete a row.

    bool GetValue(TreeModelIterC &rowIter,IntT col, RealT & value);
    //: Get real value 

    bool GetValue(TreeModelIterC &rowIter,IntT col, IntT &value);
    //: Get int value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, Int64T &value);
    //: Get 64-bit int value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, bool &value);
    //: Get bool value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, StringC &value);
    //: Get string value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, PixbufC &value);
    //: Get pixbuf value.

    bool SetValue(TreeModelIterC &rowIter, IntT col, RealT value);
    //: Set real value

    bool SetValue(TreeModelIterC &rowIter,IntT col, IntT value);
    //: Set int value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, bool value);
    //: Set bool value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, const StringC &value);
    //: Set string value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value);
    //: Set pixbuf value.

    void Empty();
    //: Clear store of all values.

    virtual bool GUIDeleteRow(TreeModelIterC &rowHandle);
    //: Delete a row.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, RealT value);
    //: Set real value

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value);
    //: Set int value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, Int64T value);
    //: Set 64-bit int value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, bool value);
    //: Set bool value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value);
    //: Set string value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value);
    //: Set pixbuf value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const char *value)
    { return GUISetValue(rowIter, col, StringC(value)); }
    //: Set pixbuf value.

    virtual void GUIEmpty();
    //: Clear store of all values.

    Signal2C<TreeModelPathC,TreeModelIterC> &Signal(const char *name);
    //: Access tree signal.
    // Where name is one of "row-changed", "row-deleted","row-has-child-toggled","row-inserted","rows-reordered"

    TreeModelIterC Path2Iter(const char *pathName);
    //: Get iterator from path text.
    // GUI Thread only!

    TreeModelIterC GetRoot();
    //: Get the root iterator

  protected:
    void ConnectUp(StringC name);
    //: Create a new signal.

    GtkTreeModel        *model;
    SArray1dC<AttributeTypeC> colTypes;
    HashC<StringC,Signal2C<TreeModelPathC,TreeModelIterC> > signals;

    friend class TreeModelIterBodyC;
  };


  //! userlevel=Normal
  //: List Store
  // Available on GTK+-2.0 and above only.

  class TreeModelC
    : public RCHandleC<TreeModelBodyC>
  {
  public:
    TreeModelC()
    {}
    //: Default constructor
    // Creates an invalid handle.

    TreeModelC(GtkTreeModel *aModel)
      : RCHandleC<TreeModelBodyC>(new TreeModelBodyC(aModel))
    {}
    //: Constructor from an existing model.
    
    TreeModelC(TreeModelBodyC &bod)
      : RCHandleC<TreeModelBodyC>(bod)
    {}
    //: Body constructor.

  protected:
    TreeModelBodyC &Body()
    { return RCHandleC<TreeModelBodyC>::Body(); }
    //: Access body.

    const TreeModelBodyC &Body() const
    { return RCHandleC<TreeModelBodyC>::Body(); }
    //: Access body.

  public:
    bool Create()
    { return Body().Create(); }
    //: Create the model

    GtkTreeModel *TreeModel()
    { return Body().TreeModel(); }
    //: Access tree model.

    AttributeValueTypeT ColumnValueType(UIntT col) const
    { return Body().ColumnValueType(col); }
    //: Get the type of a given column.

    AttributeTypeC ColumnType(UIntT col) const
    { return Body().ColumnType(col) ; }
    //: Access column type info.

    UIntT Cols() const
    { return Body().Cols(); }
    //: Access the number of columns .

    UIntT GUIRows() const
    { return Body().GUIRows(); }
    //: Access number of top level rows in store

    bool AppendRow(TreeModelIterC &rowHandle)
    { return Body().AppendRow(rowHandle); }
    //: Append a row.

    bool DeleteRow(TreeModelIterC &rowHandle)
    { return Body().DeleteRow(rowHandle); }
    //: Delete a row.

    UIntT ColNumber(const StringC &name) const
    { return Body().ColNumber(name); }
    //: Look up column number of named column.

    bool GetValue(TreeModelIterC &rowIter,IntT col, RealT &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Get real value

    bool GetValue(TreeModelIterC &rowIter,IntT col, IntT &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Set int value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, Int64T &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Set int value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, bool &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Set bool value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, StringC &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Set bool value.

    bool GetValue(TreeModelIterC &rowIter,IntT col, PixbufC &value)
    { return Body().GetValue(rowIter,col,value); }
    //: Set bool value.

    bool GUIDeleteRow(TreeModelIterC &rowHandle)
    { return Body().GUIDeleteRow(rowHandle); }
    //: Delete a row.

    bool GUISetValueReal(TreeModelIterC &rowIter,IntT col, RealT value)
    { return Body().GUISetValue(rowIter,col,value); }

    bool GUISetValueInt(TreeModelIterC &rowIter,IntT col, IntT value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set int value.

    bool GUISetValueBool(TreeModelIterC &rowIter,IntT col, bool value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set bool value.

    bool GUISetValueString(TreeModelIterC &rowIter,IntT col, const StringC &value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set string value.

    bool GUISetValuePixbuf(TreeModelIterC &rowIter,IntT col, const PixbufC &value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set pixbuf value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, RealT value)
    { return Body().GUISetValue(rowIter,col,value); }

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set int value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, Int64T value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set int value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, bool value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set bool value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set string value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set pixbuf value.

    bool GUISetValue(TreeModelIterC &rowIter,IntT col, const char *value)
    { return Body().GUISetValue(rowIter,col,value); }
    //: Set 'C' style string value.

    void GUIEmpty()
    { Body().GUIEmpty(); }
    //: Clear store of all values.

    void Empty()
    { Body().Empty(); }
    //: Clear store of all values.

    bool SetValue(TreeModelIterC &rowIter, IntT col, RealT value)
    { return Body().SetValue(rowIter, col, value); }

    bool SetValue(TreeModelIterC &rowIter,IntT col, IntT value)
    { return Body().SetValue(rowIter,col,value); }
    //: Set int value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, bool value)
    { return Body().SetValue(rowIter,col,value); }
    //: Set bool value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, const StringC &value)
    { return Body().SetValue(rowIter,col,value); }
    //: Set string value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, const char *value)
    { return Body().SetValue(rowIter,col,StringC(value)); }
    //: Set 'C' style string value.

    bool SetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value)
    { return Body().SetValue(rowIter,col,value); }
    //: Set bool value.

    Signal2C<TreeModelPathC,TreeModelIterC> &Signal(const char *name)
    { return Body().Signal(name); }
    //: Access tree signal.
    // Where name is one of "row-changed", "row-deleted","row-has-child-toggled","row-inserted","rows-reordered"

    TreeModelIterC Path2Iter(const char *pathName)
    { return Body().Path2Iter(pathName); }
    //: Get iterator from path text.
    // GUI Thread only

    TreeModelIterC GetRoot()
    { return Body().GetRoot(); }
    //: Get the root iterator

    friend class TreeModelBodyC;
    friend class TreeModelIterBodyC;
    friend class TreeModelIterC;
  };

}

#endif
