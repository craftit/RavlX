// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace

#include "Ravl/Face/EnrolSession.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/Exception.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    EnrolSessionC::EnrolSessionC()
    {
    }
    //: Constructor

    EnrolSessionC::EnrolSessionC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a EnrolSession
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "enrolsession")
        ;
      if (!xml)
        return;

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "enrol") {
          EnrolC enrol;
          xml >> enrol;
          if (IsElm(enrol.ActualId())) {
            throw ExceptionOperationFailedC("Failed to enrol: " + enrol.ActualId() + "\n");
          } ONDEBUG(cout << "Enrolling: " << enrol.ActualId() << endl);
          Insert(enrol.ActualId(), enrol);
        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end enrol session tag
      return;
    }

    XMLOStreamC &EnrolSessionC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml << XMLStartTag("enrolsession") << XMLContent;

      for (HashIterC < StringC, EnrolC > it(*this); it; it++) {
        xml << *it;
        ;
      }

      // End Tag
      xml << XMLEndTag;
      ;
      return xml;
    }
    
    DListC<StringC> EnrolSessionC::AllIds() const
    {
      DListC < StringC > list;
      for (HashIterC < StringC, EnrolC > it(*this); it; it++)
        list.InsLast(it.Key());
      return list;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolSessionC &data)
    {
      data = EnrolSessionC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolSessionC& data)
    {
      return data.SaveXML(xml);
    }

    void InitEnrolSessionFormat()
    {
    }
    
    static TypeNameC typeNameEnrolSessionC(typeid(EnrolSessionC), "FaceN::EnrolSessionC");

    FileFormatXMLStreamC<EnrolSessionC> FileFormatXMLStream_EnrolSessionC;

  }
}
