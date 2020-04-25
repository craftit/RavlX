// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_ATTRIBUTEEDITOR_HEADER
#define RAVLGUI_ATTRIBUTEEDITOR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! file="Ravl/GUI/Util/AttributeEditor.hh"
//! docentry="Ravl.API.Graphics.GTK.Util"
//! author="Charles Galambos"
//! example=exAttributeEditor.cc

#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/DP/AttributeType.hh"
#include "Ravl/GUI/Table.hh"
#include "Ravl/Trigger.hh"
#include "Ravl/Tuple3.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Attribute editor widget.
  
  class AttributeEditorBodyC
    : public TableBodyC
  {
  public:
    AttributeEditorBodyC(const AttributeCtrlC &ctrl,    
                         bool showReadWrite = true,
                         bool showAttrName = true,
                         bool showAttrDescription = false
                         );
    //: Construct on attribute control
    
    ~AttributeEditorBodyC();
    //: Destructor.
    
    void BuildWidgets();
    //: Build widgets needed for the current attribute control.
    
    bool SetAttribBool(bool &val,StringC &name);
    //: Set a bool attribute.
    
    bool SetAttribString(StringC &val,StringC &name);
    //: Set a string attribute.
    
    bool SetAttribEnum(StringC &val,StringC &name);
    //: Set a enum attribute.
    
    bool SetAttribReal(RealT &val,StringC &name);
    //: Set a string attribute.

    bool SetAttribInt(RealT &val,StringC &name);
    //: Set a string attribute.
    
    bool UpdateAttribBool(StringC &name,WidgetC &widge);
    //: Update a bool attribute.
    
    bool UpdateAttribString(StringC &name,WidgetC &widge);
    //: Update a string attribute.
    
    bool UpdateAttribReal(StringC &name,WidgetC &widge);
    //: Update a real attribute.
    
    bool UpdateAttribInt(StringC &name,WidgetC &widge);
    //: Update a int attribute.
    
    bool UpdateAttribEnum(StringC &name,WidgetC &widge);
    //: Update a enum attribute.
    
    bool UpdateAttribLabel(StringC &name,WidgetC &widge);
    //: Update a label attribute.
    
    bool LoadAttributes(const StringC &fileName);
    //: Load attributes from file.
    
    bool SaveAttributes(const StringC &fileName);
    //: Save attributes to a file.
    
    bool UpdateAll();
    //: Update control values.
    
  protected:
    virtual bool Create();
    //: Create widget.
    
    HashC<StringC,Tuple3C<WidgetC,TriggerC,IntT> > controls;
    AttributeCtrlC attribCtrl;
    
    bool showReadWrite;
    bool showAttrName;
    bool showAttrDescription;
  };
  
  //! userlevel=Normal
  //: Attribute editor widget.
  
  class AttributeEditorC
    : public TableC
  {
  public:
    AttributeEditorC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    AttributeEditorC(const AttributeCtrlC &ctrl,
                     bool showReadWrite = true,
                     bool showAttrName = true,
                     bool showAttrDescription = false
                     )
      : TableC(*new AttributeEditorBodyC(ctrl,showReadWrite,showAttrName,showAttrDescription))
    {}
    //: Construct on attribute control    
    
  protected:
    AttributeEditorC(AttributeEditorBodyC &bod)
      : TableC(bod)
    {}
    //: Body constructor.
    
    AttributeEditorBodyC &Body()
    { return static_cast<AttributeEditorBodyC &>(WidgetC::Body()); }
    //: Access body.

    const AttributeEditorBodyC &Body() const
    { return static_cast<const AttributeEditorBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool LoadAttributes(const StringC &fileName)
    { return Body().LoadAttributes(fileName); }
    //: Load attributes from file.
    
    bool SaveAttributes(const StringC &fileName)
    { return Body().SaveAttributes(fileName); }
    //: Save attributes to a file.
    
    bool UpdateAll()
    { return Body().UpdateAll(); }
    //: Update control values.

  };
  
}

#endif
