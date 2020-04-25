// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="23/8/2003"
//! userlevel=Normal
//! lib=RavlGUIUtil
//! file="Ravl/GUI/Util/exAttributeEditor.cc"
//! docentry="Ravl.API.Graphics.GTK.Util"

#include "Ravl/GUI/AttributeEditor.hh"
#include "Ravl/GUI/AttributeEditorWindow.hh"
#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/Option.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Frame.hh"

using namespace RavlN;
using namespace RavlGUIN;

//: Setup an example attribute object.

class ExAttributeCtrlBodyC
  : public AttributeCtrlBodyC
{
public:
  ExAttributeCtrlBodyC();
  //: Constructor.
  
  virtual ~ExAttributeCtrlBodyC();
  //: Destructor.
  
  virtual bool GetAttr(const StringC &attrName,StringC &attrValue);
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool SetAttr(const StringC &attrName,const StringC &attrValue);
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool GetAttr(const StringC &attrName,IntT &attrValue);
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.

  virtual bool SetAttr(const StringC &attrName,const IntT &attrValue);
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.

  virtual bool GetAttr(const StringC &attrName,Int64T &attrValue);
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.

  virtual bool SetAttr(const StringC &attrName,const Int64T &attrValue);
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.

  virtual bool GetAttr(const StringC &attrName,RealT &attrValue);
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool SetAttr(const StringC &attrName,const RealT &attrValue);
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool GetAttr(const StringC &attrName,bool &attrValue);
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  virtual bool SetAttr(const StringC &attrName,const bool &attrValue);
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
protected:
  bool boolValue;
  StringC stringValue;
  RealT realValue;
  StringC enumValue;
  IntT intValue;
  Int64T int64Value;
};

class ExAttributeCtrlC
  : public AttributeCtrlC
{
public:
  ExAttributeCtrlC()
    : DPEntityC(*new ExAttributeCtrlBodyC())
  {}
};

ExAttributeCtrlBodyC::ExAttributeCtrlBodyC()
  : boolValue(true),
    stringValue("Igloo"),
    realValue(0.2),
    enumValue("Green"),
    intValue(10),
    int64Value(4294967296ll)
{
  RegisterAttribute(AttributeTypeBoolC("Bool","Boolean value",true,true,true));
  RegisterAttribute(AttributeTypeStringC("String","String value",true,true,"Hello"));
  RegisterAttribute(AttributeTypeNumC<RealT>("Real","Real value",true,true,0,10,0.1,2));
  RegisterAttribute(AttributeTypeNumC<IntT>("Int","Int value",true,true,-1,65000,1,100));
  RegisterAttribute(AttributeTypeNumC<Int64T>("Int64","Int64 value",true,true,-1,9223372036854775807ll,1,100));
  DListC<StringC> opts;
  opts.InsLast("Red");
  opts.InsLast("Green");
  opts.InsLast("Blue");
  RegisterAttribute(AttributeTypeEnumC("Enum","Real value",true,true,opts,"Green"));
}

//: Destructor.

ExAttributeCtrlBodyC::~ExAttributeCtrlBodyC()
{}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::SetAttr(const StringC &attrName,const StringC &attrValue) {
  if(attrName=="String") {
    if(stringValue != attrValue) {
      stringValue = attrValue;
      SignalChange("String");
    }
    return true;
  }
  if(attrName=="Enum") {
    if(enumValue != attrValue) {
      enumValue = attrValue;
      SignalChange("Enum");
    }
    return true;
  }
  return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::GetAttr(const StringC &attrName,StringC &attrValue) {
  if(attrName=="String") {
    attrValue = stringValue;
    return true;
  }
  if(attrName=="Enum") {
    attrValue = enumValue;
    return true;
  }
  return AttributeCtrlBodyC::GetAttr(attrName,attrValue);  
}


//: Get a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::GetAttr(const StringC &attrName,IntT &attrValue) {
  if(attrName=="Int") {
    attrValue = intValue;
    return true;
  }
  return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::SetAttr(const StringC &attrName,const IntT &attrValue) {
  if(attrName=="Int") {
    if(intValue != attrValue) {
      intValue = attrValue;
      SignalChange("Int");
    }
    return true;
  }
  return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
}

//: Get a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::GetAttr(const StringC &attrName,Int64T &attrValue) {
  if(attrName=="Int64") {
    attrValue = int64Value;
    return true;
  }
  return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::SetAttr(const StringC &attrName,const Int64T &attrValue) {
  if(attrName=="Int64") {
    if(int64Value != attrValue) {
      int64Value = attrValue;
      SignalChange("Int64");
    }
    return true;
  }
  return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
}

//: Get a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::GetAttr(const StringC &attrName,bool &attrValue) {
  if(attrName=="Bool") {
    attrValue = (int) boolValue;
    return true;
  }
  return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::SetAttr(const StringC &attrName,const bool &attrValue) {
  if(attrName=="Bool") {
    if(boolValue != attrValue) {
      boolValue = attrValue;
      SignalChange("Bool");      
    }
    return true;
  }
  return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
}

//: Get a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::GetAttr(const StringC &attrName,RealT &attrValue) {
  if(attrName=="Real") {
    attrValue = realValue;
    return true;
  }
  return AttributeCtrlBodyC::GetAttr(attrName,attrValue);  
}

//: Set a stream attribute.
// Returns false if the attribute name is unknown.
// This is for handling stream attributes such as frame rate, and compression ratios.

bool ExAttributeCtrlBodyC::SetAttr(const StringC &attrName,const RealT &attrValue) {
  if(attrName=="Real") {
    // Numericaly this is bad, but we're interested in avoiding repeated setting of exactly the same value.
    if(realValue != attrValue) {
      realValue = attrValue;
      SignalChange("Real");
    }
    return true;
  }
  return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
}

int main(int nargs,char **argv) {
  Manager.Init(nargs,argv);
  
  OptionC opts(nargs,argv);
  bool multiWindow = opts.Boolean("m",false,"Multi window example ");
  bool showRW = opts.Boolean("rw",true,"Show read/write status");
  bool showName = opts.Boolean("nm",true,"Show name ");
  bool showDesc = opts.Boolean("d",false,"Show description ");
  bool showSL = opts.Boolean("sl",true,"Show save/load ");
  opts.Check();
 

  ExAttributeCtrlC attribs;
  
  //AttributeEditorC editor(attribs);
  //WindowC win(100,100,"Hello");
  //  win.Add(VBox(FrameC(editor,"Hello")));
  //win.Show();
  
  AttributeEditorWindowC editorwin1("test1",attribs,showRW,showName,showDesc,showSL);
  AttributeEditorWindowC editorwin2,editorwin3;
  editorwin1.Show();
  if(multiWindow) {
    editorwin2 = AttributeEditorWindowC("test2",attribs);
    editorwin2.Show();
    editorwin3 = AttributeEditorWindowC("test3",attribs);
    editorwin3.Show();
  }
  Manager.Start();
  
  return 0;
}
