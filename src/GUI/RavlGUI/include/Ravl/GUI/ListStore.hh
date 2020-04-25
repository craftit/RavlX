// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_LISTSTORE_HEADER
#define RAVLGUI_LISTSTORE_HEADER 1
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! example=exTreeStore.cc
//! lib=RavlGUI
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/ListStore.hh"

#include "Ravl/config.h"
#include "Ravl/GUI/TreeModel.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: List store body.
  // Available on GTK+-2.0 and above only.

  class ListStoreBodyC
    : public TreeModelBodyC
  {
  public:
    ListStoreBodyC();
    //: Constructor.

    ListStoreBodyC(const SArray1dC<AttributeTypeC> &nColTypes, const IntT sortColumn = -1, const bool sortAscending = true);
    //: List store.
    
    ListStoreBodyC(GtkTreeModel *aModel);
    //: Constructor. 
    // It the user's resonsiblity to check the model is of the
    // correct type.
    
    virtual bool Create();
    //: Create the widget.

    virtual bool AppendRow(TreeModelIterC &rowHandle);
    //: Append a row.

    virtual bool GUIDeleteRow(TreeModelIterC &rowHandle);
    //: Delete a row.

    virtual bool InsertRowBefore(TreeModelIterC &rowHandle, TreeModelIterC &targetRowHandle);
    //: Insert a row before the target row.

    using TreeModelBodyC::GUISetValue;
    //: Pull in all the base 'set value' functions

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value);
    //: Set int value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, Int64T value);
    //: Set int value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, bool value);
    //: Set bool value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value);
    //: Set string value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value);
    //: Set pixbuf value.

    virtual void GUIEmpty();
    //: Clear store of all values.

    bool GUIMoveBefore(const TreeModelIterC &target,const TreeModelIterC &position);
    //: Move target row before position
    // if position is null, it will be moved to the end of list.

    bool GUIMoveAfter(const TreeModelIterC &target,const TreeModelIterC &position);
    //: Move target row after position
    // if position is null, it will be moved to the start of list.

    bool GUIReorder(const SArray1dC<IntT> &newOrder);
    //: Reorder list.

  protected:
    const IntT m_sortColumn;
    const bool m_sortAscending;
  };

  //! userlevel=Normal
  //: List Store
  // Available on GTK+-2.0 and above only.

  class ListStoreC
    : public TreeModelC
  {
  public:
    ListStoreC()
    {}
    //: Default constructor
    // Creates an invalid handle.

    ListStoreC(const SArray1dC<AttributeTypeC> &nColTypes, const IntT sortColumn = -1, const bool sortAscending = true)
      : TreeModelC(*new ListStoreBodyC(nColTypes, sortColumn, sortAscending))
    {}
    //: List store.

    ListStoreC(ListStoreBodyC &bod)
      : TreeModelC(bod)
    {}
    //: Body constructor.
    
    ListStoreC(GtkTreeModel *aModel)
      : TreeModelC(*new ListStoreBodyC(aModel))
    {}
    //: Constructor. 
    // It the user's resonsiblity to check the model is of the
    // correct type.
    
  protected:
    ListStoreBodyC &Body()
    { return static_cast<ListStoreBodyC &>(TreeModelC::Body()); }
    //: Access body.

    const ListStoreBodyC &Body() const
    { return static_cast<const ListStoreBodyC &>(TreeModelC::Body()); }
    //: Access body.

  public:
    bool GUIReorder(const SArray1dC<IntT> &newOrder)
    { return Body().GUIReorder(newOrder); }
    //: Reorder list.

    bool GUIMoveBefore(const TreeModelIterC &target,const TreeModelIterC &position)
    { return Body().GUIMoveBefore(target,position); }
    //: Move target row before position
    // if position is null, it will be moved to the end of list.

    bool GUIMoveAfter(const TreeModelIterC &target,const TreeModelIterC &position)
    { return Body().GUIMoveAfter(target,position); }
    //: Move target row after position
    // if position is null, it will be moved to the start of list.
  };

}
#endif
