// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_ATTRIBUTEEDITORWINDOW_HEADER
#define RAVLGUI_ATTRIBUTEEDITORWINDOW_HEADER 1
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! file="Ravl/GUI/Util/AttributeEditorWindow.hh"
//! example=exAttributeEditor.cc
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Util"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/AttributeEditor.hh"
#include "Ravl/GUI/FileSelector.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Window for editing attribute.
  
  class AttributeEditorWindowBodyC
    : public WindowBodyC
  {
  public:
    AttributeEditorWindowBodyC(const StringC &name,const AttributeCtrlC &ctrl,
                               bool showReadWrite = true,
                               bool showAttrName = true,
                               bool showAttrDescription = false,
                               bool showLoadSave = true,
                               bool dontDelete = true
                               );
    //: Constructor.
    
    bool LoadAttributes(StringC &name);
    //: Load attributes from file.
    
    bool SaveAttributes(StringC &name);
    //: Save attributes to a file.
    
    bool LoadSave(StringC &name);
    //: Do load/save.
    
    bool openLoadSave(bool &doLoad);
    //: Open load window.
    
  protected:
    
    bool doLoad;
    FileSelectorC fileSelector;
    AttributeEditorC editor;
  };

  //! userlevel=Normal
  //: Window for editing attribute.
  
  class AttributeEditorWindowC
    : public WindowC
  {
  public:
    AttributeEditorWindowC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    AttributeEditorWindowC(const StringC &name,const AttributeCtrlC &ctrl,
                           bool showReadWrite = true,
                           bool showAttrName = true,
                           bool showAttrDescription = false,
                           bool showLoadSave = true,
                           bool dontDelete = true
                           )
      : WindowC(*new AttributeEditorWindowBodyC(name,ctrl,showReadWrite,showAttrName,showAttrDescription,showLoadSave,dontDelete))
    {}

  protected:
    AttributeEditorWindowC(AttributeEditorWindowBodyC &bod)
      : WindowC(bod)
    {}
    //: Body constructor.
    
    AttributeEditorWindowBodyC& Body()
    { return static_cast<AttributeEditorWindowBodyC&>(WidgetC::Body()); }
    //: Access body.
    
    const AttributeEditorWindowBodyC& Body() const
    { return static_cast<const AttributeEditorWindowBodyC&>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool LoadAttributes(StringC &name)
    { return Body().LoadAttributes(name); }
    //: Load attributes from file.
    
    bool SaveAttributes(StringC &name)
    { return Body().SaveAttributes(name); }
    //: Save attributes to a file.
    
    bool LoadSave(StringC &name)
    { return Body().LoadSave(name); }
    //: Do load/save.
    
    friend class AttributeEditorWindowBodyC;
  };
}


#endif
