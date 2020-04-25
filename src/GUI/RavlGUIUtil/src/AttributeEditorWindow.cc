// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! file="Ravl/GUI/Util/AttributeEditorWindow.cc"

#include "Ravl/GUI/AttributeEditorWindow.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/ScrolledArea.hh"
#include "Ravl/Threads/Signal1.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Constructor.
  
  AttributeEditorWindowBodyC::AttributeEditorWindowBodyC(const StringC &name,
                                                         const AttributeCtrlC &ctrl,
                                                         bool showReadWrite,
                                                         bool showAttrName,
                                                         bool showAttrDescription,
                                                         bool showLoadSave,
                                                         bool dontDelete
                                                         )
    : WindowBodyC(100,100,name,GTK_WINDOW_TOPLEVEL,4,false),
      fileSelector("Attribute IO","attribute.xml"),
      editor(ctrl,showReadWrite,showAttrName,showAttrDescription)
  {
    AttributeEditorWindowC me(*this);
    if(showLoadSave) {
      Connect(fileSelector.Selected(),me,&AttributeEditorWindowC::LoadSave);
      MenuBarC menuBar(MenuItemR("Save",*this,&AttributeEditorWindowBodyC::openLoadSave,false) + 
                       MenuItemR("Load",*this,&AttributeEditorWindowBodyC::openLoadSave,true));
      Add(VBox(PackInfoC(menuBar,false,true) + 
               PackInfoC(FrameC(ScrolledAreaC(editor,500,200),"",5),true,true) + 
               PackInfoC(RavlGUIN::ButtonR("Close",(WidgetBodyC &)*this,&WindowBodyC::GUIHide), false,true)));
    } else {
      Connect(fileSelector.Selected(),me,&AttributeEditorWindowC::LoadSave);
      MenuBarC menuBar;
      Add(VBox(PackInfoC(FrameC(ScrolledAreaC(editor,500,200),"",5),true,true) + 
               PackInfoC(RavlGUIN::ButtonR("Close",(WidgetBodyC &)*this,&WindowBodyC::GUIHide),false,true)));
    }
    if(dontDelete)
      ConnectRef(Signal("delete_event"),static_cast<WidgetBodyC &>(*this), &WindowBodyC::GUIHide);
    }
  
  //: Load attributes from file.
  
  bool AttributeEditorWindowBodyC::LoadAttributes(StringC &name) {
    ONDEBUG(cerr << "AttributeEditorWindowBodyC::LoadAttributes(), \n");
    if(editor.LoadAttributes(name))
      editor.UpdateAll(); // Make sure everythings updated.
    return true;
  }
  
  //: Save attributes to a file.
  
  bool AttributeEditorWindowBodyC::SaveAttributes(StringC &name) {
    ONDEBUG(cerr << "AttributeEditorWindowBodyC::SaveAttributes(), \n");
    return editor.SaveAttributes(name);
  }
  
  //: Do load/save.
  
  bool AttributeEditorWindowBodyC::LoadSave(StringC &name) {
    ONDEBUG(cerr << "AttributeEditorWindowBodyC::LoadSave(), Called. \n");
    if(doLoad)
      LoadAttributes(name);
    else
      SaveAttributes(name);
    return true;
  }
  
  //: Open load window.
  
  bool AttributeEditorWindowBodyC::openLoadSave(bool &nDoLoad) {
    ONDEBUG(cerr << "AttributeEditorWindowBodyC::openLoadSave(), Called. " << nDoLoad << "\n");
    doLoad = nDoLoad;
    if(doLoad) 
      fileSelector.SetTitle("Load Attributes");
    else
      fileSelector.SetTitle("Save Attributes");
    fileSelector.Show();
    return true;
  }
  
}
