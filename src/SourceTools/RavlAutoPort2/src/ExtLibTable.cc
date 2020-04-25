// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2003, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// file-header-ends-here
//! rcsid="$Id: ExtLibTable.cc 4777 2005-01-18 14:28:18Z ees1wc $"
//! lib=RavlAutoPort2
//! file="Ravl/SourceTools/VisualCPP/ExtLibTable.cc"


#include "Ravl/AutoPort2/ExtLibTable.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/IO.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN  {

       
  ExtLibTableC::ExtLibTableC(XMLIStreamC& xml) {
    StringC strType("");

    // Read tags until we get a ExtLibTable
    while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "extlibs");
    if (!xml) return;

    // Read features
    while (xml && xml.PeekTag(strType)!=XMLEndTag) {
      if (strType == "extlib")  {
	ExtLibC extLib;
	xml >> extLib;
	Insert(extLib.Name(), extLib);
      }
      else {
	// Skip this set of tags
	xml.ReadTag(strType);
	xml.ReadTag(strType);
      }
    }
    xml.ReadTag(strType); // the end enrol session tag
    return;
  }
  
  XMLOStreamC &ExtLibTableC::SaveXML(XMLOStreamC &xml) const {  

    // Set auto-indent mode
    xml.AutoIndent();

    // Start tag
    xml << XMLStartTag("extlibs");
    // Set name attribute
    xml << XMLContent;

    //: print it out sorted
    for (HashIterC<StringC, ExtLibC> it(*this); it; it++) {
      xml << *it;
    }
    
    // End Tag
    xml << XMLEndTag;;
    return xml;
  }

  
  XMLIStreamC &operator>>(XMLIStreamC &xml,ExtLibTableC &data) {
    data = ExtLibTableC(xml);
    return xml;
  }
  
  XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibTableC& data) {
    return data.SaveXML(xml); 
  }

  void InitExtLibTableFormat() {}
  
  static TypeNameC typeNameExtLibTableC(typeid(ExtLibTableC),"RavlN::ExtLibTableC");
  
  FileFormatXMLStreamC<ExtLibTableC> FileFormatXMLStream_ExtLibTableC;

}

