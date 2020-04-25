// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2003, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: ExtLib.cc 4777 2005-01-18 14:28:18Z ees1wc $"
//! lib=RavlAutoPort2
//! file="Ravl/SourceTools/VisualCPP/ExtLib.cc"

#include "Ravl/AutoPort2/ExtLib.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"


namespace RavlN {
  
  ExtLibBodyC::ExtLibBodyC(const StringC & iname, const DListC<StringC> & iincludePaths, DListC<StringC> & ilibPaths, DListC<StringC> & ilibs) 
    : name(iname), includePaths(iincludePaths), libPaths(ilibPaths), libs(ilibs) {
  }
    
  ExtLibBodyC::ExtLibBodyC(XMLIStreamC& xml) {

  StringC strType("");
    // Read tags until we get a ExtLib
    while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "faceinfo");
    if (!xml) return;

    //: Get the attributes of the tag
    xml.GetAttrib("name", name);

    //: The next tag is the actual id
    while (xml && xml.PeekTag(strType) !=XMLEndTag) { 
      StringC data;
      if(strType == "includepath") {
	xml.ReadTag(strType);
	xml >> data;
	data = data.TopAndTail();
	xml.ReadTag(strType);
	includePaths.InsLast(data);
      } else if(strType == "libpath") {
	xml.ReadTag(strType);
	xml >> data;
	data = data.TopAndTail();
	xml.ReadTag(strType);
	libPaths.InsLast(data);
      } else if(strType == "lib") {
	xml.ReadTag(strType);
	xml >> data;
	data = data.TopAndTail();
	xml.ReadTag(strType);
	libs.InsLast(data);
      } else {
	xml.ReadTag(strType);
	xml.SkipElement();
      }
    }
    //: read the end tag of enrol
    xml.ReadTag(strType);
    
    return;
  }
  
  
  XMLOStreamC &ExtLibBodyC::SaveXML(XMLOStreamC &xml) const {  

    // Set auto-indent mode
    xml.AutoIndent();

    xml.StartTag("extlib"); 
    xml.SetAttrib("name", name);
    xml.StartContents();

    for(DLIterC<StringC>it(includePaths);it;it++) {
      xml.StartTag("includepath"); 
      xml << *it;
      xml.EndTag(); 
    }

    for(DLIterC<StringC>it(libPaths);it;it++) {
      xml.StartTag("libpath"); 
      xml << *it;
      xml.EndTag(); 
    }

    for(DLIterC<StringC>it(libs);it;it++) {
      xml.StartTag("libs"); 
      xml << *it;
      xml.EndTag(); 
    }

    //: End extlib tag
    xml.EndTag();

    return xml;
  }


  XMLIStreamC &operator>>(XMLIStreamC &xml,ExtLibC &data) {
    data = ExtLibC(xml);
    return xml;
  }
  
  XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibC& data) {
    return data.SaveXML(xml); 
  }

  BinOStreamC &operator<<(BinOStreamC &strm, const ExtLibC &data) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Output to binary stream.
  // Not implemented.
  
  BinIStreamC &operator>>(BinIStreamC &strm, ExtLibC &data) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Read from binary stream.
  // Not implemented.

  std::ostream &operator<<(std::ostream &strm, const ExtLibC &data) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Output to binary stream.
  // Not implemented.
  
  std::istream &operator>>(std::istream &strm, ExtLibC &data) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Read from binary stream.
  // Not implemented.


  void InitExtLibFormat() {}
  
  static TypeNameC typeNameExtLibC(typeid(ExtLibC),"RavlN::ExtLibC");
  
  FileFormatXMLStreamC<ExtLibC> FileFormatXMLStream_ExtLibC;
  
}

