// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_TREESTORE_HEADER
#define RAVLGUI_TREESTORE_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! lib=RavlGUI
//! example=exTreeStore.cc
//! date="23/9/2003"
//! file="Ravl/GUI/GTK/TreeStore.hh"

#include "Ravl/config.h"
#include "Ravl/GUI/TreeModel.hh"

extern "C" {
  typedef struct _GtkTreeStore       GtkTreeStore;
}

namespace RavlGUIN {
  class PixmapC;

  //! userlevel=Develop
  //: List store body.
  // Available on GTK+-2.0 and above only.

  class TreeStoreBodyC
    : public TreeModelBodyC
  {
  public:
    TreeStoreBodyC(const SArray1dC<AttributeTypeC> &nColTypes);
    //: Constructor.

    TreeStoreBodyC(const SArray1dC<AttributeTypeC> &nColTypes, int sortColumn, bool sortAscending);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.

    bool AppendRow(TreeModelIterC &rowIter,TreeModelIterC &parentIter);
    //: Append a row.

    virtual bool AppendRow(TreeModelIterC &rowHandle);
    //: Append a row.

    virtual bool GUIDeleteRow(TreeModelIterC &rowHandle);
    //: Delete a row.
    // After return, the iterator will point to the next row at the same level,
    // or will be invalidated if such a row does not exist.)

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, RealT value);
    //: Set real value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, IntT value);
    //: Set int value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, bool value);
    //: Set bool value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const StringC &value);
    //: Set string value.

    virtual bool GUISetValue(TreeModelIterC &rowIter,IntT col, const PixbufC &value);
    //: Set pixbuf value.

    virtual void GUIEmpty();
    //: Clear store of all values.

  protected:
    const IntT m_sortColumn;
    const bool m_sortAscending;
  };

  //! userlevel=Normal
  //: List Store
  // Available on GTK+-2.0 and above only.

  class TreeStoreC
    : public TreeModelC
  {
  public:
    TreeStoreC()
    {}
    //: Default constructor
    // Creates an invalid handle.

    TreeStoreC(const SArray1dC<AttributeTypeC> &nColTypes)
      : TreeModelC(*new TreeStoreBodyC(nColTypes))
    {}
    //: Constructor.

    TreeStoreC(const SArray1dC<AttributeTypeC> &nColTypes, int sortColumn, bool sortAscending = true)
      : TreeModelC(*new TreeStoreBodyC(nColTypes, sortColumn, sortAscending))
    {}
    //: Constructor.

    TreeStoreC(TreeStoreBodyC &bod)
      : TreeModelC(bod)
    {}
    //: Body constructor.

  protected:
    TreeStoreBodyC &Body()
    { return static_cast<TreeStoreBodyC &>(TreeModelC::Body()); }
    //: Access body.

    const TreeStoreBodyC &Body() const
    { return static_cast<const TreeStoreBodyC &>(TreeModelC::Body()); }
    //: Access body.

  public:
    bool AppendRow(TreeModelIterC &rowHandle,TreeModelIterC &parentHandle)
    { return Body().AppendRow(rowHandle,parentHandle); }
    //: Append a row.

    bool AppendRow(TreeModelIterC &rowHandle)
    { return Body().AppendRow(rowHandle); }
    //: Append a row.

  };

}
#endif
