// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/Protocol.cc"

#include "Ravl/Face/Protocol.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  namespace FaceN {

    ProtocolBodyC::ProtocolBodyC(const StringC & nm) :
        name(nm)
    {
    }
    
    ProtocolBodyC::ProtocolBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a Protocol
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "protocol")
        ;
      if (!xml)
        return;

      xml.GetAttrib("name", name);
      xml.GetAttrib("type", type);

      //: The next tag is the actual id
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        StringC str;
        if (strType == "enrolfile") {
          xml.ReadTag(strType);
          xml >> str;
#ifdef VISUAL_CPP
          str.gsub("/vol/db", "h:/db");
#endif
          enrolFile.InsLast(str.TopAndTail());
          xml.ReadTag(strType);
        } else if (strType == "evalfile") {
          xml.ReadTag(strType);
          xml >> str;
#ifdef VISUAL_CPP
          str.gsub("/vol/db", "h:/db");
#endif	
          evalFile.InsLast(str.TopAndTail());
          xml.ReadTag(strType);
        } else if (strType == "testfile") {
          xml.ReadTag(strType);
          xml >> str;
#ifdef VISUAL_CPP
          str.gsub("/vol/db", "h:/db");
#endif
          testFile.InsLast(str.TopAndTail());
          xml.ReadTag(strType);
        } else {
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      //: read the end tag of enrol
      xml.ReadTag(strType);
      return;
    }

    XMLOStreamC &ProtocolBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("protocol");

      xml.SetAttrib("name", name);
      xml.SetAttrib("type", type);

      // Write child tags
      xml.StartContents();

      if (!enrolFile.IsEmpty()) {
        for (DLIterC < FilenameC > it(enrolFile); it; it++) {
          xml.StartTag("enrolfile");
          xml << *it;
          xml.EndTag();
        }
      }
      if (!evalFile.IsEmpty()) {
        for (DLIterC < FilenameC > it(evalFile); it; it++) {
          xml.StartTag("evalfile");
          xml << *it;
          xml.EndTag();
        }
      }
      if (!testFile.IsEmpty()) {
        for (DLIterC < FilenameC > it(testFile); it; it++) {
          xml.StartTag("testfile");
          xml << *it;
          xml.EndTag();
        }
      }
      
      //: End Tag
      xml.EndTag();

      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ProtocolC &data)
    {
      data = ProtocolC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ProtocolC& data)
    {
      return data.SaveXML(xml);
    }

    void InitProtocolFormat()
    {
    }
    
    static TypeNameC typeNameProtocolC(typeid(ProtocolC), "FaceN::ProtocolC");

    FileFormatXMLStreamC<ProtocolC> FileFormatXMLStream_ProtocolC;
  
  }
}

