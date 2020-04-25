// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_ICONVIEW_HEADER
#define RAVLGUI_ICONVIEW_HEADER 1
//! rcsid="$Id: TreeView.hh 6040 2007-02-01 18:42:02Z craftit $"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! lib=RavlGUI
//! date="23/9/2007"
//! file="Ravl/GUI/GTK/IconView.hh"

#include "Ravl/config.h"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/TreeModel.hh"
#include "Ravl/Threads/Signal1.hh"

extern "C" {
  typedef struct _GtkTreeSelection  GtkTreeSelection;
  typedef struct _GtkCellRenderer   GtkCellRenderer;
}

namespace RavlGUIN {

  //! Icon view body

  class IconViewBodyC
    : public WidgetBodyC
  {
  public:
    IconViewBodyC(const TreeModelC &treeModel,UIntT textColumn,UIntT pixbufColumn,UIntT itemWidth = 0);
    //: Constructor.

    DListC<TreeModelIterC> GUISelected();
    //: Get list of selected entries.

    bool GUISelectPath(TreeModelPathC &path);
    //: Select the specified path
    //: GUI thread

    bool GUIDeselectAll();
    //: Clear the selection
    //: GUI thread

  protected:
    virtual bool Create();
    //: Create widget.

    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.

    TreeModelC m_treeModel;

    UIntT m_textColumn;
    UIntT m_pixbufColumn;
    UIntT m_iconWidth;

  };

  //! Icon view

  class IconViewC
    : public WidgetC
  {
  public:
    IconViewC()
    {}
    //: Default constructor

    IconViewC(const TreeModelC &treeModel,UIntT textColumn,UIntT pixbufColumn,UIntT itemWidth = 0)
      : WidgetC(*new IconViewBodyC(treeModel,textColumn,pixbufColumn,itemWidth))
    {}
    //: Constructor.

    DListC<TreeModelIterC> GUISelected()
    { return Body().GUISelected(); }
    //: Get list of selected entries.
    //: GUI thread

    bool GUISelectPath(TreeModelPathC &path)
    { return Body().GUISelectPath(path); }
    //: Select the specified path
    //: GUI thread

    bool GUIDeselectAll()
    { return Body().GUIDeselectAll(); }
    //: Clear the selection
    //: GUI thread

  protected:
    IconViewBodyC &Body()
    { return static_cast<IconViewBodyC &>(WidgetC::Body()); }
    //: Access body.

    const IconViewBodyC &Body() const
    { return static_cast<const IconViewBodyC &>(WidgetC::Body()); }
    //: Access body.
  };

}

#endif
