// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUIUtil
//! file="Ravl/GUI/Util/AttributeEditor.cc"


#include "Ravl/GUI/AttributeEditor.hh"
#include "Ravl/GUI/ToggleButton.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/Combo.hh"
#include "Ravl/GUI/SpinButton.hh"
#include "Ravl/GUI/Table.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/HSet.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/CallMethods.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN {
  
  //: Construct on attribute control
  
  AttributeEditorBodyC::AttributeEditorBodyC(const AttributeCtrlC &ctrl,
                                             bool _showReadWrite,
                                             bool _showAttrName,
                                             bool _showAttrDescription
                                             ) 
    : TableBodyC(1,1,false),
      attribCtrl(ctrl),
      showReadWrite(_showReadWrite),
      showAttrName(_showAttrName),
      showAttrDescription(_showAttrDescription)
  {}
  
  //: Destructor.
  
  AttributeEditorBodyC::~AttributeEditorBodyC() {
    if(!attribCtrl.IsValid())
      return ;
    for(HashIterC<StringC,Tuple3C<WidgetC,TriggerC,IntT> > it(controls);it;it++)
      attribCtrl.RemoveChangedSignal(it.Data().Data3());
  }
  
  //: Create widget.
  
  bool AttributeEditorBodyC::Create() {
    BuildWidgets();
    return TableBodyC::Create();
  }
  
  //: Build widgets needed for the current attribute control.
  
  void AttributeEditorBodyC::BuildWidgets() {
    DListC<AttributeTypeC> list;
    if(!attribCtrl.IsValid())
      return ;
    attribCtrl.GetAttrTypes(list);
    
    // Count the number of attributes to display and
    // remove duplicates.
    
    HSetC<StringC> done;
    int noAttribs = 0;
    for(DLIterC<AttributeTypeC> it(list);it;it++) {
      if(done[it->Name()]) { // Ignore duplicate entries.
	it.Del();
	continue;
      }
      done += it->Name();
      noAttribs++;
    }
    
    sx = 3;
    sy = noAttribs;
    TriggerC updateTrigger;
    
    // Go through the list creating controls for each attribute.
    
    int lineNo = 0;
    for(DLIterC<AttributeTypeC> it(list);it;it++,lineNo++) {      
      WidgetC widge;
      if(it->CanWrite() || it->CanRead()) {
	switch(it->ValueType()) {
	case AVT_Bool: {
	  bool initState;
	  if(it->CanRead())
	    attribCtrl.GetAttr(it->Name(),initState);
	  else
	    initState = false;
	  if(it->CanWrite()) {
            StringC optDesc(" ");
            if(showAttrDescription) 
              optDesc = it->Description();
	    CheckButtonC cb = CheckButtonR(optDesc,(bool)initState,*this,&AttributeEditorBodyC::SetAttribBool,it->Name());
	    widge = cb;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribBool,it->Name(),widge);
	  } else {
	    StringC s;
	    if(initState)
	      s = "true";
	    else 
	      s = "false";
	    LabelC label(s);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	case AVT_String:{
	  StringC initValue;
	  attribCtrl.GetAttr(it->Name(),initValue);
	  if(it->CanWrite()) {
	    TextEntryC te = TextEntryR(initValue,*this,&AttributeEditorBodyC::SetAttribString,it->Name());
	    widge = te;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribString,it->Name(),widge);
	  } else {
	    LabelC label(initValue);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	case AVT_Int: {
	  IntT val;
	  attribCtrl.GetAttr(it->Name(),val);
	  AttributeTypeNumC<IntT> attr(*it);
	  RavlAssert(attr.IsValid());
	  if(attr.CanWrite()) {
	    SliderC sl((RealT) val,(RealT) attr.Min(),(RealT) attr.Max(),(RealT) attr.Step());
	    ConnectRef(sl.SigChanged(),*this,&AttributeEditorBodyC::SetAttribInt,val,it->Name());
	    widge = sl;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribInt,it->Name(),widge);
	    ConnectRef(sl.SigReleased(),*this,&AttributeEditorBodyC::UpdateAttribInt,it->Name(),widge);
	  } else {
	    StringC str(val);
	    LabelC label(str);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	case AVT_Int64: {
	  Int64T val;
	  attribCtrl.GetAttr(it->Name(),val);
	  if(it->CanWrite()) {
	    TextEntryC te = TextEntryR(StringC(val),*this,&AttributeEditorBodyC::SetAttribString,it->Name());
	    widge = te;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribString,it->Name(),widge);
	  } else {
	    LabelC label(val);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	case AVT_Real: {
	  RealT val;
	  attribCtrl.GetAttr(it->Name(),val);
	  AttributeTypeNumC<RealT> attr(*it);
	  RavlAssert(attr.IsValid());
	  if(attr.CanWrite()) {
	    SpinButtonC sl(4,attr.Step(),val,attr.Min(),attr.Max(),attr.Step() * 2);
	    ConnectRef(sl.SigChanged(),*this,&AttributeEditorBodyC::SetAttribReal,val,it->Name());
	    widge = sl;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribReal,it->Name(),widge);
	  } else {
	    StringC str(val);
	    LabelC label(str);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	case AVT_Enum: {
	  StringC val;
	  attribCtrl.GetAttr(it->Name(),val);
	  AttributeTypeEnumC attr(*it);
	  RavlAssert(attr.IsValid());
	  if(attr.CanWrite()) {
	    ComboC cb(attr.Values());
	    cb.SetSelection(val);	
	    ConnectRef(cb.SigSelected(),*this,&AttributeEditorBodyC::SetAttribEnum,val,it->Name());
	    widge = cb;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribEnum,it->Name(),widge);
	  } else {
	    StringC str(val);
	    LabelC label(str);
	    widge = label;
	    updateTrigger = TriggerR(*this,&AttributeEditorBodyC::UpdateAttribLabel,it->Name(),widge);
	  }
	} break;
	default:
	  cerr << "WARNING: Unhandled attribute type. ";
	  break;
	}
      } else
	widge = LabelC("?"); // Can't write or read the attribute ?!?
      
      // Add widget and its label to the layout.
      if(widge.IsValid()) {
	int updateId;
	updateId = attribCtrl.RegisterChangedSignal(it->Name(),updateTrigger);
	controls[it->Name()] = Tuple3C<WidgetC,TriggerC,IntT>(widge,updateTrigger,updateId);
        
	StringC name;
        if(showAttrName)
          name = it->Name();
	//cerr << "Name='" << name << "'\n";
        
	StringC rwtype;
        if(!showReadWrite)
          rwtype = StringC("");
        else {
          if(it->CanRead()) {
            if(it->CanWrite())
              rwtype="RW ";
            else
              rwtype="RO ";
          } else {
            if(it->CanWrite())
              rwtype="WO ";
            else
              rwtype="NA ";
          }
        }
        
	GUIAddObject(LabelC(rwtype),0,1,lineNo,lineNo+1,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL),2,3);	  
	GUIAddObject(LabelC(name)  ,1,2,lineNo,lineNo+1,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL),2,5); // | GTK_EXPAND
	GUIAddObject(widge         ,2,3,lineNo,lineNo+1);
        
        if(showAttrDescription) {
          LabelC labDesc(it->Description());
          if(it->ValueType() == AVT_Bool) // We use the checkbox description...
            labDesc = StringC("");
          GUIAddObject(labDesc  ,3,4,lineNo,lineNo+1,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL),2,5); // | GTK_EXPAND
        }
      }
    }
    //Add(table);
  }

  //: Set a string attribute.
  
  bool AttributeEditorBodyC::SetAttribString(StringC &val,StringC &name) {
    ONDEBUG(cerr << "AttributeEditorBodyC::SetAttribString(), Called. Name=" << name << " Value=" << val << " \n");
    attribCtrl.SetAttr(name,val);
    return true;
  }
  
  //: Set a enum attribute.
  
  bool AttributeEditorBodyC::SetAttribEnum(StringC &val,StringC &name) {
    ONDEBUG(cerr << "AttributeEditorBodyC::SetAttribEnum(), Called. Name=" << name << " Value=" << val << " \n");
    AttributeTypeEnumC attrType = attribCtrl.GetAttrType(name);
    if(!attrType.IsValid()) {
      cerr << "INTERNAL ERROR: '" << name << "' not an enum .\n";
      return true;
    }
    DLIterC<StringC> it(attrType.Values());
    for(;it;it++) {
      if(*it == val)
	break;
    }
    if(it.IsElm()) { // Is it a valid value ?
      attribCtrl.SetAttr(name,val);
    } else {
      // Reset it to some valid ?
    }
    return true;
  }

  //: Set a bool attribute.
  
  bool AttributeEditorBodyC::SetAttribBool(bool &val,StringC &name) {
    ONDEBUG(cerr << "AttributeEditorBodyC::SetAttribBool(), Called. Name=" << name << " Value=" << val << " \n");
    attribCtrl.SetAttr(name,val);
    return true;
  }
  
  //: Set a string attribute.
  
  bool AttributeEditorBodyC::SetAttribReal(RealT &val,StringC &name) {
    ONDEBUG(cerr << "AttributeEditorBodyC::SetAttribReal(), Called. Name=" << name << " Value=" << val << " \n");    
    attribCtrl.SetAttr(name,val);
    return true;
  }

  //: Set a string attribute.
  
  bool AttributeEditorBodyC::SetAttribInt(RealT &val,StringC &name) {
    ONDEBUG(cerr << "AttributeEditorBodyC::SetAttribInt(), Called. Name=" << name << " Value=" << val << " \n");    
    attribCtrl.SetAttr(name,Round(val));
    return true;
  }
  
  
  //: Update a bool attribute.
  
  bool AttributeEditorBodyC::UpdateAttribBool(StringC &name,WidgetC &widge) {
    CheckButtonC cb(widge);
    RavlAssert(cb.IsValid());
    bool value;
    if(attribCtrl.GetAttr(name,value))
      cb.GUISetToggle(value);
    return true;
  }
  
  //: Update a string attribute.
  
  bool AttributeEditorBodyC::UpdateAttribString(StringC &name,WidgetC &widge) {
    TextEntryC cb(widge);
    RavlAssert(cb.IsValid());
    StringC value;
    if(attribCtrl.GetAttr(name,value))
      cb.GUISetText(value);
    return true;
  }
  
  //: Update a string attribute.
  
  bool AttributeEditorBodyC::UpdateAttribReal(StringC &name,WidgetC &widge) {
    SpinButtonC sb(widge);
    RealT value;
    if(attribCtrl.GetAttr(name,value))
      sb.GUISetValue(value);
    return true;
  }
  
  //: Update a string attribute.
  
  bool AttributeEditorBodyC::UpdateAttribInt(StringC &name,WidgetC &widge) {
    SliderC sl(widge);
    IntT value;
    if(attribCtrl.GetAttr(name,value))
      sl.GUIUpdateValue(value);
    return true;
  }
  
  //: Update a label attribute.
  
  bool AttributeEditorBodyC::UpdateAttribLabel(StringC &name,WidgetC &widge) {
    LabelC lab(widge);
    StringC value;
    if(attribCtrl.GetAttr(name,value))
      lab.GUISetLabel(value);
    return true;
  }

  //: Update a enum attribute.
  
  bool AttributeEditorBodyC::UpdateAttribEnum(StringC &name,WidgetC &widge) {
    ComboC cb(widge);
    StringC value;
    if(attribCtrl.GetAttr(name,value))
      cb.GUISetSelection(value);
    return true;
  }
  
  //: Load attributes from file.
  
  bool AttributeEditorBodyC::LoadAttributes(const StringC &fileName) {
    XMLIStreamC xmlfile(fileName);
    attribCtrl.LoadAttributes(xmlfile);
    return true;
  }
  
  //: Save attributes to a file.
  
  bool AttributeEditorBodyC::SaveAttributes(const StringC &fileName) {
    XMLOStreamC xmlfile(fileName);
    attribCtrl.SaveAttributes(xmlfile);
    return true;
  }
  
  //: Update control values.
  
  bool AttributeEditorBodyC::UpdateAll() {
    for(HashIterC<StringC,Tuple3C<WidgetC,TriggerC,IntT> > it(controls);it;it++)
      it.Data().Data2().Invoke();
    return true;
  }
  
  
}
